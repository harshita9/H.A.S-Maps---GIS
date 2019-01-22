/* 
 * Copyright 2018 University of Toronto
 *
 * Permission is hereby granted, to use this software and associated 
 * documentation files (the "Software") in course work at the University 
 * of Toronto, or for personal use. Other uses are prohibited, in 
 * particular the distribution of the Software either publicly or to third 
 * parties.
 *
 * The above copyright notice and this permission notice shall be included in 
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "m1.h"
#include "m2_2.h"
#include "StreetsDatabaseAPI.h"
#include "map_data.h"
#include "RTreeLatLon.h"
#include <cmath> 
#include <iterator>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/register/point.hpp>
#include <boost/geometry/index/rtree.hpp>

namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;

//R-Trees           
typedef std::pair<RTreeLatLon, unsigned> poi_rtree_pair;
bgi::rtree<poi_rtree_pair, bgi::quadratic<16>> poiRTree;

typedef std::pair<RTreeLatLon, unsigned> intersection_rtree_pair;
bgi::rtree<intersection_rtree_pair, bgi::quadratic<16>> intersectionRTree;

//Helper Functions
//Find the intersection in the street_segment, depending on one way or two way
void adjacent_intersection_helper(StreetSegmentInfo segment_info, 
                                unsigned intersection);
//Calculates the distance of a street segment
void calculate_distance_street_segment_helper(StreetSegmentInfo info, 
                                    unsigned streetSegmentID);

//Calculates time travel for a street segment
void calculate_travel_time_helper(StreetSegmentInfo info, 
                                unsigned street_segment_id);
//Resizes all the global variable used for data structure
void resize_All_Global_variables();
    
map_data map;

//Check if load map is successful and contains all the data structures for better performance
bool load_map(std::string map_name) {
    bool load_successful = false; //Indicates whether the map has loaded 
                                  //successfully
    //Load map
    load_successful = loadStreetsDatabaseBIN(map_name);
    
    
    if(load_successful){
   
        std::vector<unsigned> street_id_vec;
        
        resize_All_Global_variables();
        
        for(unsigned i = 0; i < getNumberOfPointsOfInterest(); i++){
            //Insert position and index of points of interest into r-tree
            poiRTree.insert(std::make_pair( 
                    RTreeLatLon(getPointOfInterestPosition(i).lat(), 
                    getPointOfInterestPosition(i).lon()), i));
        }
     
        for(unsigned intersection = 0; intersection < getNumberOfIntersections(); 
                intersection++){
//            Insert position and index of intersection into r-tree
            intersectionRTree.insert(std::make_pair( 
                    RTreeLatLon(getIntersectionPosition(intersection).lat(), 
                    getIntersectionPosition(intersection).lon()), intersection));
            
            for(unsigned count = 0; count < 
                    getIntersectionStreetSegmentCount(intersection); count++){
                //Add id to the vector for easy access
                auto const street_segment_id = 
                        getIntersectionStreetSegment(intersection, count);
                map.intersection_Street_Segments[intersection].
                                    push_back(street_segment_id);
                
                //Add the street names for easy access
                auto segment_info = getStreetSegmentInfo(street_segment_id);
                unsigned street_id = segment_info.streetID;               
                
                //Get the adjacent intersections
                adjacent_intersection_helper(segment_info, intersection);
                 
                map.street_intersection[street_id].push_back(segment_info.from);
                map.street_intersection[street_id].push_back(segment_info.to);
                
                //Get Distance of a street Segment
                map.distance_street_segment[street_segment_id] = 0.0;
                calculate_distance_street_segment_helper
                        (segment_info, street_segment_id);

                //Calculate Travel Time            
                map.distance_travel_time[street_segment_id] = 0.0;
                calculate_travel_time_helper(segment_info, street_segment_id);

                //Get the street segments for street ID
                map.street_street_segments[street_id].push_back(street_segment_id);
            }
        }
        
        for(unsigned i = 0; i < getNumberOfStreets(); i++){
            
            std::string street_name = getStreetName(i);
             
            //If the name already exists add the street Id in the vector
            auto search = map.street_ids_from_name.find(street_name);
            
            if(search != map.street_ids_from_name.end()){
                 map.street_ids_from_name[street_name].push_back(i);
            }
            else{
                street_id_vec.push_back(i);
                map.street_ids_from_name.emplace(street_name, street_id_vec);
            }

            street_id_vec.clear();      
            street_id_vec.shrink_to_fit(); 
        }
        get_data();
    }
    
    
    return load_successful;
}


void close_map() {
    //Clean-up map related data structures
    closeStreetDatabase(); 
    
    //Clear all the global vectors
    map.intersection_Street_Segments.clear();
    map.adjacent_intersections.clear();
    map.street_street_segments.clear();
    map.distance_street_segment.clear();
    map.distance_travel_time.clear();
    map.street_intersection.clear();
    
    //Clear global unordered map
    map.street_ids_from_name.clear();
    
    //Clear all r-trees
    poiRTree.clear();
    intersectionRTree.clear();
    
    delete_all();
    
}

//Returns street id(s) for the given street name
//If no street with this name exists, returns a 0-length vector.
std::vector<unsigned> find_street_ids_from_name(std::string street_name) {
//If the street name is not found returns an empty vector
    if(map.street_ids_from_name.find(street_name) == map.street_ids_from_name.end())
        return {};
        
    return map.street_ids_from_name[street_name];
    
}

//Returns the street segments for the given intersection 
std::vector<unsigned> find_intersection_street_segments(unsigned intersection_id)
{
   return map.intersection_Street_Segments[intersection_id];
    
}

//Returns the street names at the given intersection (includes duplicate street 
//names in returned vector)
std::vector<std::string> find_intersection_street_names(unsigned intersection_id)
{   
    StreetSegmentInfo street;    
    std::vector<std::string> street_names;
    
    //Get the street name by getting street ID
    for(auto it = map.intersection_Street_Segments[intersection_id].begin(); 
            it != map.intersection_Street_Segments[intersection_id].end(); ++it){
        street = getStreetSegmentInfo(*it);      
        street_names.push_back(getStreetName(street.streetID));           
    }
        
    return street_names;
}

//Returns true if you can get from intersection1 to intersection2 using a single 
//street segment (checks for 1-way streets too)
//an intersection is considered to be connected to itself
bool are_directly_connected(unsigned intersection_id1, unsigned intersection_id2)
{
   //Get the adjacent intersection then compare
    auto adj_int = find_adjacent_intersections(intersection_id1);
    
    //Check the corner case
    if(intersection_id1 == intersection_id2)
        return true;
    
    auto it = std::find(adj_int.begin(), adj_int.end(), intersection_id2);
    
    //Check if the intersection is found
    if(it == adj_int.end())
        return false;
    else 
        return true;
    
}

//Returns all intersections reachable by traveling down one street segment 
//from given intersection (hint: you can't travel the wrong way on a 1-way street)
//the returned vector should NOT contain duplicate intersections
std::vector<unsigned> find_adjacent_intersections(unsigned intersection_id) {
    //Use the global variable
    auto adj_inter = map.adjacent_intersections[intersection_id];
    delete_duplicates(adj_inter);
    return adj_inter;
  
}

//Returns all street segments for the given street
std::vector<unsigned> find_street_street_segments(unsigned street_id) {
    //Delete duplicates
    delete_duplicates(map.street_street_segments[street_id]);
    
    return map.street_street_segments[street_id];
}

//Returns all intersections along the a given street
std::vector<unsigned> find_all_street_intersections(unsigned street_id) {
   //Delete duplicates
   delete_duplicates(map.street_intersection[street_id]);
  
   return map.street_intersection[street_id]; 
    
}

//Return all intersection ids for two intersecting streets
//This function will typically return one intersection id.
//However street names are not guarenteed to be unique, so more than 1 intersection id
//may exist
std::vector<unsigned> find_intersection_ids_from_street_names(
                            std::string street_name1, std::string street_name2){
    std::vector<unsigned> ids;
    
    std::vector<unsigned>street_intersections1;
    std::vector<unsigned>street_intersections2;
    
    
    //Get street info from name
    auto street1_id = find_street_ids_from_name(street_name1);
    auto street2_id = find_street_ids_from_name(street_name2);
    
    //Split all the street into its intersection ids
    for(unsigned j = 0; j < street1_id.size(); j++){
        auto inter = find_all_street_intersections(street1_id[j]);
        //For the intersections create a hash table
        for(unsigned k = 0; k < inter.size(); k++){
            street_intersections1.push_back(inter[k]);
        }
    }
    for(unsigned j = 0; j < street2_id.size(); j++){
        auto inter2 = find_all_street_intersections(street2_id[j]);
        //For every intersection, if there is a intersection ID that is already 
        //visited, then store that value
        for(unsigned k = 0; k < inter2.size(); k++){
            street_intersections2.push_back(inter2[k]);
        }
    }
    
    //Find the common intersections in the two vectors 
    std::sort(street_intersections2.begin(), street_intersections2.end());
    std::sort(street_intersections1.begin(), street_intersections1.end());
    
    //Gets the common ids and puts in the vector to return
    std::set_intersection(street_intersections1.begin(), 
            street_intersections1.end(), street_intersections2.begin(), 
            street_intersections2.end(), std::back_inserter(ids));

    
    return ids;
   
}

//Returns the distance between two coordinates in meters
double find_distance_between_two_points(LatLon point1, LatLon point2) {
    
    double distance = 0.0; 
    
    // Calculate the average latitude
    // Latitude and longitude are given in degrees 
    double lat_avg = ((DEG_TO_RAD * point1.lat()) + (DEG_TO_RAD * point2.lat())) 
       / 2.0; 
   
    // Convert to coordinate (x,y) points
    // First point
    double x1 = (DEG_TO_RAD * point1.lon()) * cos(lat_avg);
    double y1 = DEG_TO_RAD * point1.lat();
    // Second point
    double x2 = (DEG_TO_RAD * point2.lon()) * cos(lat_avg);
    double y2 = DEG_TO_RAD * point2.lat();
    
    // Distance between two points = R*sqrt((y2-y1)^2+(x2-x1)^2)
    // R is the constant EARTH_RADIUS_IN_METERS 
    distance = EARTH_RADIUS_IN_METERS * (sqrt(((y2 - y1) * (y2 - y1)) + 
            ((x2 - x1) * (x2 - x1)))); 
    
    return distance; 
}

//Returns the length of the given street segment in meters
double find_street_segment_length(unsigned street_segment_id) {
  
    return map.distance_street_segment[street_segment_id];
}

//Returns the length of the specified street in meters
double find_street_length(unsigned street_id) {
    double street_length = 0.0;
    
    //Get all the street_segment ids in the street
    auto street_ids = find_street_street_segments(street_id);
    unsigned id_size = street_ids.size();
    
    for(unsigned segment = 0; segment < id_size; segment++){
        //Add all the length of the street segments
        street_length += find_street_segment_length(street_ids[segment]);
    }
    
   return street_length; 
}

//Returns the travel time to drive a street segment in seconds 
//(time = distance/speed_limit)
double find_street_segment_travel_time(unsigned street_segment_id) {
    
    return map.distance_travel_time[street_segment_id];
}

//Returns the nearest point of interest to the given position
unsigned find_closest_point_of_interest(LatLon my_position) {
    //Find nearest point of interest and store in the vector named closest
    std::vector<poi_rtree_pair> closest;
    poiRTree.query(bgi::nearest(RTreeLatLon(my_position.lat(), 
        my_position.lon()), 1), 
        std::back_inserter(closest));

    //Returns index to nearest point of interest
    unsigned smallest_distance_index = closest[0].second;
    
    return smallest_distance_index;     
}

//Returns the the nearest intersection to the given position
unsigned find_closest_intersection(LatLon my_position) {
    //Find nearest intersection and store in the vector named closest
    std::vector<intersection_rtree_pair> closest;
    intersectionRTree.query(bgi::nearest(RTreeLatLon(my_position.lat(), 
        my_position.lon()), 1), 
        std::back_inserter(closest));
    
    //Returns index to nearest intersection
    unsigned smallest_distance_index = closest[0].second;
    
    return smallest_distance_index; 

}

/************************All the helper Functions*****************************/

