/*============================================================================*/
/*                                                                            */
/*                                                                            */
/*                              boOX 0-279_zenon                              */
/*                                                                            */
/*                  (C) Copyright 2018 - 2019 Pavel Surynek                   */
/*                                                                            */
/*                http://www.surynek.com | <pavel@surynek.com>                */
/*       http://users.fit.cvut.cz/surynek | <pavel.surynek@fit.cvut.cz>       */
/*                                                                            */
/*============================================================================*/
/* cbsR_test.cpp / 0-279_zenon                                                */
/*----------------------------------------------------------------------------*/
//
// Test of semi-continuous version of conflict-based search.
//
/*----------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "config.h"
#include "compile.h"
#include "defs.h"
#include "version.h"

#include "core/graph.h"
#include "core/agent.h"
#include "core/mapR.h"
#include "core/cbsR.h"

#include "util/statistics.h"

#include "test/cbsR_test.h"


using namespace boOX;


/*----------------------------------------------------------------------------*/

namespace boOX
{


/*----------------------------------------------------------------------------*/

    static const sDouble s_wait_factor = 0.2;

    
/*----------------------------------------------------------------------------*/

    void print_Introduction(void)
    {
	printf("----------------------------------------------------------------\n");
	printf("%s : Test of Semi-continuous Conflict-based Search\n", sPRODUCT);
	printf("%s\n", sCOPYRIGHT);
	printf("================================================================\n");
    }

    
    void test_CBS_R_1(void)
    {
	printf("CBS-R test 1 ...\n");
	
	s2DMap square_map(4);

	square_map.add_Location(0, 0.0, 0.0);
	square_map.add_Location(1, 1.0, 0.0);
	square_map.add_Location(2, 0.0, 1.0);
	square_map.add_Location(3, 1.0, 1.0);

	square_map.calc_AllPairsStraightDistances();
	square_map.populate_Network();
	square_map.to_Screen();

	sKruhobot kruhobot(1, sKruhobot::Properties(1.0, 1.0, 1.0, M_PI / 4, M_PI / 6, s_wait_factor), sKruhobot::State(M_PI / 4, sKruhobot::Position(0.0, 0.0)));
	kruhobot.to_Screen();

	sRealConjunction start_conjunction(&square_map, 1);
	start_conjunction.place_Kruhobot(1, 0);

	sRealConjunction goal_conjunction(&square_map, 1);
	goal_conjunction.place_Kruhobot(1, 3);

	start_conjunction.to_Screen();
	goal_conjunction.to_Screen();

	sRealInstance instance(start_conjunction, goal_conjunction);
	instance.to_Screen();

	instance.add_Kruhobot(1, kruhobot);
	instance.to_Screen();
	
	sRealCBS::LocationConflicts__umap location_Conflicts;
	sRealCBS::LinearConflicts__map linear_Conflicts;
	sRealCBS::Schedule_vector kruhobot_Schedule;

	sRealCBS real_CBS(&instance);

	sRealCBS::LocationConflict location_conflict_0_1(0, sRealCBS::Interval(2.1, 2.2));
	sRealCBS::LocationConflict location_conflict_0_2(0, sRealCBS::Interval(3.3, 3.4));
	location_Conflicts[0].insert(sRealCBS::LocationConflicts_map::value_type(location_conflict_0_1.m_interval, location_conflict_0_1));
	location_Conflicts[0].insert(sRealCBS::LocationConflicts_map::value_type(location_conflict_0_2.m_interval, location_conflict_0_2));

	sRealCBS::LocationConflict location_conflict_1_1(1, sRealCBS::Interval(1.3, 1.4));
	sRealCBS::LocationConflict location_conflict_1_2(1, sRealCBS::Interval(0.5, 0.7));
	location_Conflicts[1].insert(sRealCBS::LocationConflicts_map::value_type(location_conflict_1_1.m_interval, location_conflict_1_1));
	location_Conflicts[1].insert(sRealCBS::LocationConflicts_map::value_type(location_conflict_1_2.m_interval, location_conflict_1_2));

	sRealCBS::LocationConflict location_conflict_2_1(2, sRealCBS::Interval(3.1, 3.4));
//	sRealCBS::LocationConflict location_conflict_2_2(2, sRealCBS::Interval(3.8, 4.7));
	location_Conflicts[2].insert(sRealCBS::LocationConflicts_map::value_type(location_conflict_2_1.m_interval, location_conflict_2_1));
//	location_Conflicts[2][location_conflict_2_2.m_interval] = location_conflict_2_2;

	sRealCBS::LocationConflict location_conflict_3_1(3, sRealCBS::Interval(0.0, 1.5));
	sRealCBS::LocationConflict location_conflict_3_2(3, sRealCBS::Interval(1.7, 3.6));
	sRealCBS::LocationConflict location_conflict_3_3(3, sRealCBS::Interval(5.7, 5.9));
	location_Conflicts[3].insert(sRealCBS::LocationConflicts_map::value_type(location_conflict_3_1.m_interval, location_conflict_3_1));
	location_Conflicts[3].insert(sRealCBS::LocationConflicts_map::value_type(location_conflict_3_2.m_interval, location_conflict_3_2));
	location_Conflicts[3].insert(sRealCBS::LocationConflicts_map::value_type(location_conflict_3_3.m_interval, location_conflict_3_3));

/*	
	sRealCBS::LocationConflict location_conflict_0_1(0, sRealCBS::Interval(1.0, 1.5));
	location_Conflicts[0][location_conflict_0_1.m_interval] = location_conflict_0_1;

	sRealCBS::LocationConflict location_conflict_1_1(0, sRealCBS::Interval(4.6, 4.7));
	location_Conflicts[1][location_conflict_1_1.m_interval] = location_conflict_1_1;

	sRealCBS::LocationConflict location_conflict_2_1(0, sRealCBS::Interval(4.6, 4.7));
	location_Conflicts[2][location_conflict_2_1.m_interval] = location_conflict_2_1;

	sRealCBS::LocationConflict location_conflict_3_1(0, sRealCBS::Interval(3.5, 5.7));
	location_Conflicts[3][location_conflict_3_1.m_interval] = location_conflict_3_1;
*/	
	real_CBS.find_KruhobotNonconflictingSchedule(kruhobot,
						     square_map,
						     0,
						     3,
						     10.0,
						     5.0,
						     location_Conflicts,
						     linear_Conflicts,
						     -1.0,
						     kruhobot_Schedule);
	
	printf("CBS-R test 1 ... finished\n");
    }


    void test_CBS_R_2(void)
    {
	printf("CBS-R test 2 ...\n");
	
	s2DMap square_map(4);

	square_map.add_Location(0, 0.0, 0.0);
	square_map.add_Location(1, 1.0, 0.0);
	square_map.add_Location(2, 0.0, 1.0);
	square_map.add_Location(3, 1.0, 1.0);

	square_map.calc_AllPairsStraightDistances();
	square_map.populate_Network();	
	square_map.to_Screen();

	sKruhobot kruhobot_1(1, sKruhobot::Properties(1.0, 1.0, 1.0, M_PI / 4, M_PI / 6, s_wait_factor), sKruhobot::State(M_PI / 4, sKruhobot::Position(0.0, 0.0)));
	kruhobot_1.to_Screen();

	sKruhobot kruhobot_2(2, sKruhobot::Properties(2.0, 1.5, 1.0, M_PI / 4, M_PI / 6, s_wait_factor), sKruhobot::State(M_PI / 4, sKruhobot::Position(0.0, 0.0)));
	kruhobot_2.to_Screen();
	

	sRealConjunction start_conjunction(&square_map, 2);
	start_conjunction.place_Kruhobot(1, 0);
	start_conjunction.place_Kruhobot(2, 1);	

	sRealConjunction goal_conjunction(&square_map, 2);
	goal_conjunction.place_Kruhobot(1, 3);
	goal_conjunction.place_Kruhobot(2, 2);	

	start_conjunction.to_Screen();
	goal_conjunction.to_Screen();

	sRealInstance instance(start_conjunction, goal_conjunction);

	instance.add_Kruhobot(1, kruhobot_1);
	instance.add_Kruhobot(2, kruhobot_2);	
	instance.to_Screen();
	
	sRealCBS::LocationConflicts__umap location_1_Conflicts;
	sRealCBS::LinearConflicts__map linear_1_Conflicts;
	sRealCBS::Schedule_vector kruhobot_1_Schedule;

	sRealCBS real_CBS(&instance);

	sRealCBS::LocationConflict location_1_conflict_0_1(0, sRealCBS::Interval(2.1, 2.2));
	sRealCBS::LocationConflict location_1_conflict_0_2(0, sRealCBS::Interval(3.3, 3.4));
	location_1_Conflicts[0].insert(sRealCBS::LocationConflicts_map::value_type(location_1_conflict_0_1.m_interval, location_1_conflict_0_1));
	location_1_Conflicts[0].insert(sRealCBS::LocationConflicts_map::value_type(location_1_conflict_0_2.m_interval, location_1_conflict_0_2));

	sRealCBS::LocationConflict location_1_conflict_1_1(1, sRealCBS::Interval(0.3, 0.4));
	sRealCBS::LocationConflict location_1_conflict_1_2(1, sRealCBS::Interval(0.5, 0.7));
	location_1_Conflicts[1].insert(sRealCBS::LocationConflicts_map::value_type(location_1_conflict_1_1.m_interval, location_1_conflict_1_1));
	location_1_Conflicts[1].insert(sRealCBS::LocationConflicts_map::value_type(location_1_conflict_1_2.m_interval, location_1_conflict_1_2));

	sRealCBS::LocationConflict location_1_conflict_2_1(2, sRealCBS::Interval(3.1, 3.4));
//	sRealCBS::LocationConflict location_1_conflict_2_2(2, sRealCBS::Interval(3.8, 4.7));
	location_1_Conflicts[2].insert(sRealCBS::LocationConflicts_map::value_type(location_1_conflict_2_1.m_interval, location_1_conflict_2_1));
//	location_1_Conflicts[2][location_1_conflict_2_2.m_interval] = location_1_conflict_2_2;

	sRealCBS::LocationConflict location_1_conflict_3_1(3, sRealCBS::Interval(0.0, 1.5));
	sRealCBS::LocationConflict location_1_conflict_3_2(3, sRealCBS::Interval(1.7, 3.6));
	sRealCBS::LocationConflict location_1_conflict_3_3(3, sRealCBS::Interval(5.7, 5.9));
	location_1_Conflicts[3].insert(sRealCBS::LocationConflicts_map::value_type(location_1_conflict_3_1.m_interval, location_1_conflict_3_1));
	location_1_Conflicts[3].insert(sRealCBS::LocationConflicts_map::value_type(location_1_conflict_3_2.m_interval, location_1_conflict_3_2));
	location_1_Conflicts[3].insert(sRealCBS::LocationConflicts_map::value_type(location_1_conflict_3_3.m_interval, location_1_conflict_3_3));
	
	sRealCBS::LocationConflicts__umap location_2_Conflicts;
	sRealCBS::LinearConflicts__map linear_2_Conflicts;
	sRealCBS::Schedule_vector kruhobot_2_Schedule;

	sRealCBS::LocationConflict location_2_conflict_0_1(0, sRealCBS::Interval(2.1, 2.2));
	sRealCBS::LocationConflict location_2_conflict_0_2(0, sRealCBS::Interval(3.3, 3.4));
	location_2_Conflicts[0].insert(sRealCBS::LocationConflicts_map::value_type(location_2_conflict_0_1.m_interval, location_2_conflict_0_1));
	location_2_Conflicts[0].insert(sRealCBS::LocationConflicts_map::value_type(location_2_conflict_0_2.m_interval, location_2_conflict_0_2));

	sRealCBS::LocationConflict location_2_conflict_1_1(1, sRealCBS::Interval(1.3, 1.4));
	sRealCBS::LocationConflict location_2_conflict_1_2(1, sRealCBS::Interval(1.5, 1.7));
	location_2_Conflicts[1].insert(sRealCBS::LocationConflicts_map::value_type(location_2_conflict_1_1.m_interval, location_2_conflict_1_1));
	location_2_Conflicts[1].insert(sRealCBS::LocationConflicts_map::value_type(location_2_conflict_1_2.m_interval, location_2_conflict_1_2));

	sRealCBS::LocationConflict location_2_conflict_2_1(2, sRealCBS::Interval(3.1, 3.4));
//	sRealCBS::LocationConflict location_2_conflict_2_2(2, sRealCBS::Interval(3.8, 4.7));
	location_2_Conflicts[2].insert(sRealCBS::LocationConflicts_map::value_type(location_2_conflict_2_1.m_interval, location_2_conflict_2_1));
//	location_2_Conflicts[2][location_2_conflict_2_2.m_interval] = location_2_conflict_2_2;

	sRealCBS::LocationConflict location_2_conflict_3_1(3, sRealCBS::Interval(0.0, 1.5));
	sRealCBS::LocationConflict location_2_conflict_3_2(3, sRealCBS::Interval(1.7, 3.6));
	sRealCBS::LocationConflict location_2_conflict_3_3(3, sRealCBS::Interval(5.7, 5.9));
	location_2_Conflicts[3].insert(sRealCBS::LocationConflicts_map::value_type(location_2_conflict_3_1.m_interval, location_2_conflict_3_1));
	location_2_Conflicts[3].insert(sRealCBS::LocationConflicts_map::value_type(location_2_conflict_3_2.m_interval, location_2_conflict_3_2));
	location_2_Conflicts[3].insert(sRealCBS::LocationConflicts_map::value_type(location_2_conflict_3_3.m_interval, location_2_conflict_3_3));

	sDouble time_1 = real_CBS.find_KruhobotNonconflictingSchedule(kruhobot_1,
								      square_map,
								      0,
								      3,
								      10.0,
								      5.0,
								      location_1_Conflicts,
								      linear_1_Conflicts,
								      -1.0,
								      kruhobot_1_Schedule);

	sDouble time_2 = real_CBS.find_KruhobotNonconflictingSchedule(kruhobot_2,
								      square_map,
								      1,
								      2,
								      10.0,
								      5.0,
								      location_2_Conflicts,
								      linear_2_Conflicts,
								      -1.0,
								      kruhobot_2_Schedule);
	printf("Times: %.3f, %.3f\n", time_1, time_2);

	sRealCBS::KruhobotSchedules_vector kruhobot_Schedules;
	sRealCBS::KruhobotCollisions_mset kruhobot_Collisions;

	kruhobot_Schedules.push_back(sRealCBS::Schedule_vector());
	kruhobot_Schedules.push_back(kruhobot_1_Schedule);
	kruhobot_Schedules.push_back(kruhobot_2_Schedule);	

	real_CBS.analyze_NonconflictingSchedules(instance, kruhobot_Schedules, kruhobot_Collisions);
	printf("CBS-R test 2 ... finished\n");
    }


