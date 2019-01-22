/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Graph.h
 * Author: achary39
 *
 * Created on March 15, 2018, 8:35 AM
 */

#ifndef GRAPH_H
#define GRAPH_H

#include "Edges.h"
#include "IntersectionNode.h"


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

class Graph {
    private:
        //unsigned intersection_id;
        
        
public:
    Graph();
    Graph(const Graph& orig);
    ~Graph();
    //Function to add edge to the graph
    void addEdge(Edges &new_edge);
    double max_speed;
    //Add intersections to the graph
    void addIntersection(IntersectionNode node);
    //Store all the intersection. The index are the intersection ids
    std::vector<IntersectionNode> all_intersections;
    //Stores the edges and contains travel time for street segments
 
   


};

#endif /* GRAPH_H */

