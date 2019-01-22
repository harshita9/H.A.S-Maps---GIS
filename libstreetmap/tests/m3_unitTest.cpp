//#include <random>
//#include <unittest++/UnitTest++.h>
//
//#include "StreetsDatabaseAPI.h"
//#include "m1.h"
//#include "m3.h"
//
//#include "unit_test_util.h"
////#include "path_verify.h"
//
//using ece297test::relative_error;
////using ece297test::path_is_legal;
//
//
//SUITE(inter_inter_path_func_public) {
//struct MapFixture {
//    MapFixture() {
//        rng = std::minstd_rand(4);
//        rand_intersection = std::uniform_int_distribution<unsigned>(0, getNumberOfIntersections()-1);
//        rand_street = std::uniform_int_distribution<unsigned>(1, getNumberOfStreets()-1);
//        rand_segment = std::uniform_int_distribution<unsigned>(0, getNumberOfStreetSegments()-1);
//        rand_poi = std::uniform_int_distribution<unsigned>(0, getNumberOfPointsOfInterest()-1);
//        rand_lat = std::uniform_real_distribution<double>(46.650653839, 46.730262756);
//        rand_lon = std::uniform_real_distribution<double>(7.667298794, 7.962547302);
//        rand_turn_penalty = std::uniform_real_distribution<double>(0., 30.);
//    }
//
//    std::minstd_rand rng;
//    std::uniform_int_distribution<unsigned> rand_intersection;
//    std::uniform_int_distribution<unsigned> rand_street;
//    std::uniform_int_distribution<unsigned> rand_segment;
//    std::uniform_int_distribution<unsigned> rand_poi;
//    std::uniform_real_distribution<double> rand_lat;
//    std::uniform_real_distribution<double> rand_lon;
//    std::uniform_real_distribution<double> rand_turn_penalty;
//};
////    TEST(find_path_between_intersections_legality) {
////        std::vector<unsigned> path;
////        
////        path = find_path_between_intersections(0, 589, 0.00000000000000000);
////        CHECK(path_is_legal(0, 589, path));
////        
////        path = find_path_between_intersections(103, 1025, 0.00000000000000000);
////        CHECK(path_is_legal(103, 1025, path));
////        
////        path = find_path_between_intersections(89, 1688, 0.00000000000000000);
////        CHECK(path_is_legal(89, 1688, path));
////        
////        path = find_path_between_intersections(496, 1329, 15.00000000000000000);
////        CHECK(path_is_legal(496, 1329, path));
////        
////        path = find_path_between_intersections(534, 1136, 15.00000000000000000);
////        CHECK(path_is_legal(534, 1136, path));
////        
////        path = find_path_between_intersections(570, 412, 0.00000000000000000);
////        CHECK(path_is_legal(570, 412, path));
////        
////        path = find_path_between_intersections(582, 1257, 15.00000000000000000);
////        CHECK(path_is_legal(582, 1257, path));
////        
////        path = find_path_between_intersections(637, 81, 0.00000000000000000);
////        CHECK(path_is_legal(637, 81, path));
////        
////        path = find_path_between_intersections(640, 649, 15.00000000000000000);
////        CHECK(path_is_legal(640, 649, path));
////        
////        path = find_path_between_intersections(690, 658, 15.00000000000000000);
////        CHECK(path_is_legal(690, 658, path));
////        
////        path = find_path_between_intersections(702, 981, 0.00000000000000000);
////        CHECK(path_is_legal(702, 981, path));
////        
////        path = find_path_between_intersections(758, 1263, 15.00000000000000000);
////        CHECK(path_is_legal(758, 1263, path));
////        
////        path = find_path_between_intersections(808, 548, 15.00000000000000000);
////        CHECK(path_is_legal(808, 548, path));
////        
////        path = find_path_between_intersections(405, 103, 0.00000000000000000);
////        CHECK(path_is_legal(405, 103, path));
////        
////        path = find_path_between_intersections(620, 418, 0.00000000000000000);
////        CHECK(path_is_legal(620, 418, path));
////        
////        path = find_path_between_intersections(1008, 275, 15.00000000000000000);
////        CHECK(path_is_legal(1008, 275, path));
////        
////        path = find_path_between_intersections(1087, 305, 0.00000000000000000);
////        CHECK(path_is_legal(1087, 305, path));
////        
////        path = find_path_between_intersections(1101, 1679, 15.00000000000000000);
////        CHECK(path_is_legal(1101, 1679, path));
////        
////        path = find_path_between_intersections(1141, 748, 0.00000000000000000);
////        CHECK(path_is_legal(1141, 748, path));
////        
////        path = find_path_between_intersections(1291, 1141, 0.00000000000000000);
////        CHECK(path_is_legal(1291, 1141, path));
////        
////        path = find_path_between_intersections(1309, 1122, 15.00000000000000000);
////        CHECK(path_is_legal(1309, 1122, path));
////        
////        path = find_path_between_intersections(1311, 306, 15.00000000000000000);
////        CHECK(path_is_legal(1311, 306, path));
////        
////        path = find_path_between_intersections(1036, 1060, 0.00000000000000000);
////        CHECK(path_is_legal(1036, 1060, path));
////        
////        path = find_path_between_intersections(1050, 1466, 0.00000000000000000);
////        CHECK(path_is_legal(1050, 1466, path));
////        
////        path = find_path_between_intersections(1550, 210, 15.00000000000000000);
////        CHECK(path_is_legal(1550, 210, path));
////        
////        path = find_path_between_intersections(1568, 1501, 0.00000000000000000);
////        CHECK(path_is_legal(1568, 1501, path));
////        
////        path = find_path_between_intersections(877, 1698, 0.00000000000000000);
////        CHECK(path_is_legal(877, 1698, path));
////        
////        path = find_path_between_intersections(1658, 444, 15.00000000000000000);
////        CHECK(path_is_legal(1658, 444, path));
////        
////        path = find_path_between_intersections(1510, 1314, 0.00000000000000000);
////        CHECK(path_is_legal(1510, 1314, path));
////        
////        path = find_path_between_intersections(1680, 158, 15.00000000000000000);
////        CHECK(path_is_legal(1680, 158, path));
////        
////    } //find_path_between_intersections_legality
//
//    TEST(find_path_between_intersections_optimality) {
//        std::vector<unsigned> path;
//        path = find_path_between_intersections(0, 589, 0.00000000000000000);
//        //CHECK(path_is_legal(0, 589, path));
//        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 176.04758212437866405);
//        
//        path = find_path_between_intersections(103, 1025, 0.00000000000000000);
//        //CHECK(path_is_legal(103, 1025, path));
//        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 312.46671284522000178);
//        
//        path = find_path_between_intersections(89, 1688, 0.00000000000000000);
//       // CHECK(path_is_legal(89, 1688, path));
//        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 288.37367034502619845);
//        
//        path = find_path_between_intersections(496, 1329, 15.00000000000000000);
//        //CHECK(path_is_legal(496, 1329, path));
//        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 473.29223115450753312);
//        
//        path = find_path_between_intersections(534, 1136, 15.00000000000000000);
//        //CHECK(path_is_legal(534, 1136, path));
//        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 469.88430178859613306);
//        
//        path = find_path_between_intersections(570, 412, 0.00000000000000000);
//        //CHECK(path_is_legal(570, 412, path));
//        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 251.70033956909844619);
//        
//        path = find_path_between_intersections(582, 1257, 15.00000000000000000);
//        //CHECK(path_is_legal(582, 1257, path));
//        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 629.74146848571137980);
//        
//        path = find_path_between_intersections(637, 81, 0.00000000000000000);
//        //CHECK(path_is_legal(637, 81, path));
//        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 251.57190324374113288);
//        
//        path = find_path_between_intersections(640, 649, 15.00000000000000000);
//       // CHECK(path_is_legal(640, 649, path));
//        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 263.12327169610568944);
//        
//        path = find_path_between_intersections(690, 658, 15.00000000000000000);
//        //CHECK(path_is_legal(690, 658, path));
//        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 378.55994881658210716);
//        
//        path = find_path_between_intersections(702, 981, 0.00000000000000000);
//        //CHECK(path_is_legal(702, 981, path));
//        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 197.52858843520897381);
//        
//        path = find_path_between_intersections(758, 1263, 15.00000000000000000);
//        //CHECK(path_is_legal(758, 1263, path));
//        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 517.96800792230317256);
//        
//        path = find_path_between_intersections(808, 548, 15.00000000000000000);
//        //CHECK(path_is_legal(808, 548, path));
//        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 150.51020625515883467);
//        
//        path = find_path_between_intersections(405, 103, 0.00000000000000000);
//        //CHECK(path_is_legal(405, 103, path));
//        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 373.74278813252647069);
//        
//        path = find_path_between_intersections(620, 418, 0.00000000000000000);
//        //CHECK(path_is_legal(620, 418, path));
//        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 94.58352910269611868);
//        
//        path = find_path_between_intersections(1008, 275, 15.00000000000000000);
//       // CHECK(path_is_legal(1008, 275, path));
//        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 734.21114432479271272);
//        
//        path = find_path_between_intersections(1087, 305, 0.00000000000000000);
//        //CHECK(path_is_legal(1087, 305, path));
//        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 232.05216292478516493);
//        
//        path = find_path_between_intersections(1101, 1679, 15.00000000000000000);
//        //CHECK(path_is_legal(1101, 1679, path));
//        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 294.13925134390609628);
//        
//        path = find_path_between_intersections(1141, 748, 0.00000000000000000);
//        //CHECK(path_is_legal(1141, 748, path));
//        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 118.46665453886328123);
//        
//        path = find_path_between_intersections(1291, 1141, 0.00000000000000000);
//       // CHECK(path_is_legal(1291, 1141, path));
//        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 295.82274331369882248);
//        
//        path = find_path_between_intersections(1309, 1122, 15.00000000000000000);
//        //CHECK(path_is_legal(1309, 1122, path));
//        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 408.67456373045638429);
//        
//        path = find_path_between_intersections(1311, 306, 15.00000000000000000);
//        //CHECK(path_is_legal(1311, 306, path));
//        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 237.55010078848246735);
//        
//        path = find_path_between_intersections(1036, 1060, 0.00000000000000000);
//        //CHECK(path_is_legal(1036, 1060, path));
//        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 187.27344042579284178);
//        
//        path = find_path_between_intersections(1050, 1466, 0.00000000000000000);
//       // CHECK(path_is_legal(1050, 1466, path));
//        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 166.85895170964596446);
//        
//        path = find_path_between_intersections(1550, 210, 15.00000000000000000);
//       // CHECK(path_is_legal(1550, 210, path));
//        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 66.79527098106851213);
//        
//        path = find_path_between_intersections(1568, 1501, 0.00000000000000000);
//       // CHECK(path_is_legal(1568, 1501, path));
//        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 430.90567142957064561);
//        
//        path = find_path_between_intersections(877, 1698, 0.00000000000000000);
//       // CHECK(path_is_legal(877, 1698, path));
//        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 554.49403131584529092);
//        
//        path = find_path_between_intersections(1658, 444, 15.00000000000000000);
//       // CHECK(path_is_legal(1658, 444, path));
//        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 165.55948126154950728);
//        
//        path = find_path_between_intersections(1510, 1314, 0.00000000000000000);
//       //// CHECK(path_is_legal(1510, 1314, path));
//        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 29.89433188865661606);
//        
//        path = find_path_between_intersections(1680, 158, 15.00000000000000000);
//       // CHECK(path_is_legal(1680, 158, path));
//        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 345.65597656842442120);
//        
//    } //find_path_between_intersections_optimality
//
//} //inter_inter_path_func_public



