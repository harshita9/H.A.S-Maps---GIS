/*
 *This program draws the graphics of a city map and allows the user to interact with 
 * it by highlighting objects when clicked and provides geographical information.
 */


#include "StreetsDatabaseAPI.h"
#include "LatLon.h"
#include "graphics.h"
#include "m1.h"
#include "m2.h"
#include "m2_2.h"
#include "m3.h"
#include <algorithm>
#include <vector>
#include <cmath>  
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/register/point.hpp>
#include <boost/geometry/index/rtree.hpp>
#include <iostream>
#include <cstdlib>
#include <readline/readline.h>
#include <readline/history.h>

#include "Edges.h"
#include "IntersectionNode.h"
#include "Graph.h"

namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;

//R-Trees           
typedef std::pair<RTreeLatLon, unsigned> feature_rtree_pair;
bgi::rtree<feature_rtree_pair, bgi::quadratic<16>> featureRTree;

unsigned feature_id_rtree = 0; //for rtree
double average_latitude;
bool clicked_subway = false;
bool clicked_poi = false;
bool clicked_get_directions = false;   
bool clicked_legend = false;  
bool clicked_enter_poi = false; 
bool clicked_help = false;  
bool clicked_clear = false;  
float overall_area;
bool has_clicked = false;
bool print_directions=false;
bool print_directions_int=false;
bool clicked_intersection=false;
bool feat = true;
bool print_where_clicked=false;
double maximum_latitude = 1000;
double minimum_latitude = 0;
double maximum_longitude = 1000;
double minimum_longitude = 0;
float const width = 0.0001;
float const height = 0.0001;
bool find_intersections = false;
bool found_start_end_intersections=false;   
LatLon clicked_on;  

 //determine which one to highlight based on smallest speed
bool highlight_int=false;
bool highlight_poi=false;
bool highlight_feature=false;
int count_num_int_clicked=0;
bool get_poi_dir=false;
unsigned first_int=-1;
unsigned second_int=-1;
 
Graph map_graph;

char** command_completion (const char* stem_text, int start, int end);
char* intersection_name_generator(const char* stem_text, int state);

//All the data structures
//Stores intersection information for each intersection ids
std::vector<intersection_data> intersections;
//Stores point of interest information for each poi ids
std::vector<poi_data> poi;
//Stores feature information for feature ids
std::vector<feature_data> features;
//Store street information for each street segment ids
std::vector<street_data> streets;
//Stores intersection ids of two streets for find
std::vector<unsigned> intersection_ids;      
std::vector<unsigned> intersection_start_ids; 
std::vector<unsigned> intersection_end_ids;
std::vector<unsigned> street_ids; 
//Stores the poi name and poi id
 std::unordered_map<std::string, std::vector<unsigned>> poi_from_name;
 //Store all the available cites
std::vector<std::string> available_city = {
    "beijing_china",
            "cairo_egypt",
            "cape-town_south-africa",
            "golden-horseshoe_canada",
            "hamilton_canada",
            "hong-kong_china",
            "iceland",
            "interlaken_switzerland",
            "london_england",
            "moscow_russia",
            "new-delhi_india",
            "new-york_usa",
            "rio-de-jeneiro_brazil",
            "saint-helena",
            "singapore",
            "sydney_australia",
            "tehran_iran",
            "tokyo_japan",
            "toronto_canada",
            "beijing_china"
};

std::vector<const char*> intersection_names;


//Loads the new map again when changing the map
bool load_my_map(std::string map_path, std::string map_path_osm){   
    std::cout<< "Your H.A.S. Maps is loading....Please wait..."<<std::endl;
    //Load the map and related data structures
    bool load_success = load_map(map_path);
    bool load_OSM = loadOSMDatabaseBIN(map_path_osm);
    
    if(!load_success) {
        std::cout << "Failed to load map. Enter again." << map_path << "'\n";
        return false;
    }
    if(!load_OSM){
        std::cout << "Failed to load map. Enter again." << map_path_osm << "'\n";
        return false;
    }

   

    return true;
}

std::string name_of_poi;
//Prints instruction for changing a map
void print_instructions(){
    
    std::cout << "Please enter the command and arguments from the list below." <<
            std::endl;
    std::cout<<std::endl;
    std::cout <<"\"load_map 'city_name'\", where city_name is a valid city" << 
            std::endl;
    std::cout << "\"default\" for map of Toronto, Canada" << std::endl;
    std::cout<<std::endl;
    std::cout << "\"exit\" for keeping the same map" << std::endl;
    std::cout<<std::endl;
    std::cout << "\"city\" for list of available cities" << std::endl;
    std::cout<<std::endl;
    std::cout << "\"help\" for repeating these lines" << std::endl;
    return;
}

void change_map(void(*draw_screen) (void)){
    std::string city_map = "";
    std::string command = ";";
    std::string city = "";
    std::string map_path;
    std::string map_path_osm;
    bool valid = false;
    
    print_instructions();
    
    
   //Keep asking user for a valid city input
    while(!valid){
        std::cout << "> ";
        getline(std::cin, city_map);    
        std::stringstream map;
        map << city_map;
    
        map >> command;
        
        //Parse the command
        if(map.fail()){
            std::cout << "Invalid Command. Enter help for list of commands." <<
                    std::endl;
        }
        else if(command== "load_map" ){
            map >> city;
            if(map.fail()){
                std::cout << "Invalid Command. Enter help for more information."
                        << std::endl;
                valid = false;
            }
            else { 
                    if(std::find(available_city.begin(),available_city.end(),city)!=available_city.end()){
                       close_map();
                       closeOSMDatabase();
                       map_path = "/cad2/ece297s/public/maps/" + city + ".streets.bin";
                       map_path_osm = "/cad2/ece297s/public/maps/" + city + ".osm.bin";
                       if(!load_my_map(map_path,map_path_osm))
                           valid = false;
                       else{
                           valid = true;
                            std::cout << "Your city "<<city<<" is successfully loaded."<<std::endl;
                            std::cout<<"Please wait while the map is fully loaded..."<<std::endl;
                       }
                }
                    else{
                        std::cout<<"The city is not available. Please enter \"city\" "
                                "to get the list of available cities"<<std::endl;
                    }
            }

        }
        else if(command=="city"){
            for(unsigned k=0;k<available_city.size();++k){
                std::cout<<available_city[k]<<std::endl;
            }
        }
        else if(command == "default"){
            close_map();
            closeOSMDatabase();
            map_path = "/cad2/ece297s/public/maps/toronto_canada.streets.bin";
            map_path_osm = "/cad2/ece297s/public/maps/toronto_canada.osm.bin";
            if(!load_my_map(map_path, map_path_osm))
               valid = false;
            else
               valid = true;
            }
        else if (command == "help"){
            print_instructions();
            valid = false;
        }
        else if(command == "exit"){
             return;
        }
        else{
            std::cout << "Invalid Command. Enter help for more list of commands." << 
                    std::endl;
            valid = false;
        }
        
        
    }
    
    //Clean-up the map data and related data structures
    delete_all();
    get_data();
    set_visible_world(
        minimum_longitude * std::cos(average_latitude),
        minimum_latitude,
        maximum_longitude * std::cos(average_latitude),
        maximum_latitude 
    );
    clearscreen();
    draw_screen();
    
    return;
}

 bool show_poi_path=false;

