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
/* mapR.cpp / 0-279_zenon                                                     */
/*----------------------------------------------------------------------------*/
//
// Repsesentation of continuous and semi-continuous MAPF instance (MAPF-R).
//
/*----------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

#include <vector>

#include "config.h"
#include "compile.h"
#include "version.h"
#include "defs.h"
#include "types.h"
#include "result.h"

#include "core/mapR.h"

#include "util/statistics.h"


using namespace std;
using namespace boOX;


/*----------------------------------------------------------------------------*/


namespace boOX
{



    
/*----------------------------------------------------------------------------*/
// s2DMap

    s2DMap::s2DMap()
    {
	// nothing
    }

    
    s2DMap::s2DMap(sInt_32 N_locations)	
    {
	m_Locations.resize(N_locations);
	m_Network.add_Vertices(N_locations);
    }

    
/*----------------------------------------------------------------------------*/

    void s2DMap::add_Location(sDouble x, sDouble y)
    {
	m_Locations.push_back(Location(m_Locations.size(), x, y));
	m_Network.add_Vertex();
    }

    
    void s2DMap::add_Location(sInt_32 id, sDouble x, sDouble y)
    {
	m_Locations[id] = Location(id, x, y);
    }


    void s2DMap::populate_Network(void)
    {
	for (sInt_32 u_id = 0; u_id < m_Locations.size() - 1; ++u_id)
	{
	    for (sInt_32 v_id = u_id + 1; v_id < m_Locations.size(); ++v_id)
	    {
		m_Network.add_Edge(u_id, v_id);		
	    }
	}
    }

    
    void s2DMap::populate_Network(sDouble max_distance)
    {
	for (sInt_32 u_id = 0; u_id < m_Locations.size() - 1; ++u_id)
	{
	    for (sInt_32 v_id = u_id + 1; v_id < m_Locations.size(); ++v_id)
	    {
		sDouble distance = calc_PointDistance(u_id, v_id);

		if (distance <= max_distance)
		{
		    m_Network.add_Edge(u_id, v_id);
		}
	    }
	}	
    }
    

/*----------------------------------------------------------------------------*/
    
    void s2DMap::calc_SingleSourceStraightDistances(sInt_32 s_id)
    {
	sInt_32 N_locations = m_Locations.size();
	m_straight_Distances[s_id].resize(N_locations);
	
	calc_SingleSourceStraightDistances(s_id, m_straight_Distances[s_id]);
    }


    void s2DMap::calc_SingleSourceStraightDistances(sInt_32 s_id, Distances_vector &straight_Distances) const
    {
	sInt_32 N_locations = m_Locations.size();
	
	for (sInt_32 t_id = 0; t_id < N_locations; ++t_id)
	{
	    sDouble dx = m_Locations[s_id].m_x - m_Locations[t_id].m_x;
	    sDouble dy = m_Locations[s_id].m_y - m_Locations[t_id].m_y;
	    
	    straight_Distances[t_id] = sqrt(dx * dx + dy * dy);
	}
    }
    

    void s2DMap::calc_AllPairsStraightDistances(void)
    {
	sInt_32 N_locations = m_Locations.size();	
	m_straight_Distances.resize(N_locations);

	for (sInt_32 s_id = 0; s_id < N_locations; ++s_id)
	{
	    calc_SingleSourceStraightDistances(s_id);
	}
    }

    
    void s2DMap::calc_AllPairsStraightDistances(Distances_2d_vector &straight_Distances) const
    {
	sInt_32 N_locations = m_Locations.size();
	
	for (sInt_32 s_id = 0; s_id < N_locations; ++s_id)
	{
	    calc_SingleSourceStraightDistances(s_id, straight_Distances[s_id]);
	}
    }


/*----------------------------------------------------------------------------*/

    void s2DMap::calc_SingleSourceShortestDistances(sInt_32 s_id)
    {
	sInt_32 N_locations = m_Locations.size();
	m_shortest_Distances[s_id].resize(N_locations);
	
	calc_SingleSourceShortestDistances(s_id, m_shortest_Distances[s_id]);
    }


