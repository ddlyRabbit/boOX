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
/* kruhoR.cpp / 0-279_zenon                                                   */
/*----------------------------------------------------------------------------*/
//
// Repsesentation of continuous and semi-continuous MAPF instance (MAPF-R).
//
/*----------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include <map>

#include "config.h"
#include "compile.h"
#include "version.h"
#include "defs.h"
#include "types.h"
#include "result.h"

#include "core/kruhoR.h"
#include "util/statistics.h"


using namespace std;
using namespace boOX;


/*----------------------------------------------------------------------------*/


namespace boOX
{



    
/*----------------------------------------------------------------------------*/
// sKruhobot

    const sDouble sKruhobot::UNIT_RADIUS = 1.0;
    const sDouble sKruhobot::UNIT_VELOCITY = 1.0;
    const sDouble sKruhobot::UNIT_ACCELERATION = 1.0;
    
/*----------------------------------------------------------------------------*/
    
    sKruhobot::sKruhobot()	
    {
	// nothing
    }

    
    sKruhobot::sKruhobot(sInt_32 id, const Properties &properties)
	: m_id(id)
	, m_properties(properties)
    {
	// nothing	
    }

    
    sKruhobot::sKruhobot(sInt_32 id, const Properties &properties, const State &state)
	: m_id(id)
	, m_properties(properties)
	, m_state(state)
    {
	// nothing
    }

    
/*----------------------------------------------------------------------------*/

    const sKruhobot::State& sKruhobot::get_State(void) const
    {
	return m_state;
    }

    
    void sKruhobot::set_State(const State &state)
    {
	m_state = state;
    }


/*----------------------------------------------------------------------------*/
    
    void sKruhobot::to_Screen(const sString &indent) const
    {
	to_Stream(stdout, indent);
    }

    
    void sKruhobot::to_Stream(FILE *fw, const sString &indent) const
    {
	fprintf(fw, "%sKruhobot: (id = %d) [\n", indent.c_str(), m_id);
	fprintf(fw, "%s%sproperties: (radius = %.3f, lv = %.3f, la = %.3f, av = %.3f, aa = %.3f, wf = %.3f)\n",
		indent.c_str(),
		s_INDENT.c_str(),
		m_properties.m_radius,
		m_properties.m_linear_velo,
		m_properties.m_linear_accel,
		m_properties.m_angular_velo,
		m_properties.m_angular_accel,
		m_properties.m_wait_factor);

	fprintf(fw, "%s%sstate: (orient = %.3f) [\n", indent.c_str(), s_INDENT.c_str(), m_state.m_orientation);

	fprintf(fw, "%s%sposition: (x = %.3f, y = %.3f)\n",
		indent.c_str(),
		s2_INDENT.c_str(),
		m_state.m_position.m_x,
		m_state.m_position.m_y);
	
	fprintf(fw, "%s%s]\n", indent.c_str(), s_INDENT.c_str());
	
	fprintf(fw, "%s]\n", indent.c_str());		
    }


    void sKruhobot::to_Stream_mpfR(FILE *fw, const sString &indent) const
    {
	fprintf(fw, "%s%d: ", indent.c_str(), m_id);
	
	fprintf(fw, "[r = %.3f, lv = %.3f, la = %.3f, av = %.3f, aa = %.3f, wf = %.3f]\n",
		m_properties.m_radius,
		m_properties.m_linear_velo,
		m_properties.m_linear_accel,
		m_properties.m_angular_velo,
		m_properties.m_angular_accel,
		m_properties.m_wait_factor);       
    }

    
    sResult sKruhobot::from_Stream_mpfR(FILE *fr)
    {
	fscanf(fr, "%d: ", &m_id);
	
	fscanf(fr, "[r = %lf, lv = %lf, la = %lf, av = %lf, aa = %lf, wf = %lf]\n",
	       &m_properties.m_radius,
	       &m_properties.m_linear_velo,
	       &m_properties.m_linear_accel,
	       &m_properties.m_angular_velo,
	       &m_properties.m_angular_accel,
	       &m_properties.m_wait_factor);

	return sRESULT_SUCCESS;
    }


	    
    
/*----------------------------------------------------------------------------*/
// sRealConjunction

    sRealConjunction::sRealConjunction(s2DMap *Map)
	: m_Map(Map)
    {
	// nothing
    }


    sRealConjunction::sRealConjunction(s2DMap *Map, sInt_32 N_Kruhobots)
	: m_Map(Map)
    {
	m_kruhobot_Locations.resize(N_Kruhobots + 1);

	for (LocationIDs_vector::iterator location = m_kruhobot_Locations.begin(); location != m_kruhobot_Locations.end(); ++location)
	{
	    *location = UNDEFINED_LOCATION;
	}
    }

    
/*----------------------------------------------------------------------------*/