//Draws map features on the screen
void draw_screen(){
    clearscreen();//Clears previously drawn
     show_clear();
    bool view_small_streets = false;
    bool view_buildings = false;
    bool view_small_streets_name = false;
    bool view_large_streets_name = false;
    bool view_highway = false;
    bool view_poi_name = false;
    
    //Constants for area
    const double SMALL_STREET_DIVISOR=16.0;
    const double INTERSECTION_DIVISOR=50.0;
    const double POI_DIVISOR=10000.0;
    const double SMALL_STREET_NAME_DIVISOR=300.0;
    const double LARGE_STREET_NAME_DIVISOR=100.0;
    const double HIGHWAY_DIVISOR=8.0;
    
    
    //Create buffers
    set_drawing_buffer(OFF_SCREEN);
    
    //Get area of current visible screen
    t_bound_box bound_box = get_visible_world();
    auto area = bound_box.area();
    
    //Divide the screens so it shows different features when different visibility
    if(area < overall_area /  SMALL_STREET_DIVISOR){
        view_small_streets = true;
    }
    if(area < overall_area / INTERSECTION_DIVISOR) {
       view_buildings = true;
        }
    if(area < overall_area / POI_DIVISOR)
        view_poi_name = true;
    
    if(area < overall_area / SMALL_STREET_NAME_DIVISOR)
        view_small_streets_name = true;
    
    if(area < overall_area / LARGE_STREET_NAME_DIVISOR)
        view_large_streets_name = true;
    
    if(area < overall_area / HIGHWAY_DIVISOR)
        view_highway = true;
    
   
   //Add features like parks, lake, rivers, etc in the map
    add_big_features();//Add lake first
    //Add Island next
    add_Islandfeatures();
    //Add all parks in the map
    add_park_features();
    //Add remaining features in the map
    add_small_features(view_buildings);

    
     //Draw the streets based on area
    draw_street(view_small_streets,view_highway );
    
    //Draw intersection and point of intersection
    draw_inter_poi();
    
    //Highlights features the user clicked
    highlight_where_user_clicked();
    
    highlight_start_end_intersection();
    
    if(clicked_poi && view_poi_name){
        for(unsigned i = 0; i < poi.size(); i++){
            auto x = longitude_to_cartesian(poi[i].position2.lon());
            auto y = latitude_to_cartesian(poi[i].position2.lat());  
            setcolor(BLACK);
            drawtext(x, y, poi[i].name, 100, 100);
        }
    }
   
     
    //Shows subway stations
    show_subway(view_poi_name);
    //show directions for clicked intersections 
    show_get_directions();
    //enter for poi directions  
    show_enter_poi();
    //highlights path for poi directions  
    show_poi_streets();
    //highlights path for directions
    highlight_path();
    //Highlight the found intersection
    highlight_found_intersection();
    
    //Prints the street name
    print_street_name(view_small_streets_name,view_large_streets_name);
    //show legend
    show_legend();
    //show help screen 
    show_help();
    
    copy_off_screen_buffer_to_screen();
    
    return;
}

//Draw map on graphic
void draw_map(){    
    
     
    init_graphics("H.A.S. Map",t_color(238,238,238));
    
    
    
    set_visible_world(
        minimum_longitude * std::cos(average_latitude),
        minimum_latitude,
        maximum_longitude * std::cos(average_latitude),
        maximum_latitude 
    );
    
    overall_area=get_visible_world().area();
    bool turn_on = true;
    update_message("Welcome to H.A.S. Map! Click on the zoom buttons to zoom, and the arrow keys to navigate. Press the Get Directions Button to get directions between two locations");
    create_button("Window", "Find", find);
    create_button("Find", "Subway", subway);
    create_button("Subway","POI", show_poi);  
    create_button("POI","Enter POI", show_enter_poi); 
    create_button("Enter POI","Change City", change_map);    
    create_button("Change City", "Get Directions", get_directions);     
    create_button("Get Directions","Intersection", show_int);    
    create_button("Intersection","Legend", show_legend); 
    create_button("Legend","Help", show_help); 
     create_button("Help","Clear", show_clear); 
    set_mouse_move_input(turn_on);
    set_keypress_input(turn_on);
    change_button_text("Zoom In", "+");
    change_button_text("Zoom Out", "-");
   
    event_loop(act_on_button_press, nullptr, act_on_key_press, draw_screen);
    clearscreen();
    close_graphics();
    delete_all();
    
    return;
}

//Alternative for pressing a button for subway, poi, find, and change map
void act_on_key_press(char key_pressed, int keysym){
    (void)keysym;
    if (key_pressed == 'c'){
        change_map(*draw_screen);
    }
    if (key_pressed == 'f'){
        find(*draw_screen);
    }
    if (key_pressed == 's'){
        subway(*draw_screen);
    }
    if (key_pressed == 'p'){
        show_poi(*draw_screen);
    }
}
//get directions to two intersections selected
void get_directions(void(*draw_screen) (void)){
  if(clicked_get_directions == false){
        clicked_get_directions = true;
        std::cout<<"Displaying Directions"<<std::endl;
        print_directions_int=true;
        
    }
    else{
        clicked_get_directions = false;
        print_directions_int=false;
        std::cout<<"Not displaying Directions"<<std::endl;
    }
    draw_screen();
}
//function to recognize that the legend button was clicked
void show_legend(void(*draw_screen) (void)){
  if(clicked_legend  == false){
        clicked_legend  = true;
        std::cout<<"Displaying Legend"<<std::endl;
        
    }
    else{
        clicked_legend  = false;
        std::cout<<"Not displaying Legend"<<std::endl;
        }
    draw_screen();
}
//recognize enter point of interest button is clicked
void show_enter_poi(void(*draw_screen) (void)){
  if(clicked_enter_poi  == false){
        clicked_enter_poi  = true;
        print_directions=true;
        
    }
    else{
        clicked_enter_poi = false;
        print_directions=false;
        }
    draw_screen();
}
//recognize that help button was clicked
void show_help(void(*draw_screen) (void)){
  if(clicked_help == false){
        clicked_help  = true;
        std::cout<<"Displaying Help"<<std::endl;
    }
    else{
        clicked_help  = false;
        std::cout<<"Not displaying Help"<<std::endl;
        }
    draw_screen();
}
//recognize that the clear button was clicked
void show_clear(void(*draw_screen) (void)){
  if(clicked_clear == false){
        clicked_clear  = true;
        std::cout<<"Cleared Screen"<<std::endl;
    }
    else{
        clicked_clear  = false;
        //std::cout<<"Not displaying Help"<<std::endl;
        }
    draw_screen();
}

//Displays subway and tramp stops only
void subway(void(*draw_screen) (void)){
    if(clicked_subway == false){
        clicked_subway = true;
        clicked_intersection=false;
        std::cout<<"Displaying Subway Stations"<<std::endl;
    }
    else{
        clicked_subway = false;
        std::cout<<"Not displaying Subway Stations"<<std::endl;
    }
    
    draw_screen();
    
}
//Displays intersection
void show_int(void(*draw_screen) (void)){
    if(clicked_intersection == false){
        clicked_intersection  = true;
        std::cout<<"Displaying Intersections"<<std::endl;
    }
    else{
        clicked_intersection  = false;
        std::cout<<"Not displaying Intersections"<<std::endl;
    }
    
    draw_screen();
    
}

//Highlights the clicked features, poi and intersections and outputs information
//in command window
void act_on_button_press(float x, float y, t_event_buttonPressed event){
    (void)event;
    
    auto const lon = cartesian_to_longitude(x);
    auto const lat = cartesian_to_latitude(y);
    
    LatLon position(lat, lon);
    
    clicked_on = position;
    has_clicked = true;
    print_where_clicked=true;
    find_intersections = false;
    found_start_end_intersections=false; //arfa added
    intersection_ids.clear();
    
    draw_screen();
}

//Shows all the poi name when zoomed in
void show_poi(void (*draw_screen) (void)) {
    if(clicked_poi == false){
        clicked_poi = true;
        std::cout << "Displaying Points of Interest" << std::endl;
    }
    else{
        clicked_poi = false;
        std::cout << "Not Displaying Points of Interest" << std::endl;
    }
    
    draw_screen();
}

