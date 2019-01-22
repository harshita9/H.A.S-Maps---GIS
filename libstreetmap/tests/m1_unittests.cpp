/////* 
//// * Copyright 2018 University of Toronto
//// *
//// * Permission is hereby granted, to use this software and associated 
//// * documentation files (the "Software") in course work at the University 
//// * of Toronto, or for personal use. Other uses are prohibited, in 
//// * particular the distribution of the Software either publicly or to third 
//// * parties.
//// *
//// * The above copyright notice and this permission notice shall be included in 
//// * all copies or substantial portions of the Software.
//// *
//// * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
//// * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
//// * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
//// * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
//// * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//// * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//// * SOFTWARE.
//// */
//
//
//#include <algorithm>
//#include <set>
//#include <unittest++/UnitTest++.h>
//#include "m1.h"
//#include "unit_test_util.h"
//#include <random>
//#include "StreetsDatabaseAPI.h"
//
////using ece297test::relative_error;
//
//
//SUITE(spatial_queries_public_saint_helena) {
////    struct MapFixture {
////        MapFixture() {
////            //Load the map
////            load_map("/cad2/ece297s/public/maps/london_england.streets.bin");
////            
////            //Initialize random number generators
////            rng = std::minstd_rand(3);
////            rand_street = std::uniform_int_distribution<unsigned>(1, getNumberOfStreets()-1);
////          
////            
////        }
////
////        ~MapFixture() {
////            //Clean-up
////            close_map();
////        }
////        std::minstd_rand rng;
////        std::uniform_int_distribution<unsigned> rand_street;
////    };
//////MADE TO FAIL
////    TEST_FIXTURE(MapFixture, MadeToFail) {
////        
////        
////        bool pass=false;
////        
////        auto actual=find_street_ids_from_name("abcdefghijklmnop");//street does not exist
////        if(actual.size()==0)
////            pass=false;
////        
////        else{
////            pass=true;
////        }
////    
////     CHECK_EQUAL(true,pass);
////        
////    } 
//    //If the street name does not exist
//    TEST_FIXTURE(MapFixture, StreetDNE) {
//        bool pass=false;
//        auto actual=find_street_ids_from_name("abcdefghijklmnop");//street does not exist
//        std::vector<unsigned> expected;
//        
//        //if(actual.size()==0)//should be empty length vector
//            //pass=true;
//        
//        //else{
//            //pass=false;
//        //}
//        std::sort(actual.begin(), actual.end());
//        if(std::equal(expected.begin(),expected.end(),actual.begin())){
//            pass=true;
//        }
//        else{
//            pass=false;
//        }
//     CHECK_EQUAL(true,pass);
//        
//    } 
//    //Check if the size is zero for empty vector
//    TEST_FIXTURE(MapFixture, StreetSize) {
//        bool pass=false;
//        auto actual=find_street_ids_from_name("abcdefghijklmnop");//street does not exist
//        std::vector<unsigned> expected;
//        
//        if(actual.size()==0)//should be empty length vector
//            pass=true;
//        
//        else{
//            pass=false;
//        }
//        
//     CHECK_EQUAL(true,pass);
//        
//    } 
//    
//    
//    
//    //Check for any duplicates street ids
//    TEST_FIXTURE(MapFixture, Duplicates) {
//        
//        unsigned index=rand()%getNumberOfStreets(); //checks for duplicates
//        std::string name=getStreetName(index);
//        auto actual=find_street_ids_from_name(name);
//        bool pass=false;
//        
//        std::sort(actual.begin(),actual.end());
//        auto last=std::unique(actual.begin(),actual.end());
//
//        if(last!=actual.end()){
//            pass=false;
//        }
//        else{
//            pass=true;
//        }
//    
//     CHECK_EQUAL(true,pass);
//        
//    } 
//    
//    //Functionality Check
//    TEST_FIXTURE(MapFixture, Random) {
//         
//        //Functionality Test
//        
//        unsigned index=rand() % getNumberOfStreets();  //random index
//        std::string name=getStreetName(index);
//        auto actual=find_street_ids_from_name(name);
//        bool pass=false;
//        std::vector<unsigned> expected;
//        for(unsigned i=0; i<getNumberOfStreets(); i++){
//            if((getStreetName(i))==name){
//                expected.push_back(i);}
//        }
//        actual=find_street_ids_from_name(name);
//        std::sort(actual.begin(), actual.end());
//        std::sort(expected.begin(), expected.end());
//        if(std::equal(expected.begin(),expected.end(),actual.begin())){
//            pass=true;
//        }
//        else{
//            pass=false;
//        }
//    
//     CHECK_EQUAL(true,pass);
//        
//    } 
//    TEST_FIXTURE(MapFixture, Random1) {
//         
//        //Functionality Test
//        
//        unsigned index=rand() % getNumberOfStreets();  //random index
//        std::string name=getStreetName(index);
//        auto actual=find_street_ids_from_name(name);
//        bool pass=false;
//        std::vector<unsigned> expected;
//        for(unsigned i=0; i<getNumberOfStreets(); i++){
//            if((getStreetName(i))==name){
//                expected.push_back(i);}
//        }
//        
//        actual=find_street_ids_from_name(name);
//        std::sort(actual.begin(), actual.end());
//        std::sort(expected.begin(), expected.end());
//        if(std::equal(expected.begin(),expected.end(),actual.begin())){
//            pass=true;
//        }
//        else{
//            pass=false;
//        }
//    
//     CHECK_EQUAL(true,pass);
//        
//    }
//    TEST_FIXTURE(MapFixture, Random2) {
//       
//        //Functionality Test
//        
//        unsigned index=rand() % getNumberOfStreets();  //random index
//        std::string name=getStreetName(index);
//        auto actual=find_street_ids_from_name(name);
//        bool pass=false;
//        std::vector<unsigned> expected;
//        for(unsigned i=0; i<getNumberOfStreets(); i++){
//            if((getStreetName(i))==name){
//                expected.push_back(i);}
//        }
//        actual=find_street_ids_from_name(name);
//        std::sort(actual.begin(), actual.end());
//        std::sort(expected.begin(), expected.end());
//        if(std::equal(expected.begin(),expected.end(),actual.begin())){
//            pass=true;
//        }
//        else{
//            pass=false;
//        }
//    
//     CHECK_EQUAL(true,pass);
//        
//    }
//    TEST_FIXTURE(MapFixture, Random3) {
//         
//        //Functionality Test
//        
//        unsigned index=rand() % getNumberOfStreets();  //random index
//        std::string name=getStreetName(index);
//        auto actual=find_street_ids_from_name(name);
//        bool pass=false;
//        std::vector<unsigned> expected;
//        for(unsigned i=0; i<getNumberOfStreets(); i++){
//            if((getStreetName(i))==name){
//                expected.push_back(i);}
//        }
//        actual=find_street_ids_from_name(name);
//        std::sort(actual.begin(), actual.end());
//        std::sort(expected.begin(), expected.end());
//        if(std::equal(expected.begin(),expected.end(),actual.begin())){
//            pass=true;
//        }
//        else{
//            pass=false;
//        }
//     CHECK_EQUAL(true,pass);
//    }
//    TEST_FIXTURE(MapFixture, Random4) {
//        //Functionality Test
//        
//        unsigned index=rand() % getNumberOfStreets();  //random index
//        std::string name=getStreetName(index);
//        auto actual=find_street_ids_from_name(name);
//        bool pass=false;
//        std::vector<unsigned> expected;
//        for(unsigned i=0; i<getNumberOfStreets(); i++){
//            if((getStreetName(i))==name){
//                expected.push_back(i);}
//        }
//        actual=find_street_ids_from_name(name);
//        std::sort(actual.begin(), actual.end());
//        std::sort(expected.begin(), expected.end());
//        if(std::equal(expected.begin(),expected.end(),actual.begin())){
//            pass=true;
//        }
//        else{
//            pass=false;
//        }
//     CHECK_EQUAL(true,pass);
//        
//    }
//    TEST_FIXTURE(MapFixture, Random5) {
//         //Functionality Test
//        
//        unsigned index=rand() % getNumberOfStreets();  //random index
//        std::string name=getStreetName(index);
//        auto actual=find_street_ids_from_name(name);
//        bool pass=false;
//        std::vector<unsigned> expected;
//        for(unsigned i=0; i<getNumberOfStreets(); i++){
//            if((getStreetName(i))==name){
//                expected.push_back(i);}
//        }
//        actual=find_street_ids_from_name(name);
//        std::sort(actual.begin(), actual.end());
//        std::sort(expected.begin(), expected.end());
//        if(std::equal(expected.begin(),expected.end(),actual.begin())){
//            pass=true;
//        }
//        else{
//            pass=false;
//        }
//     CHECK_EQUAL(true,pass);
//             
//    }
//    TEST_FIXTURE(MapFixture, Random6) {
//         //Functionality Test
//        
//        unsigned index=rand() % getNumberOfStreets();  //random index
//        std::string name=getStreetName(index);
//        auto actual=find_street_ids_from_name(name);
//        bool pass=false;
//        std::vector<unsigned> expected;
//        for(unsigned i=0; i<getNumberOfStreets(); i++){
//            if((getStreetName(i))==name){
//                expected.push_back(i);}
//        }
//        actual=find_street_ids_from_name(name);
//        std::sort(actual.begin(), actual.end());
//        std::sort(expected.begin(), expected.end());
//        if(std::equal(expected.begin(),expected.end(),actual.begin())){
//            pass=true;
//        }
//        else{
//            pass=false;
//        }
//     CHECK_EQUAL(true,pass);
//             
//    }
//    TEST_FIXTURE(MapFixture, Random7) {
//         //Functionality Test
//        
//        unsigned index=rand() % getNumberOfStreets();  //random index
//        std::string name=getStreetName(index);
//        auto actual=find_street_ids_from_name(name);
//        bool pass=false;
//        std::vector<unsigned> expected;
//        for(unsigned i=0; i<getNumberOfStreets(); i++){
//            if((getStreetName(i))==name){
//                expected.push_back(i);}
//        }
//        actual=find_street_ids_from_name(name);
//        std::sort(actual.begin(), actual.end());
//        std::sort(expected.begin(), expected.end());
//        if(std::equal(expected.begin(),expected.end(),actual.begin())){
//            pass=true;
//        }
//        else{
//            pass=false;
//        }
//     CHECK_EQUAL(true,pass);
//             
//    }
//    TEST_FIXTURE(MapFixture, Random8) {
//         //Functionality Test
//        
//        unsigned index=rand() % getNumberOfStreets();  //random index
//        std::string name=getStreetName(index);
//        auto actual=find_street_ids_from_name(name);
//        bool pass=false;
//        std::vector<unsigned> expected;
//        for(unsigned i=0; i<getNumberOfStreets(); i++){
//            if((getStreetName(i))==name){
//                expected.push_back(i);}
//        }
//        actual=find_street_ids_from_name(name);
//        std::sort(actual.begin(), actual.end());
//        std::sort(expected.begin(), expected.end());
//        if(std::equal(expected.begin(),expected.end(),actual.begin())){
//            pass=true;
//        }
//        else{
//            pass=false;
//        }
//     CHECK_EQUAL(true,pass);
//             
//    }
//    TEST_FIXTURE(MapFixture, Random9) {
//         //Functionality Test
//        
//        unsigned index=rand() % getNumberOfStreets();  //random index
//        std::string name=getStreetName(index);
//        auto actual=find_street_ids_from_name(name);
//        bool pass=false;
//        std::vector<unsigned> expected;
//        for(unsigned i=0; i<getNumberOfStreets(); i++){
//            if((getStreetName(i))==name){
//                expected.push_back(i);}
//        }
//        actual=find_street_ids_from_name(name);
//        std::sort(actual.begin(), actual.end());
//        std::sort(expected.begin(), expected.end());
//        if(std::equal(expected.begin(),expected.end(),actual.begin())){
//            pass=true;
//        }
//        else{
//            pass=false;
//        }
//     CHECK_EQUAL(true,pass);
//             
//    }
//    TEST_FIXTURE(MapFixture, Random10) {
//         //Functionality Test
//        
//        unsigned index=rand() % getNumberOfStreets();  //random index
//        std::string name=getStreetName(index);
//        auto actual=find_street_ids_from_name(name);
//        bool pass=false;
//        std::vector<unsigned> expected;
//        for(unsigned i=0; i<getNumberOfStreets(); i++){
//            if((getStreetName(i))==name){
//                expected.push_back(i);}
//        }
//        actual=find_street_ids_from_name(name);
//        std::sort(actual.begin(), actual.end());
//        std::sort(expected.begin(), expected.end());
//        if(std::equal(expected.begin(),expected.end(),actual.begin())){
//            pass=true;
//        }
//        else{
//            pass=false;
//        }
//     CHECK_EQUAL(true,pass);
//             
//    }
//     TEST_FIXTURE(MapFixture, Random11) {
//         //Functionality Test
//        
//        unsigned index=rand() % getNumberOfStreets();  //random index
//        std::string name=getStreetName(index);
//        auto actual=find_street_ids_from_name(name);
//        bool pass=false;
//        std::vector<unsigned> expected;
//        for(unsigned i=0; i<getNumberOfStreets(); i++){
//            if((getStreetName(i))==name){
//                expected.push_back(i);}
//        }
//        actual=find_street_ids_from_name(name);
//        std::sort(actual.begin(), actual.end());
//        std::sort(expected.begin(), expected.end());
//        if(std::equal(expected.begin(),expected.end(),actual.begin())){
//            pass=true;
//        }
//        else{
//            pass=false;
//        }
//     CHECK_EQUAL(true,pass);
//             
//    }
//      TEST_FIXTURE(MapFixture, Random12) {
//         //Functionality Test
//        
//        unsigned index=rand() % getNumberOfStreets();  //random index
//        std::string name=getStreetName(index);
//        auto actual=find_street_ids_from_name(name);
//        bool pass=false;
//        std::vector<unsigned> expected;
//        for(unsigned i=0; i<getNumberOfStreets(); i++){
//            if((getStreetName(i))==name){
//                expected.push_back(i);}
//        }
//        actual=find_street_ids_from_name(name);
//        std::sort(actual.begin(), actual.end());
//        std::sort(expected.begin(), expected.end());
//        if(std::equal(expected.begin(),expected.end(),actual.begin())){
//            pass=true;
//        }
//        else{
//            pass=false;
//        }
//     CHECK_EQUAL(true,pass);
//             
//    }
//       TEST_FIXTURE(MapFixture, Random13) {
//         //Functionality Test
//        
//        unsigned index=rand() % getNumberOfStreets();  //random index
//        std::string name=getStreetName(index);
//        auto actual=find_street_ids_from_name(name);
//        bool pass=false;
//        std::vector<unsigned> expected;
//        for(unsigned i=0; i<getNumberOfStreets(); i++){
//            if((getStreetName(i))==name){
//                expected.push_back(i);}
//        }
//        actual=find_street_ids_from_name(name);
//        std::sort(actual.begin(), actual.end());
//        std::sort(expected.begin(), expected.end());
//        if(std::equal(expected.begin(),expected.end(),actual.begin())){
//            pass=true;
//        }
//        else{
//            pass=false;
//        }
//     CHECK_EQUAL(true,pass);
//             
//    }
//        TEST_FIXTURE(MapFixture, Random14) {
//         //Functionality Test
//        
//        unsigned index=rand() % getNumberOfStreets();  //random index
//        std::string name=getStreetName(index);
//        auto actual=find_street_ids_from_name(name);
//        bool pass=false;
//        std::vector<unsigned> expected;
//        for(unsigned i=0; i<getNumberOfStreets(); i++){
//            if((getStreetName(i))==name){
//                expected.push_back(i);}
//        }
//        actual=find_street_ids_from_name(name);
//        std::sort(actual.begin(), actual.end());
//        std::sort(expected.begin(), expected.end());
//        if(std::equal(expected.begin(),expected.end(),actual.begin())){
//            pass=true;
//        }
//        else{
//            pass=false;
//        }
//     CHECK_EQUAL(true,pass);
//             
//    }
//         TEST_FIXTURE(MapFixture, Random15) {
//         //Functionality Test
//        
//        unsigned index=rand() % getNumberOfStreets();  //random index
//        std::string name=getStreetName(index);
//        auto actual=find_street_ids_from_name(name);
//        bool pass=false;
//        std::vector<unsigned> expected;
//        for(unsigned i=0; i<getNumberOfStreets(); i++){
//            if((getStreetName(i))==name){
//                expected.push_back(i);}
//        }
//        actual=find_street_ids_from_name(name);
//        std::sort(actual.begin(), actual.end());
//        std::sort(expected.begin(), expected.end());
//        if(std::equal(expected.begin(),expected.end(),actual.begin())){
//            pass=true;
//        }
//        else{
//            pass=false;
//        }
//     CHECK_EQUAL(true,pass);
//             
//    }
//         
//    //Check the performance
//    TEST_FIXTURE(MapFixture, TestPerformance) {
//     //Generate random inputs
//        std::vector<unsigned> street_ids;
//        for(size_t i = 0; i < 1000000; i++) {
//            street_ids.push_back(rand_street(rng));
//        }
//        {
//            //Timed Test
//           {
//            ECE297_TIME_CONSTRAINT(250);
//            std::vector<unsigned> result;
//            for(size_t i = 0; i < 100000; i++) {
//                std::string name1=getStreetName(street_ids[i]);              
//                result=find_street_ids_from_name(name1);
//               
//            }
//            }
//        }
//   }
//
//} 
//
