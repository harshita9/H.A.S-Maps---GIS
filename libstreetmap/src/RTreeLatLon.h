/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   RTreeLatLon.h
 * Author: tupilisa
 *
 * Created on February 5, 2018, 11:31 AM
 */

#ifndef RTREELATLON_H
#define RTREELATLON_H

#include "LatLon.h"

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/register/point.hpp>
#include <boost/geometry/index/rtree.hpp>

namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;

//Structure for latitudes and longitudes that will be stored in the two R-trees 
struct RTreeLatLon {
    RTreeLatLon() {
    }
    RTreeLatLon(double lat, double lon)
                    : latlon(lat, lon){
    }
    //Set latitude and longitude using LatLon 
    void set_longitude(double lon) {
        latlon = LatLon(latlon.lat(), lon);
        return;
    }
    void set_latitude(double lat) {
        latlon = LatLon(lat, latlon.lon());
        return;
    }
    //Get latitude and longitude
    double get_longitude() const { 
        return latlon.lon(); 
    }
    double get_latitude() const { 
        return latlon.lat(); 
    }

    LatLon latlon;
};

BOOST_GEOMETRY_REGISTER_POINT_2D_GET_SET(RTreeLatLon, 
        double, bg::cs::spherical_equatorial<bg::degree>, get_longitude, 
        get_latitude, set_longitude, set_latitude)

#endif /* RTREELATLON_H */