    sInt_32 sRealConjunction::get_KruhobotCount(void) const
    {
	sASSERT(m_kruhobot_Locations.size() > 0);
	return m_kruhobot_Locations.size() - 1;
    }

    
    void sRealConjunction::place_Kruhobot(sInt_32 kruhobot_id, sInt_32 location_id)
    {
	sASSERT(kruhobot_id > 0 && kruhobot_id < m_kruhobot_Locations.size() && location_id < m_Map->m_Locations.size());	
	m_kruhobot_Locations[kruhobot_id] = location_id;
    }

    
    void sRealConjunction::remove_Kruhobot(sInt_32 kruhobot_id)
    {
	sASSERT(kruhobot_id > 0 && kruhobot_id < m_kruhobot_Locations.size());		
	m_kruhobot_Locations[kruhobot_id] = UNDEFINED_LOCATION;
    }

    
 /*----------------------------------------------------------------------------*/
    
    void sRealConjunction::to_Screen(const sString &indent) const
    {
	to_Stream(stdout, indent);
    }

    
    void sRealConjunction::to_Stream(FILE *fw, const sString &indent) const
    {
	fprintf(fw, "%sKruhobot conjunction: (|K| = %ld, |M| = %ld) [\n", indent.c_str(), m_kruhobot_Locations.size() - 1, m_Map->m_Locations.size());

	fprintf(fw, "%s%skruhobot locations: {", indent.c_str(), s_INDENT.c_str());
	
	sInt_32 N_Kruhobots_1 = m_kruhobot_Locations.size();
	for (sInt_32 i = 1; i < N_Kruhobots_1; ++i)
	{
	    fprintf(fw, "%d->%d ", i, m_kruhobot_Locations[i]);
	}
	fprintf(fw, "}\n");	
	
	fprintf(fw, "%s]\n", indent.c_str());
    }
    
    
    void sRealConjunction::to_Stream_mpfR(FILE *fw, const sString &indent) const
    {
	fprintf(fw, "%sAssignments: %ld\n", indent.c_str(), m_kruhobot_Locations.size() - 1);

	sInt_32 N_Kruhobots_1 = m_kruhobot_Locations.size();	
	for (sInt_32 i = 1; i < N_Kruhobots_1; ++i)
	{
	    fprintf(fw, "%s%s%d->%d\n", indent.c_str(), s_INDENT.c_str(), i, m_kruhobot_Locations[i]);
	}	
	fprintf(fw, "%s\n", indent.c_str());	
    }
    

    sResult sRealConjunction::from_Stream_mpfR(FILE *fr)
    {
	sInt_32 N_kruhobot_Locations;	
	fscanf(fr, "Assignments: %d\n", &N_kruhobot_Locations);
	sInt_32 N_kruhobot_Locations_1 = N_kruhobot_Locations + 1;
	m_kruhobot_Locations.resize(N_kruhobot_Locations_1);

	for (sInt_32 i = 1; i < N_kruhobot_Locations_1; ++i)
	{
	    sInt_32 kruhobot_id;
	    fscanf(fr, "%d->%d\n", &kruhobot_id, &m_kruhobot_Locations[i]);
	    sASSERT(kruhobot_id == i);
	}	
	
	return sRESULT_SUCCESS;
    }



    
/*----------------------------------------------------------------------------*/
// sRealInstance
    
    sRealInstance::sRealInstance(s2DMap *Map)
	: m_start_conjunction(Map)
	, m_goal_conjunction(Map)
    {
	m_Kruhobots.push_back(sKruhobot());
    }

    
    sRealInstance::sRealInstance(const sRealConjunction &start_conjunction, const sRealConjunction &goal_conjunction)
	: m_start_conjunction(start_conjunction)
	, m_goal_conjunction(goal_conjunction)
    {
	m_Kruhobots.push_back(sKruhobot());
    }


/*----------------------------------------------------------------------------*/

    void sRealInstance::add_Kruhobot(const sKruhobot &kruhobot)
    {
	m_Kruhobots.push_back(kruhobot);
	m_Kruhobots.back().m_id = m_Kruhobots.size() - 1;
    }

    
    void sRealInstance::add_Kruhobot(sInt_32 id, const sKruhobot &kruhobot)
    {
	if (m_Kruhobots.size() >= id)	    
	{
	    m_Kruhobots.resize(id + 1);
	}
	m_Kruhobots[id] = kruhobot;
	m_Kruhobots[id].m_id = id;
    }
    

/*----------------------------------------------------------------------------*/
    
    void sRealInstance::to_Screen(const sString &indent) const
    {
	to_Stream(stdout, indent);
    }
    
    
    void sRealInstance::to_Stream(FILE *fw, const sString &indent) const
    {
	fprintf(fw, "%sReal instance [\n", indent.c_str());

	fprintf(fw, "%s%sstart_conjunction = (\n", indent.c_str(), s_INDENT.c_str());
	m_start_conjunction.to_Stream(fw, indent + s2_INDENT);
	fprintf(fw, "%s%s)\n", indent.c_str(), s_INDENT.c_str());

	fprintf(fw, "%s%sgoal_conjunction = (\n", indent.c_str(), s_INDENT.c_str());
	m_goal_conjunction.to_Stream(fw, indent + s2_INDENT);
	fprintf(fw, "%s%s)\n", indent.c_str(), s_INDENT.c_str());	

	fprintf(fw, "%s%skruhobots = {\n", indent.c_str(), s_INDENT.c_str());

	sInt_32 N_Kruhobots_1 = m_Kruhobots.size();
	for (sInt_32 i = 1; i < N_Kruhobots_1; ++i)
	{
	    m_Kruhobots[i].to_Stream(fw, indent + s2_INDENT);
	}	
	fprintf(fw, "%s%s}\n", indent.c_str(), s_INDENT.c_str());
	
	fprintf(fw, "%s]\n", indent.c_str());	
    }