    void s2DMap::calc_SingleSourceShortestDistances(sInt_32 sUNUSED(s_id), Distances_vector &sUNUSED(shortest_Distances)) const
    {
	/* TODO
	sInt_32 N_locations = m_Locations.size();
	
	for (sInt_32 t_id = 0; t_id < N_locations; ++t_id)
	{
	    sDouble dx = m_Locations[s_id].m_x - m_Locations[t_id].m_x;
	    sDouble dy = m_Locations[s_id].m_y - m_Locations[t_id].m_y;
	    
	    shortest_Distances[t_id] = sqrt(dx * dx + dy * dy);
	}
	*/
    }
    

    void s2DMap::calc_AllPairsShortestDistances(void)
    {
	sInt_32 N_locations = m_Locations.size();	
	m_shortest_Distances.resize(N_locations);

	for (sInt_32 s_id = 0; s_id < N_locations; ++s_id)
	{
	    calc_SingleSourceShortestDistances(s_id);
	}
    }

    
    void s2DMap::calc_AllPairsShortestDistances(Distances_2d_vector &shortest_Distances) const
    {
	sInt_32 N_locations = m_Locations.size();
		
	for (sInt_32 s_id = 0; s_id < N_locations; ++s_id)
	{
	    calc_SingleSourceShortestDistances(s_id, shortest_Distances[s_id]);
	}
    }
    

/*----------------------------------------------------------------------------*/

