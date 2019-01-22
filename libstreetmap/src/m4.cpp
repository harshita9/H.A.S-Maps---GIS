/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


#include "StreetsDatabaseAPI.h"
#include "LatLon.h"
#include "m1.h"
#include "m2.h"
#include "m2_2.h"
#include "m3.h"
#include "m4.h"
#include "m3_2.h"
#include "m4_2.h"
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <cmath> 
#include <queue> 
#include <thread> 
#include "RTreeLatLon.h"
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/register/point.hpp>
#include <boost/geometry/index/rtree.hpp>
#include "Edges.h"
#include "IntersectionNode.h"
#include "Graph.h"

//void do_three_opt(std::vector pick_up, int improve, std::vector &deliveries){
//    deliveries = three_opt_algorithm(pick_up, improve);
//}

// This routine takes in a vector of N deliveries (pickUp, dropOff
// intersection pairs), another vector of M intersections that
// are legal start and end points for the path (depots) and a turn 
// penalty in seconds (see m3.h for details on turn penalties). 
//
// The first vector 'deliveries' gives the delivery information: a set of 
// pickUp/dropOff pairs of intersection ids which specify the 
// deliveries to be made. A delivery can only be dropped-off after 
// the associated item has been picked-up. 
// 
// The second vector 'depots' gives the intersection
// ids of courier company depots containing trucks; you start at any
// one of these depots and end at any one of the depots.
//
// This routine returns a vector of street segment ids that form a
// path, where the first street segment id is connected to a depot
// intersection, and the last street segment id also connects to a
// depot intersection.  The path must traverse all the delivery
// intersections in an order that allows all deliveries to be made --
// i.e. a package won't be dropped off if you haven't picked it up
// yet.
//
// You can assume that N is always at least one, and M is always at
// least one (i.e. both input vectors are non-empty).
//
// It is legal for the same intersection to appear multiple times in
// the pickUp or dropOff list (e.g. you might have two deliveries with
// a pickUp intersection id of #50). The same intersection can also
// appear as both a pickUp location and a dropOff location.
//        
// If you have two pickUps to make at an intersection, 
// traversing the intersection once is sufficient
// to pick up both packages, and similarly one traversal of an 
// intersection is sufficient to drop off all the (already picked up) 
// packages that need to be dropped off at that intersection.
//
// Depots will never appear as pickUp or dropOff locations for deliveries.
//  
// If no path connecting all the delivery locations
// and a start and end depot exists, this routine must return an
// empty (size == 0) vector.
std::vector<unsigned> traveling_courier(const std::vector<DeliveryInfo>& deliveries, 
                                        const std::vector<unsigned>& depots, 
                                        const float turn_penalty){
    std::vector<unsigned> path;
    std::vector<unsigned> subpath;

    std::vector<unsigned> pick_up;
    std::vector<unsigned> drop_off;
    std::vector<unsigned> pick_ups;
    std::vector<unsigned> drop_offs;
    int improve = 0;
    
    std::unordered_map<unsigned, std::vector<unsigned>>drop_pick;
    std::unordered_map<unsigned, std::vector<unsigned>>pick_drop;

    unsigned current_delivery;

    
    unsigned chosen_depot=0;
    double min_time=0.0;
    unsigned second_depot=0;
    double min_second_time=0.0;
    for(unsigned n=0; n<depots.size(); n++)
    {
       double new_time=get_heuristic(depots[n], deliveries[0].pickUp);
       if(n==0 || new_time<min_time ){
           min_time=new_time;
           chosen_depot=depots[n];
       }
       else if(n==1|| new_time<min_second_time)
       {
           second_depot=depots[n];
           min_second_time=new_time;
       }
    }
   
    pick_up.push_back(chosen_depot);
    
    if (deliveries.size() > 100){
        improve = 1;
    }
    else{
        improve = 2;
    }
    
    pick_up.resize(deliveries.size());
    drop_off.resize(deliveries.size());
    for (unsigned m = 0; m < deliveries.size(); m++){

        pick_up.push_back(deliveries[m].pickUp);
        drop_off.push_back(deliveries[m].dropOff);
        if(drop_pick.find(deliveries[m].dropOff)!=drop_pick.end()){
            drop_pick[deliveries[m].dropOff].push_back(deliveries[m].pickUp);
        }
        else{
            std::vector<unsigned> pick;
            pick.push_back(deliveries[m].pickUp);
            drop_pick.emplace(deliveries[m].dropOff, pick);
        }
        
        if(pick_drop.find(deliveries[m].pickUp)!=pick_drop.end()){
            pick_drop[deliveries[m].pickUp].push_back(deliveries[m].dropOff);
        }
        else{
            std::vector<unsigned> drop;
            drop.push_back(deliveries[m].dropOff);
            pick_drop.emplace(deliveries[m].pickUp,drop);
        }
        
    }
    

    
    delete_duplicates(pick_up);
    delete_duplicates(drop_off);
    
    if(deliveries.size()>100)
        improve=1;
    else
        improve=2;
    
   std::vector<unsigned> pick_up1;
   std::vector<unsigned> pick_up2;
   std::vector<unsigned> drop_off1;
   std::vector<unsigned> drop_off2;
   
   if(deliveries.size()>2){
       
        pick_up1.insert(pick_up1.end(),pick_up.begin(),pick_up.begin()+(pick_up.size()-1)/2);
        pick_up2.insert(pick_up2.end(),pick_up.begin()+(pick_up.size()-1)/2,pick_up.end());
        
        pick_up1 =two_opt_algorithm1(pick_up1);
        pick_up2 =two_opt_algorithm1(pick_up2);
        pick_up1.insert(pick_up1.end(),pick_up2.begin(),pick_up2.end());
        pick_ups =three_opt_algorithm(pick_up1,improve);

        
       
        drop_off1.insert(drop_off1.end(),drop_off.begin(),drop_off.begin()+(drop_off.size()-1)/2);
        drop_off2.insert(drop_off2.end(),drop_off.begin()+(drop_off.size()-1)/2,drop_off.end());

        drop_off1 =two_opt_algorithm1(drop_off1);
        drop_off2 =two_opt_algorithm1(drop_off2);
        drop_off1.insert(drop_off1.end(),drop_off2.begin(),drop_off2.end());
        drop_offs =three_opt_algorithm(drop_off1,improve);
        
   }
   else if(deliveries.size()==2){
         pick_ups =three_opt_algorithm(pick_up,improve);
         drop_offs =three_opt_algorithm(drop_off,improve);
         
   }
   else{
         pick_ups =pick_up;
          drop_offs =drop_off;
   }
    
   
    
    pick_ups.insert(pick_ups.end(),drop_offs.begin(),drop_offs.end());
    pick_ups =two_opt_algorithm(pick_ups,drop_pick,pick_drop);
   

    
    
    subpath = find_path_between_intersections(pick_ups[0], 
                pick_ups[1], turn_penalty);
    
   unsigned size_d1=1;
    unsigned d1=1; 
    while(subpath.size()==0 && depots.size() > d1){
        subpath.clear();
        if(size_d1==1){
           current_delivery = second_depot;
           d1=0;
           size_d1=0;
        }
        else{
            current_delivery = depots[d1];
            d1++;
        }
       subpath = find_path_between_intersections(current_delivery, 
                pick_ups[1], turn_penalty);
    }
    
    if(subpath.size()==0 ){
        path.clear();
        return path;
    }
    
    path.insert(path.end(), subpath.begin(), subpath.end());
    subpath.clear();
    

    
    
   
    for( unsigned i=1; i<pick_ups.size()-1;i++){
         subpath = find_path_between_intersections(
           pick_ups[i],pick_ups[i+1], turn_penalty);
            path.insert(path.end(), subpath.begin(), subpath.end());
            subpath.clear();
    }
    
    

    subpath = a_star_poi(pick_ups[pick_ups.size()-1], depots, turn_penalty);

    path.insert(path.end(), subpath.begin(), subpath.end());
   
    
  
    return path;
}