    sResult sRealInstance::to_File_mpfR(const sString &filename, const sString &indent) const
    {
	FILE *fw;

	if ((fw = fopen(filename.c_str(), "w")) == NULL)
	{
	    return sREAL_INSTANCE_OPEN_ERROR;
	}
	
	to_Stream_mpfR(fw, indent);
	fclose(fw);

	return sRESULT_SUCCESS;	
    }

    
    void sRealInstance::to_Stream_mpfR(FILE *fw, const sString &indent) const
    {
	sInt_32 N_Kruhobots_1 = m_Kruhobots.size();		
	fprintf(fw, "%sKruhobots: %d\n", indent.c_str(), N_Kruhobots_1 - 1);

	for (sInt_32 i = 1; i < N_Kruhobots_1; ++i)
	{
	    m_Kruhobots[i].to_Stream_mpfR(fw, indent + s_INDENT);
	}
	fprintf(fw, "%sStart\n", indent.c_str());
	m_start_conjunction.to_Stream_mpfR(fw, indent + s_INDENT);
	fprintf(fw, "%sGoal\n", indent.c_str());
	m_goal_conjunction.to_Stream_mpfR(fw, indent + s_INDENT);	
    }

    
    sResult sRealInstance::from_File_mpfR(const sString &filename)
    {
	sResult result;
	FILE *fr;

	if ((fr = fopen(filename.c_str(), "r")) == NULL)
	{
	    return sREAL_INSTANCE_OPEN_ERROR;
	}
	
	if (sFAILED(result = from_Stream_mpfR(fr)))
	{
	    fclose(fr);
	    return result;
	}
	fclose(fr);

	return sRESULT_SUCCESS;	
    }

    
    sResult sRealInstance::from_Stream_mpfR(FILE *fr)
    {
	sResult result;
	
	sInt_32 N_Kruhobots;		
	fscanf(fr, "Kruhobots: %d\n", &N_Kruhobots);
	sInt_32 N_Kruhobots_1 = N_Kruhobots + 1;	
	m_Kruhobots.resize(N_Kruhobots_1);
	
	for (sInt_32 i = 1; i < N_Kruhobots_1; ++i)
	{
	    m_Kruhobots[i].to_Screen();
	    if (sFAILED(result = m_Kruhobots[i].from_Stream_mpfR(fr)))
	    {
		return result;
	    }
	    m_Kruhobots[i].to_Screen();
	}
	
	fscanf(fr, "Start\n");
	if (sFAILED(result = m_start_conjunction.from_Stream_mpfR(fr)))
	{
	    return result;
	}
	fscanf(fr, "Goal\n");
	if (sFAILED(result = m_goal_conjunction.from_Stream_mpfR(fr)))
	{
	    return result;
	}

	return sRESULT_SUCCESS;
    }


    

/*----------------------------------------------------------------------------*/
// sRealSolution

    const sRealSolution::Motion sRealSolution::UNDEFINED_MOTION = sRealSolution::Motion(-1, -1, -1, 0.0, 0.0);

    
/*----------------------------------------------------------------------------*/
    
    sRealSolution::sRealSolution()
    {
	// nothing
    }
    
	
    bool sRealSolution::is_Null(void) const
    {
	return (m_Motions.empty());
    }

    
    sInt_32 sRealSolution::get_MotionCount(void) const
    {
	return (m_Motions.size());
    }
    
    
/*----------------------------------------------------------------------------*/
    
    void sRealSolution::to_Screen(const sString &indent) const
    {
	to_Stream(stdout, indent);
    }

    
    void sRealSolution::to_Stream(FILE *fw, const sString &indent) const
    {
	fprintf(fw, "%sSemi-continuous agent solution: (|motions| = %ld) [\n", indent.c_str(), m_Motions.size());

	for (Motions_set::const_iterator motion = m_Motions.begin(); motion != m_Motions.end(); ++motion)
	{
	    fprintf(fw, "%s%s%d: %d --> %d <%.3f,%.3f>\n",
		    indent.c_str(),
		    s_INDENT.c_str(),
		    motion->m_kruhobot_id,
		    motion->m_src_loc_id,
		    motion->m_dest_loc_id,
		    motion->m_duration.m_start_time,
		    motion->m_duration.m_finish_time);	    
	}
	fprintf(fw, "%s]\n", indent.c_str());
    }



    
/*----------------------------------------------------------------------------*/

} // namespace boOX
