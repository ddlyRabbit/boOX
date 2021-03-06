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
/* mapR_convert_main.h / 0-279_zenon                                          */
/*----------------------------------------------------------------------------*/
//
// Continuous Multi-Agent Path Finding (MAPF-R) map convertor - main program.
//
// This program takes a grid map and converts it into a continuous map
// with locations having real valued coordinates.
//
/*----------------------------------------------------------------------------*/


#ifndef __MAP_R_CONVERT_MAIN_H__
#define __MAP_R_CONVERT_MAIN_H__

#include "config.h"
#include "compile.h"
#include "defs.h"
#include "version.h"

using namespace std;


/*----------------------------------------------------------------------------*/

namespace boOX
{


/*----------------------------------------------------------------------------*/

    struct sCommandParameters
    {
	sCommandParameters();
        /*--------------------------------*/

	sString m_input_map_filename;		
	sString m_output_mapR_filename;
    };


/*----------------------------------------------------------------------------*/

    void print_IntroductoryMessage(void);
    void print_ConcludingMessage(void);
    void print_Help(void);
    
    sResult parse_CommandLineParameter(const sString &parameter, sCommandParameters &parameters);
    sResult convert_GridMap2RealMap(const sCommandParameters &parameters);


/*----------------------------------------------------------------------------*/

} // namespace boOX


#endif /* __MAP_R_CONVERT_MAIN_H__ */