    sDouble s2DMap::calc_PointDistance(sInt_32 point_1_id, sInt_32 point_2_id) const
    {
	sDouble x1, y1, x2, y2;

	x1 = m_Locations[point_1_id].m_x;
	y1 = m_Locations[point_1_id].m_y;

	x2 = m_Locations[point_2_id].m_x;
	y2 = m_Locations[point_2_id].m_y;
	
	return calc_PointDistance(x1, y1, x2, y2);
    }

    
    sDouble s2DMap::calc_PointDistance(sDouble x1, sDouble y1, sDouble x2, sDouble y2) const
    {
	sDouble dx = x2 - x1;
	sDouble dy = y2 - y1;

	return (sqrt(dx * dx + dy * dy));
    }

    
    sDouble s2DMap::calc_LineDistance(sInt_32 line_1_u_id,
				      sInt_32 line_1_v_id,
				      sInt_32 line_2_u_id,
				      sInt_32 line_2_v_id) const
    {
	sDouble px1, py1, px2, py2;
	sDouble qx1, qy1, qx2, qy2;

	px1 = m_Locations[line_1_u_id].m_x;
	py1 = m_Locations[line_1_u_id].m_y;

	px2 = m_Locations[line_1_v_id].m_x;
	py2 = m_Locations[line_1_v_id].m_y;

	qx1 = m_Locations[line_2_u_id].m_x;
	qy1 = m_Locations[line_2_u_id].m_y;

	qx2 = m_Locations[line_2_v_id].m_x;
	qy2 = m_Locations[line_2_v_id].m_y;

	return calc_LineDistance(px1, py1, px2, py2, qx1, qy1, qx2, qy2);
    }

    
    sDouble s2DMap::calc_LineDistance(sDouble px1, sDouble py1, sDouble px2, sDouble py2,
				      sDouble qx1, sDouble qy1, sDouble qx2, sDouble qy2) const		
    {
	#ifdef sDEBUG
	{
	    /*
	    printf("Line P: %.3f, %.3f, %.3f, %.3f\n", px1, py1, px2, py2);
	    printf("Line Q: %.3f, %.3f, %.3f, %.3f\n", qx1, qy1, qx2, qy2);	    
	    */
        }
	#endif

	sDouble dpx = px2 - px1;
	sDouble dpy = py2 - py1;

	sDouble npx = dpy;
	sDouble npy = -dpx;	
	
	sDouble dqx = qx2 - qx1;
	sDouble dqy = qy2 - qy1;

	sDouble nqx = dqy;
	sDouble nqy = -dqx;

//	px1 + s * dpx = qx1 + t * dqx;
//	py1 + s * dpy = qy1 + t * dqy;

	sDouble px1_ = px1;
	sDouble py1_ = py1;

	sDouble px1__ = px1 + dpx;
	sDouble py1__ = py1 + dpy;	

//	px1_ + ss * npx = qx1 + tt * dqx;
//	py1_ + ss * npy = qy1 + tt * dqy;

	sDouble qx1_ = qx1;
	sDouble qy1_ = qy1;

	sDouble qx1__ = qx1 + dqx;
	sDouble qy1__ = qy1 + dqy;		
	
//	px1 + ss * dpx = qx1_ + tt * nqx;
//	py1 + ss * dpy = qy1_ + tt * nqy;	

	sDouble dd = dpx * dqy - dpy * dqx;
	sDouble line_distance = -1.0;

	if (sABS(dd) < s_EPSILON) /* parallel lines */
	{
	    sDouble ddd = npx * dqy - npy * dqx;	    
	    sDouble ss = (py1 * dqx - qy1 * dqx - px1 * dqy + qx1 * dqy) / ddd;
	    sDouble tt;
	    
	    if (sABS(dqx) > sABS(dqy))
	    {
		tt = (px1 + ss * npx - qx1) / dqx;
	    }
	    else
	    {
		tt = (py1 + ss * npy - qy1) / dqy;
	    }

	    sDouble dist = calc_PointDistance(px1, py1, qx1 + tt * dqx, qy1 + tt * dqy);

	    if (line_distance < 0.0)
	    {
		line_distance = dist;
	    }
	    else
	    {
		line_distance = sMIN(line_distance, dist);
	    }
	}
	else
	{
	    sDouble s = (py1 * dqx - qy1 * dqx - px1 * dqy + qx1 * dqy) / dd;
	    sDouble t;
	    
	    if (sABS(dqx) > sABS(dqy))
	    {
		t = (px1 + s * dpx - qx1) / dqx;
	    }
	    else
	    {
		t = (py1 + s * dpy - qy1) / dqy;
	    }

	    if (s >= 0.0 && s <= 1.0)
	    {
		if (t >= 0.0 && t <= 1.0)
		{
		    return 0.0;
		}
	    }

	    sDouble ddd = npx * dqy - npy * dqx;	    
	    
	    if (sABS(ddd) < s_EPSILON) /* perpendicular */
	    {
		if (s >= 0.0 && s <= 1.0)
		{
		    if (t >= 0.0 && t <= 1.0)
		    {
			sASSERT(false);
		    }
		    else
		    {
			sDouble dist1 = calc_PointDistance(qx1, qy1, px1 + s * dpx, py1 + s * dpy);

			if (line_distance < 0.0)
			{
			    line_distance = dist1;
			}
			else
			{
			    line_distance = sMIN(line_distance, dist1);
			}

			sDouble dist2 = calc_PointDistance(qx2, qy2, px1 + s * dpx, py1 + s * dpy);

			if (line_distance < 0.0)
			{
			    line_distance = dist2;
			}
			else
			{
			    line_distance = sMIN(line_distance, dist2);
			}			
		    }
		}
		else
		{
		    if (t >= 0.0 && t <= 1.0)
		    {
			sDouble dist1 = calc_PointDistance(px1, py1, qx1 + t * dqx, qy1 + t * dqy);

			if (line_distance < 0.0)
			{
			    line_distance = dist1;
			}
			else
			{
			    line_distance = sMIN(line_distance, dist1);
			}

			sDouble dist2 = calc_PointDistance(px2, py2, qx1 + t * dqx, qy1 + t * dqy);

			if (line_distance < 0.0)
			{
			    line_distance = dist2;
			}
			else
			{
			    line_distance = sMIN(line_distance, dist2);
			}			
		    }
		    else
		    {
			/* no meaning */
		    }		    
		}
	    }
	    else /* no perpendicular */
	    {
		sDouble ss = (py1_ * dqx - qy1 * dqx - px1_ * dqy + qx1 * dqy) / ddd;
		sDouble ss_ = (py1__ * dqx - qy1 * dqx - px1__ * dqy + qx1 * dqy) / ddd;
		sDouble tt, tt_;

		if (sABS(dqx) > sABS(dqy))
		{
		    tt = (px1_ + ss * npx - qx1) / dqx;
		}
		else
		{
		    tt = (py1_ + ss * npy - qy1) / dqy;
		}

		if (sABS(dqx) > sABS(dqy))
		{
		    tt_ = (px1__ + ss_ * npx - qx1) / dqx;
		}
		else
		{
		    tt_ = (py1__ + ss_ * npy - qy1) / dqy;
		}

		if (tt >= 0.0 && tt <= 1.0)
		{
		    sDouble dist = calc_PointDistance(px1_, py1_, qx1 + tt * dqx, qy1 + tt * dqy);

		    if (line_distance < 0.0)
		    {
			line_distance = dist;
		    }
		    else
		    {
			line_distance = sMIN(line_distance, dist);
		    }
		}
		if (tt_ >= 0 && tt_ <= 1.0)
		{
		    sDouble dist = calc_PointDistance(px1__, py1__, qx1 + tt_ * dqx, qy1 + tt_ * dqy);

		    if (line_distance < 0.0)
		    {
			line_distance = dist;
		    }
		    else
		    {
			line_distance = sMIN(line_distance, dist);
		    }		    
		}		    
		{
		    sDouble ddd_ = dpx * nqy - dpy * nqx;	    
		    sASSERT(sABS(ddd_) >= s_EPSILON); /* perpendicular should not be here */

//		sDouble ss = (py1_ * dqx - qy1 * dqx - px1_ * dqy + qx1 * dqy) / ddd;
//		sDouble ss_ = (py1__ * dqx - qy1 * dqx - px1__ * dqy + qx1 * dqy) / ddd;		    
		    
		    sDouble tt = (-qy1_ * dpx + py1 * dpx + qx1_ * dpy - px1 * dpy) / ddd_;
		    sDouble tt_ = (-qy1__ * dpx + py1 * dpx + qx1__ * dpy - px1 * dpy) / ddd_;
		    sDouble ss, ss_;
		    
		    if (sABS(nqx) > sABS(nqy))
		    {
			ss = (-px1 + tt * nqx + qx1_) / dpx;
		    }
		    else
		    {
			ss = (-py1 + tt * nqy + qy1_) / dpy;
		    }
		    
		    if (sABS(nqx) > sABS(nqy))
		    {
			ss_ = (-px1 + tt_ * nqx + qx1__) / dpx;
		    }
		    else
		    {
			ss_ = (-py1 + tt_ * nqy + qy1__) / dpy;
		    }
		    
		    if (ss >= 0 && ss <= 1.0)
		    {
			sDouble dist = calc_PointDistance(px1 + ss * dpx, py1 + ss * dpy, qx1_, qy1_);
			
			if (line_distance < 0.0)
			{
			    line_distance = dist;
			}
			else
			{
			    line_distance = sMIN(line_distance, dist);
			}
		    }
		    if (ss_ >= 0 && ss_ <= 1.0)
		    {
			sDouble dist = calc_PointDistance(px1 + ss_ * dpx, py1 + ss_ * dpy, qx1__, qy1__);
			
			if (line_distance < 0.0)
			{
			    line_distance = dist;
			}
			else
			{
			    line_distance = sMIN(line_distance, dist);
			}
		    }		    
		}
	    }
	}

	sDouble pair_dist1 = calc_PointDistance(px1, py1, qx1, qy1);
	sDouble pair_dist2 = calc_PointDistance(px1, py1, qx2, qy2);
	sDouble pair_dist3 = calc_PointDistance(px2, py2, qx1, qy1);
	sDouble pair_dist4 = calc_PointDistance(px2, py2, qx2, qy2);

	if (line_distance < 0.0)
	{
	    line_distance = sMIN(pair_dist1, sMIN(pair_dist2, sMIN(pair_dist3, pair_dist4)));
	}
	else
	{
	    line_distance = sMIN(line_distance, sMIN(pair_dist1, sMIN(pair_dist2, sMIN(pair_dist3, pair_dist4))));
	}
	
	return line_distance;
    }