//Calculates the travel time for each street segment
void calculate_travel_time_helper(StreetSegmentInfo info, 
        unsigned street_segment_id){
    
        // Get distance in meters 
        double distance = map.distance_street_segment[street_segment_id];
        double speed_limit = info.speedLimit;
        double const KM_TO_M = 1000.0;
        double const HR_TO_SEC = 3600.0;
        
        // Get speed_limit in meters/second (m/s)
        speed_limit *= (KM_TO_M / HR_TO_SEC);

        // Get time in seconds 
        map.distance_travel_time[street_segment_id] = distance / speed_limit;
    
}

//Calculates distance of street segment and stores it in a global variable
void calculate_distance_street_segment_helper(StreetSegmentInfo info, 
        unsigned streetSegmentID){
    //Calculate the distance of a street segment
    //Get the LatLon of both intersections
    auto intersection_1_latlon = getIntersectionPosition(info.from);
    auto intersection_2_latlon = getIntersectionPosition(info.to);
    auto nCurvePoints = info.curvePointCount;

    //Store all the curve points LatLon in one vector 
    //Need to check if this actually stores all the LatLon of each curve point
    if(nCurvePoints != 0){
        for(unsigned j = 0; j < nCurvePoints-1; j++){
            map.distance_street_segment[streetSegmentID] += 
                    find_distance_between_two_points(getStreetSegmentCurvePoint
                    (streetSegmentID, j), getStreetSegmentCurvePoint(
                    streetSegmentID, j+1));
        }
        //Get the distance between intersection and the curve point
        map.distance_street_segment[streetSegmentID] += 
                find_distance_between_two_points(intersection_1_latlon, 
                        getStreetSegmentCurvePoint(streetSegmentID, 0));

        map.distance_street_segment[streetSegmentID] += 
                find_distance_between_two_points(intersection_2_latlon, 
                getStreetSegmentCurvePoint(streetSegmentID, nCurvePoints-1)); 

    }
    //Else if no curve points....distance just the difference between the 
    //two intersection points
    else {
        map.distance_street_segment[streetSegmentID] = 
                find_distance_between_two_points(intersection_1_latlon, 
                intersection_2_latlon);
    }
 
    return;
}

