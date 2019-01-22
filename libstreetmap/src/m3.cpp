/*
 * This file finds the fastest path from a intersection id to another id or
 * to a point of interest name
 */



#include "StreetsDatabaseAPI.h"
#include "LatLon.h"
#include "graphics.h"
#include "m1.h"
#include "m2.h"
#include "m2_2.h"
#include "m3.h"
#include "m3_2.h"
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <cmath> 
#include <queue> 


#include "Edges.h"
#include "IntersectionNode.h"
#include "Graph.h"



//This function reconstructs the intersection ids to segment id to find the path
std::vector<unsigned> reconstruct_path( std::vector<unsigned> &came_from, double turn_penalty){

    unsigned node_id1;
    unsigned node_id2;
    unsigned id=0;
    unsigned st_id=0.0;
    unsigned st_id2=0.0;
    double min_trav=0.0;
    double trav=0.0;
    bool first=true;
    int num=-1;
    
    std::vector<unsigned> total_path;
    //For all the intersection ids find the street segments
    for (unsigned i = 0; i < came_from.size() - 1; i++){
        node_id1 = came_from[i];
        node_id2 = came_from[i+1];
        auto adjacent = map_graph.all_intersections[node_id1].adjacent_intersection_ids;
        //If there is no streets found yet then penalty does not need to be taken into account
        if(num>-1){
            st_id=getStreetSegmentInfo(total_path[num]).streetID;
        }
        
         //If the to intersection id matches then get the  street segment
        //If more than one street segment exists the choose the one with low travel time and
        //Also considers the turn penalty
        for(unsigned k=0; k < adjacent.size(); k++){
           
            if(adjacent[k].get_to() == node_id2 ){
                trav=adjacent[k].travel_time;
                if(num>-1&&turn_penalty!=0){
                    st_id2=getStreetSegmentInfo(adjacent[k].street_ss_id).streetID;
                    if(st_id != st_id2)
                        trav+=turn_penalty;
                }
                if(first||min_trav>trav){
                    min_trav=trav;
                    id=adjacent[k].street_ss_id;
                    first=false;
                }
                                           
            }
        }
        //Store the street segment id
        total_path.push_back(id);
        num++;
        first=true;
        
    }
    
    return total_path;
}

//For the a star calculate estimated time from the successor_id to end id
//It uses the max speed of the graph as the speed and uses diagonal distance for time
double get_heuristic(unsigned successor_id, unsigned end_id){
    LatLon point1 = getIntersectionPosition(successor_id);
    LatLon point2 = getIntersectionPosition(end_id);
    double distance = find_distance_between_two_points(point1, point2);
    // get max speed for loaded map 
    double speed = map_graph.max_speed; 
    // calculate expected time 
    double time = distance / speed;
    
    return time;
}

//Constructs a path with intersection ids by going backward from end(current) id 
std::vector<unsigned> construct_path(std::unordered_map<unsigned,unsigned> &camefrom, unsigned current){
   
    //If the end id is not found then the start is found
    //Start here
    if(camefrom.find(current) == camefrom.end()){
        std::vector<unsigned > path2;
        path2.push_back(current);
        return path2;
    }
    //Find the intersection ids connected to current until start is found
   auto path=construct_path(camefrom, camefrom[current]);
   //Store the id is path
    path.push_back(current);
    
    return path;
}




//Finds the intersection id with f_min as its travel time score
unsigned find_inter(std::unordered_map<unsigned,double> &f_Score, double f_min){
    for(auto i=f_Score.begin();i!=f_Score.end();++i){
        if((i->second)==f_min)
            return (i->first);
    }
    return 0;
}