std::vector<unsigned> two_opt_algorithm(const std::vector<unsigned>& paths,std::unordered_map<unsigned,
        std::vector<unsigned>>& drop_pick,  std::unordered_map<unsigned, std::vector<unsigned>> &pick_drop){
    
    int repeat=0;
    bool start=true;
    const int REPEAT_FACTOR=2;
    
    std::vector<unsigned> latest_path;
    std::vector<unsigned> new_path;
    latest_path=paths;
     double new_time=0.0;
     double best_time=0.0;
    while(repeat<REPEAT_FACTOR)
    {
        
        if(start){
            best_time= estimate_total_time(latest_path);
            start=false;
        }
        
        for(unsigned i=1;i<latest_path.size()-1;i++)
        {
            for(unsigned k=i+1;k<latest_path.size();k++)
            {
               new_path=two_opt_swap(latest_path,i,k, drop_pick,pick_drop);
               new_time=estimate_total_time(new_path);
                
                if(new_time<best_time)
                {
                    repeat=0;
                    latest_path=new_path;
                    best_time=new_time;
                    start=true;
                }
            }
        }
        repeat++;        
    }
    
    return latest_path;
}

//Used pseduocode of wikipedia: https://en.wikipedia.org/wiki/2-opt
//Funtions that swaps and checks if it is a legal swap for pick up and drop off combined
std::vector<unsigned> two_opt_swap(const std::vector<unsigned>& paths, 
        unsigned i, unsigned k, std::unordered_map<unsigned, std::vector<unsigned>> &drop_pick,
         std::unordered_map<unsigned, std::vector<unsigned>> &pick_drop){
    std::vector<unsigned> new_path;
    new_path.resize(paths.size());
    bool p_uped=true;
    for(unsigned l=0;l<=i-1;l++)
    {
        new_path[l]=paths[l];
    }
    
    unsigned n=0;
    unsigned o=k;
    for(std::vector<unsigned>::iterator l=new_path.begin()+k;l!=(new_path.begin()+((k+i)/2));l--)
    {
        //Check if the it is a drop off intersection and if it has already been picked up
        if(drop_pick.find(paths[o])!=drop_pick.end()){
            std::vector<unsigned> picks=drop_pick[paths[o]];
           
            for(unsigned m=0; m< picks.size();++m){
                auto found=std::find(paths.begin(),paths.begin()+i+n,picks[m]);
                        
                if(found==(paths.begin()+i+n)){
                    p_uped=false;
                    break;
                 }

            }
        }
         if(drop_pick.find(paths[i+n])!=drop_pick.end()){
            std::vector<unsigned> picks2=drop_pick[paths[i+n]];
            for(unsigned m=0; m< picks2.size();++m){
              auto found2=std::find(paths.begin(),paths.begin()+i+n,picks2[m]);
        
                if(found2==(paths.begin()+i+n)){
                    p_uped=false;
                    break;
                }   
                
            }
         }
        
        //if it a pick up intersection then check if the drop of intersection lies in 
        //between the two switching ids
         if(pick_drop.find(paths[i+n])!=pick_drop.end()){
            std::vector<unsigned> drops2=pick_drop[paths[i+n]];
            for(unsigned m=0; m< drops2.size();++m){
                
              auto found2=std::find(paths.begin()+i+n+1,paths.begin()+o, drops2[m]);
        
                if(found2!=(paths.begin()+o)){
                    p_uped=false;
                    break;
                }   
                
            }
         }
        
            
        if(!p_uped){
            new_path[o]=paths[o];
            new_path[i+n]=paths[i+n];
            p_uped=true;
        }
        else{
            new_path[i+n]=paths[o];
            new_path[o]=paths[i+n];
        }
      
        n++;
        o--;
        
    }
    
    if((i+k)%2==0){
        new_path[(i+k)/2]=paths[(i+k)/2];
    }
    
    
    for(unsigned l=k+1;l<paths.size();l++)
    {
        new_path[l]=paths[l];
    }
    
    return new_path;
    
}