#include <random>
#include <iostream>
#include <unittest++/UnitTest++.h>

#include "StreetsDatabaseAPI.h"
#include "m1.h"
#include "m3.h"
#include "m4.h"

#include "unit_test_util.h"
//#include "courier_verify.h"

using ece297test::relative_error;
//using ece297test::courier_path_is_legal;


SUITE(simple_legality_toronto_canada_public) {
    TEST(simple_legality_toronto_canada) {
        std::vector<DeliveryInfo> deliveries;
        std::vector<unsigned> depots;
        std::vector<unsigned> result_path;
        float turn_penalty;
        
        deliveries = {DeliveryInfo(73593, 104453), DeliveryInfo(1981, 47060), DeliveryInfo(58204, 31403), DeliveryInfo(102129, 63975), DeliveryInfo(41153, 63608)};
        depots = {2521, 5153, 41138};
        turn_penalty = 15;
        result_path = traveling_courier(deliveries, depots, turn_penalty);
        //CHECK(courier_path_is_legal(deliveries, depots, result_path));
        
        deliveries = {DeliveryInfo(102343, 40664), DeliveryInfo(66501, 33150), DeliveryInfo(64220, 70192), DeliveryInfo(10143, 89556), DeliveryInfo(51798, 85214)};
        depots = {14526, 21436, 41416};
        turn_penalty = 15;
        result_path = traveling_courier(deliveries, depots, turn_penalty);
        //CHECK(courier_path_is_legal(deliveries, depots, result_path));
        
        deliveries = {DeliveryInfo(9256, 65463)};
        depots = {2};
        turn_penalty = 15;
        result_path = traveling_courier(deliveries, depots, turn_penalty);
        //CHECK(courier_path_is_legal(deliveries, depots, result_path));
        
        deliveries = {DeliveryInfo(9140, 19741), DeliveryInfo(67133, 63045)};
        depots = {24341, 84950};
        turn_penalty = 15;
        result_path = traveling_courier(deliveries, depots, turn_penalty);
        //CHECK(courier_path_is_legal(deliveries, depots, result_path));
        
        deliveries = {DeliveryInfo(7552, 11395), DeliveryInfo(20877, 76067), DeliveryInfo(76915, 76067), DeliveryInfo(108204, 76067), DeliveryInfo(108204, 99189), DeliveryInfo(32523, 99189), DeliveryInfo(32523, 92242), DeliveryInfo(32523, 99189), DeliveryInfo(32523, 52119)};
        depots = {32736, 27838, 28149};
        turn_penalty = 15;
        result_path = traveling_courier(deliveries, depots, turn_penalty);
        //CHECK(courier_path_is_legal(deliveries, depots, result_path));
        
        deliveries = {DeliveryInfo(82141, 93835), DeliveryInfo(72558, 3972), DeliveryInfo(56282, 90416), DeliveryInfo(107044, 44237), DeliveryInfo(67389, 44237), DeliveryInfo(80027, 98370), DeliveryInfo(99714, 93835), DeliveryInfo(20449, 93835)};
        depots = {33451, 24583, 88889};
        turn_penalty = 15;
        result_path = traveling_courier(deliveries, depots, turn_penalty);
        //CHECK(courier_path_is_legal(deliveries, depots, result_path));
        
        deliveries = {DeliveryInfo(43046, 85976)};
        depots = {33578, 56051};
        turn_penalty = 15;
        result_path = traveling_courier(deliveries, depots, turn_penalty);
        //CHECK(courier_path_is_legal(deliveries, depots, result_path));
        
        deliveries = {DeliveryInfo(51656, 73523), DeliveryInfo(13931, 48963), DeliveryInfo(59889, 44527), DeliveryInfo(43679, 67308), DeliveryInfo(22657, 81046)};
        depots = {36090, 38968, 33098};
        turn_penalty = 15;
        result_path = traveling_courier(deliveries, depots, turn_penalty);
        //CHECK(courier_path_is_legal(deliveries, depots, result_path));
        
        deliveries = {DeliveryInfo(42364, 28031), DeliveryInfo(47309, 59485), DeliveryInfo(19950, 83619), DeliveryInfo(19950, 28031), DeliveryInfo(15381, 59485), DeliveryInfo(42364, 59485), DeliveryInfo(19950, 66464), DeliveryInfo(89161, 21391), DeliveryInfo(19950, 28031)};
        depots = {42952, 27383, 50571};
        turn_penalty = 15;
        result_path = traveling_courier(deliveries, depots, turn_penalty);
        //CHECK(courier_path_is_legal(deliveries, depots, result_path));
        
        deliveries = {DeliveryInfo(18320, 85366), DeliveryInfo(59662, 97286), DeliveryInfo(98769, 79154), DeliveryInfo(81171, 35000), DeliveryInfo(89377, 15810)};
        depots = {53592, 35108, 73562};
        turn_penalty = 15;
        result_path = traveling_courier(deliveries, depots, turn_penalty);
        //CHECK(courier_path_is_legal(deliveries, depots, result_path));
        
        deliveries = {DeliveryInfo(43327, 28619)};
        depots = {56060};
        turn_penalty = 15;
        result_path = traveling_courier(deliveries, depots, turn_penalty);
        //CHECK(courier_path_is_legal(deliveries, depots, result_path));
        
        deliveries = {DeliveryInfo(103810, 3723), DeliveryInfo(34578, 90920), DeliveryInfo(76994, 35921), DeliveryInfo(47909, 90920), DeliveryInfo(80021, 39409), DeliveryInfo(106449, 35921), DeliveryInfo(22113, 35921), DeliveryInfo(5385, 95758)};
        depots = {58218, 19461, 56760};
        turn_penalty = 15;
        result_path = traveling_courier(deliveries, depots, turn_penalty);
        //CHECK(courier_path_is_legal(deliveries, depots, result_path));
        
        deliveries = {DeliveryInfo(52747, 24225), DeliveryInfo(52747, 22444), DeliveryInfo(12519, 65813), DeliveryInfo(52747, 62211), DeliveryInfo(26565, 72357), DeliveryInfo(18521, 75984), DeliveryInfo(80021, 789), DeliveryInfo(80021, 50674)};
        depots = {58853, 4637, 59870};
        turn_penalty = 15;
        result_path = traveling_courier(deliveries, depots, turn_penalty);
        //CHECK(courier_path_is_legal(deliveries, depots, result_path));
        
        deliveries = {DeliveryInfo(64489, 10192), DeliveryInfo(66348, 47055)};
        depots = {75020, 59249};
        turn_penalty = 15;
        result_path = traveling_courier(deliveries, depots, turn_penalty);
        //CHECK(courier_path_is_legal(deliveries, depots, result_path));
        
        deliveries = {DeliveryInfo(7261, 90648), DeliveryInfo(42431, 20777), DeliveryInfo(18005, 104347)};
        depots = {63031, 73383, 10168};
        turn_penalty = 15;
        result_path = traveling_courier(deliveries, depots, turn_penalty);
        ///CHECK(courier_path_is_legal(deliveries, depots, result_path));
        
        deliveries = {DeliveryInfo(55705, 95430), DeliveryInfo(108325, 79055)};
        depots = {64436};
        turn_penalty = 15;
        result_path = traveling_courier(deliveries, depots, turn_penalty);
        //CHECK(courier_path_is_legal(deliveries, depots, result_path));
        
        deliveries = {DeliveryInfo(53600, 84132), DeliveryInfo(7801, 20885)};
        depots = {65267, 91978};
        turn_penalty = 15;
        result_path = traveling_courier(deliveries, depots, turn_penalty);
        //CHECK(courier_path_is_legal(deliveries, depots, result_path));
        
        deliveries = {DeliveryInfo(14020, 18998), DeliveryInfo(48091, 4060), DeliveryInfo(54883, 50882)};
        depots = {36451, 49616, 8297};
        turn_penalty = 15;
        result_path = traveling_courier(deliveries, depots, turn_penalty);
        //CHECK(courier_path_is_legal(deliveries, depots, result_path));
        
        deliveries = {DeliveryInfo(23069, 80185), DeliveryInfo(23459, 60949), DeliveryInfo(62940, 18214), DeliveryInfo(81951, 19441), DeliveryInfo(86025, 46362)};
        depots = {80138, 25819, 5507};
        turn_penalty = 15;
        result_path = traveling_courier(deliveries, depots, turn_penalty);
        //CHECK(courier_path_is_legal(deliveries, depots, result_path));
        
        deliveries = {DeliveryInfo(9748, 61004), DeliveryInfo(63381, 88129)};
        depots = {80938};
        turn_penalty = 15;
        result_path = traveling_courier(deliveries, depots, turn_penalty);
        //CHECK(courier_path_is_legal(deliveries, depots, result_path));
        
        deliveries = {DeliveryInfo(90183, 84955), DeliveryInfo(57973, 86285), DeliveryInfo(57973, 63719), DeliveryInfo(99338, 85306), DeliveryInfo(1024, 16646), DeliveryInfo(86476, 62274), DeliveryInfo(57973, 103682), DeliveryInfo(90183, 16062)};
        depots = {81982, 75396, 34731};
        turn_penalty = 15;
        result_path = traveling_courier(deliveries, depots, turn_penalty);
        //CHECK(courier_path_is_legal(deliveries, depots, result_path));
        
        deliveries = {DeliveryInfo(44848, 98941), DeliveryInfo(108022, 62818), DeliveryInfo(17844, 76041)};
        depots = {80481, 34196, 47772};
        turn_penalty = 15;
        result_path = traveling_courier(deliveries, depots, turn_penalty);
        //CHECK(courier_path_is_legal(deliveries, depots, result_path));
        
        deliveries = {DeliveryInfo(15508, 102530), DeliveryInfo(91823, 22518), DeliveryInfo(104224, 42933), DeliveryInfo(91823, 102530), DeliveryInfo(79815, 102530), DeliveryInfo(76026, 19841), DeliveryInfo(76026, 22518), DeliveryInfo(76026, 19489), DeliveryInfo(76026, 22518)};
        depots = {27131, 69454, 50227};
        turn_penalty = 15;
        result_path = traveling_courier(deliveries, depots, turn_penalty);
        //CHECK(courier_path_is_legal(deliveries, depots, result_path));
        
        deliveries = {DeliveryInfo(105371, 20649)};
        depots = {97060};
        turn_penalty = 15;
        result_path = traveling_courier(deliveries, depots, turn_penalty);
        //CHECK(courier_path_is_legal(deliveries, depots, result_path));
        
        deliveries = {DeliveryInfo(17930, 93400)};
        depots = {98141, 104672};
        turn_penalty = 15;
        result_path = traveling_courier(deliveries, depots, turn_penalty);
        //CHECK(courier_path_is_legal(deliveries, depots, result_path));
        
        deliveries = {DeliveryInfo(101919, 45138)};
        depots = {98719, 32007};
        turn_penalty = 15;
        result_path = traveling_courier(deliveries, depots, turn_penalty);
        //CHECK(courier_path_is_legal(deliveries, depots, result_path));
        
        deliveries = {DeliveryInfo(25692, 87297), DeliveryInfo(33040, 6927), DeliveryInfo(104990, 106963), DeliveryInfo(99873, 6118), DeliveryInfo(24190, 106963), DeliveryInfo(66817, 6927), DeliveryInfo(48830, 6927), DeliveryInfo(47500, 107097)};
        depots = {99219, 33748, 86378};
        turn_penalty = 15;
        result_path = traveling_courier(deliveries, depots, turn_penalty);
        //CHECK(courier_path_is_legal(deliveries, depots, result_path));
        
        deliveries = {DeliveryInfo(26409, 44178), DeliveryInfo(71224, 80079), DeliveryInfo(102350, 45533), DeliveryInfo(64908, 100186), DeliveryInfo(98394, 12677)};
        depots = {104947, 105823, 71751};
        turn_penalty = 15;
        result_path = traveling_courier(deliveries, depots, turn_penalty);
        //CHECK(courier_path_is_legal(deliveries, depots, result_path));
        
        deliveries = {DeliveryInfo(32342, 46379), DeliveryInfo(97919, 71085)};
        depots = {105225};
        turn_penalty = 15;
        result_path = traveling_courier(deliveries, depots, turn_penalty);
        //CHECK(courier_path_is_legal(deliveries, depots, result_path));
        
        deliveries = {DeliveryInfo(29579, 21548), DeliveryInfo(73225, 16427), DeliveryInfo(73225, 27760), DeliveryInfo(67738, 66845), DeliveryInfo(67738, 63977), DeliveryInfo(73225, 86972), DeliveryInfo(64559, 97609), DeliveryInfo(29444, 43148)};
        depots = {107859, 12362, 94777};
        turn_penalty = 15;
        result_path = traveling_courier(deliveries, depots, turn_penalty);
        //CHECK(courier_path_is_legal(deliveries, depots, result_path));
        
    } //simple_legality_toronto_canada

} //simple_legality_toronto_canada_public

