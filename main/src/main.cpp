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
#include <iostream>
#include <string>

#include "m1.h"
#include "m2.h"
#include "m2_2.h"
#include "m3.h"
#include "m4.h"
#include "m4_2.h"


#include "graphics.h"

//Program exit codes
constexpr int SUCCESS_EXIT_CODE = 0;        //Everyting went OK
constexpr int ERROR_EXIT_CODE = 1;          //An error occured
constexpr int BAD_ARGUMENTS_EXIT_CODE = 2;  //Invalid command-line usage

//The default map to load if none is specified
std::string default_map_path = "/cad2/ece297s/public/maps/toronto_canada.streets.bin";
std::string default_map_path_OSM = "/cad2/ece297s/public/maps/toronto_canada.osm.bin";

int main(int argc, char** argv) {
    
    std::string map_path;
    std::string map_path_osm;
    if(argc == 1) {
        //Use a default map
        map_path = default_map_path;
        map_path_osm=default_map_path_OSM;//Add osm map
    } 
  
    else if (argc == 3) {
        //Get the map from the command line
        map_path = argv[1];
         map_path_osm=argv[2];
    } else {
        //Invalid arguments
        std::cerr << "Usage: " << argv[0] << " [map_file_path]\n";
        std::cerr << "  If no map_file_path is provided a default map is loaded.\n";
        return BAD_ARGUMENTS_EXIT_CODE;
    }
    std::cout<< "Your H.A.S. Maps is loading....Please wait..."<<std::endl;
    //Load the map and related data structures
    bool load_success = load_map(map_path);
    bool load_OSM=loadOSMDatabaseBIN(map_path_osm);
    
    
    if(!load_success) {
        std::cerr << "Failed to load map '" << map_path << "'\n";
        return ERROR_EXIT_CODE;
    }
    if(!load_OSM){
        std::cerr << "Failed to load map '" << map_path_osm << "'\n";
        return ERROR_EXIT_CODE;
    }

    std::cout << "Successfully loaded map of Toronto, Canada" << "'\n";
      
    //You can now do something with the map data
   draw_map(); 
    
    

    std::cout << "Closing map\n";
   
    delete_all();
    close_map();
    closeOSMDatabase();

    return SUCCESS_EXIT_CODE;
}