//Finds the intersection of two street names
void find(void (*draw_screen) (void)) {
 
    std::cout << "Enter \"input1 and input2 search\" to search." << std::endl;
    std::cout << "Enter \"exit\" to go back to map." << std::endl;
    std::cout << "Find: ";
    
    std::vector<std::string> name1;
    std::vector<std::string> name2;
    
    //Get input from user
    std::string temp;
    std::cin >> temp;
    bool second_input = false; 
    
    if(temp=="exit")
        return;
    
    while (temp != "search"){
        if (temp == "and" && !second_input){
            second_input = true; 
        }
        else if (temp == "and" && second_input){
            std::cout << "Invalid input, press 'f'!" << std::endl;
        }
        else if (!second_input){
            name1.push_back(temp);
        }
        else {
            name2.push_back(temp);
        }
        std::cin >> temp;
    }
    
    std::string input1 = "";
    std::string input2 = "";
    
    //Build street name using user's input
    for(unsigned i = 0; i < name1.size(); i++) {
        if (i != name1.size() - 1)
            input1 += name1[i] + " ";
        else 
            input1 += name1[i];
    }
    std::cout << input1 << std::endl;
    
    for(unsigned i = 0; i < name2.size(); i++) {
        if (i != name2.size() - 1)
            input2 += name2[i] + " ";
        else 
            input2 += name2[i];
    }
    std::cout << input2 << std::endl;
    
    intersection_ids = find_intersection_ids_from_street_names(
                           input1, input2);
    //Find intersection from names
    if (intersection_ids.size() == 0){
        std::cout << "Intersections not found! Invalid streets!" << std::endl;
        return;
    }
    
    else{
        std::cout << "Intersections: ";

        for(unsigned i = 0; i < intersection_ids.size(); i++){
            std::cout << getIntersectionName(intersection_ids[i]) << std::endl;
        }
    }
    
    
    //Zoom in and highlight the intersection
    find_intersections = true;
    auto xcen = longitude_to_cartesian(getIntersectionPosition(intersection_ids[0])
                                .lon());
    auto ycen = latitude_to_cartesian(getIntersectionPosition(intersection_ids[0])
                                .lat());
    handle_zoom_in(xcen, ycen, draw_screen);
    t_bound_box bound_box = get_visible_world();
    auto area = bound_box.area();
    while(area >= overall_area/10000){
        handle_zoom_in(xcen, ycen, draw_screen);
        bound_box = get_visible_world();
        area = bound_box.area();

    }

}
//print the street names
void print_street_names_in_bounds(double x1, double x2, double y1, 
        double y2, std::string street_name){
    
    // draws street names based on position of street segment ends 
    // creates bound box and calls helper function to draw text 
    if (x1 > x2 && y1 > y2) {
        t_point point = t_point(x2, y2);
        t_bound_box text_box  = t_bound_box(point, abs(x1 - x2), abs(y1 - y2));
        print_street_names_in_bounds_helper(street_name, text_box);
    } 
    else if (x1 < x2 && y1 < y2) {
        t_point point = t_point(x1, y1);
        t_bound_box text_box = t_bound_box(point, abs(x1 - x2), abs(y1 - y2));
        print_street_names_in_bounds_helper(street_name, text_box);
    }
    else if (x1 < x2 && y1 > y2) {
        t_point point = t_point(x1, y2);
        t_bound_box text_box = t_bound_box(point, abs(x1 - x2), abs(y1 - y2));
        print_street_names_in_bounds_helper(street_name, text_box);
    }
    else {
        t_point point = t_point(x2, y1);
        t_bound_box text_box  = t_bound_box(point, abs(x1 - x2), abs(y1 - y2));
        print_street_names_in_bounds_helper(street_name, text_box);
    } 
    return;
}

void print_street_names_in_bounds_helper(std::string street_name, t_bound_box text_box){
    
    // draw text using the bound box 
    auto x = text_box.get_xcenter();
    auto y = text_box.get_ycenter();
    auto text_width = text_box.get_width();
    auto text_height = text_box.get_height();
    
    drawtext(x, y, street_name, text_width, text_height); 
    
    return;
}

//Print street names in command window
void show_street_names(unsigned street_id, unsigned position_id, int size){
    
    settextrotation(0);
    
    // gets visible world 
    t_bound_box visible_window = get_visible_world();
    
    // gets x and y position  
    double x1 = streets[street_id].x_yposition[position_id].x;
    double x2 = streets[street_id].x_yposition[position_id + 1].x;
    double y1 = streets[street_id].x_yposition[position_id].y;
    double y2 = streets[street_id].x_yposition[position_id + 1].y;
          
    // checks if positions of street segment ends are in bounds 
    // prints names by checking if street segment is one way or not 
    // prints names using helper functions  
    if (streets[street_id].name 
            != "<unknown>" && (visible_window.intersects(x1, y1) || 
            visible_window.intersects(x2, y2))) {
        //One Way streets
        if(streets[street_id].oneway){
            //Create one way street name
            setcolor(BLACK);
            std::string text = "  ->  ";
            std::string name = text + streets[street_id].name + text;
            settextrotation(0);
            settextrotation(streets[street_id].angle);
            setfontsize(size);
            print_street_names_in_bounds(x1, x2, y1, y2, name);
        }
        else{    
            setcolor(BLACK);
            settextrotation(0);
            settextrotation(streets[street_id].angle);
            setfontsize(size);
            print_street_names_in_bounds(x1, x2, y1, y2, streets[street_id].name);
        }
    }
    
    settextrotation(0);
    
    return;
}

//Change longitude system to x
float longitude_to_cartesian(double longitude){
    double x = (longitude) * std::cos(average_latitude);
    
    return x;
}

//Change latitude to y
float latitude_to_cartesian(double latitude){
    double y = latitude;
    
    return y;
}

//Change x to longitude
float cartesian_to_longitude(double x){
    double longitude = x / std::cos(average_latitude);
    
    return longitude;
}
//Change y to latitude
float cartesian_to_latitude(double y){
    double latitude = y;
    
    return latitude;
}


//Add big features in the map
void add_big_features(){
    //Draw feature
    for(unsigned k=0;k<features.size();k++){
        if(feat){
            //Allocate x_y points
            features[k].x_ypoints=new t_point[features[k].position3.size()];

            for(unsigned i=0;i<features[k].position3.size();i++){

                t_point point;
                point.x=longitude_to_cartesian(features[k].position3[i].lon());
                point.y=latitude_to_cartesian(features[k].position3[i].lat());
                features[k].x_ypoints[i]=point;

            }
        }
        if(k==features.size()-1)
            feat=false;
        if(features[k].type==Lake){
            //Turquoise
            draw_features(features[k],135,206,250);
            
        }
        
        else if(features[k].type==Shoreline){
            //light blue sky
            draw_features(features[k], 135,206,235);
            
        }
        
    }
    return;
    
}

//Add Island
void add_Islandfeatures(){
    
    for(unsigned k=0;k<features.size();k++){
        if(features[k].type==Island){           
                    //bisque
                    draw_features(features[k], 	219,219,219);
                }
    }
}
//Add all parks in the map
void add_park_features(){
    //Draw feature
    for(unsigned k=0;k<features.size();k++){
        
        //Chose different colors depending on the feature
        if(features[k].type==Park){
           //set color to green
           draw_features(features[k],152,251,152);
           
        }
       
        else if(features[k].type==Greenspace ){

            //darkgreen
            draw_features(features[k], 144,238,144);
        }
            
        else if(features[k].type==Golfcourse){
            
            //lime green
           draw_features(features[k], 144,238,144);
                
            }
        
        
    }
    return;
    
}

//Add remaining features in the map
void add_small_features(bool view_buildings){
    //Draw feature
    for(unsigned k=0;k<features.size();k++){        
       
        if(features[k].type==Beach){
            //set color to yellow
           
            draw_features(features[k], 255,250,205);
         
            
        }
       
        else if(features[k].type==River){
           //blue
            draw_features(features[k],  135,206,250);
            
        }
       
        else if(features[k].type==Building && view_buildings){
            
           //Thistle color
            draw_features(features[k], 	209,209,209);
            
        }
       
        else if(features[k].type==Stream){
            //blue
            draw_features(features[k], 135,206,250);
            //
        }
        
    }
    return;
    
}

//Draw individual streets with different color depending on closed surface or open
void draw_features(feature_data feature, int r,int g, int b){
    float xmax, xmin, ymax, ymin;
    min_max_x_y(feature, xmax, xmin, ymax, ymin, feature.position3.size());
    auto xcen = (xmax + xmin) / 2;
    auto ycen = (ymax + ymin) / 2;
    
    setlinewidth(1);
    setcolor(r,g,b);
    //Draw polygon if closed surface
    if(feature.closed == true){
        fillpoly(feature.x_ypoints, feature.position3.size());
        
        setcolor(1);
        if(feature.name != "<noname>" && feature.name != "<unspecified>")
            drawtext(xcen, ycen, feature.name, xmax - xcen, ymax - ycen);
        
    }
    //Draw lines if not closed surface
    else{
        for(unsigned i = 0;i < feature.position3.size() - 1; ++i){
            drawline(feature.x_ypoints[i].x,feature.x_ypoints[i].y,
                    feature.x_ypoints[i + 1].x,feature.x_ypoints[i + 1].y);
        }
       
        setcolor(1);
         if(feature.name != "<noname>" && feature.name != "<unspecified>")
            drawtext(xcen, ycen, feature.name, xmax - xcen, ymax - ycen);
    }
    return;
}


//find max and min x and y
void min_max_x_y(feature_data feature, float &xmax,float &xmin, float& ymax, float &ymin,unsigned size){
    float x, y;
    
    for(unsigned j = 0; j < size; ++j){
        x = feature.x_ypoints[j].x;
        y = feature.x_ypoints[j].y;
        
        if(x > xmax || j == 0)
            xmax = x;
        if(y > ymax || j == 0)
            ymax = y;
        if(x < xmin || j == 0)
            xmin = x;
        if(y < ymin || j == 0)
            ymin = y;
    }
    return;
}