    sDouble s2DMap::calc_PointDistance(sInt_32 line_1_u_id,
				       sInt_32 line_1_v_id,
				       sInt_32 point_id) const
    {
	sDouble px1, py1, px2, py2;
	sDouble x, y;

	px1 = m_Locations[line_1_u_id].m_x;
	py1 = m_Locations[line_1_u_id].m_y;

	px2 = m_Locations[line_1_v_id].m_x;
	py2 = m_Locations[line_1_v_id].m_y;

	x = m_Locations[point_id].m_x;
	y = m_Locations[point_id].m_y;

	return calc_PointDistance(px1, py1, px2, py2, x, y);
    }

    
    sDouble s2DMap::calc_PointDistance(sDouble px1, sDouble py1, sDouble px2, sDouble py2,
				       sDouble x, sDouble y) const		
    {
	sDouble point_distance = -1.0;
	#ifdef sDEBUG
	{
	    /*
	    printf("Line P: %.3f, %.3f, %.3f, %.3f\n", px1, py1, px2, py2);
	    printf("Point: %.3f, %.3f\n", x, y);	    
	    */
        }
	#endif

	sDouble dpx = px2 - px1;
	sDouble dpy = py2 - py1;

	sDouble npx = dpy;
	sDouble npy = -dpx;	
	
//	x + ss * npx = px1 + tt * dpx;
//	y + ss * npy = py1 + tt * dpy;
	

	sDouble dd = npx * dpy - npy * dpx;
	sASSERT(sABS(dd) > s_EPSILON);
	
	sDouble ss = (y * dpx - py1 * dpx - x * dpy + px1 * dpy) / dd;
	sDouble tt;

	if (sABS(dpx) > sABS(dpy))
	{
	    tt = (x + ss * npx - px1) / dpx;
	}
	else
	{
	    tt = (y + ss * npy - py1) / dpy;
	}
	
	if (tt >= 0.0 && tt <= 1.0)
	{
	    sDouble dist = calc_PointDistance(x, y, px1 + tt * dpx, py1 + tt * dpy);
	    
	    if (point_distance < 0.0)
	    {
		point_distance = dist;
	    }
	    else
	    {
		point_distance = sMIN(point_distance, dist);
	    }
	}

	sDouble pair_dist1 = calc_PointDistance(px1, py1, x, y);
	sDouble pair_dist2 = calc_PointDistance(px2, py2, x, y);

	if (point_distance < 0.0)
	{
	    point_distance = sMIN(pair_dist1, pair_dist2);
	}
	else
	{
	    point_distance = sMIN(point_distance, sMIN(pair_dist1, pair_dist2));
	}
	
	return point_distance;	
    }    

    
/*----------------------------------------------------------------------------*/
    