//Deletes duplicates in an unordered vector
void delete_duplicates(std::vector<unsigned> &Info){
    auto last = Info.end();
    //Checks if the value exists in the vector and moves it to the end
        for(auto value = Info.begin(); value!= last; value++){
            last = std::remove(value+1, last, *value);//Pointing to the duplicate values
         }
    //delete all the value at the end which are the duplicates
    Info.erase(last,Info.end());
}

//Finds adjacent_intersection and stores it in a global variable with the index 
//as intersection ID
void adjacent_intersection_helper(StreetSegmentInfo segment_info, 
                            unsigned intersection){

    //Check if the Street is one way
    if(!segment_info.oneWay){
        //add the intersection that is not given to the vector
        if(segment_info.from == intersection){                       
           map.adjacent_intersections[intersection].push_back(segment_info.to);

        }
        else{
            map.adjacent_intersections[intersection].push_back(segment_info.from);

        }

    }
    //Add the from point of intersection in one way street
    else if(segment_info.from == intersection){
           map.adjacent_intersections[intersection].push_back(segment_info.to);
    }
    
    return;
}
//Resizes all the global variables
void resize_All_Global_variables(){
    //Resize the intersection to remove empty space
    map.intersection_Street_Segments.resize(getNumberOfIntersections());
    map.street_street_segments.resize(getNumberOfStreets());
    map.adjacent_intersections.resize(getNumberOfIntersections());

    //For street_segments
    map.street_intersection.resize(getNumberOfStreets());
    map.distance_street_segment.resize(getNumberOfStreetSegments());
    map.distance_travel_time.resize(getNumberOfStreetSegments());

  return;
}