//Deletes all dynamically allocated object and clears the data structures;
void delete_all(){
    for(unsigned i = 0; i < features.size(); i++){
        delete [] features[i].x_ypoints;
    }
      
    featureRTree.clear();
    feature_id_rtree = 0; 
    clicked_subway = false;
    clicked_get_directions=false;
    clicked_legend=false;
    clicked_enter_poi = false; 
    clicked_help=false;
    clicked_clear=false;
    has_clicked = false;
    highlight_int=false;
    show_poi_path=false;
    highlight_poi=false;
    highlight_feature=false;
    get_poi_dir=false;
    count_num_int_clicked=0;
    clicked_intersection=false;
    maximum_latitude = 1000;
    minimum_latitude = 0;
    maximum_longitude = 1000;
    minimum_longitude = 0;
    find_intersections = false;
    print_where_clicked=false;
    found_start_end_intersections=false;
     feat=true;
    intersection_start_ids.clear();
    intersection_end_ids.clear();
    intersections.clear();
    poi.clear();
    features.clear();
   streets.clear();
    intersection_ids.clear();
    
   map_graph.all_intersections.clear();
    
    
}


//Returns the nearest feature to the given position
unsigned find_closest_feature(LatLon my_position) {
    //Find nearest feature  and store in the vector named closest
    std::vector<feature_rtree_pair> closest;
    featureRTree.query(bgi::nearest(RTreeLatLon(my_position.lat(), 
        my_position.lon()), 1), 
        std::back_inserter(closest));

    //Returns index to nearest feature
    LatLon smallest_distance_position = closest[0].first.latlon;
    unsigned smallest_distance_index = 0; 
    
    for(unsigned k = 0; k < features.size(); k++){
        for(unsigned i = 0; i < features[k].position3.size(); i++){
        //Insert position and index of feature into r-tree
            if (getFeaturePoint(k, i).lat() == smallest_distance_position.lat() 
                    && getFeaturePoint(k, i).lon() == smallest_distance_position.lon()) {
                smallest_distance_index = k;
                break;
            }
        }
    }
    
    return smallest_distance_index;     
}

//Highlights nearest poi, intersection and features when clicked
void highlight_where_user_clicked(){
     //highlight intersection, feature, poi, based on distance from where user clicked
    if(has_clicked == true){
    
        //determine distance from the nearest intersection, poi, feature
        auto const id1 = find_closest_intersection(clicked_on);        
        auto const id2 = find_closest_point_of_interest(clicked_on);
        auto const id3 =find_closest_feature(clicked_on);
        auto intdis=find_distance_between_two_points(clicked_on,intersections[id1].position1);
        auto poidis=find_distance_between_two_points(clicked_on,poi[id2].position2);
        auto featuresdis=find_distance_between_two_points(clicked_on,features[id3].position3[0])-0.1;

   
    
        if((featuresdis<poidis)&&(featuresdis<intdis)){

            highlight_int=false;
            highlight_poi=false;
            highlight_feature=true;
            count_num_int_clicked=0;
            get_poi_dir=false;

        }
        else if((poidis<intdis)&&(poidis<featuresdis)){
            if((highlight_int==true)&&(!get_poi_dir)){  
            //highlight intersection was previously true
            count_num_int_clicked=0;
            get_poi_dir=true;
            highlight_int=false;
            highlight_poi=true;
            highlight_feature=false;

            }        
            else{   
            count_num_int_clicked=0;
            highlight_int=false;
            highlight_poi=true;
            highlight_feature=false;
            }


        }
            else{

            if(highlight_int==true){  
            //highlight intersection was previously true
            count_num_int_clicked=2;
            highlight_int=true;
            highlight_feature=false;
            highlight_poi=false;

            }  


            else{   
            get_poi_dir=false;
            count_num_int_clicked=1;
            highlight_int=true;
            highlight_feature=false;
            highlight_poi=false;
            }

        }
        if(highlight_int){
            
            if((count_num_int_clicked==2)){
                second_int=id1;
                get_poi_dir=false;
                    //draw the first int
                auto const x_first=longitude_to_cartesian(intersections[first_int].position1.lon());
                auto const y_first=latitude_to_cartesian(intersections[first_int].position1.lat());
                
                //Set color to green    
                setcolor(0,100,0);
                fillarc(x_first, y_first,  width/2.0,0,360);          
               
                //draw the second int
                //draw the nearest intersection
                auto const x2=longitude_to_cartesian(intersections[id1].position1.lon());
                auto const y2=latitude_to_cartesian(intersections[id1].position1.lat());
                
                //Set color to red        
                setcolor(4);
                fillarc(x2, y2,  width/2.0,0,360);
               
                //print the nearest intersection name and poi name
                if(count_num_int_clicked==2 && print_where_clicked){
                    
                    std::cout << "You have clicked on the intersection: " << intersections[id1].name << "\n";
                    std::cout<<std::endl;
                    std::cout << "Closest point of interest from this intersection is: " << poi[id2].name << "\n";
                    
                    auto adj_int=find_adjacent_intersections(id1);
                    std::cout<<std::endl;
                    std::cout<< "Nearby intersections are: "<<std::endl;
                    
                    for(unsigned i=0; i<adj_int.size(); i++){
                        std::cout<<intersections[adj_int[i]].name<<std::endl;
                    }
                    std::cout<<std::endl;
                    std::cout << "If you want to get directions from " << intersections[first_int].name 
                            <<" (GREEN) to "<<intersections[second_int].name<<" (RED) click the get directions button"<< "\n";

                    print_where_clicked=false;
                    }
                  }
            
            
            else{
                //store as the first intersection
                first_int=id1;
                second_int=-1;
                 //draw the nearest intersection
                auto const x2=longitude_to_cartesian(intersections[id1].position1.lon());
                auto const y2=latitude_to_cartesian(intersections[id1].position1.lat());
                
                //Set color to red        
                setcolor(4);
                fillarc(x2, y2,  width/2.0,0,360);
                if( found_start_end_intersections==false){
                    //print the nearest intersection name and poi name
                    std::cout << "You have clicked on the intersection: " << intersections[id1].name << "\n";
                    std::cout<<std::endl;
                    std::cout << "Closest point of interest from this intersection is: " << poi[id2].name << "\n";

                    auto adj_int=find_adjacent_intersections(id1);

                    std::cout<<std::endl;
                    std::cout<< "Nearby intersections are: "<<std::endl;
                    for(unsigned i=0; i<adj_int.size(); i++){
                        std::cout<<intersections[adj_int[i]].name<<std::endl;
                }
                }
            }
        }
    else if(highlight_poi){
        //highlight the nearest poi
        auto const x_pos=longitude_to_cartesian(poi[id2].position2.lon());
        auto const y_pos=latitude_to_cartesian(poi[id2].position2.lat());
       //Set color to red
        setcolor(4);
        fillarc(x_pos, y_pos, width/2.0,0,360);
       
        if(print_where_clicked){       
        std::cout << "You have clicked on a point of interest named: " << poi[id2].name << "\n";
        std::cout<<std::endl;
        std::cout << "The nearest intersection is : " << intersections[id1].name << "\n";
        std::cout<<std::endl;
        print_where_clicked=false;
        }
       
        if(get_poi_dir==true){
       
        auto const x_first=longitude_to_cartesian(intersections[first_int].position1.lon());
        auto const y_first=latitude_to_cartesian(intersections[first_int].position1.lat());
       
        //Set color to green    
        setcolor(0,100,0);
        fillarc(x_first, y_first,  width/2.0,0,360);          
       
        //draw the second interscetion
        //draw the nearest intersection
        auto const x2=longitude_to_cartesian(intersections[id1].position1.lon());
        auto const y2=latitude_to_cartesian(intersections[id1].position1.lat());
        
        //Set color to red        
        setcolor(4);
        fillarc(x2, y2,  width/2.0,0,360);
        second_int=id1;
       
        if(found_start_end_intersections==false){
            std::cout<<std::endl;
            std::cout << "If you want to get directions from the intersection" << intersections[first_int].name 
                    <<" (GREEN) to the point of interest called"<<poi[id2].name <<" (RED) click the get directions button"<< "\n";
            get_poi_dir=true;
       }
            
        else {
                if(!found_start_end_intersections)
                    std::cout << "You have clicked on a point of interest named: " << poi[id2].name << "\n";       
        
        }
    
    }
    }
    if(highlight_feature){
        //highlight the nearest feature
       draw_features(features[id3], 255,0,0);
       if(print_where_clicked){
        std::cout << "You have clicked on a feature of interest called: " << features[id3].name << "\n";
        std::cout<< "This is a "<<type_of_feature(id3)<<"\n";   
        print_where_clicked=false;
       }
       
    }
       
    }

}