//Calculates the fastest time from start intersection to end intersection using a star algorithm
//If no path is found it returns an empty vector otherwise it returns street segments in order
std::vector<unsigned> a_star(unsigned start_id, unsigned end_id,const double turn_penalty){
    
    //Contains all the intersections that have been evaluated
    std::unordered_map<unsigned, IntersectionNode> closed_set;
    //Stores the Total Score to find the f_min
    std::vector<double> f_list;
    //Stores the street segment ids to return
    std::vector<unsigned> path;
    //Contains all the intersections that needs to be evaluated
    std::unordered_map<unsigned, IntersectionNode> open_set;

    
    IntersectionNode start = map_graph.all_intersections[start_id];
    //Stores the intersection ids as <to_intersection,from_intersection>
    std::unordered_map<unsigned, unsigned>came_from; 
    //Stores gScore for each intersection id
    std::unordered_map<unsigned, double>gScore;
    //Stores fScore for each intersection id
    std::unordered_map<unsigned,double>fScore2;
  
    //Add the start->Time to travel to start is 0
    gScore.emplace(start_id, 0.0);
    //Estimate the time from start to end and add it into fScore
    double score = get_heuristic(start_id, end_id);    
    fScore2.emplace(start_id,score);
    f_list.push_back(score);
    //Add this node on the to be evaluated lists
    open_set.emplace(start_id, start);

    //Intersection nodes for current and successor nodes 
    IntersectionNode node_successor;
    IntersectionNode node_current;
    
    unsigned node_current_id = start_id;
    //Loop until there are still intersection ids to explore and arrive to the end destination
    while(open_set.size() != 0){
        
        //Get the node Score in open list with the smallest f_min to evaluate
        double f_min = *std::min_element(f_list.begin(), f_list.end());
      
        node_current_id = find_inter(fScore2, f_min);
        node_current = map_graph.all_intersections[node_current_id];
        
        //if current node is the destination then exit loop
        if (node_current_id == end_id ){
            //Reconstruct path backwards
            std::vector<unsigned> way = construct_path(came_from, node_current_id);
            path = reconstruct_path(way,turn_penalty);
            return path;
        } 
        
        //remove current node from open list and add it to the closed list
        f_list.erase(std::find(f_list.begin(),f_list.end(),f_min));
        open_set.erase(node_current_id);
        fScore2.erase(node_current_id);
        closed_set.emplace(node_current_id, node_current);
      
        //Get connected street Segments
        std::vector<Edges> adj = map_graph.all_intersections[node_current_id].adjacent_intersection_ids;
        // for each successor 
        for (unsigned i = 0; i < adj.size(); i++) {
            //Get the intersection id of the connected street segment
            unsigned node_successor_id = adj[i].get_to();
            node_successor = map_graph.all_intersections[node_successor_id];
            
            //Get the travel time
            double g = adj[i].travel_time;
            double h_successor=0.0;
            //Get the estimated time to the end
            h_successor = get_heuristic(node_successor_id, end_id);
            
            //Get the street segment id 
            unsigned ss2 = adj[i].street_ss_id;

            
            double turn_pen = 0.0;
            
            //If the street ids are different get the turn penalty
            if(came_from.find(node_current_id) != came_from.end() && turn_penalty!=0.0){
                std::vector<unsigned> inters;               
                if(came_from.find(came_from[node_current_id]) != came_from.end()){
                      inters.push_back(came_from[node_current_id]);
                }
                inters.push_back(came_from[node_current_id]);
                inters.push_back(node_current_id);
                auto ss_id=reconstruct_path(inters,turn_penalty);
                unsigned ss=0;
                if(ss_id.size()>1){
                    ss=ss_id[1];
                }
                else                   
                    ss=ss_id[0];
                
                auto street_segment_info = getStreetSegmentInfo(ss);
                auto street_ID = street_segment_info.streetID;
                
                auto street_segment_info2 = getStreetSegmentInfo(ss2);
                auto street_ID2 = street_segment_info2.streetID;
                
                if(street_ID != street_ID2)
                    turn_pen = turn_penalty;
                
                
            }
            //Caclulate the estimated travel time from beginning to current node
            double successor_current_cost = gScore[node_current_id] + g + turn_pen;
            //Caclulate the estimated travel time from beginning to end
            double f_successor = successor_current_cost + h_successor;


             //If the intersection is in open set or closed set and their current time is greater
            //than previous skip this step otherwise add it in the  open set
            std::unordered_map<unsigned, IntersectionNode>::const_iterator found_os = open_set.find(node_successor_id);
            std::unordered_map<unsigned, IntersectionNode>::const_iterator found_cs = closed_set.find(node_successor_id);
           
            if(found_os!=open_set.end()){
                if(gScore[node_successor_id]<=successor_current_cost)
                    continue;
            }
            else if(found_cs != closed_set.end()){
                if (gScore[node_successor_id] <= successor_current_cost)
                    continue;
                f_list.push_back(f_successor);
                open_set.emplace(node_successor_id, node_successor);
                closed_set.erase(node_successor_id);
  
                fScore2[node_successor_id]=f_successor;
                
            }
            else{
                open_set.emplace(node_successor_id,node_successor);
                f_list.push_back(f_successor);
                fScore2.emplace(node_successor_id,f_successor);
            }
            gScore[node_successor_id] = successor_current_cost;
            if(came_from.find(node_successor_id) != came_from.end()){
               came_from[node_successor_id] = node_current_id;
            }
            else{
                came_from.emplace(node_successor_id,node_current_id);
            }
        }   
    }

    path.clear();

 return path;
}

