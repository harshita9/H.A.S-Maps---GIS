/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   IntersectionNode.h
 * Author: achary39
 *
 * Created on March 14, 2018, 6:23 PM
 */

#ifndef INTERSECTIONNODE_H
#define INTERSECTIONNODE_H

#include <utility>
#include <vector>
#include "Edges.h"

class IntersectionNode {
public:
    IntersectionNode();
    IntersectionNode(unsigned in_id);
    IntersectionNode(const IntersectionNode& orig);
     ~IntersectionNode();
    //IntersectionNode& operator=(const IntersectionNode& orig);
   
     unsigned intersection_id;
    //List of adjacent intersection ids connected to the current intersection
    std::vector<Edges>adjacent_intersection_ids;
    

};

#endif /* INTERSECTIONNODE_H */