//Indicates what type of feature the user clicked
std::string type_of_feature(unsigned k){
    std::string text;
 if(features[k].type==Park){
           //set color to green
     text="Park";
     return text;
           
        }
        else if(features[k].type==Beach){
            //set color to yellow
           text="Beach";
            return text;
          
            
        }
        else if(features[k].type==Lake){
            //Turquoise
             text="Lake";
            return text;
            
        }
        else if(features[k].type==River){
           
            text="River";
            return text;
            
        }
        else if(features[k].type==Island){
          text="Island";
           return text;
        }
        else if(features[k].type==Shoreline){
           text="Shoreline";
           return text;
        }
        else if(features[k].type==Building){
            
            text="Building";
            return text;
            
        }
        else if(features[k].type==Greenspace ){
            text="Greenspace";
            return text;
        }
            
        else if(features[k].type==Golfcourse){
            text="Golfcourse";
            return text;
                
            }
        else if(features[k].type==Stream){
            
            text="Stream";
            return text;
            
        }
    return "Unknown";
}

//Draws street on the map based on speed limit and visible of the screen
void draw_street(bool view_small_streets,bool view_highway ){
    
    const int SLOW_SPEED=50;
    const int HIGH_SPEED=79;
    //Draw streets
    //For Streets

    
    for(unsigned k = 0; k < streets.size(); ++k){
        t_point point;
        
        for(unsigned i = 0; i < streets[k].position4.size(); ++i){
            point.x = longitude_to_cartesian(streets[k].position4[i].lon());
            point.y = latitude_to_cartesian(streets[k].position4[i].lat());            
            streets[k].x_yposition.push_back(point); 
            
            if(i > 0 ){
                //Small street
                if(streets[k].speed_limit <= SLOW_SPEED && view_small_streets){

                    setlinewidth(8);
                    setcolor(WHITE);
                     drawline(streets[k].x_yposition[i-1].x,streets[k].x_yposition[i-1].y,
                            streets[k].x_yposition[i].x,streets[k].x_yposition[i].y);

                }
                //Medium street
                else if(streets[k].speed_limit <= HIGH_SPEED && streets[k].speed_limit > SLOW_SPEED
                        && view_highway){
                     if(found_start_end_intersections){
                        std::vector<unsigned> ::iterator it;
                        it=find(street_ids.begin(), street_ids.end(), streets[k].streetid);
                        if(it!=street_ids.end()){
                         setlinewidth(9);
                            setcolor(240,128,128);
                            drawline(streets[k].x_yposition[i-1].x,streets[k].x_yposition[i-1].y,
                                streets[k].x_yposition[i].x,streets[k].x_yposition[i].y);
                        }
                        else {
                            setlinewidth(9);
                            setcolor(WHITE);
                            drawline(streets[k].x_yposition[i-1].x,streets[k].x_yposition[i-1].y,
                                streets[k].x_yposition[i].x,streets[k].x_yposition[i].y);
                        
                        }
                    
                    }
                   
                else{   
                    setlinewidth(8);
                    setcolor(WHITE);
                     drawline(streets[k].x_yposition[i-1].x,streets[k].x_yposition[i-1].y,
                            streets[k].x_yposition[i].x,streets[k].x_yposition[i].y);
                }
                
                }
                
                //For highways
                else if (streets[k].speed_limit>HIGH_SPEED && view_highway){
                    setlinewidth(9);                   
                    setcolor(255,228,181);

                    drawline(streets[k].x_yposition[i-1].x,streets[k].x_yposition[i-1].y,
                        streets[k].x_yposition[i].x,streets[k].x_yposition[i].y);
                  
                }
            }
        }
    }
    
   return;
}

//Draws intersection and point of intersection on the screen
void draw_inter_poi(){
    
    if((clicked_intersection && !clicked_subway) ||(clicked_subway && clicked_intersection)){
        for(unsigned i = 0; i < intersections.size(); ++i){
            auto const x1 = longitude_to_cartesian(intersections[i].position1.lon());
            auto const y1 = latitude_to_cartesian(intersections[i].position1.lat());
            //Set intersection color to yellow
            setcolor(6);
            fillarc(x1, y1, width/2.0,0,360);

        }
    }
    if(!clicked_subway){
        //Get all the point of interests in the map
        for(unsigned j = 0; j < poi.size(); ++j){
            auto const x2 = longitude_to_cartesian(poi[j].position2.lon());
            auto const y2 = latitude_to_cartesian(poi[j].position2.lat());

            //Set point of intersection to orange
            setcolor(5);
            fillarc(x2, y2, width/2.0,0,360);

        }
    }
    return;
}

//Highlights the found intersection
void highlight_found_intersection(){
    
    if (find_intersections) {
        for(unsigned i = 0; i < intersection_ids.size(); i++){
            auto const x = longitude_to_cartesian(getIntersectionPosition(
                            intersection_ids[i]).lon());
            auto const y = latitude_to_cartesian(getIntersectionPosition(
                            intersection_ids[i]).lat());
            //Set color to red
            setcolor(4);
            fillarc(x, y, width/2.0,0,360);
        }
       
    }
    return;
}
//hightlight starting and final destination intersections if selected
void highlight_start_end_intersection(){
   
    if (found_start_end_intersections) {
         
        if((intersection_start_ids.size()!= 0) && (intersection_end_ids.size()!=0) ){   
        for(unsigned i = 0; i < intersection_start_ids.size(); i++){
            auto  x = longitude_to_cartesian(getIntersectionPosition(
                            intersection_start_ids[i]).lon());
            auto y = latitude_to_cartesian(getIntersectionPosition(
                            intersection_start_ids[i]).lat());
            
            x=x-0.000101;
            y=y+0.00035;
            draw_surface(load_png_from_file("libstreetmap/resources/green_2.png"),x,y);
  
        }
        
        for(unsigned i = 0; i < intersection_end_ids.size(); i++){
        
            auto x = longitude_to_cartesian(getIntersectionPosition(
                            intersection_end_ids[i]).lon());
            auto y = latitude_to_cartesian(getIntersectionPosition(
                            intersection_end_ids[i]).lat());
            x=x-0.000101;
            y=y+0.00035;
            draw_surface(load_png_from_file("libstreetmap/resources/red_2.png"),x,y); 
             
        }
       
    }
   
    }
    return;
}
 //Prints the street name
void print_street_name(bool view_small_streets_name,bool view_large_streets_name){
    const int SLOW_SPEED=50;
    const int HIGH_SPEED=79;
    
     //Prints the street name
    if (!clicked_poi) {
        for(unsigned k = 0; k < streets.size(); ++k){
            for(unsigned i = 1; i < streets[k].position4.size(); i = i + 6){
                //Print small streets
                if(streets[k].speed_limit <= SLOW_SPEED && view_small_streets_name){
                    show_street_names(k, i, 6);
                }
                //Print medium streets
                else if(streets[k].speed_limit <= HIGH_SPEED && streets[k].speed_limit > SLOW_SPEED
                        && view_large_streets_name) {
                    show_street_names(k, i, 6);
                }
                //Print highways
                else if (streets[k].speed_limit > HIGH_SPEED && view_large_streets_name){//For highways
                    show_street_names(k, i, 6);

                }
            }
        }
    }
}