    void test_CBS_R_3(void)
    {
	printf("CBS-R test 3 ...\n");
	
	s2DMap square_map(4);

	square_map.add_Location(0, 0.0, 0.0);
	square_map.add_Location(1, 1.0, 0.0);
	square_map.add_Location(2, 0.0, 1.0);
	square_map.add_Location(3, 1.0, 1.0);

	square_map.calc_AllPairsStraightDistances();
	square_map.populate_Network();	
	square_map.to_Screen();

	sKruhobot kruhobot_1(1, sKruhobot::Properties(0.25, 1.0, 1.0, M_PI / 4, M_PI / 6, s_wait_factor), sKruhobot::State(M_PI / 4, sKruhobot::Position(0.0, 0.0)));
	kruhobot_1.to_Screen();

	sKruhobot kruhobot_2(2, sKruhobot::Properties(0.25, 1.0, 1.0, M_PI / 4, M_PI / 6, s_wait_factor), sKruhobot::State(M_PI / 4, sKruhobot::Position(0.0, 0.0)));
	kruhobot_2.to_Screen();
	

	sRealConjunction start_conjunction(&square_map, 2);
	start_conjunction.place_Kruhobot(1, 0);
	start_conjunction.place_Kruhobot(2, 1);

	sRealConjunction goal_conjunction(&square_map, 2);
	goal_conjunction.place_Kruhobot(1, 3);
	goal_conjunction.place_Kruhobot(2, 2);	

	start_conjunction.to_Screen();
	goal_conjunction.to_Screen();

	sRealInstance instance(start_conjunction, goal_conjunction);

	instance.add_Kruhobot(1, kruhobot_1);
	instance.add_Kruhobot(2, kruhobot_2);	
	instance.to_Screen();

	sRealCBS real_CBS(&instance);
	sRealCBS::KruhobotLocationConflicts_vector kruhobot_location_Conflicts;
	sRealCBS::KruhobotLinearConflicts_vector kruhobot_linear_Conflicts;
	sRealCBS::KruhobotSchedules_vector kruhobot_Schedules;

	kruhobot_location_Conflicts.resize(start_conjunction.get_KruhobotCount() + 1);
	kruhobot_linear_Conflicts.resize(start_conjunction.get_KruhobotCount() + 1);
	kruhobot_Schedules.resize(start_conjunction.get_KruhobotCount() + 1);

	real_CBS.find_NonconflictingSchedules(instance,
					      kruhobot_location_Conflicts,
					      kruhobot_linear_Conflicts,
					      kruhobot_Schedules,
					      10.0,
					      10.0);
	sRealCBSBase::to_Screen(kruhobot_Schedules);

	sDouble solution_cost = sRealCBS::calc_ScheduleCost(instance, kruhobot_Schedules);
	sDouble solution_makespan = sRealCBS::calc_ScheduleMakespan(instance, kruhobot_Schedules);
	
	printf("Found solution: cost = %.3f, makespan = %.3f\n", solution_cost, solution_makespan);

	printf("CBS-R test 3 ... finished\n");
    }


    void test_CBS_R_4(void)
    {
	printf("CBS-R test 4 ...\n");
	
	s2DMap square_map(4);

	square_map.add_Location(0, 0.0, 0.0);
	square_map.add_Location(1, 1.0, 0.0);
	square_map.add_Location(2, 0.0, 1.0);
	square_map.add_Location(3, 1.0, 1.0);

	square_map.calc_AllPairsStraightDistances();
	square_map.populate_Network();	
	square_map.to_Screen();

	sKruhobot kruhobot_1(1, sKruhobot::Properties(0.25, 1.0, 1.0, M_PI / 4, M_PI / 6, s_wait_factor), sKruhobot::State(M_PI / 4, sKruhobot::Position(0.0, 0.0)));
	kruhobot_1.to_Screen();

	sKruhobot kruhobot_2(2, sKruhobot::Properties(0.25, 1.0, 1.0, M_PI / 4, M_PI / 6, s_wait_factor), sKruhobot::State(M_PI / 4, sKruhobot::Position(0.0, 0.0)));
	kruhobot_2.to_Screen();

	sKruhobot kruhobot_3(3, sKruhobot::Properties(0.25, 1.0, 1.0, M_PI / 4, M_PI / 6, s_wait_factor), sKruhobot::State(M_PI / 4, sKruhobot::Position(0.0, 0.0)));
	kruhobot_3.to_Screen();		

	sRealConjunction start_conjunction(&square_map, 3);
	start_conjunction.place_Kruhobot(1, 0);
	start_conjunction.place_Kruhobot(2, 1);
	start_conjunction.place_Kruhobot(3, 3);	

	sRealConjunction goal_conjunction(&square_map, 3);
	goal_conjunction.place_Kruhobot(1, 1);
	goal_conjunction.place_Kruhobot(2, 3);
	goal_conjunction.place_Kruhobot(3, 2);	

	start_conjunction.to_Screen();
	goal_conjunction.to_Screen();

	sRealInstance instance(start_conjunction, goal_conjunction);

	instance.add_Kruhobot(1, kruhobot_1);
	instance.add_Kruhobot(2, kruhobot_2);
	instance.add_Kruhobot(3, kruhobot_3);		
	instance.to_Screen();

	sRealCBS real_CBS(&instance);
	sRealCBS::KruhobotLocationConflicts_vector kruhobot_location_Conflicts;
	sRealCBS::KruhobotLinearConflicts_vector kruhobot_linear_Conflicts;
	sRealCBS::KruhobotSchedules_vector kruhobot_Schedules;

	kruhobot_location_Conflicts.resize(start_conjunction.get_KruhobotCount() + 1);
	kruhobot_linear_Conflicts.resize(start_conjunction.get_KruhobotCount() + 1);
	kruhobot_Schedules.resize(start_conjunction.get_KruhobotCount() + 1);

	real_CBS.find_NonconflictingSchedules(instance,
					      kruhobot_location_Conflicts,
					      kruhobot_linear_Conflicts,
					      kruhobot_Schedules,
					      10.0,
					      10.0);
	sRealCBSBase::to_Screen(kruhobot_Schedules);

	sDouble solution_cost = sRealCBS::calc_ScheduleCost(instance, kruhobot_Schedules);
	sDouble solution_makespan = sRealCBS::calc_ScheduleMakespan(instance, kruhobot_Schedules);
	
	printf("Found solution: cost = %.3f, makespan = %.3f\n", solution_cost, solution_makespan);

	printf("CBS-R test 4 ... finished\n");
    }