double estimate_total_time(const std::vector<unsigned>&paths){
    
    double time=0.0;
    for(unsigned i=0;i<paths.size()-1;i++){
        time+=get_heuristic(paths[i],paths[i+1]);
       
    }
    
    return time;
    
}



std::vector<unsigned> three_opt_algorithm(const std::vector<unsigned>& paths, int improve_limit){
    
    int repeat=0;
    bool start=true;
    
    std::vector<unsigned> latest_path;
    std::vector<unsigned> new_path;
    latest_path=paths;
    
    double new_time=0.0;
    double best_time=0.0;
    while(repeat<improve_limit)
    {
        if(start){
            best_time = estimate_total_time(latest_path);
            start = false;
        }
        if (latest_path.size() > 1) {
            for(unsigned i = 1; i < latest_path.size() - 2; i++)
            {
                for(unsigned k = i + 1; k < latest_path.size() - 1; k++)
                {
                    for(unsigned l = k + 1; l < latest_path.size(); l++)
                    {
                      
                        new_path = two_opt_swap1(latest_path, i, l);
                        new_path = two_opt_swap1(new_path, k, l);
                        new_time = estimate_total_time(new_path);

                        if(new_time < best_time)
                        {
                            repeat = 0;
                            latest_path = new_path;
                            best_time = new_time;
                            start = true;
                        }
                    }
                }
            }
        }
        repeat++;        
    }
    
    return latest_path;
}

     
 //Used pseduocode of wikipedia: https://en.wikipedia.org/wiki/2-opt    