    sInt_32 s2DMap::find_NearestLocation(sInt_32 location_id) const
    {
	sDouble dummy_distance;

	return find_NearestLocation(location_id, dummy_distance);
    }

    
    sInt_32 s2DMap::find_NearestLocation(sInt_32 location_id, sDouble &distance) const
    {
	sInt_32 nearest_id = -1;
	sDouble nearest_distance = -1.0;
	
	for (Locations_vector::const_iterator location = m_Locations.begin(); location != m_Locations.end(); ++location)
	{
	    sInt_32 neighbor_location_id = location->m_id;

	    if (location_id != neighbor_location_id)
	    {
		if (nearest_id < 0)
		{
		    nearest_id = neighbor_location_id;
		    nearest_distance = m_shortest_Distances[location_id][neighbor_location_id];
		}
		else
		{
		    sDouble neighbor_distance = m_shortest_Distances[location_id][neighbor_location_id];

		    if (neighbor_distance < nearest_distance)
		    {
			nearest_id = neighbor_location_id;
			nearest_distance = neighbor_distance;
		    }
		}
	    }
	}
	distance = nearest_distance;
	
	return nearest_id;
    }

/*----------------------------------------------------------------------------*/
    
    void s2DMap::to_Screen(const sString &indent) const
    {
	to_Stream(stdout, indent);
    }

    
    void s2DMap::to_Stream(FILE *fw, const sString &indent) const
    {
	sInt_32 N_locations = m_Locations.size();
	fprintf(fw, "%s2DMap: (|Locations| = %d)\n", indent.c_str(), N_locations);


	if (!m_straight_Distances.empty())
	{
	    sInt_32 N_straight_distances = m_straight_Distances.size();
	    fprintf(fw, "%s%sstraight distances {\n", indent.c_str(), s_INDENT.c_str());
	
	    for (sInt_32 s_id = 0; s_id < N_straight_distances; ++s_id)
	    {
		fprintf(fw, "%s%s%d (%.3f, %.3f):", indent.c_str(), s2_INDENT.c_str(), s_id, m_Locations[s_id].m_x, m_Locations[s_id].m_y);

		sInt_32 N_location_straight_distances = m_straight_Distances[s_id].size();
		for (sInt_32 t_id = 0; t_id < N_location_straight_distances; ++t_id)
		{
		    fprintf(fw, " %.3f", m_straight_Distances[s_id][t_id]);
		}
		fprintf(fw, "}\n");
	    }
	    fprintf(fw, "%s%s}\n", indent.c_str(), s_INDENT.c_str());
	}
	
	if (!m_shortest_Distances.empty())
	{
	    sInt_32 N_shortest_distances = m_shortest_Distances.size();
	    fprintf(fw, "%s%sshortest distances {\n", indent.c_str(), s_INDENT.c_str());
	
	    for (sInt_32 s_id = 0; s_id < N_shortest_distances; ++s_id)
	    {
		fprintf(fw, "%s%s%d (%.3f, %.3f):", indent.c_str(), s2_INDENT.c_str(), s_id, m_Locations[s_id].m_x, m_Locations[s_id].m_y);
		
		sInt_32 N_location_shortest_distances = m_shortest_Distances[s_id].size();
		for (sInt_32 t_id = 0; t_id < N_location_shortest_distances; ++t_id)
		{
		    fprintf(fw, " %.3f", m_shortest_Distances[s_id][t_id]);
		}
		fprintf(fw, "}\n");
	    }
	    fprintf(fw, "%s%s}\n", indent.c_str(), s_INDENT.c_str());
	}
	
	fprintf(fw, "%s%snetwork {\n", indent.c_str(), s_INDENT.c_str());
	m_Network.to_Stream(fw, indent + s2_INDENT);
	fprintf(fw, "%s%s}\n", indent.c_str(), s_INDENT.c_str());		
    }	 

    
    sResult s2DMap::to_File_mapR(const sString &filename, const sString &indent) const
    {
	FILE *fw;

	if ((fw = fopen(filename.c_str(), "w")) == NULL)
	{
	    return s2D_MAP_OPEN_ERROR;
	}
	
	to_Stream_mapR(fw, indent);
	fclose(fw);

	return sRESULT_SUCCESS;
    }

    
    void s2DMap::to_Stream_mapR(FILE *fw, const sString &indent) const
    {
	sInt_32 N_locations = m_Locations.size();
	fprintf(fw, "%sLocations: %d\n", indent.c_str(), N_locations);
	
	for (sInt_32 id = 0; id < N_locations; ++id)
	{
	    fprintf(fw, "%s%d: %.3f, %.3f\n", indent.c_str(), id, m_Locations[id].m_x, m_Locations[id].m_y); 
	}
	m_Network.to_Stream_mapR(fw, indent);
    }
    