    void test_CBS_R_5(void)
    {
	printf("CBS-R test 5 ...\n");	

	s2DMap crossing_map(7);

	crossing_map.add_Location(0, -2.0, 0.0);
	crossing_map.add_Location(1, -1.0, 0.0);
	crossing_map.add_Location(2, 0.0, 0.0);
    	crossing_map.add_Location(3, 1.0, 0.0);
    	crossing_map.add_Location(4, 2.0, 0.0);
    
    	crossing_map.add_Location(5, 0.0, 1.0);
    	crossing_map.add_Location(6, 0.0, 2.0);    
    
	crossing_map.calc_AllPairsStraightDistances();
	crossing_map.populate_Network();	
	crossing_map.to_Screen();

	sKruhobot kruhobot_1(1, sKruhobot::Properties(0.6, 1.0, 1.0, M_PI / 4, M_PI / 6, s_wait_factor), sKruhobot::State(M_PI / 4, sKruhobot::Position(0.0, 0.0)));
	kruhobot_1.to_Screen();

	sKruhobot kruhobot_2(2, sKruhobot::Properties(0.6, 1.0, 1.0, M_PI / 4, M_PI / 6, s_wait_factor), sKruhobot::State(M_PI / 4, sKruhobot::Position(0.0, 0.0)));
	kruhobot_2.to_Screen();

	sRealConjunction start_conjunction(&crossing_map, 2);
	start_conjunction.place_Kruhobot(1, 0);
	start_conjunction.place_Kruhobot(2, 6);

	sRealConjunction goal_conjunction(&crossing_map, 2);
	goal_conjunction.place_Kruhobot(1, 4);
	goal_conjunction.place_Kruhobot(2, 5);

	start_conjunction.to_Screen();
	goal_conjunction.to_Screen();

	sRealInstance instance(start_conjunction, goal_conjunction);

	instance.add_Kruhobot(1, kruhobot_1);
	instance.add_Kruhobot(2, kruhobot_2);
	instance.to_Screen();

	sRealCBS real_CBS(&instance);
	sRealCBS::KruhobotLocationConflicts_vector kruhobot_location_Conflicts;
	sRealCBS::KruhobotLinearConflicts_vector kruhobot_linear_Conflicts;
	sRealCBS::KruhobotSchedules_vector kruhobot_Schedules;

	kruhobot_location_Conflicts.resize(start_conjunction.get_KruhobotCount() + 1);
	kruhobot_linear_Conflicts.resize(start_conjunction.get_KruhobotCount() + 1);
	kruhobot_Schedules.resize(start_conjunction.get_KruhobotCount() + 1);

	real_CBS.find_NonconflictingSchedules(instance,
					      kruhobot_location_Conflicts,
					      kruhobot_linear_Conflicts,
					      kruhobot_Schedules,
					      10.0,
					      10.0);
	sRealCBSBase::to_Screen(kruhobot_Schedules);

	sDouble solution_cost = sRealCBS::calc_ScheduleCost(instance, kruhobot_Schedules);
	sDouble solution_makespan = sRealCBS::calc_ScheduleMakespan(instance, kruhobot_Schedules);
	
	printf("Found solution: cost = %.3f, makespan = %.3f\n", solution_cost, solution_makespan);

	printf("CBS-R test 5 ... finished\n");
    }


    void test_CBS_R_6(void)
    {
	printf("CBS-R test 6 ...\n");	

	s2DMap crossing_map(7);

	crossing_map.add_Location(0, -2.0, 0.0);
	crossing_map.add_Location(1, -1.0, 0.0);
	crossing_map.add_Location(2, 0.0, 0.0);
    	crossing_map.add_Location(3, 1.0, 0.0);
    	crossing_map.add_Location(4, 2.0, 0.0);
    
    	crossing_map.add_Location(5, 0.0, 1.0);
    	crossing_map.add_Location(6, 0.0, 2.0);    
    
	crossing_map.calc_AllPairsStraightDistances();
	crossing_map.populate_Network();	
	crossing_map.to_Screen();

	sKruhobot kruhobot_1(1, sKruhobot::Properties(0.4, 1.0, 1.0, M_PI / 4, M_PI / 6, s_wait_factor), sKruhobot::State(M_PI / 4, sKruhobot::Position(0.0, 0.0)));
	kruhobot_1.to_Screen();

	sKruhobot kruhobot_2(2, sKruhobot::Properties(0.4, 1.0, 1.0, M_PI / 4, M_PI / 6, s_wait_factor), sKruhobot::State(M_PI / 4, sKruhobot::Position(0.0, 0.0)));
	kruhobot_2.to_Screen();

	sRealConjunction start_conjunction(&crossing_map, 2);
	start_conjunction.place_Kruhobot(1, 0);
	start_conjunction.place_Kruhobot(2, 2);

	sRealConjunction goal_conjunction(&crossing_map, 2);
	goal_conjunction.place_Kruhobot(1, 4);
	goal_conjunction.place_Kruhobot(2, 2);

	start_conjunction.to_Screen();
	goal_conjunction.to_Screen();

	sRealInstance instance(start_conjunction, goal_conjunction);

	instance.add_Kruhobot(1, kruhobot_1);
	instance.add_Kruhobot(2, kruhobot_2);
	instance.to_Screen();

	sRealCBS real_CBS(&instance);
	sRealCBS::KruhobotLocationConflicts_vector kruhobot_location_Conflicts;
	sRealCBS::KruhobotLinearConflicts_vector kruhobot_linear_Conflicts;
	sRealCBS::KruhobotSchedules_vector kruhobot_Schedules;

	kruhobot_location_Conflicts.resize(start_conjunction.get_KruhobotCount() + 1);
	kruhobot_linear_Conflicts.resize(start_conjunction.get_KruhobotCount() + 1);
	kruhobot_Schedules.resize(start_conjunction.get_KruhobotCount() + 1);

	real_CBS.find_NonconflictingSchedules(instance,
					      kruhobot_location_Conflicts,
					      kruhobot_linear_Conflicts,
					      kruhobot_Schedules,
					      100.0,
					      100.0);
	sRealCBSBase::to_Screen(kruhobot_Schedules);

	sDouble solution_cost = sRealCBS::calc_ScheduleCost(instance, kruhobot_Schedules);
	sDouble solution_makespan = sRealCBS::calc_ScheduleMakespan(instance, kruhobot_Schedules);
	
	printf("Found solution: cost = %.3f, makespan = %.3f\n", solution_cost, solution_makespan);

	printf("CBS-R test 6 ... finished\n");
    }


    void test_CBS_R_7(void)
    {
	printf("CBS-R test 7 ...\n");	

	s2DMap crossing_map(9);

	crossing_map.add_Location(0, 0.0, 0.0);
	crossing_map.add_Location(1, 1.0, 0.0);
	crossing_map.add_Location(2, 2.0, 0.0);

    	crossing_map.add_Location(3, 0.0, 1.0);	
    	crossing_map.add_Location(4, 1.0, 1.0);	
	crossing_map.add_Location(5, 2.0, 1.0);
    
    	crossing_map.add_Location(6, 0.0, 2.0);	
    	crossing_map.add_Location(7, 1.0, 2.0);	
    	crossing_map.add_Location(8, 2.0, 2.0);
/*
    	crossing_map.add_Location(8, 0.0, 3.0);	
    	crossing_map.add_Location(9, 1.0, 3.0);	
    	crossing_map.add_Location(10, 2.0, 3.0);	
*/	
	crossing_map.calc_AllPairsStraightDistances();
	crossing_map.populate_Network();	
	crossing_map.to_Screen();

	sKruhobot kruhobot_1(1, sKruhobot::Properties(0.1, 1.0, 1.0, M_PI / 4, M_PI / 6, s_wait_factor), sKruhobot::State(M_PI / 4, sKruhobot::Position(0.0, 0.0)));
	kruhobot_1.to_Screen();

	sKruhobot kruhobot_2(2, sKruhobot::Properties(0.1, 1.0, 1.0, M_PI / 4, M_PI / 6, s_wait_factor), sKruhobot::State(M_PI / 4, sKruhobot::Position(0.0, 0.0)));
	kruhobot_2.to_Screen();

	sKruhobot kruhobot_3(3, sKruhobot::Properties(0.1, 1.0, 1.0, M_PI / 4, M_PI / 6, s_wait_factor), sKruhobot::State(M_PI / 4, sKruhobot::Position(0.0, 0.0)));
	kruhobot_3.to_Screen();	

	sRealConjunction start_conjunction(&crossing_map, 3);
	start_conjunction.place_Kruhobot(1, 0);
	start_conjunction.place_Kruhobot(2, 2);
	start_conjunction.place_Kruhobot(3, 3);	

	sRealConjunction goal_conjunction(&crossing_map, 3);
	goal_conjunction.place_Kruhobot(1, 8);
	goal_conjunction.place_Kruhobot(2, 7);
	goal_conjunction.place_Kruhobot(3, 6);	

	start_conjunction.to_Screen();
	goal_conjunction.to_Screen();

	sRealInstance instance(start_conjunction, goal_conjunction);

	instance.add_Kruhobot(1, kruhobot_1);
	instance.add_Kruhobot(2, kruhobot_2);
	instance.add_Kruhobot(3, kruhobot_3);	
	instance.to_Screen();

	sRealCBS real_CBS(&instance);
	sRealCBS::KruhobotLocationConflicts_vector kruhobot_location_Conflicts;
	sRealCBS::KruhobotLinearConflicts_vector kruhobot_linear_Conflicts;
	sRealCBS::KruhobotSchedules_vector kruhobot_Schedules;

	kruhobot_location_Conflicts.resize(start_conjunction.get_KruhobotCount() + 1);
	kruhobot_linear_Conflicts.resize(start_conjunction.get_KruhobotCount() + 1);
	kruhobot_Schedules.resize(start_conjunction.get_KruhobotCount() + 1);

	real_CBS.find_NonconflictingSchedules(instance,
					      kruhobot_location_Conflicts,
					      kruhobot_linear_Conflicts,
					      kruhobot_Schedules,
					      100.0,
					      100.0);
	sRealCBSBase::to_Screen(kruhobot_Schedules);

	sDouble solution_cost = sRealCBS::calc_ScheduleCost(instance, kruhobot_Schedules);
	sDouble solution_makespan = sRealCBS::calc_ScheduleMakespan(instance, kruhobot_Schedules);
	
	printf("Found solution: cost = %.3f, makespan = %.3f\n", solution_cost, solution_makespan);

	printf("CBS-R test 7 ... finished\n");
    }


