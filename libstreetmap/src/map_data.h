/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   map_data.h
 * Author: tupilisa
 *
 * Created on February 5, 2018, 10:34 AM
 */

#ifndef MAP_DATA_H
#define MAP_DATA_H

#include <vector>
#include <unordered_map>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/register/point.hpp>
#include <boost/geometry/index/rtree.hpp>

namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;

class map_data {
public:
    
    //Global Vectors 
    //Stores street segments for given intersection id
    std::vector<std::vector<unsigned>>intersection_Street_Segments;
    //Stores street segment for given street id
    std::vector<std::vector<unsigned>>street_street_segments;
    //Stores adjacent intersections for given intersection id
    std::vector<std::vector<unsigned>>adjacent_intersections;
    //Stores all intersections in a street
    std::vector<std::vector<unsigned>>street_intersection;
    //Stores distances of street segment
    std::vector<double> distance_street_segment;
    //Stores travel time for street segment
    std::vector<double> distance_travel_time;

    //Global Unordered Map  
    //Stores street ids for given street name
    std::unordered_map<std::string, std::vector<unsigned>> street_ids_from_name;

};

extern map_data my_map;

#endif /* MAP_DATA_H */

