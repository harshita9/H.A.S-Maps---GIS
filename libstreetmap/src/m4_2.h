/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   m4_2.h
 * Author: achary39
 *
 * Created on March 29, 2018, 12:17 PM
 */

#ifndef M4_2_H
#define M4_2_H

#include "StreetsDatabaseAPI.h"
#include "LatLon.h"
#include "graphics.h"
#include "m1.h"
#include "m2.h"
#include "m2_2.h"
#include "m3.h"
#include "m3_2.h"
#include "m4.h"
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <cmath> 
#include <queue> 


#include "Edges.h"
#include "IntersectionNode.h"
#include "Graph.h"


void load_RTrees(const std::vector<DeliveryInfo>& deliveries, 
                                        const std::vector<unsigned>& depots);

std::vector<unsigned> two_opt_swap(const std::vector<unsigned>& paths, 
        unsigned i, unsigned k,  std::unordered_map<unsigned, std::vector<unsigned>> &drop_pick,
         std::unordered_map<unsigned, std::vector<unsigned>>&pick_drop);

std::vector<unsigned>  two_opt_algorithm(const std::vector<unsigned>& paths,  std::unordered_map<unsigned, std::vector<unsigned>> &drop_pick,
         std::unordered_map<unsigned, std::vector<unsigned>> &pick_drop);

double estimate_total_time(const std::vector<unsigned>&paths);


std::vector<unsigned> three_opt_swap(const std::vector<unsigned>& paths, 
        unsigned i, unsigned k, unsigned l);

std::vector<unsigned> three_opt_algorithm1(const std::vector<unsigned>& paths, int improve_limit
        , std::unordered_map<unsigned, std::vector<unsigned>>& drop_pick,  std::unordered_map<unsigned, std::vector<unsigned>> &pick_drop);
std::vector<unsigned> three_opt_algorithm(const std::vector<unsigned>& paths, int improve_limit);
std::vector<unsigned> two_opt_algorithm1(const std::vector<unsigned>& paths);
std::vector<unsigned> two_opt_swap1(const std::vector<unsigned>& paths, 
        unsigned i, unsigned k);

//void do_three_opt(std::vector pick_up, int improve, std::vector &deliveries);


#endif /* M4_2_H */