    void test_CBS_R_8(void)
    {
	printf("CBS-R test 8 ...\n");
	
	s2DMap square_map(4);

	square_map.add_Location(0, 0.0, 0.0);
	square_map.add_Location(1, 1.0, 0.0);
	square_map.add_Location(2, 0.0, 1.0);
	square_map.add_Location(3, 1.0, 1.0);

	square_map.calc_AllPairsStraightDistances();
	square_map.populate_Network();	
	square_map.to_Screen();

	sKruhobot kruhobot_1(1, sKruhobot::Properties(0.25, 1.0, 1.0, M_PI / 4, M_PI / 6, s_wait_factor), sKruhobot::State(M_PI / 4, sKruhobot::Position(0.0, 0.0)));
	kruhobot_1.to_Screen();

	sKruhobot kruhobot_2(2, sKruhobot::Properties(0.25, 1.0, 1.0, M_PI / 4, M_PI / 6, s_wait_factor), sKruhobot::State(M_PI / 4, sKruhobot::Position(0.0, 0.0)));
	kruhobot_2.to_Screen();

	sRealConjunction start_conjunction(&square_map, 2);
	start_conjunction.place_Kruhobot(1, 0);
	start_conjunction.place_Kruhobot(2, 2);

	sRealConjunction goal_conjunction(&square_map, 2);
	goal_conjunction.place_Kruhobot(1, 3);
	goal_conjunction.place_Kruhobot(2, 1);

	start_conjunction.to_Screen();
	goal_conjunction.to_Screen();

	sRealInstance instance(start_conjunction, goal_conjunction);

	instance.add_Kruhobot(1, kruhobot_1);
	instance.add_Kruhobot(2, kruhobot_2);
	instance.to_Screen();

	sRealCBS real_CBS(&instance);
	sRealCBS::KruhobotLocationConflicts_vector kruhobot_location_Conflicts;
	sRealCBS::KruhobotLinearConflicts_vector kruhobot_linear_Conflicts;
	sRealCBS::KruhobotSchedules_vector kruhobot_Schedules;

	kruhobot_location_Conflicts.resize(start_conjunction.get_KruhobotCount() + 1);
	kruhobot_linear_Conflicts.resize(start_conjunction.get_KruhobotCount() + 1);
	kruhobot_Schedules.resize(start_conjunction.get_KruhobotCount() + 1);

	real_CBS.find_NonconflictingSchedules(instance,
					      kruhobot_location_Conflicts,
					      kruhobot_linear_Conflicts,
					      kruhobot_Schedules,
					      100.0,
					      100.0);
	sRealCBSBase::to_Screen(kruhobot_Schedules);

	sDouble solution_cost = sRealCBS::calc_ScheduleCost(instance, kruhobot_Schedules);
	sDouble solution_makespan = sRealCBS::calc_ScheduleMakespan(instance, kruhobot_Schedules);
	
	printf("Found solution: cost = %.3f, makespan = %.3f\n", solution_cost, solution_makespan);

	printf("CBS-R test 8 ... finished\n");
    }    


    void test_CBS_R_9(void)
    {
	printf("CBS-R test 9 ...\n");	

	s2DMap crossing_map(6);

	crossing_map.add_Location(0, 0.0, 0.0);
	crossing_map.add_Location(1, 1.0, 0.0);
	crossing_map.add_Location(2, 2.0, 0.0);

    	crossing_map.add_Location(3, 0.0, 1.0);	
    	crossing_map.add_Location(4, 1.0, 1.0);	
    	crossing_map.add_Location(5, 2.0, 1.0);
    	
	crossing_map.calc_AllPairsStraightDistances();
	crossing_map.populate_Network();	
	crossing_map.to_Screen();

	sKruhobot kruhobot_1(1, sKruhobot::Properties(0.01, 1.0, 1.0, M_PI / 4, M_PI / 6, s_wait_factor), sKruhobot::State(M_PI / 4, sKruhobot::Position(0.0, 0.0)));
	kruhobot_1.to_Screen();

	sKruhobot kruhobot_2(2, sKruhobot::Properties(0.01, 1.0, 1.0, M_PI / 4, M_PI / 6, s_wait_factor), sKruhobot::State(M_PI / 4, sKruhobot::Position(0.0, 0.0)));
	kruhobot_2.to_Screen();

	sKruhobot kruhobot_3(3, sKruhobot::Properties(0.01, 1.0, 1.0, M_PI / 4, M_PI / 6, s_wait_factor), sKruhobot::State(M_PI / 4, sKruhobot::Position(0.0, 0.0)));
	kruhobot_3.to_Screen();	

	sRealConjunction start_conjunction(&crossing_map, 3);
	start_conjunction.place_Kruhobot(1, 0);
	start_conjunction.place_Kruhobot(2, 1);
	start_conjunction.place_Kruhobot(3, 2);	

	sRealConjunction goal_conjunction(&crossing_map, 3);
	goal_conjunction.place_Kruhobot(1, 5);
	goal_conjunction.place_Kruhobot(2, 4);
	goal_conjunction.place_Kruhobot(3, 3);	

	start_conjunction.to_Screen();
	goal_conjunction.to_Screen();

	sRealInstance instance(start_conjunction, goal_conjunction);

	instance.add_Kruhobot(1, kruhobot_1);
	instance.add_Kruhobot(2, kruhobot_2);
	instance.add_Kruhobot(3, kruhobot_3);	
	instance.to_Screen();

	sRealCBS real_CBS(&instance);
	sRealCBS::KruhobotLocationConflicts_vector kruhobot_location_Conflicts;
	sRealCBS::KruhobotLinearConflicts_vector kruhobot_linear_Conflicts;
	sRealCBS::KruhobotSchedules_vector kruhobot_Schedules;

	kruhobot_location_Conflicts.resize(start_conjunction.get_KruhobotCount() + 1);
	kruhobot_linear_Conflicts.resize(start_conjunction.get_KruhobotCount() + 1);
	kruhobot_Schedules.resize(start_conjunction.get_KruhobotCount() + 1);

	real_CBS.find_NonconflictingSchedules(instance,
					      kruhobot_location_Conflicts,
					      kruhobot_linear_Conflicts,
					      kruhobot_Schedules,
					      100.0,
					      100.0);
	sRealCBSBase::to_Screen(kruhobot_Schedules);

	sDouble solution_cost = sRealCBS::calc_ScheduleCost(instance, kruhobot_Schedules);
	sDouble solution_makespan = sRealCBS::calc_ScheduleMakespan(instance, kruhobot_Schedules);
	
	printf("Found solution: cost = %.3f, makespan = %.3f\n", solution_cost, solution_makespan);

	printf("CBS-R test 9 ... finished\n");
    }
 

    void test_CBS_R_A(void)
    {
	printf("CBS-R test A ...\n");	

	s2DMap crossing_map(6);

	crossing_map.add_Location(0, 0.0, 0.0);
	crossing_map.add_Location(1, 1.0, 0.0);
	crossing_map.add_Location(2, 2.0, 0.0);

//    	crossing_map.add_Location(3, 20.0, 1.0);	
    
    	crossing_map.add_Location(3, 0.0, 2.0);	
    	crossing_map.add_Location(4, 1.0, 2.0);	
    	crossing_map.add_Location(5, 2.0, 2.0);
	
	crossing_map.calc_AllPairsStraightDistances();
	crossing_map.populate_Network();	
	crossing_map.to_Screen();

	sKruhobot kruhobot_1(1, sKruhobot::Properties(0.1, 1.0, 1.0, M_PI / 4, M_PI / 6, s_wait_factor), sKruhobot::State(M_PI / 4, sKruhobot::Position(0.0, 0.0)));
	kruhobot_1.to_Screen();

	sKruhobot kruhobot_2(2, sKruhobot::Properties(0.1, 1.0, 1.0, M_PI / 4, M_PI / 6, s_wait_factor), sKruhobot::State(M_PI / 4, sKruhobot::Position(0.0, 0.0)));
	kruhobot_2.to_Screen();

	sKruhobot kruhobot_3(3, sKruhobot::Properties(0.1, 1.0, 1.0, M_PI / 4, M_PI / 6, s_wait_factor), sKruhobot::State(M_PI / 4, sKruhobot::Position(0.0, 0.0)));
	kruhobot_3.to_Screen();	

	sRealConjunction start_conjunction(&crossing_map, 3);
	start_conjunction.place_Kruhobot(1, 0);
	start_conjunction.place_Kruhobot(2, 1);
	start_conjunction.place_Kruhobot(3, 2);	

	sRealConjunction goal_conjunction(&crossing_map, 3);
	goal_conjunction.place_Kruhobot(1, 5);
	goal_conjunction.place_Kruhobot(2, 4);
	goal_conjunction.place_Kruhobot(3, 3);	

	start_conjunction.to_Screen();
	goal_conjunction.to_Screen();

	sRealInstance instance(start_conjunction, goal_conjunction);

	instance.add_Kruhobot(1, kruhobot_1);
	instance.add_Kruhobot(2, kruhobot_2);
	instance.add_Kruhobot(3, kruhobot_3);	
	instance.to_Screen();

	sRealCBS real_CBS(&instance);
	sRealCBS::KruhobotLocationConflicts_vector kruhobot_location_Conflicts;
	sRealCBS::KruhobotLinearConflicts_vector kruhobot_linear_Conflicts;
	sRealCBS::KruhobotSchedules_vector kruhobot_Schedules;

	kruhobot_location_Conflicts.resize(start_conjunction.get_KruhobotCount() + 1);
	kruhobot_linear_Conflicts.resize(start_conjunction.get_KruhobotCount() + 1);
	kruhobot_Schedules.resize(start_conjunction.get_KruhobotCount() + 1);

	real_CBS.find_NonconflictingSchedules(instance,
					      kruhobot_location_Conflicts,
					      kruhobot_linear_Conflicts,
					      kruhobot_Schedules,
					      100.0,
					      100.0);
	sRealCBSBase::to_Screen(kruhobot_Schedules);

	sDouble solution_cost = sRealCBS::calc_ScheduleCost(instance, kruhobot_Schedules);
	sDouble solution_makespan = sRealCBS::calc_ScheduleMakespan(instance, kruhobot_Schedules);
	
	printf("Found solution: cost = %.3f, makespan = %.3f\n", solution_cost, solution_makespan);

	printf("CBS-R test A ... finished\n");
    }


