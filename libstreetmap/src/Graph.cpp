/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Graph.cpp
 * Author: achary39
 * 
 * Created on March 15, 2018, 8:35 AM
 */

#include "Graph.h"

Graph::Graph() {
    
    
    
}

Graph::Graph(const Graph& orig) {
    (void) orig;
}

//Function to add edge to the graph
void Graph::addEdge(Edges &new_edge){
 
    all_intersections[new_edge.get_from()].adjacent_intersection_ids.push_back(new_edge);
}


Graph::~Graph() {
}

