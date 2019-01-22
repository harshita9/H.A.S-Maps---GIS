/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   m3_2.h
 * Author: tupilisa
 *
 * Created on March 10, 2018, 1:27 PM
 */

#ifndef M3_2_H
#define M3_2_H

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

#include "IntersectionNode.h"
#include "Edges.h"
#include "Graph.h"


//Calculates the fastest time from start intersection to end intersection using a star algorithm
//If no path is found it returns an empty vector otherwise it returns street segments in order
std::vector<unsigned> a_star(unsigned start_id, unsigned end_id, const double turn_penalty);

//This function reconstructs the intersection ids to segment id to find the path
std::vector<unsigned> reconstruct_path( std::vector<unsigned> &came_from, double turn_penalty);

//For the a star calculate estimated time from the successor_id to end id
//It uses the max speed of the graph as the speed and uses diagonal distance for time
double get_heuristic(unsigned successor_id, unsigned end_id);

//Finds the intersection id with f_min as its travel time score
unsigned find_inter(std::unordered_map<unsigned,double> &f_Score, double f_min);

//Constructs a path with intersection ids by going backward from end(current) id 
std::vector<unsigned> construct_path(std::unordered_map<unsigned,unsigned> &camefrom,unsigned current);
//Calculates the fastest time from start intersection to end intersection using a star algorithm
//If no path is found it returns an empty vector otherwise it returns street segments in order
std::vector<unsigned> a_star_poi(unsigned start_id, std::vector<unsigned> end_id,const double turn_penalty);




#endif /* M3_2_H */

