/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Edges.h
 * Author: achary39
 *
 * Created on March 15, 2018, 8:53 AM
 */

#ifndef EDGES_H
#define EDGES_H



class Edges {
    private:
       //Store the two intersection of the streets connecting it
    unsigned from_id;
    unsigned to_id;
   
    
public:
    Edges();
    Edges(unsigned ss_id, double tra_time,unsigned fromid,unsigned toid);
    Edges(const Edges& orig);
    ~Edges();
    
    Edges& operator=(const Edges& orig);
    
     bool directed;//Checks if the edge is only one way or two way
    //
    //Set the from and to intersection node
    void set_from(unsigned from) ;
    void set_to(unsigned to) ;
    
    //get the from and to intersection node
    unsigned get_from()const;
    unsigned get_to()const;

    unsigned street_ss_id;//Store the street id 
    double travel_time;//Store the time it takes to travel in the street segment
 
    bool open;
    bool closed;
    //unsigned street_id;

};

#endif /* EDGES_H */