    void test_CBS_R_B(void)
    {
	printf("CBS-R test B ...\n");	

	s2DMap crossing_map(6);

	crossing_map.add_Location(0, 0.0, 0.0);
	crossing_map.add_Location(1, 1.0, 0.0);

	crossing_map.add_Location(2, 0.0, 1.0);
	crossing_map.add_Location(3, 1.0, 1.0);

	crossing_map.add_Location(4, 0.0, 2.0);
	crossing_map.add_Location(5, 1.0, 2.0);
	
	crossing_map.calc_AllPairsStraightDistances();
	crossing_map.populate_Network();	
	crossing_map.to_Screen();

	sKruhobot kruhobot_1(1, sKruhobot::Properties(0.4, 1.0, 1.0, M_PI / 4, M_PI / 6, s_wait_factor), sKruhobot::State(M_PI / 4, sKruhobot::Position(0.0, 0.0)));
	kruhobot_1.to_Screen();

	sKruhobot kruhobot_2(2, sKruhobot::Properties(0.4, 1.0, 1.0, M_PI / 4, M_PI / 6, s_wait_factor), sKruhobot::State(M_PI / 4, sKruhobot::Position(0.0, 0.0)));
	kruhobot_2.to_Screen();

	sRealConjunction start_conjunction(&crossing_map, 2);
	start_conjunction.place_Kruhobot(1, 0);
	start_conjunction.place_Kruhobot(2, 1);

	sRealConjunction goal_conjunction(&crossing_map, 2);
	goal_conjunction.place_Kruhobot(1, 5);
	goal_conjunction.place_Kruhobot(2, 4);

	start_conjunction.to_Screen();
	goal_conjunction.to_Screen();

	sRealInstance instance(start_conjunction, goal_conjunction);

	instance.add_Kruhobot(1, kruhobot_1);
	instance.add_Kruhobot(2, kruhobot_2);
	instance.to_Screen();

	sRealCBS real_CBS(&instance);
	sRealCBS::KruhobotLocationConflicts_vector kruhobot_location_Conflicts;
	sRealCBS::KruhobotLinearConflicts_vector kruhobot_linear_Conflicts;
	sRealCBS::KruhobotSchedules_vector kruhobot_Schedules;

	kruhobot_location_Conflicts.resize(start_conjunction.get_KruhobotCount() + 1);
	kruhobot_linear_Conflicts.resize(start_conjunction.get_KruhobotCount() + 1);
	kruhobot_Schedules.resize(start_conjunction.get_KruhobotCount() + 1);

	real_CBS.find_NonconflictingSchedules(instance,
					      kruhobot_location_Conflicts,
					      kruhobot_linear_Conflicts,
					      kruhobot_Schedules,
					      100.0,
					      10.0);
	sRealCBSBase::to_Screen(kruhobot_Schedules);

	sDouble solution_cost = sRealCBS::calc_ScheduleCost(instance, kruhobot_Schedules);
	sDouble solution_makespan = sRealCBS::calc_ScheduleMakespan(instance, kruhobot_Schedules);
	
	printf("Found solution: cost = %.3f, makespan = %.3f\n", solution_cost, solution_makespan);

	printf("CBS-R test B ... finished\n");
    }


    void test_CBS_R_C(void)
    {
	printf("CBS-R test C ...\n");	

	s2DMap crossing_map(7);

	crossing_map.add_Location(0, 0.0, 0.0);
	crossing_map.add_Location(1, 1.0, 0.0);
	crossing_map.add_Location(2, 2.0, 0.0);

    	crossing_map.add_Location(3, 1.0, 1.0);	
    
    	crossing_map.add_Location(4, 0.0, 2.0);	
    	crossing_map.add_Location(5, 1.0, 2.0);	
    	crossing_map.add_Location(6, 2.0, 2.0);
	
	crossing_map.calc_AllPairsStraightDistances();
	crossing_map.populate_Network();	
	crossing_map.to_Screen();

	sKruhobot kruhobot_1(1, sKruhobot::Properties(0.1, 1.0, 1.0, M_PI / 4, M_PI / 6, s_wait_factor), sKruhobot::State(M_PI / 4, sKruhobot::Position(0.0, 0.0)));
	kruhobot_1.to_Screen();

	sKruhobot kruhobot_2(2, sKruhobot::Properties(0.1, 1.0, 1.0, M_PI / 4, M_PI / 6, s_wait_factor), sKruhobot::State(M_PI / 4, sKruhobot::Position(0.0, 0.0)));
	kruhobot_2.to_Screen();

	sKruhobot kruhobot_3(3, sKruhobot::Properties(0.1, 1.0, 1.0, M_PI / 4, M_PI / 6, s_wait_factor), sKruhobot::State(M_PI / 4, sKruhobot::Position(0.0, 0.0)));
	kruhobot_3.to_Screen();	

	sRealConjunction start_conjunction(&crossing_map, 3);
	start_conjunction.place_Kruhobot(1, 0);
	start_conjunction.place_Kruhobot(2, 1);
	start_conjunction.place_Kruhobot(3, 2);	

	sRealConjunction goal_conjunction(&crossing_map, 3);
	goal_conjunction.place_Kruhobot(1, 6);
	goal_conjunction.place_Kruhobot(2, 5);
	goal_conjunction.place_Kruhobot(3, 4);	

	start_conjunction.to_Screen();
	goal_conjunction.to_Screen();

	sRealInstance instance(start_conjunction, goal_conjunction);

	instance.add_Kruhobot(1, kruhobot_1);
	instance.add_Kruhobot(2, kruhobot_2);
	instance.add_Kruhobot(3, kruhobot_3);	
	instance.to_Screen();

	sRealCBS real_CBS(&instance);
	sRealCBS::KruhobotLocationConflicts_vector kruhobot_location_Conflicts;
	sRealCBS::KruhobotLinearConflicts_vector kruhobot_linear_Conflicts;
	sRealCBS::KruhobotSchedules_vector kruhobot_Schedules;

	kruhobot_location_Conflicts.resize(start_conjunction.get_KruhobotCount() + 1);
	kruhobot_linear_Conflicts.resize(start_conjunction.get_KruhobotCount() + 1);
	kruhobot_Schedules.resize(start_conjunction.get_KruhobotCount() + 1);

	real_CBS.find_NonconflictingSchedules(instance,
					      kruhobot_location_Conflicts,
					      kruhobot_linear_Conflicts,
					      kruhobot_Schedules,
					      100.0,
					      100.0);
	sRealCBSBase::to_Screen(kruhobot_Schedules);

	sDouble solution_cost = sRealCBS::calc_ScheduleCost(instance, kruhobot_Schedules);
	sDouble solution_makespan = sRealCBS::calc_ScheduleMakespan(instance, kruhobot_Schedules);
	
	printf("Found solution: cost = %.3f, makespan = %.3f\n", solution_cost, solution_makespan);

	printf("CBS-R test C ... finished\n");
    }


    void test_CBS_R_D(void)
    {
	printf("CBS-R test D ...\n");	

	s2DMap crossing_map(8);

	crossing_map.add_Location(0, 0.0, 0.0);
	crossing_map.add_Location(1, 1.0, 0.0);
	crossing_map.add_Location(2, 2.0, 0.0);

    	crossing_map.add_Location(3, 0.0, 1.0);
    	crossing_map.add_Location(4, 2.0, 1.0);		
    
    	crossing_map.add_Location(5, 0.0, 2.0);	
    	crossing_map.add_Location(6, 1.0, 2.0);	
    	crossing_map.add_Location(7, 2.0, 2.0);
	
	crossing_map.calc_AllPairsStraightDistances();
	crossing_map.populate_Network();	
	crossing_map.to_Screen();

	sKruhobot kruhobot_1(1, sKruhobot::Properties(0.1, 1.0, 1.0, M_PI / 4, M_PI / 6, s_wait_factor), sKruhobot::State(M_PI / 4, sKruhobot::Position(0.0, 0.0)));
	kruhobot_1.to_Screen();

 	sKruhobot kruhobot_2(2, sKruhobot::Properties(0.1, 1.0, 1.0, M_PI / 4, M_PI / 6, s_wait_factor), sKruhobot::State(M_PI / 4, sKruhobot::Position(0.0, 0.0)));
	kruhobot_2.to_Screen();

	sKruhobot kruhobot_3(3, sKruhobot::Properties(0.1, 1.0, 1.0, M_PI / 4, M_PI / 6, s_wait_factor), sKruhobot::State(M_PI / 4, sKruhobot::Position(0.0, 0.0)));
	kruhobot_3.to_Screen();	

	sRealConjunction start_conjunction(&crossing_map, 3);
	start_conjunction.place_Kruhobot(1, 0);
	start_conjunction.place_Kruhobot(2, 1);
	start_conjunction.place_Kruhobot(3, 2);	

	sRealConjunction goal_conjunction(&crossing_map, 3);
	goal_conjunction.place_Kruhobot(1, 7);
	goal_conjunction.place_Kruhobot(2, 6);
	goal_conjunction.place_Kruhobot(3, 5);	

	start_conjunction.to_Screen();
	goal_conjunction.to_Screen();

	sRealInstance instance(start_conjunction, goal_conjunction);

	instance.add_Kruhobot(1, kruhobot_1);
	instance.add_Kruhobot(2, kruhobot_2);
	instance.add_Kruhobot(3, kruhobot_3);	
	instance.to_Screen();

	sRealCBS real_CBS(&instance);
	sRealCBS::KruhobotLocationConflicts_vector kruhobot_location_Conflicts;
	sRealCBS::KruhobotLinearConflicts_vector kruhobot_linear_Conflicts;
	sRealCBS::KruhobotSchedules_vector kruhobot_Schedules;

	kruhobot_location_Conflicts.resize(start_conjunction.get_KruhobotCount() + 1);
	kruhobot_linear_Conflicts.resize(start_conjunction.get_KruhobotCount() + 1);
	kruhobot_Schedules.resize(start_conjunction.get_KruhobotCount() + 1);

	real_CBS.find_NonconflictingSchedules(instance,
					      kruhobot_location_Conflicts,
					      kruhobot_linear_Conflicts,
					      kruhobot_Schedules,
					      100.0,
					      100.0);
	sRealCBSBase::to_Screen(kruhobot_Schedules);

	sDouble solution_cost = sRealCBS::calc_ScheduleCost(instance, kruhobot_Schedules);
	sDouble solution_makespan = sRealCBS::calc_ScheduleMakespan(instance, kruhobot_Schedules);
	
	printf("Found solution: cost = %.3f, makespan = %.3f\n", solution_cost, solution_makespan);

	printf("CBS-R test D ... finished\n");
    }        