// Returns the time required to travel along the path specified, in seconds. 
// The path is given as a vector of street segment ids, and this function 
// can assume the vector either forms a legal path or has size == 0.
// The travel time is the sum of the length/speed-limit of each street 
// segment, plus the given turn_penalty (in seconds) per turn implied by the path. 
// A turn occurs when two consecutive street segments have different street IDs.
double compute_path_travel_time(const std::vector<unsigned>& path, 
                                const double turn_penalty){
    double travel_time = 0;
    int num_turns = 0;
    double street_segment_travel_time = 0;
    
    if (path.size() == 0){
        return travel_time;
    }
    
    for (unsigned i = 0; i < path.size(); i++){
        //travel time is the sum of the length/speed-limit of each street segment
        street_segment_travel_time = find_street_segment_travel_time(path[i]);
        
        if (i != path.size() - 1){
            
            auto street_segment_info = getStreetSegmentInfo(path[i]);
            auto street_segment_ID = street_segment_info.streetID;
                    
            auto street_segment_info1 = getStreetSegmentInfo(path[i + 1]);
            auto street_segment_ID1 = street_segment_info1.streetID;
           
            // adds to number of turns if streets are different 
            if (street_segment_ID != street_segment_ID1){
                num_turns++;
            } 
        }
        travel_time = travel_time + street_segment_travel_time;
    }
    // calculates turn penalty and adds to total time 
    travel_time += num_turns * turn_penalty;
    
    return travel_time;
}


// Returns a path (route) between the start intersection and the end 
// intersection, if one exists. This routine should return the shortest path
// between the given intersections when the time penalty to turn (change
// street IDs) is given by turn_penalty (in seconds).
// If no path exists, this routine returns an empty (size == 0) vector. 
// If more than one path exists, the path with the shortest travel time is 
// returned. The path is returned as a vector of street segment ids; traversing 
// these street segments, in the returned order, would take one from the start 
// to the end intersection.
std::vector<unsigned> find_path_between_intersections(const unsigned intersect_id_start, 
                                                      const unsigned intersect_id_end,
                                                      const double turn_penalty){
    
    
    
    //If the intersections are connected directly then return one segment
    std::vector<unsigned> path_between_intersections;
    
    //Check that the intersect ids are within range of available intersections
    if(intersect_id_start>(getNumberOfIntersections()-1)||intersect_id_end>(getNumberOfIntersections()-1)||intersect_id_start==intersect_id_end)
        return path_between_intersections;
        

     unsigned start=intersect_id_start;
     unsigned end=intersect_id_end;
    path_between_intersections =  a_star(start,end, turn_penalty);
    
    return path_between_intersections;
    
}


// Returns the shortest travel time path (vector of street segments) from 
// the start intersection to a point of interest with the specified name.
// The path will begin at the specified intersection, and end on the 
// intersection that is closest (in Euclidean distance) to the point of 
// interest.
// If no such path exists, returns an empty (size == 0) vector.
std::vector<unsigned> find_path_to_point_of_interest(const unsigned intersect_id_start, 
                                               const std::string point_of_interest_name,
                                               const double turn_penalty){
    
    
    
    std::vector<unsigned> path_to_point_of_interest;//Store the fastest path
   
    //If the intersect id is out of bounds return empty
    if(intersect_id_start>getNumberOfIntersections()-1){
        return path_to_point_of_interest;
    }
    
    //Get poi_ids from the name
    
    auto poi_ids = find_poi_from_name(point_of_interest_name);

    if(poi_ids.size()==0)
        return path_to_point_of_interest;
    std::vector<unsigned>intersection_closest;
 
    //Find the closest intersection for all the poi
    for(unsigned i=0;i<poi_ids.size();++i){
        auto position= get_poi_data(poi_ids[i]).position2;
        intersection_closest.push_back(find_closest_intersection(position));
     
    }
    
    //Find the path to the closest point of interest
    path_to_point_of_interest= a_star_poi(intersect_id_start,intersection_closest,turn_penalty);
    
    
    
    return path_to_point_of_interest;
    
    
}