//Shows subway stations when the button is clicked
void show_subway(bool view_poi_name){
    
    //If the subway button is clicked
    if(clicked_subway){
       
        for(unsigned m = 0; m < getNumberOfNodes(); ++m){
            //Loop through all the tags
            for(unsigned i = 0; i < getTagCount(getNodeByIndex(m)); ++i){
                
                //Get the key and value and draw if it  is subway or tram or rail
                std::string key,value;
                std::tie(key,value) = getTagPair(getNodeByIndex(m), i);
                if(key == "railway" && (value == "subway_entrance" || value == "tram_stop" || 
                        value == "rail" || value == "monorail"||value=="subway")){
                    auto pos = getNodeByIndex(m)->coords();
                    auto x = longitude_to_cartesian(pos.lon());
                    auto y = latitude_to_cartesian(pos.lat());
                    
                    setcolor(FIREBRICK);
                    fillarc(x, y, width, 0, 360);

                    if (view_poi_name){
                        setcolor(BLACK);
                        drawtext(x, y, value, 100, 100);
                    }
                }
            }
        }
  
    }
    return;
}
//print out the directions to the start and end intersections
void show_get_directions(){
    
    if(clicked_get_directions ){
        
        
        if(get_poi_dir==true){
            found_start_end_intersections=true;
        
            std::cout << "You are getting directions from " << intersections[first_int].name 
                    <<" (GREEN) to "<<intersections[second_int].name<<" (RED)"<< "\n";
            intersection_start_ids.clear();
            intersection_end_ids.clear();
            intersection_start_ids.push_back(first_int);
            intersection_end_ids.push_back(second_int);
            clicked_get_directions=false;
            get_poi_dir=false;
            draw_screen();
        }
        
        else if((count_num_int_clicked==2)){
            std::cout << "You are getting directions from " << intersections[first_int].name 
                    <<" (GREEN) to "<<intersections[second_int].name<<" (RED)"<< "\n";
            intersection_start_ids.clear();
            intersection_end_ids.clear();
            intersection_start_ids.push_back(first_int);
            intersection_end_ids.push_back(second_int);
            clicked_get_directions=false;
            found_start_end_intersections=true;
      
            draw_screen();
        }
        
        
        
        
        
    else{
          
        std::cout << "Enter \"input1 and input2 search\" for starting intersection." << std::endl;
        std::cout<<std::endl;
        std::cout << "Enter \"exit\" to go back to map." << std::endl;
        std::cout<<std::endl;
        std::cout<< "For example: Lola Road and Duplex Avenue search"<<std::endl;
        std::cout<<std::endl;
        std::cout<<"NOTE: you must write \"search\" after the intersection name"<<std::endl;
        std::cout<<std::endl;
        
        bool first_valid=false;
        std::string input1 = "";
        std::string input2 = "";
        
        while(!first_valid){
            std::cout << "Starting intersection: ";

            std::vector<std::string> name1;
            std::vector<std::string> name2;

            //Get input from user
            std::string temp;
            std::cin >> temp;
            bool second_input = false; 

        if(temp=="exit"){
            clicked_get_directions=false;
            print_directions_int=false;
           
            return;
        }

        while (temp != "search"){
            if (temp == "and" && !second_input){
                second_input = true; 
            }
            else if (temp == "and" && second_input){
                std::cout << "Invalid input!" << std::endl;
            }
            else if (!second_input){
                name1.push_back(temp);
            }
            else {
                name2.push_back(temp);
            }
            std::cin >> temp;
        }

        //Build street name using user's input
        for(unsigned i = 0; i < name1.size(); i++) {
            if (i != name1.size() - 1)
                input1 += name1[i] + " ";
            else 
                input1 += name1[i];
        }
        std::cout << input1 << std::endl;

        for(unsigned i = 0; i < name2.size(); i++) {
            if (i != name2.size() - 1)
                input2 += name2[i] + " ";
            else 
                input2 += name2[i];
        }
        std::cout << input2 << std::endl;
        intersection_start_ids.clear();
        intersection_start_ids = find_intersection_ids_from_street_names(
                               input1, input2);
      
        //Find intersection from names
        if (intersection_start_ids.size() == 0){
            std::cout << "Intersection not found! Invalid streets!" << std::endl;
            first_valid=false;
        }

        else{
            if(print_directions_int){
                std::cout << " You have chosen Starting Intersection: ";
                std::cout<<std::endl;
                for(unsigned i = 0; i < intersection_start_ids.size(); i++){
                    std::cout << getIntersectionName(intersection_start_ids[i]) << std::endl;
                }
                std::cout<<" as your Starting Intersection "<<std::endl;
            }
            first_valid=true;
        }
        }
      
      
        bool valid=false;
        
    while(!valid){
        
        std::cout<<std::endl;
        std::cout << "Destination intersection: ";
        std::cout<<std::endl;
        std::cout<< "For example: Lola Road and Duplex Avenue search"<<std::endl;
        std::cout<<std::endl;
        std::cout<<"NOTE: you must write \"search\" after the intersection name"<<std::endl;
        std::cout<<std::endl;
        std::vector<std::string> name11;
        std::vector<std::string> name22;
    
        //Get input from user
        std::string temp2;
        std::cin >> temp2;
        bool second_input_2 = false; 
        std::string input11 = "";
        std::string input22 = "";
    
        if(temp2=="exit"){
            clicked_get_directions=false;
            print_directions_int=false;
            return;
        }
    
    while (temp2 != "search"){
        if (temp2 == "and" && !second_input_2){
            second_input_2 = true; 
        }
        else if (temp2 == "and" && second_input_2){
            std::cout << "Invalid input, press 'g'!" << std::endl;
        }
        else if (!second_input_2){
            name11.push_back(temp2);
        }
        else {
            name22.push_back(temp2);
        }
        std::cin >> temp2;
    }
     
    
    //Build street name using user's input
    for(unsigned i = 0; i < name11.size(); i++) {
        if (i != name11.size() - 1)
            input11 += name11[i] + " ";
        else 
            input11 += name11[i];
    }
    std::cout << input11 << std::endl;
    
    for(unsigned i = 0; i < name22.size(); i++) {
        if (i != name22.size() - 1)
            input22 += name22[i] + " ";
        else 
            input22 += name22[i];
    }
    std::cout << input22 << std::endl;
    intersection_end_ids.clear();
    intersection_end_ids = find_intersection_ids_from_street_names(
                           input11, input22);
    //Find intersection from names
    if (intersection_end_ids.size() == 0){
        std::cout << "Intersection not found! Invalid streets!" << std::endl;
        valid=false;
    }
    
    else {
        std::cout<<std::endl;
        std::cout << " You have chosen ";
        valid=true;
        for(unsigned i = 0; i < intersection_end_ids.size(); i++){
            std::cout << getIntersectionName(intersection_end_ids[i]) << std::endl;
        }
        if(print_directions_int){
            std::cout<<" as your Destination Intersection"<<std::endl;
        }
        found_start_end_intersections=true;
    }
    }
    }
    }
    
    
    clicked_get_directions=false;
    return;
}

//Shows legend when the button is clicked
void show_legend(){
    //If the legend button is clicked
    if(clicked_legend){
 
             t_bound_box box= get_visible_world();
             auto x= box.left();
             auto y=box.top();
                    
                        
             draw_surface(load_png_from_file("libstreetmap/resources/legend_1.png"),x ,y);
      
    }
    return;
}
//when the button for enter point of interest is clicked
void show_enter_poi(){
    //If the subway button is clicked
    //show_poi_path=false;
    bool val_poi=false;
    if(clicked_enter_poi){
        
    while(!val_poi){
        std::cout << "Enter the point of interest you would like to get directions to or \"exit\" to go back to map:"<<std::endl;
        std::cout<<std::endl;


        getline(std::cin, name_of_poi);
        if(name_of_poi=="exit"){
            clicked_enter_poi=false;        
            return;
        }
        auto inte=find_poi_from_name(name_of_poi);
        if(inte.size()==0){
            val_poi=false;
            std::cout<<"Could not find "<<name_of_poi<<std::endl;
        }
        else{
            val_poi=true;
        }
    }
        bool fin_int=false;
   
    while(!fin_int){ 
            std::cout<<std::endl;
            std::cout << "Enter the intersection you would like to get directions to :"<< std::endl;
            std::cout<<std::endl;
            std::cout<< "For example: Lola Road and Duplex Avenue search"<<std::endl;
            std::cout<<std::endl;
            std::cout<<"NOTE: you must write \"search\" after the intersection name"<<std::endl;
            std::cout<<std::endl;
            std::vector<std::string> name1;
            std::vector<std::string> name2;
    
            //Get input from user
            std::string temp;
            std::cin >> temp;
            bool second_input = false; 
    
            if(temp=="exit"){
                clicked_enter_poi=false;

                return;
    }
    while (temp != "search"){
            if (temp == "and" && !second_input){
                second_input = true; 
            }
            else if (temp == "and" && second_input){
                std::cout << "Invalid input, press 'g'!" << std::endl;
            }
            else if (!second_input){
                name1.push_back(temp);
            }
            else {
                name2.push_back(temp);
            }
            std::cin >> temp;
    }
    
    std::string input1 = "";
    std::string input2 = "";
    
    //Build street name using user's input
    for(unsigned i = 0; i < name1.size(); i++) {
        if (i != name1.size() - 1)
            input1 += name1[i] + " ";
        else 
            input1 += name1[i];
    }
    
    for(unsigned i = 0; i < name2.size(); i++) {
        if (i != name2.size() - 1)
            input2 += name2[i] + " ";
        else 
            input2 += name2[i];
    }
   
    intersection_start_ids.clear();
    intersection_start_ids = find_intersection_ids_from_street_names(
                           input1, input2);
    //Find intersection from names
    if (intersection_start_ids.size() == 0){
        std::cout << "Intersection not found! Invalid streets!" << std::endl;
        fin_int=false;
    }
    
    else {
        fin_int=true;
        show_poi_path=true;
    }     
   }
    clicked_enter_poi=false;
    return;
    
}
}
    
