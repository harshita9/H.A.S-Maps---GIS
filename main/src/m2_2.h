/*
 *Contains function declaration for m2.cpp 
 */

/* 
 * File:   m2_2.h
 * Author: tupilisa
 *
 * Created on February 16, 2018, 9:56 PM
 */

#ifndef M2_2_H
#define M2_2_H

#include "m1.h"
#include "RTreeLatLon.h"
#include "StreetsDatabaseAPI.h"
#include "OSMDatabaseAPI.h"
#include "graphics.h"
#include "LatLon.h"
#include <cmath> 
#include <iterator>
#include <algorithm>
#include <vector>
#include <iostream>
#include <sstream>

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/register/point.hpp>
#include <boost/geometry/index/rtree.hpp>


#include "Edges.h"
#include "IntersectionNode.h"
#include "Graph.h"


namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;

//Stores intersection position in latitude and longitude and the name
struct intersection_data{
    LatLon position1;
    std::string name;
};

//Stores point of interest position and name
struct poi_data{
    LatLon position2;
    std::string name;
};
//Stores feature data position in x and y and latitude and longitude and name
struct feature_data{
    FeatureType type;    
    std::vector<LatLon> position3;
    bool closed;
    std::string name;
    TypedOSMID osm;
    t_point *x_ypoints;
};
    

//Stores all the information for a street segment
struct street_data{
    std::string name;
    unsigned streetid;
    std::vector<LatLon> position4;
    double distance;
    bool oneway;
    float speed_limit;
    std::vector<t_point> x_yposition;
    unsigned num_curve;
    double angle; 
};
extern Graph map_graph;
//function to show legend when clicked legend button
void show_legend(void(*draw_screen) (void));
//function to show the legend button
void show_legend();
//function to act on help button press
void show_help(void(*draw_screen) (void));
//function to show help menu
void show_help();
//function that shows the help menu button
void show_enter_poi(void(*draw_screen) (void));
//clears the screen
void show_clear(void(*draw_screen) (void));
//act on enter point of interest button
void show_enter_poi();
//shows path to the point of interest
void show_poi_streets();
//clears the user selections
void show_clear();
//highlights the path between two intersections if a path is found
//otherwise does not highlight anythign
void highlight_path();
//Draws map features on the screen
void draw_screen();
//Draws street on the map based on speed limit and visible of the screen
void draw_street(bool view_small_streets,bool view_highway );
//Draws intersection and point of intersection on the screen
void draw_inter_poi();
//Highlights the found intersection
void highlight_found_intersection();
//Displays intersection
void show_int(void(*draw_screen) (void));
 //Prints the street name
void print_street_name(bool view_small_streets_name,bool view_large_streets_name);
//Shows subway stations when the button is clicked
void show_subway(bool view_poi_name);
void show_get_directions();
//Gets all the data from API for global vectors
void get_data();
//Highlights the clicked features, poi and intersections and outputs information
//in command window
void act_on_button_press(float x, float y, t_event_buttonPressed event);
//Alternative for pressing a button for subway, poi, find, and change map
void act_on_key_press(char key_pressed, int keysym);
//Shows all the poi name when zoomed in
void show_poi(void (*draw_screen) (void));
//highlight start and  end intersection
void highlight_start_end_intersection();
//Finds the intersection of two street names
void find(void (*draw_screen) (void));
//Displays subway and tramp stops only
void subway(void(*draw_screen) (void));//Displays subway
//Print street names helper function
void print_street_names_in_bounds(double x1, double x2, 
        double y1, double y2, std::string street_name);
//Print street names in bounds helper function
void print_street_names_in_bounds_helper(std::string street_name, 
        t_bound_box text_box);
//Print street names in command window
void show_street_names(unsigned street_id, unsigned position_id, int size);
//Add big features in the map
void add_big_features();
//Add Island
void add_Islandfeatures();
//Add all parks in the map
void add_park_features();
//Add remaining features in the map
void add_small_features(bool view_buildings);
void get_directions(void(*draw_screen) (void));
//Draws individual features on the screen
void draw_features(feature_data feature, int r,int g, int b);
//Highlights nearest poi, intersection and features when clicked
void highlight_where_user_clicked();
//Finds index of the closest feature
unsigned find_closest_feature(LatLon my_position);
//Indicates what type of feature the user clicked
std::string type_of_feature(unsigned k);
//Change longitude system to x
float longitude_to_cartesian(double longitude);
//Change latitude to y
float latitude_to_cartesian(double latitude);
//Change x to longitude
float cartesian_to_longitude(double x);
//Change y to latitude
float cartesian_to_latitude(double y);
//Loads the new map again when changing the map
bool load_my_map(std::string map_path, std::string map_path_osm);
//Changes the map of the city if clicked on the button
void change_map(void(*draw_screen) (void));
//Prints instruction for changing a map
void print_instructions();
//Finds max and min x and y of feature
void min_max_x_y(feature_data feature,float &xmax,float &xmin, float& ymax,
        float &ymin,unsigned size);
//Deletes all dynamically allocated object and clears the data structures;
void delete_all();
//loads the angle of each street segment
void load_angle();
//Returns the point of interest id from name
std::vector<unsigned> find_poi_from_name(std::string poi_name);
//Returns poi data from poi id
poi_data get_poi_data(unsigned poi_id);

//Deletes duplicates in an unordered vector
void delete_duplicates(std::vector<unsigned> &Info); 

#endif /* M2_2_H */