    void test_CBS_R_E(void)
    {
	printf("CBS-R test E ...\n");	

	s2DMap crossing_map(8);

	crossing_map.add_Location(0, 0.0, 0.0);
	crossing_map.add_Location(1, 1.0, 0.0);
	crossing_map.add_Location(2, 2.0, 0.0);
    	crossing_map.add_Location(3, 3.0, 0.0);
	
    	crossing_map.add_Location(4, 0.0, 1.0);		    
    	crossing_map.add_Location(5, 1.0, 1.0);	
    	crossing_map.add_Location(6, 2.0, 1.0);	
    	crossing_map.add_Location(7, 3.0, 1.0);
	
	crossing_map.calc_AllPairsStraightDistances();
	crossing_map.populate_Network(3.0);	
	crossing_map.to_Screen();

	sKruhobot kruhobot_1(1, sKruhobot::Properties(0.1, 1.0, 1.0, M_PI / 4, M_PI / 6, s_wait_factor), sKruhobot::State(M_PI / 4, sKruhobot::Position(0.0, 0.0)));
	kruhobot_1.to_Screen();

	sKruhobot kruhobot_2(2, sKruhobot::Properties(0.1, 1.0, 1.0, M_PI / 4, M_PI / 6, s_wait_factor), sKruhobot::State(M_PI / 4, sKruhobot::Position(0.0, 0.0)));
	kruhobot_2.to_Screen();

	sKruhobot kruhobot_3(3, sKruhobot::Properties(0.1, 1.0, 1.0, M_PI / 4, M_PI / 6, s_wait_factor), sKruhobot::State(M_PI / 4, sKruhobot::Position(0.0, 0.0)));
	kruhobot_3.to_Screen();

	sKruhobot kruhobot_4(4, sKruhobot::Properties(0.1, 1.0, 1.0, M_PI / 4, M_PI / 6, s_wait_factor), sKruhobot::State(M_PI / 4, sKruhobot::Position(0.0, 0.0)));
	kruhobot_4.to_Screen();

	sRealConjunction start_conjunction(&crossing_map, 4);
	start_conjunction.place_Kruhobot(1, 0);
	start_conjunction.place_Kruhobot(2, 1);
	start_conjunction.place_Kruhobot(3, 2);
	start_conjunction.place_Kruhobot(4, 3);		

	sRealConjunction goal_conjunction(&crossing_map, 4);
	goal_conjunction.place_Kruhobot(1, 7);
	goal_conjunction.place_Kruhobot(2, 6);
	goal_conjunction.place_Kruhobot(3, 5);
	goal_conjunction.place_Kruhobot(4, 4);		

	start_conjunction.to_Screen();
	goal_conjunction.to_Screen();

	sRealInstance instance(start_conjunction, goal_conjunction);

	instance.add_Kruhobot(1, kruhobot_1);
	instance.add_Kruhobot(2, kruhobot_2);
	instance.add_Kruhobot(3, kruhobot_3);	
	instance.add_Kruhobot(4, kruhobot_4);	
	instance.to_Screen();

	sRealCBS real_CBS(&instance);
	sRealCBS::KruhobotLocationConflicts_vector kruhobot_location_Conflicts;
	sRealCBS::KruhobotLinearConflicts_vector kruhobot_linear_Conflicts;
	sRealCBS::KruhobotSchedules_vector kruhobot_Schedules;

	kruhobot_location_Conflicts.resize(start_conjunction.get_KruhobotCount() + 1);
	kruhobot_linear_Conflicts.resize(start_conjunction.get_KruhobotCount() + 1);
	kruhobot_Schedules.resize(start_conjunction.get_KruhobotCount() + 1);

	real_CBS.find_NonconflictingSchedules(instance,
					      kruhobot_location_Conflicts,
					      kruhobot_linear_Conflicts,
					      kruhobot_Schedules,
					      100.0,
					      100.0);
	sRealCBSBase::to_Screen(kruhobot_Schedules);

	sDouble solution_cost = sRealCBS::calc_ScheduleCost(instance, kruhobot_Schedules);
	sDouble solution_makespan = sRealCBS::calc_ScheduleMakespan(instance, kruhobot_Schedules);
	
	printf("Found solution: cost = %.3f, makespan = %.3f\n", solution_cost, solution_makespan);

	printf("CBS-R test E ... finished\n");
    }


    void test_CBS_R_F(void)
    {
	printf("CBS-R test F ...\n");	

	s2DMap crossing_map(5);

	crossing_map.add_Location(0, 0.0, 0.0);
	crossing_map.add_Location(1, 1.0, 0.0);
	crossing_map.add_Location(2, 2.0, 0.0);

	crossing_map.add_Location(3, 0.5, 1.0);
	crossing_map.add_Location(4, 1.5, 1.0);		
	
	crossing_map.calc_AllPairsStraightDistances();
	crossing_map.populate_Network();	
	crossing_map.to_Screen();

	sKruhobot kruhobot_1(1, sKruhobot::Properties(0.1, 1.0, 1.0, M_PI / 4, M_PI / 6, s_wait_factor), sKruhobot::State(M_PI / 4, sKruhobot::Position(0.0, 0.0)));
	kruhobot_1.to_Screen();

	sKruhobot kruhobot_2(2, sKruhobot::Properties(0.1, 1.0, 1.0, M_PI / 4, M_PI / 6, s_wait_factor), sKruhobot::State(M_PI / 4, sKruhobot::Position(0.0, 0.0)));
	kruhobot_2.to_Screen();

	sKruhobot kruhobot_3(3, sKruhobot::Properties(0.1, 1.0, 1.0, M_PI / 4, M_PI / 6, s_wait_factor), sKruhobot::State(M_PI / 4, sKruhobot::Position(0.0, 0.0)));
	kruhobot_3.to_Screen();	

	sRealConjunction start_conjunction(&crossing_map, 3);
	start_conjunction.place_Kruhobot(1, 0);
	start_conjunction.place_Kruhobot(2, 1);
	start_conjunction.place_Kruhobot(3, 2);	

	sRealConjunction goal_conjunction(&crossing_map, 3);
	goal_conjunction.place_Kruhobot(1, 3);
	goal_conjunction.place_Kruhobot(2, 4);
	goal_conjunction.place_Kruhobot(3, 0);	

	start_conjunction.to_Screen();
	goal_conjunction.to_Screen();

	sRealInstance instance(start_conjunction, goal_conjunction);

	instance.add_Kruhobot(1, kruhobot_1);
	instance.add_Kruhobot(2, kruhobot_2);
	instance.add_Kruhobot(3, kruhobot_3);	
	instance.to_Screen();

	sRealCBS real_CBS(&instance);
	sRealCBS::KruhobotLocationConflicts_vector kruhobot_location_Conflicts;
	sRealCBS::KruhobotLinearConflicts_vector kruhobot_linear_Conflicts;
	sRealCBS::KruhobotSchedules_vector kruhobot_Schedules;

	kruhobot_location_Conflicts.resize(start_conjunction.get_KruhobotCount() + 1);
	kruhobot_linear_Conflicts.resize(start_conjunction.get_KruhobotCount() + 1);
	kruhobot_Schedules.resize(start_conjunction.get_KruhobotCount() + 1);

	real_CBS.find_NonconflictingSchedules(instance,
					      kruhobot_location_Conflicts,
					      kruhobot_linear_Conflicts,
					      kruhobot_Schedules,
					      100.0,
					      100.0);
	sRealCBSBase::to_Screen(kruhobot_Schedules);

	sDouble solution_cost = sRealCBS::calc_ScheduleCost(instance, kruhobot_Schedules);
	sDouble solution_makespan = sRealCBS::calc_ScheduleMakespan(instance, kruhobot_Schedules);
	
	printf("Found solution: cost = %.3f, makespan = %.3f\n", solution_cost, solution_makespan);

	printf("CBS-R test F ... finished\n");
    }


    void test_CBS_R_G(void)
    {
	printf("CBS-R test G ...\n");	

	s2DMap crossing_map(5);

	crossing_map.add_Location(0, 0.0, 0.0);
	crossing_map.add_Location(1, 1.0, 0.0);
	crossing_map.add_Location(2, 2.0, 0.0);

	crossing_map.add_Location(3, 0.5, 1.0);
	crossing_map.add_Location(4, 1.5, 1.0);		
	
	crossing_map.calc_AllPairsStraightDistances();
	crossing_map.populate_Network(1.9);	
	crossing_map.to_Screen();

	sKruhobot kruhobot_1(1, sKruhobot::Properties(0.1, 1.0, 1.0, M_PI / 4, M_PI / 6, s_wait_factor), sKruhobot::State(M_PI / 4, sKruhobot::Position(0.0, 0.0)));
	kruhobot_1.to_Screen();

	sKruhobot kruhobot_2(2, sKruhobot::Properties(0.1, 1.0, 1.0, M_PI / 4, M_PI / 6, s_wait_factor), sKruhobot::State(M_PI / 4, sKruhobot::Position(0.0, 0.0)));
	kruhobot_2.to_Screen();

	sKruhobot kruhobot_3(3, sKruhobot::Properties(0.1, 1.0, 1.0, M_PI / 4, M_PI / 6, s_wait_factor), sKruhobot::State(M_PI / 4, sKruhobot::Position(0.0, 0.0)));
	kruhobot_3.to_Screen();

	sKruhobot kruhobot_4(4, sKruhobot::Properties(0.1, 1.0, 1.0, M_PI / 4, M_PI / 6, s_wait_factor), sKruhobot::State(M_PI / 4, sKruhobot::Position(0.0, 0.0)));
	kruhobot_4.to_Screen();		

	sRealConjunction start_conjunction(&crossing_map, 4);
	start_conjunction.place_Kruhobot(1, 0);
	start_conjunction.place_Kruhobot(2, 1);
	start_conjunction.place_Kruhobot(3, 2);
	start_conjunction.place_Kruhobot(4, 3);		

	sRealConjunction goal_conjunction(&crossing_map, 4);
	goal_conjunction.place_Kruhobot(1, 3);
	goal_conjunction.place_Kruhobot(2, 4);
	goal_conjunction.place_Kruhobot(3, 0);
	goal_conjunction.place_Kruhobot(4, 1);		

	start_conjunction.to_Screen();
	goal_conjunction.to_Screen();

	sRealInstance instance(start_conjunction, goal_conjunction);

	instance.add_Kruhobot(1, kruhobot_1);
	instance.add_Kruhobot(2, kruhobot_2);
	instance.add_Kruhobot(3, kruhobot_3);
	instance.add_Kruhobot(4, kruhobot_4);		
	instance.to_Screen();

	sRealCBS real_CBS(&instance);
	sRealCBS::KruhobotLocationConflicts_vector kruhobot_location_Conflicts;
	sRealCBS::KruhobotLinearConflicts_vector kruhobot_linear_Conflicts;
	sRealCBS::KruhobotSchedules_vector kruhobot_Schedules;

	kruhobot_location_Conflicts.resize(start_conjunction.get_KruhobotCount() + 1);
	kruhobot_linear_Conflicts.resize(start_conjunction.get_KruhobotCount() + 1);
	kruhobot_Schedules.resize(start_conjunction.get_KruhobotCount() + 1);

	real_CBS.find_NonconflictingSchedules(instance,
					      kruhobot_location_Conflicts,
					      kruhobot_linear_Conflicts,
					      kruhobot_Schedules,
					      100.0,
					      10.0);
	sRealCBSBase::to_Screen(kruhobot_Schedules);	

	sDouble solution_cost = sRealCBS::calc_ScheduleCost(instance, kruhobot_Schedules);
	sDouble solution_makespan = sRealCBS::calc_ScheduleMakespan(instance, kruhobot_Schedules);
	
	printf("Found solution: cost = %.3f, makespan = %.3f\n", solution_cost, solution_makespan);		
	printf("Found cost: %.3f\n", solution_cost);

	printf("CBS-R test G ... finished\n");
    }