// highlights path from intersection to closest intersection of poi when 
// directions are being shown 
void show_poi_streets(){
    auto point_comparison_x=0;
    auto point_comparison_y=0;
     double distance=0.0;
    if(show_poi_path){
        if(print_directions){
            std::cout << " You have chosen Starting Intersection: ";
            std::cout<<std::endl;
        for(unsigned i = 0; i < intersection_start_ids.size(); i++){
            std::cout << getIntersectionName(intersection_start_ids[i]) << std::endl;
        }
    }
    
   
    auto path=find_path_to_point_of_interest(intersection_start_ids[0],name_of_poi, 15.0);
    
    //find last intersection to display the destination icons
    for(unsigned i=0;i<path.size();++i){
        unsigned inte=0;
        if(path.size()>1){
        auto ss1=path[path.size()-1];
        auto ss2=path[path.size()-2];
        auto inf=getStreetSegmentInfo(ss1);
        auto inf1=getStreetSegmentInfo(ss2);
        if(inf1.to==inf.from)
            inte=inf.to;
        else if(inf1.to==inf.to)
             inte=inf.from;
        else if(!inf1.oneWay){
            if(inf1.from==inf.from)
                 inte=inf.to;
            else if(inf1.from==inf.to)
                 inte=inf.from;
         }
        }
        else{
            auto ss1=path[path.size()-1];
            auto inf=getStreetSegmentInfo(ss1);
            if(intersection_start_ids[0]==inf.from)
             inte=inf.to;
            else if(intersection_start_ids[0]==inf.to)
             inte=inf.from;
            
        }
         draw_surface (load_png_from_file("libstreetmap/resources/green_2.png"),
                 longitude_to_cartesian(getIntersectionPosition(intersection_start_ids[0]).lon()),
                 latitude_to_cartesian(getIntersectionPosition(intersection_start_ids[0]).lat()) );
        
         draw_surface (load_png_from_file("libstreetmap/resources/red_2.png"),longitude_to_cartesian(getIntersectionPosition(inte).lon())
                 , latitude_to_cartesian(getIntersectionPosition(inte).lat())); 
         
        for(unsigned k=0;k<streets[path[i]].position4.size();++k){
             
             if(k>0){ 
                setlinewidth(8);
                setcolor(252, 133, 133);
                drawline(streets[path[i]].x_yposition[k-1].x,streets[path[i]].x_yposition[k-1].y,
                                streets[path[i]].x_yposition[k].x,streets[path[i]].x_yposition[k].y);
            
             }
             
            
        }
         if(i==0 && print_directions){
             
             auto travel_time_path=compute_path_travel_time( path,15.00);
             std::cout<<std::endl;
             std::cout<<"Total estimated time (minutes) = "<<travel_time_path/60.0;
              /*         print directions       */
             // determinant equation gotten from
            //https://math.stackexchange.com/questions/274712/calculate-on-which-side-of-a-straight-line-is-a-given-point-located
           
                std::cout<<std::endl;
                std::cout<<" DIRECTIONS "<<std::endl;
                std::cout<<"Head straight on " << streets[path[i]].name<<std::endl;
             
             }
         
             distance=distance +find_street_segment_length(path[i]);
             if(i>0 && print_directions){
               if((streets[path[i]].name)!=(streets[path[i-1]].name)){
                   std::cout<<"Go Straight for "<<distance<<" meters."<<std::endl;
                   distance=0.0;
            
                   //use algorithm to tell if a line is to the left or the right of the line 
                    auto ncurvepoints = streets[path[i-1]].num_curve;
                    auto point_1_x=longitude_to_cartesian(streets[path[i-1]].position4[ncurvepoints].lon());
                    auto point_2_x=longitude_to_cartesian(streets[path[i-1]].position4[ncurvepoints + 1].lon());
                    auto point_1_y=latitude_to_cartesian(streets[path[i-1]].position4[ncurvepoints].lat());
                    auto point_2_y=latitude_to_cartesian(streets[path[i-1]].position4[ncurvepoints + 1].lat());
                    
                    
                    auto ncurvepoints2 = streets[path[i]].num_curve;
                    
                    if(ncurvepoints2==0){
                         point_comparison_x=longitude_to_cartesian(streets[path[i]].position4[0].lon()); 
                        point_comparison_y=latitude_to_cartesian(streets[path[i]].position4[0].lat());  
                    }
                    else{ 
                        point_comparison_x=longitude_to_cartesian(streets[path[i]].position4[1].lon()); 
                        point_comparison_y=latitude_to_cartesian(streets[path[i]].position4[1].lat());       
                    }
                    auto determinant=(point_comparison_x-point_1_x)*(point_2_y-point_1_y)-(point_comparison_y-point_1_y)*(point_2_x-point_1_x);
                    
                    if(determinant>0){
                    std::cout<<"Turn left onto "<<streets[path[i]].name << std::endl;
                    
                    }
                    else{
                    std::cout<<"Turn right onto "<<streets[path[i]].name << std::endl;
                    }
                     
               }
              
             }
                 
     }
    if(print_directions)
     std::cout<<" Arriving at your destination "<<std::endl;
     
}
    print_directions=false;
}

//if help button is clicked, then display the help menu1
void show_help(){
    //If the subway button is clicked
    if(clicked_help){
        
            t_bound_box box= get_visible_world();
            auto x= box.left();
            auto y=box.top();
                  
                        
            draw_surface(load_png_from_file("libstreetmap/resources/help_button.png"),x ,y);
        
    
       
    }
    return;
}
//if the clear button is clicked, then clear all slections
void show_clear(){
    //If the subway button is clicked
    if(clicked_clear==true){
    intersection_ids.clear();
    clicked_subway = false;
    print_where_clicked=false;
    clicked_get_directions=false;
    clicked_legend=false;
    clicked_help=false;
    clicked_clear=false;
    has_clicked = false;
    clicked_intersection=false;
    find_intersections = false;
    found_start_end_intersections=false;
    intersection_start_ids.clear();
    intersection_end_ids.clear();
    count_num_int_clicked=0;
     get_poi_dir=false;
    highlight_int=false;
    clicked_enter_poi = false; 
    highlight_poi=false;
    highlight_feature=false;
    show_poi_path=false;
    
    draw_screen();     
       
    }
    return;
}
/*******************************************************************************/

