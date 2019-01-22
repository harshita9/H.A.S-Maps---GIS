/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Edges.cpp
 * Author: achary39
 * 
 * Created on March 15, 2018, 8:53 AM
 */

#include "Edges.h"

Edges::Edges(unsigned ss_id, double tra_time,unsigned fromid,unsigned toid) {
    street_ss_id=ss_id;
    travel_time=tra_time;
    from_id=fromid;
    to_id = toid;
    directed=false;
    open=false;
    closed=false;
  
    
}
Edges::Edges(){
    
    
}
//Set the from and to intersection node
void Edges::set_from(unsigned from){
    from_id=from;
    return;
}
void Edges::set_to(unsigned to){
    to_id=to;
    return;
}

//get the from and to intersection node
unsigned Edges::get_from()const{
    return from_id;
}
unsigned Edges::get_to()const{
    return to_id;
}


Edges::Edges(const Edges& orig) {
    street_ss_id=orig.street_ss_id;
    travel_time=orig.travel_time;
    from_id=orig.from_id;
    to_id=orig.to_id;
    directed=orig.directed;
    open=orig.open;
    closed=orig.closed;
  
}

Edges& Edges::operator=(const Edges& orig) {
    street_ss_id=orig.street_ss_id;
    travel_time=orig.travel_time;
    from_id=orig.from_id;
    to_id=orig.to_id;
    directed=orig.directed;
   open=orig.open;
    closed=orig.closed;
  
    
    return *this;
}

Edges::~Edges() {
}