    void test_CBS_R_H(void)
    {
	printf("CBS-R test H ...\n");	

	s2DMap crossing_map(4);

	crossing_map.add_Location(0, 0.0, 0.0);
	crossing_map.add_Location(1, 1.0, 0.0);
	crossing_map.add_Location(2, 0.0, 1.0);
	crossing_map.add_Location(3, 1.0, 1.0);	

	crossing_map.calc_AllPairsStraightDistances();
	crossing_map.populate_Network();	
	crossing_map.to_Screen();

	sKruhobot kruhobot_1(1, sKruhobot::Properties(0.1, 1.0, 1.0, M_PI / 4, M_PI / 6, s_wait_factor), sKruhobot::State(M_PI / 4, sKruhobot::Position(0.0, 0.0)));
	kruhobot_1.to_Screen();

	sKruhobot kruhobot_2(2, sKruhobot::Properties(0.1, 1.0, 1.0, M_PI / 4, M_PI / 6, s_wait_factor), sKruhobot::State(M_PI / 4, sKruhobot::Position(0.0, 0.0)));
	kruhobot_2.to_Screen();

	sKruhobot kruhobot_3(3, sKruhobot::Properties(0.1, 1.0, 1.0, M_PI / 4, M_PI / 6, s_wait_factor), sKruhobot::State(M_PI / 4, sKruhobot::Position(0.0, 0.0)));
	kruhobot_3.to_Screen();

	sKruhobot kruhobot_4(4, sKruhobot::Properties(0.1, 1.0, 1.0, M_PI / 4, M_PI / 6, s_wait_factor), sKruhobot::State(M_PI / 4, sKruhobot::Position(0.0, 0.0)));
	kruhobot_4.to_Screen();		

	sRealConjunction start_conjunction(&crossing_map, 3);
	start_conjunction.place_Kruhobot(1, 0);
	start_conjunction.place_Kruhobot(2, 1);
	start_conjunction.place_Kruhobot(3, 2);

	sRealConjunction goal_conjunction(&crossing_map, 3);
	goal_conjunction.place_Kruhobot(1, 3);
	goal_conjunction.place_Kruhobot(2, 2);
	goal_conjunction.place_Kruhobot(3, 1);
	
	start_conjunction.to_Screen();
	goal_conjunction.to_Screen();

	sRealInstance instance(start_conjunction, goal_conjunction);

	instance.add_Kruhobot(1, kruhobot_1);
	instance.add_Kruhobot(2, kruhobot_2);
	instance.add_Kruhobot(3, kruhobot_3);
	instance.to_Screen();

	sRealCBS real_CBS(&instance);
	sRealCBS::KruhobotLocationConflicts_vector kruhobot_location_Conflicts;
	sRealCBS::KruhobotLinearConflicts_vector kruhobot_linear_Conflicts;
	sRealCBS::KruhobotSchedules_vector kruhobot_Schedules;

	kruhobot_location_Conflicts.resize(start_conjunction.get_KruhobotCount() + 1);
	kruhobot_linear_Conflicts.resize(start_conjunction.get_KruhobotCount() + 1);
	kruhobot_Schedules.resize(start_conjunction.get_KruhobotCount() + 1);

	real_CBS.find_NonconflictingSchedules(instance,
					      kruhobot_location_Conflicts,
					      kruhobot_linear_Conflicts,
					      kruhobot_Schedules,
					      100.0,
					      0.0);
	sRealCBSBase::to_Screen(kruhobot_Schedules);

	sDouble solution_cost = sRealCBS::calc_ScheduleCost(instance, kruhobot_Schedules);
	sDouble solution_makespan = sRealCBS::calc_ScheduleMakespan(instance, kruhobot_Schedules);
	
	printf("Found solution: cost = %.3f, makespan = %.3f\n", solution_cost, solution_makespan);	

	printf("CBS-R test H ... finished\n");
    }            
    

    void test_CBS_R_I(void)
    {
	printf("CBS-R test I ...\n");	

	s2DMap crossing_map(5);

	crossing_map.add_Location(0, 0.0, 0.0);
	crossing_map.add_Location(1, 1.0, 0.0);
	crossing_map.add_Location(2, 0.0, 1.0);
	crossing_map.add_Location(3, 1.0, 1.0);
	crossing_map.add_Location(4, 3.3, 3.3);		

	crossing_map.calc_AllPairsStraightDistances();
	crossing_map.populate_Network();	
	crossing_map.to_Screen();

	sKruhobot kruhobot_1(1, sKruhobot::Properties(0.1, 1.0, 1.0, M_PI / 4, M_PI / 6, s_wait_factor), sKruhobot::State(M_PI / 4, sKruhobot::Position(0.0, 0.0)));
	kruhobot_1.to_Screen();

	sKruhobot kruhobot_2(2, sKruhobot::Properties(0.6, 1.0, 1.0, M_PI / 4, M_PI / 6, s_wait_factor), sKruhobot::State(M_PI / 4, sKruhobot::Position(0.0, 0.0)));
	kruhobot_2.to_Screen();

	sKruhobot kruhobot_3(3, sKruhobot::Properties(0.6, 1.5, 1.0, M_PI / 4, M_PI / 6, s_wait_factor), sKruhobot::State(M_PI / 4, sKruhobot::Position(0.0, 0.0)));
	kruhobot_3.to_Screen();

	sKruhobot kruhobot_4(4, sKruhobot::Properties(0.1, 1.0, 1.0, M_PI / 4, M_PI / 6, s_wait_factor), sKruhobot::State(M_PI / 4, sKruhobot::Position(0.0, 0.0)));
	kruhobot_4.to_Screen();		

	sRealConjunction start_conjunction(&crossing_map, 3);
	start_conjunction.place_Kruhobot(1, 0);
	start_conjunction.place_Kruhobot(2, 1);
	start_conjunction.place_Kruhobot(3, 2);

	sRealConjunction goal_conjunction(&crossing_map, 3);
	goal_conjunction.place_Kruhobot(1, 0);
	goal_conjunction.place_Kruhobot(2, 2);
	goal_conjunction.place_Kruhobot(3, 1);
	
	start_conjunction.to_Screen();
	goal_conjunction.to_Screen();

	sRealInstance instance(start_conjunction, goal_conjunction);

	instance.add_Kruhobot(1, kruhobot_1);
	instance.add_Kruhobot(2, kruhobot_2);
	instance.add_Kruhobot(3, kruhobot_3);
	instance.to_Screen();

	sRealCBS real_CBS(&instance);
	sRealCBS::KruhobotLocationConflicts_vector kruhobot_location_Conflicts;
	sRealCBS::KruhobotLinearConflicts_vector kruhobot_linear_Conflicts;
	sRealCBS::KruhobotSchedules_vector kruhobot_Schedules;

	kruhobot_location_Conflicts.resize(start_conjunction.get_KruhobotCount() + 1);
	kruhobot_linear_Conflicts.resize(start_conjunction.get_KruhobotCount() + 1);
	kruhobot_Schedules.resize(start_conjunction.get_KruhobotCount() + 1);

	real_CBS.find_NonconflictingSchedules(instance,
					      kruhobot_location_Conflicts,
					      kruhobot_linear_Conflicts,
					      kruhobot_Schedules,
					      100.0,
					      0.0);
	sRealCBSBase::to_Screen(kruhobot_Schedules);

	sDouble solution_cost = sRealCBS::calc_ScheduleCost(instance, kruhobot_Schedules);
	sDouble solution_makespan = sRealCBS::calc_ScheduleMakespan(instance, kruhobot_Schedules);
	
	printf("Found solution: cost = %.3f, makespan = %.3f\n", solution_cost, solution_makespan);	

	printf("CBS-R test I ... finished\n");
    }


    void test_CBS_R_J(void)
    {
	printf("CBS-R test J ...\n");	

	s2DMap crossing_map(3);

	crossing_map.add_Location(0, 0.0, 0.0);
	crossing_map.add_Location(1, 1.0, 0.0);
	crossing_map.add_Location(2, 2.0, 0.0);

	crossing_map.calc_AllPairsStraightDistances();
	crossing_map.populate_Network();	
	crossing_map.to_Screen();

	sKruhobot kruhobot_1(1, sKruhobot::Properties(0.1, 1.0, 1.0, M_PI / 4, M_PI / 6, s_wait_factor), sKruhobot::State(M_PI / 4, sKruhobot::Position(0.0, 0.0)));
	kruhobot_1.to_Screen();

	sKruhobot kruhobot_2(2, sKruhobot::Properties(0.1, 1.0, 1.0, M_PI / 4, M_PI / 6, s_wait_factor), sKruhobot::State(M_PI / 4, sKruhobot::Position(0.0, 0.0)));
	kruhobot_2.to_Screen();

	sRealConjunction start_conjunction(&crossing_map, 2);
	start_conjunction.place_Kruhobot(1, 1);
	start_conjunction.place_Kruhobot(2, 2);

	sRealConjunction goal_conjunction(&crossing_map, 2);
	goal_conjunction.place_Kruhobot(1, 1);
	goal_conjunction.place_Kruhobot(2, 0);
	
	start_conjunction.to_Screen();
	goal_conjunction.to_Screen();

	sRealInstance instance(start_conjunction, goal_conjunction);

	instance.add_Kruhobot(1, kruhobot_1);
	instance.add_Kruhobot(2, kruhobot_2);
	instance.to_Screen();

	sRealCBS real_CBS(&instance);
	sRealCBS::KruhobotLocationConflicts_vector kruhobot_location_Conflicts;
	sRealCBS::KruhobotLinearConflicts_vector kruhobot_linear_Conflicts;
	sRealCBS::KruhobotSchedules_vector kruhobot_Schedules;

	kruhobot_location_Conflicts.resize(start_conjunction.get_KruhobotCount() + 1);
	kruhobot_linear_Conflicts.resize(start_conjunction.get_KruhobotCount() + 1);
	kruhobot_Schedules.resize(start_conjunction.get_KruhobotCount() + 1);

	real_CBS.find_NonconflictingSchedules(instance,
					      kruhobot_location_Conflicts,
					      kruhobot_linear_Conflicts,
					      kruhobot_Schedules,
					      100.0,
					      0.0);
	sRealCBSBase::to_Screen(kruhobot_Schedules);

	sDouble solution_cost = sRealCBS::calc_ScheduleCost(instance, kruhobot_Schedules);
	sDouble solution_makespan = sRealCBS::calc_ScheduleMakespan(instance, kruhobot_Schedules);
	
	printf("Found solution: cost = %.3f, makespan = %.3f\n", solution_cost, solution_makespan);

	printf("CBS-R test J ... finished\n");
    }