//Gets all the data from API for global vectors
void get_data(){
    
    maximum_latitude = getIntersectionPosition(0).lat();
    minimum_latitude = maximum_latitude;
    maximum_longitude = getIntersectionPosition(0).lon();
    minimum_longitude = maximum_longitude;
    
   
    
//    Get minimum and maximum data for intersections
 //   Store intersection data
   intersections.resize(getNumberOfIntersections());
    for(unsigned id = 0; id < getNumberOfIntersections(); ++id){
        intersections[id].position1 = getIntersectionPosition(id);
        intersections[id].name = getIntersectionName(id);
        
        maximum_latitude = std::max(maximum_latitude, 
                intersections[id].position1.lat());
        maximum_longitude = std::max(maximum_longitude, 
                intersections[id].position1.lon());
        
        minimum_latitude = std::min(minimum_latitude, 
                intersections[id].position1.lat());
        minimum_longitude = std::min(minimum_longitude, 
                intersections[id].position1.lon());
        
    }
    
//    Get minimum and maximum data for point of intersections
 //   Store poi data
    poi.resize(getNumberOfPointsOfInterest());
    std::vector<unsigned> poi_id_vec;
    for(unsigned id = 0; id < getNumberOfPointsOfInterest(); ++id){
        
        poi[id].position2 = getPointOfInterestPosition(id);
        poi[id].name = getPointOfInterestName(id);
        
        maximum_latitude = std::max(maximum_latitude, 
                poi[id].position2.lat());
        maximum_longitude = std::max(maximum_longitude, 
                poi[id].position2.lon());
        
        minimum_latitude = std::min(minimum_latitude, 
                poi[id].position2.lat());
        minimum_longitude = std::min(minimum_longitude, 
                poi[id].position2.lon());
        
      
        //build an unordered map to find poi from name     
            //If the name already exists add the poi Id in the vector
        auto search = poi_from_name.find( poi[id].name);

        if(search != poi_from_name.end()){
             poi_from_name[poi[id].name].push_back(id);
        }
        else{
            poi_id_vec.push_back(id);
            poi_from_name.emplace(poi[id].name, poi_id_vec);
        }

         poi_id_vec.clear();      
         poi_id_vec.shrink_to_fit(); 
        
        
    }
//    Get minimum and maximum position for features
  //  Store feature data
    features.resize(getNumberOfFeatures());
    for(unsigned id=0;id<getNumberOfFeatures();++id){
        features[id].name=getFeatureName(id);
        features[id].type=getFeatureType(id);
        features[id].osm=getFeatureOSMID(id);
        
        unsigned i;
        for(i = 0; i < getFeaturePointCount(id); ++i){
            LatLon pos = getFeaturePoint(id, i);
                      
            features[id].position3.push_back(pos);
            
            
            featureRTree.insert(std::make_pair( 
                    RTreeLatLon(getFeaturePoint(id, i).lat(), 
                    getFeaturePoint(id, i).lon()), feature_id_rtree));
            feature_id_rtree++;
            
  
            maximum_latitude = std::max(maximum_latitude, 
                    pos.lat());
            maximum_longitude = std::max(maximum_longitude, 
                    pos.lon());
            minimum_latitude = std::min(minimum_latitude, 
                    pos.lat());
            minimum_longitude = std::min(minimum_longitude, 
                    pos.lon());
            
        }
        if(getFeaturePoint(id,0).lat()==getFeaturePoint(id,i-1).lat()&& 
              getFeaturePoint(id,0).lon()==getFeaturePoint(id,i-1).lon()  ){
            features[id].closed=true;
        }
        else
            features[id].closed=false;
        
    }
    
   map_graph.all_intersections.resize(getNumberOfIntersections());
   double maximum_speed=0.0;
   double minimum_speed=0.0;
    //street segment
    streets.resize(getNumberOfStreetSegments());
    for(unsigned id = 0; id < getNumberOfStreetSegments(); ++id){
        
        auto ss_info = getStreetSegmentInfo(id);
        streets[id].num_curve = ss_info.curvePointCount;
        streets[id].oneway = ss_info.oneWay;
        streets[id].name = getStreetName(ss_info.streetID);
        streets[id].speed_limit = ss_info.speedLimit;
        streets[id].streetid = ss_info.streetID;
        
        if(id == 0 || maximum_speed < streets[id].speed_limit){
            maximum_speed = streets[id].speed_limit;
        }
        if(id==0||minimum_speed>streets[id].speed_limit)
            minimum_speed=streets[id].speed_limit;
        auto int_pos = getIntersectionPosition(ss_info.from);
        streets[id].position4.push_back(int_pos);
        
        auto ncurve = ss_info.curvePointCount;
        //For curve points
        if(ncurve > 0){             
            for(unsigned n = 0; n < ncurve; ++n){
                int_pos = getStreetSegmentCurvePoint(id, n);
                streets[id].position4.push_back(int_pos);
            }
        }
        //Add the last intersection
        int_pos = getIntersectionPosition(ss_info.to);

        streets[id].position4.push_back(int_pos);
        
        
        //**********Graph****************
        
        
        double travel_time = find_street_segment_travel_time(id);
      
        unsigned position1 = ss_info.from;
        
        unsigned position2 = ss_info.to;
          
        
        //Create the edges of the graph
        Edges new_street_edge(id,travel_time,position1,position2); 
        
        
         
        //Add all the vertex and edges to the graph
        //For one way add the edge only from source to destination       
        if(!streets[id].oneway){
            
            map_graph.all_intersections[position1].intersection_id=position1;
            //Add the edges pointing to both directions
            new_street_edge.directed=false;
            map_graph.addEdge(new_street_edge);
            //Change destination to from and vice versa
            map_graph.all_intersections[position2].intersection_id=position2;
            Edges new_street_edge1(id,travel_time,position2,position1); 
            map_graph.addEdge(new_street_edge1);
                  
        }
        else{
            //Add only from to to
            map_graph.all_intersections[position1].intersection_id=position1;
            new_street_edge.directed=true;
            map_graph.addEdge(new_street_edge);
            
        }
        
        
      
        
    }
    
    average_latitude = DEG_TO_RAD * (maximum_latitude + 
            minimum_latitude) * 0.5;
    minimum_speed=1000.0/36000.0*minimum_speed;
    maximum_speed=1000.0/3600.0*maximum_speed;
    map_graph.max_speed=maximum_speed;
    
    load_angle();
}
//determines the angle of each street segment
void load_angle(){
    double angle;
    const double RIGHT_ANGLE = 90.0;
    const double STRAIGHT_ANGLE = 180.0;
    for(unsigned id = 0; id < streets.size(); ++id){
        auto ncurvepoints = streets[id].num_curve;
        auto x1=longitude_to_cartesian(streets[id].position4[0].lon());
        auto x2=longitude_to_cartesian(streets[id].position4[ncurvepoints + 1].lon());
         
        auto y1=latitude_to_cartesian(streets[id].position4[0].lat());
        auto y2=latitude_to_cartesian(streets[id].position4[ncurvepoints + 1].lat());
         
        double delta_y = y2 - y1;
        double delta_x = x2 - x1;
        
        if (delta_x == 0){
            angle = RIGHT_ANGLE;
        }
        else if (delta_y == 0){
            angle = STRAIGHT_ANGLE;
        }
        else{
            angle = atan(delta_y / delta_x) / DEG_TO_RAD;
        }
        
        streets[id].angle = angle;
     }
    
    
    for(unsigned i = 0; i < intersections.size(); i++){
//        char* my_string = intersections[i].name;
//        intersection_names.push_back(my_string);
    }
}

//Returns the point of interest id from name
std::vector<unsigned> find_poi_from_name(std::string poi_name) {
//If the poi name is not found returns an empty vector
    if(poi_from_name.find(poi_name) == poi_from_name.end())
        return {};
    return poi_from_name[poi_name];
    
}

//Return poi data from poi ids
poi_data get_poi_data(unsigned poi_id){
    return poi[poi_id];
}
// highlights path from intersection to intersection when 
// directions are being shown 
void highlight_path(){
    
    if(found_start_end_intersections){
        auto distance=0;
        //algorithm adapted from 
        //https://math.stackexchange.com/questions/274712/calculate-on-which-side-of-a-straight-line-is-a-given-point-located
        //this function was used to determine if the next street segment and the current segment are on the left or on the right
        auto point_comparison_x=0;
        auto point_comparison_y=0;
        auto path=find_path_between_intersections(intersection_start_ids[0],intersection_end_ids[0],10.0);
        //
        for(unsigned i=0;i<path.size();++i){
            for(unsigned k=0;k<streets[path[i]].position4.size();++k){
            //path found highlighting         
             if(k>0){ 
                setlinewidth(8);
                setcolor(252, 133, 133);
                drawline(streets[path[i]].x_yposition[k-1].x,streets[path[i]].x_yposition[k-1].y,
                                streets[path[i]].x_yposition[k].x,streets[path[i]].x_yposition[k].y);
            
             }
             
        }
            // determinant equation gotten from
            //https://math.stackexchange.com/questions/274712/calculate-on-which-side-of-a-straight-line-is-a-given-point-located
         if(i==0 && print_directions_int){ 
             
             /*         print directions       */
                 std::cout<<std::endl;
                 std::cout<<" DIRECTIONS "<<std::endl;
                 std::cout<<std::endl;
                 std::cout<<"Head straight on " << streets[path[i]].name<<std::endl;
             
             }
            distance=distance +find_street_segment_length(path[i]);
             if(i>0 && print_directions_int){

               //if streets changed, then output left and right turn
               if((streets[path[i]].name)!=(streets[path[i-1]].name)){
                   std::cout<<"Go Straight for "<<distance<<" meters."<<std::endl;
                   distance=0.0;
                    auto ncurvepoints = streets[path[i-1]].num_curve; 
                    
                    auto point_1_x=longitude_to_cartesian(streets[path[i-1]].position4[ncurvepoints].lon());
                    auto point_2_x=longitude_to_cartesian(streets[path[i-1]].position4[ncurvepoints + 1].lon());
                    auto point_1_y=latitude_to_cartesian(streets[path[i-1]].position4[ncurvepoints].lat());
                    auto point_2_y=latitude_to_cartesian(streets[path[i-1]].position4[ncurvepoints + 1].lat());
                    
                    

                    point_comparison_x=longitude_to_cartesian(streets[path[i]].position4[1].lon()); 
                    point_comparison_y=latitude_to_cartesian(streets[path[i]].position4[1].lat());       
                    
                    auto determinant=(point_comparison_x-point_1_x)*(point_2_y-point_1_y)-(point_comparison_y-point_1_y)*(point_2_x-point_1_x);
                    
                    if(determinant>0){
                       
                        std::cout<<"Turn left onto "<<streets[path[i]].name << std::endl;
                    }
                    else{
                    std::cout<<"Turn right onto "<<streets[path[i]].name << std::endl;
                    }
                     
               }
               
             }
     }
    
     if(print_directions_int)
        std::cout<<"Arriving at your destination "<<std::endl;
    }
    print_directions_int=false;
}