std::vector<unsigned> two_opt_swap1(const std::vector<unsigned>& paths, 
        unsigned i, unsigned k){
    
     std::vector<unsigned> new_path;

 
    for(unsigned m=0;m<=i-1;m++)
    {
        new_path.push_back(paths[m]);
    }
    
    
    for(unsigned m=k;m>=i;m--){
        new_path.push_back(paths[m]);
    }
    
        
    
    
    for(unsigned m=k+1;m<paths.size();m++)
    {
        new_path.push_back(paths[m]);
    }
    
    return new_path;
    
}

//Used pseduocode of wikipedia: https://en.wikipedia.org/wiki/2-opt

std::vector<unsigned> two_opt_algorithm1(const std::vector<unsigned>& paths){
    
    int repeat=0;
    bool start=true;
    
    std::vector<unsigned> latest_path;
    std::vector<unsigned> new_path;
    latest_path=paths;
     double new_time=0.0;
     double best_time=0.0;
    while(repeat<2)
    {
        
        if(start){
            best_time= estimate_total_time(latest_path);
            start=false;
        }
        
        for(unsigned i=1;i<latest_path.size()-1;i++)
        {
            for(unsigned k=i+1;k<latest_path.size();k++)
            {
               
               new_path=two_opt_swap1(latest_path,i,k);
               new_time=estimate_total_time(new_path);
                
                if(new_time<best_time)
                {
                    repeat=0;
                    latest_path=new_path;
                    best_time=new_time;
                    start=true;
                }
            }
        }
        repeat++;        
    }
    
    return latest_path;
}
     
     
std::vector<unsigned> three_opt_swap(const std::vector<unsigned>& paths, 
        unsigned i, unsigned k, unsigned l){
    
    std::vector<unsigned> new_path;
    //new_path.resize(paths.size());
 
    for(unsigned m = 0; m <= i - 1; m++)
    {
        new_path.push_back(paths[m]);
    }
    
    
    for(unsigned m = k; m >= i; m--){
        new_path.push_back(paths[m]);
    }
    
    for(unsigned m = l; m > k; m--){
        new_path.push_back(paths[m]);
    }
    
    for(unsigned m = l + 1; m < paths.size(); m++)
    {
        new_path.push_back(paths[m]);
    }
    
    return new_path;
    
}