    void test_CBS_R_K(void)
    {
	printf("CBS-R test K ...\n");	

	s2DMap crossing_map(4);

	crossing_map.add_Location(0, 0.0, 0.0);
	crossing_map.add_Location(1, 1.0, 0.0);
	crossing_map.add_Location(2, 2.0, 0.0);
	crossing_map.add_Location(3, 1.0, 1.0);	

	crossing_map.calc_AllPairsStraightDistances();
	crossing_map.populate_Network();	
	crossing_map.to_Screen();

	sKruhobot kruhobot_1(1, sKruhobot::Properties(0.1, 1.0, 1.0, M_PI / 4, M_PI / 6, s_wait_factor), sKruhobot::State(M_PI / 4, sKruhobot::Position(0.0, 0.0)));
	kruhobot_1.to_Screen();

	sKruhobot kruhobot_2(2, sKruhobot::Properties(0.1, 1.0, 1.0, M_PI / 4, M_PI / 6, s_wait_factor), sKruhobot::State(M_PI / 4, sKruhobot::Position(0.0, 0.0)));
	kruhobot_2.to_Screen();

	sKruhobot kruhobot_3(3, sKruhobot::Properties(0.1, 1.0, 1.0, M_PI / 4, M_PI / 6, s_wait_factor), sKruhobot::State(M_PI / 4, sKruhobot::Position(0.0, 0.0)));
	kruhobot_3.to_Screen();	

	sRealConjunction start_conjunction(&crossing_map, 3);
	start_conjunction.place_Kruhobot(1, 0);
	start_conjunction.place_Kruhobot(2, 1);
	start_conjunction.place_Kruhobot(3, 2);	

	sRealConjunction goal_conjunction(&crossing_map, 3);
	goal_conjunction.place_Kruhobot(1, 2);
	goal_conjunction.place_Kruhobot(2, 1);
	goal_conjunction.place_Kruhobot(3, 0);	
	
	start_conjunction.to_Screen();
	goal_conjunction.to_Screen();

	sRealInstance instance(start_conjunction, goal_conjunction);

	instance.add_Kruhobot(1, kruhobot_1);
	instance.add_Kruhobot(2, kruhobot_2);
	instance.add_Kruhobot(3, kruhobot_3);	
	instance.to_Screen();

	sRealCBS real_CBS(&instance);
	sRealCBS::KruhobotLocationConflicts_vector kruhobot_location_Conflicts;
	sRealCBS::KruhobotLinearConflicts_vector kruhobot_linear_Conflicts;
	sRealCBS::KruhobotSchedules_vector kruhobot_Schedules;

	kruhobot_location_Conflicts.resize(start_conjunction.get_KruhobotCount() + 1);
	kruhobot_linear_Conflicts.resize(start_conjunction.get_KruhobotCount() + 1);
	kruhobot_Schedules.resize(start_conjunction.get_KruhobotCount() + 1);

	real_CBS.find_NonconflictingSchedules(instance,
					      kruhobot_location_Conflicts,
					      kruhobot_linear_Conflicts,
					      kruhobot_Schedules,
					      9.0,
					      0.0);
	sRealCBSBase::to_Screen(kruhobot_Schedules);

	sDouble solution_cost = sRealCBS::calc_ScheduleCost(instance, kruhobot_Schedules);
	sDouble solution_makespan = sRealCBS::calc_ScheduleMakespan(instance, kruhobot_Schedules);
	
	printf("Found solution: cost = %.3f, makespan = %.3f\n", solution_cost, solution_makespan);	

	printf("CBS-R test K ... finished\n");
    }


    void test_CBS_R_L(void)
    {
	printf("CBS-R test L ...\n");	

	s2DMap crossing_map(4);

	crossing_map.add_Location(0, 0.0, 0.0);
	crossing_map.add_Location(1, 1.0, 0.0);
	crossing_map.add_Location(2, 2.0, 0.0);
	crossing_map.add_Location(3, 1.0, 1.0);	

	crossing_map.calc_AllPairsStraightDistances();
	crossing_map.populate_Network();	
	crossing_map.to_Screen();

	sKruhobot kruhobot_1(1, sKruhobot::Properties(0.1, 1.0, 1.0, M_PI / 4, M_PI / 6, s_wait_factor), sKruhobot::State(M_PI / 4, sKruhobot::Position(1.0, 2.0)));
	kruhobot_1.to_Screen();

	sKruhobot kruhobot_2(2, sKruhobot::Properties(0.2, 2.0, 2.0, M_PI / 3, M_PI / 7, s_wait_factor), sKruhobot::State(M_PI / 5, sKruhobot::Position(3.0, 4.0)));
	kruhobot_2.to_Screen();

	sKruhobot kruhobot_3(3, sKruhobot::Properties(0.3, 3.0, 3.0, M_PI / 2, M_PI / 8, s_wait_factor), sKruhobot::State(M_PI / 6, sKruhobot::Position(5.0, 6.0)));
	kruhobot_3.to_Screen();	

	sRealConjunction start_conjunction(&crossing_map, 3);
	start_conjunction.place_Kruhobot(1, 0);
	start_conjunction.place_Kruhobot(2, 1);
	start_conjunction.place_Kruhobot(3, 2);	

	sRealConjunction goal_conjunction(&crossing_map, 3);
	goal_conjunction.place_Kruhobot(1, 2);
	goal_conjunction.place_Kruhobot(2, 1);
	goal_conjunction.place_Kruhobot(3, 0);	
	
	start_conjunction.to_Screen();
	goal_conjunction.to_Screen();

	sRealInstance instance(start_conjunction, goal_conjunction);

	instance.add_Kruhobot(1, kruhobot_1);
	instance.add_Kruhobot(2, kruhobot_2);
	instance.add_Kruhobot(3, kruhobot_3);	
	instance.to_Screen();

	instance.to_File_mpfR("small002.mpfR");
	crossing_map.to_File_mapR("small002.mapR");

	s2DMap loaded_map;
	loaded_map.from_File_mapR("small002.mapR");
	loaded_map.calc_AllPairsStraightDistances();
	
	printf("Loaded map\n");
	loaded_map.to_Screen();

		
	sRealInstance loaded_instance(&loaded_map);	
	loaded_instance.from_File_mpfR("small002.mpfR");

	printf("Loaded real instance\n");
	loaded_instance.to_Screen();	

	printf("CBS-R test L ... finished\n");
    }


    void test_CBS_R_E8(void)
    {
	printf("SMT-CBS-R test E8 ...\n");	

	s2DMap grid_map(16);

	grid_map.add_Location(0, 0.0, 0.0);
	grid_map.add_Location(1, 1.0, 0.0);
	grid_map.add_Location(2, 2.0, 0.0);
	grid_map.add_Location(3, 3.0, 0.0);	

	grid_map.add_Location(4, 0.0, 1.0);
	grid_map.add_Location(5, 1.0, 1.0);
	grid_map.add_Location(6, 2.0, 1.0);
	grid_map.add_Location(7, 3.0, 1.0);	

	grid_map.add_Location(8,  0.0, 2.0);
	grid_map.add_Location(9,  1.0, 2.0);
	grid_map.add_Location(10, 2.0, 2.0);
	grid_map.add_Location(11, 3.0, 2.0);

	grid_map.add_Location(12, 0.0, 3.0);
	grid_map.add_Location(13, 1.0, 3.0);
	grid_map.add_Location(14, 2.0, 3.0);
	grid_map.add_Location(15, 3.0, 3.0);	       		
	    
	grid_map.calc_AllPairsStraightDistances();
	grid_map.populate_Network(1.5);	
	grid_map.to_Screen();

	sKruhobot kruhobot_1(1, sKruhobot::Properties(0.1, 1.0, 1.0, M_PI / 4, M_PI / 6, s_wait_factor), sKruhobot::State(M_PI / 4, sKruhobot::Position(0.0, 0.0)));
	kruhobot_1.to_Screen();

	sKruhobot kruhobot_2(2, sKruhobot::Properties(0.1, 1.0, 1.0, M_PI / 4, M_PI / 6, s_wait_factor), sKruhobot::State(M_PI / 4, sKruhobot::Position(0.0, 0.0)));
	kruhobot_2.to_Screen();

	sKruhobot kruhobot_3(3, sKruhobot::Properties(0.1, 1.0, 1.0, M_PI / 4, M_PI / 6, s_wait_factor), sKruhobot::State(M_PI / 4, sKruhobot::Position(0.0, 0.0)));
	kruhobot_3.to_Screen();

	sKruhobot kruhobot_4(4, sKruhobot::Properties(0.1, 1.0, 1.0, M_PI / 4, M_PI / 6, s_wait_factor), sKruhobot::State(M_PI / 4, sKruhobot::Position(0.0, 0.0)));
	kruhobot_4.to_Screen();	

	sRealConjunction start_conjunction(&grid_map, 4);
	start_conjunction.place_Kruhobot(1, 0);
	start_conjunction.place_Kruhobot(2, 1);
	start_conjunction.place_Kruhobot(3, 2);
	start_conjunction.place_Kruhobot(4, 3);	

	sRealConjunction goal_conjunction(&grid_map, 4);
	goal_conjunction.place_Kruhobot(1, 15);
	goal_conjunction.place_Kruhobot(2, 10);
	goal_conjunction.place_Kruhobot(3, 11);
	goal_conjunction.place_Kruhobot(4, 12);	

	start_conjunction.to_Screen();
	goal_conjunction.to_Screen();

	sRealInstance instance(start_conjunction, goal_conjunction);

	instance.add_Kruhobot(1, kruhobot_1);
	instance.add_Kruhobot(2, kruhobot_2);
	instance.add_Kruhobot(3, kruhobot_3);
	instance.add_Kruhobot(4, kruhobot_4);	
	instance.to_Screen();

	sRealCBS real_CBS(&instance);
	sRealCBS::KruhobotLocationConflicts_vector kruhobot_location_Conflicts;
	sRealCBS::KruhobotLinearConflicts_vector kruhobot_linear_Conflicts;
	sRealCBS::KruhobotSchedules_vector kruhobot_Schedules;

	kruhobot_location_Conflicts.resize(start_conjunction.get_KruhobotCount() + 1);
	kruhobot_linear_Conflicts.resize(start_conjunction.get_KruhobotCount() + 1);
	kruhobot_Schedules.resize(start_conjunction.get_KruhobotCount() + 1);

	real_CBS.find_NonconflictingSchedules(instance,
					      kruhobot_location_Conflicts,
					      kruhobot_linear_Conflicts,
					      kruhobot_Schedules,
					      9.0,
					      0.0);
	sRealCBSBase::to_Screen(kruhobot_Schedules);

	sDouble solution_cost = sRealCBS::calc_ScheduleCost(instance, kruhobot_Schedules);
	sDouble solution_makespan = sRealCBS::calc_ScheduleMakespan(instance, kruhobot_Schedules);
	
	printf("Found solution: cost = %.3f, makespan = %.3f\n", solution_cost, solution_makespan);	

	printf("CBS-R test 8E ... finished\n");
    }


    
    
/*----------------------------------------------------------------------------*/

} // namespace boOX


/*----------------------------------------------------------------------------*/
// main program

int main(int sUNUSED(argc), char **sUNUSED(argv))
{
    print_Introduction();
    
//    test_CBS_R_1();
//    test_CBS_R_2();
//    test_CBS_R_3();
//    test_CBS_R_4();
//    test_CBS_R_5();
//    test_CBS_R_6();
//    test_CBS_R_7();
//    test_CBS_R_8();
//    test_CBS_R_9();
//    test_CBS_R_A();
//    test_CBS_R_B();
//    test_CBS_R_C();
//    test_CBS_R_D();
//    test_CBS_R_E();
//    test_CBS_R_F();
//    test_CBS_R_G();
//    test_CBS_R_H();
//    test_CBS_R_I();
//    test_CBS_R_J();
//    test_CBS_R_K();
    test_CBS_R_L();    
    
//    test_CBS_R_E8();    
}