//Calculates the fastest time from start intersection to end intersection using a star algorithm
//If no path is found it returns an empty vector otherwise it returns street segments in order
//This function uses heuristic as 0
std::vector<unsigned> a_star_poi(unsigned start_id, std::vector<unsigned> end_id,const double turn_penalty){

    //Store all the intersections that have been tested
    std::unordered_map<unsigned, IntersectionNode> closed_set;
    //store estimated travel time of the nodes in open set
    std::vector<double> f_list;
    //Stores the street segment to return
    std::vector<unsigned> path;
    //Store all the interesections that are yet to be tested
    std::unordered_map<unsigned, IntersectionNode> open_set;
    
    IntersectionNode start = map_graph.all_intersections[start_id];    
    
    // stores the intersection ids as <to_intersection,from_intersection>
    std::unordered_map<unsigned, unsigned>came_from; 
    //Stores gScore for each intersection id
    std::unordered_map<unsigned, double>gScore;

    //Stores fScore for each intersection id
    std::unordered_map<unsigned,double>fScore2;
  
    //Starting node has 0 travel time
    gScore.emplace(start_id, 0.0);
    double score = 0.0;
    //Add the estimated travel time and put it in the open set
    fScore2.emplace(start_id,score);    
    f_list.push_back(score);
    open_set.emplace(start_id, start);

    //Intersection nodes for current and successor nodes 
    IntersectionNode node_successor;
    IntersectionNode node_current;
    
    unsigned node_current_id = start_id;
    //Until all the possible nodes has been explored keep trying to find the final node
    while(open_set.size() != 0){
        //Find the node in open set with the lowest estimated travel time
        double f_min = *std::min_element(f_list.begin(), f_list.end());
       
        node_current_id = find_inter(fScore2, f_min);
        node_current = map_graph.all_intersections[node_current_id];
        
        
       
        //if current node is the destination then exit loop
            if (std::find(end_id.begin(),end_id.end(),node_current_id)!=end_id.end() ){
                    std::vector<unsigned> way = construct_path(came_from, node_current_id);
                    path = reconstruct_path(way,turn_penalty);
                    return path;
               
            } 
        
         //remove current node from open list and add it to the closed list
        f_list.erase(std::find(f_list.begin(),f_list.end(),f_min));   
        open_set.erase(node_current_id);
        fScore2.erase(node_current_id);
        closed_set.emplace(node_current_id, node_current);
      
        
        //Get connected street Segments
        std::vector<Edges> adj = map_graph.all_intersections[node_current_id].adjacent_intersection_ids;
        // for each successor 
        for (unsigned i = 0; i < adj.size(); i++) {
          
            unsigned node_successor_id = adj[i].get_to();
            node_successor = map_graph.all_intersections[node_successor_id];
            

            double g = adj[i].travel_time;     
            

            unsigned ss2=adj[i].street_ss_id;
            
            double turn_pen=0.0;
            //Calculate turn_penalty
            if(came_from.find(node_current_id) != came_from.end() && turn_penalty!=0.0){
                std::vector<unsigned> inters;               
                 if(came_from.find(came_from[node_current_id]) != came_from.end()){
                      inters.push_back(came_from[node_current_id]);
                
                 }
                inters.push_back(came_from[node_current_id]);
                inters.push_back(node_current_id);
                auto ss_id=reconstruct_path(inters,turn_penalty);
                 unsigned ss=0;
                if(ss_id.size()>1){
                    ss=ss_id[1];
                }
                else                   
                    ss=ss_id[0];
                
                 
                
                auto street_segment_info = getStreetSegmentInfo(ss);
                auto street_ID = street_segment_info.streetID;
                
                auto street_segment_info2 = getStreetSegmentInfo(ss2);
                auto street_ID2 = street_segment_info2.streetID;
                
                if(street_ID != street_ID2)
                    turn_pen = turn_penalty;
                
                
            }
            //calculate current cost and estimated  cost
            double successor_current_cost = gScore[node_current_id] + g + turn_pen;
            double f_successor = successor_current_cost;
            
            std::unordered_map<unsigned, IntersectionNode>::const_iterator found_os = open_set.find(node_successor_id);
            std::unordered_map<unsigned, IntersectionNode>::const_iterator found_cs = closed_set.find(node_successor_id);
                           
            //If the intersection is in open set or closed set and their current time is greater
            //than previous skip this step otherwise add it in the  open set
            if(found_os!=open_set.end()){
                if(gScore[node_successor_id]<=successor_current_cost)
                    continue;
            }
            else if(found_cs != closed_set.end()){
                if (gScore[node_successor_id] <= successor_current_cost)
                    continue;
                f_list.push_back(f_successor);
                open_set.emplace(node_successor_id, node_successor);
                closed_set.erase(node_successor_id);
                 fScore2[node_successor_id]=f_successor;
                
            }
            else{
                open_set.emplace(node_successor_id,node_successor);
                f_list.push_back(f_successor);
                fScore2.emplace(node_successor_id,f_successor);
            }
               gScore[node_successor_id] = successor_current_cost;
               if(came_from.find(node_successor_id) != came_from.end()){
                    came_from[node_successor_id] = node_current_id;
                }
                else{
                    came_from.emplace(node_successor_id,node_current_id);
                }
                

            }
          
    }
    //If no paths are found
     path.clear();

 return path;
}