    sResult s2DMap::from_File_mapR(const sString &filename)
    {
	sResult result;
	FILE *fr;

	if ((fr = fopen(filename.c_str(), "r")) == NULL)
	{
	    return s2D_MAP_OPEN_ERROR;
	}
	
	if (sFAILED(result = from_Stream_mapR(fr)))
	{
	    fclose(fr);
	    return result;
	}
	fclose(fr);

	return sRESULT_SUCCESS;
    }

    
    sResult s2DMap::from_Stream_mapR(FILE *fr)
    {
	sResult result;
	
	sInt_32 N_locations;
	fscanf(fr, "Locations: %d\n", &N_locations);
	
	m_Locations.resize(N_locations);

	for (sInt_32 id = 0; id < N_locations; ++id)
	{
	    Location location;
	    
	    fscanf(fr, "%d: %lf, %lf\n", &location.m_id, &location.m_x, &location.m_y);
	    sASSERT(id == location.m_id);

	    m_Locations[location.m_id] = location;
	}
	if (sFAILED(result = m_Network.from_Stream_mapR(fr)))
	{
	    return result;
	}	
	return sRESULT_SUCCESS;
    }


    sResult s2DMap::from_File_map(const sString &filename)
    {
	sResult result;
	FILE *fr;

	if ((fr = fopen(filename.c_str(), "r")) == NULL)
	{
	    return s2D_MAP_OPEN_ERROR;
	}
	
	if (sFAILED(result = from_Stream_map(fr)))
	{
	    fclose(fr);
	    return result;
	}
	fclose(fr);

	return sRESULT_SUCCESS;	
    }

    
    sResult s2DMap::from_Stream_map(FILE *fr)
    {
	sResult result;
	
	if (sFAILED(result = m_Network.from_Stream_map(fr)))
	{
	    return result;
	}

	m_Locations.resize(m_Network.m_Vertices.size());

	for (sInt_32 y = 0; y < m_Network.m_y_size; ++y)
	{
	    for (sInt_32 x = 0; x < m_Network.m_x_size; ++x)
	    {
		sInt_32 location_id = m_Network.m_Matrix[y * m_Network.m_x_size + x];
		
		if (location_id >= 0)
		{
		    m_Locations[location_id] = Location(location_id, x, y);
		}
	    }
	}

	return sRESULT_SUCCESS;
    }
        

/*----------------------------------------------------------------------------*/

} // namespace boOX
    
