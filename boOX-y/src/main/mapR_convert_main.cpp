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
/* mapR_convert_main.cpp / 0-279_zenon                                        */
/*----------------------------------------------------------------------------*/
//
// Continuous Multi-Agent Path Finding (MAPF-R) map convertor - main program.
//
// This program takes a grid map and converts it into a continuous map (mapR)
// with locations having real valued coordinates.
//
/*----------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>
#include <unistd.h>

#include "config.h"
#include "compile.h"
#include "defs.h"
#include "version.h"

#include "core/graph.h"
#include "core/mapR.h"

#include "util/statistics.h"

#include "main/mapR_convert_main.h"


using namespace std;


/*----------------------------------------------------------------------------*/

namespace boOX
{


/*----------------------------------------------------------------------------*/

  sCommandParameters::sCommandParameters()
  {
      // nothing
  }


/*----------------------------------------------------------------------------*/

    void print_IntroductoryMessage(void)
    {
	printf("----------------------------------------------------------------\n");
	printf("%s : Continuous Map (mapR) Convertor\n", sPRODUCT); 
	printf("%s\n", sCOPYRIGHT);
	printf("================================================================\n");	
    }


    void print_ConcludingMessage(void)
    {
	printf("----------------------------------------------------------------\n");
    }


    void print_Help(void)
    {
	printf("Usage:\n");
	printf("mapR_convert_boOX  --input-map-file=<string>\n");
	printf("                   --output-mapR-file=<sting>\n");
	printf("\n");
	printf("Examples:\n");
	printf("mapR_convert_boOX --input-map-file=ost003d.map\n");
	printf("                  --output-file=ost003d.mapR\n");
	printf("\n");
	printf("Defaults: <there are no defaults>\n");
	printf("\n");
    }


    sResult convert_GridMap2RealMap(const sCommandParameters &parameters)
    {
	sResult result;
	s2DMap real_Map;

        #ifdef sSTATISTICS
	{
	    s_GlobalStatistics.enter_Phase("CONVERSION");
	}
  	#endif
	
	if (!parameters.m_input_map_filename.empty())
	{
	    result = real_Map.from_File_map(parameters.m_input_map_filename);

	    if (sFAILED(result))
	    {
		printf("Error: Failed to open grid map file %s (code = %d).\n", parameters.m_input_map_filename.c_str(), result);
		return result;
	    }
	}

	if (!parameters.m_output_mapR_filename.empty())
	{
	    result = real_Map.to_File_mapR(parameters.m_output_mapR_filename);

	    if (sFAILED(result))
	    {
		printf("Error: Failed to write continuous map (mapR) file %s (code = %d).\n", parameters.m_output_mapR_filename.c_str(), result);
		return result;
	    }
	}	

        #ifdef sSTATISTICS
	{
	    s_GlobalStatistics.leave_Phase();
	}
	#endif
	
	#ifdef sSTATISTICS
	{
	    s_GlobalStatistics.to_Screen();
	}
	#endif

	return sRESULT_SUCCESS;
    }


    sResult parse_CommandLineParameter(const sString &parameter, sCommandParameters &command_parameters)
    {
	if (parameter.find("--input-map-file=") == 0)
	{
	    command_parameters.m_input_map_filename = parameter.substr(17, parameter.size());
	}
	else if (parameter.find("--output-mapR-file=") == 0)
	{
	    command_parameters.m_output_mapR_filename = parameter.substr(19, parameter.size());
	}
	else
	{
	    return sMAP_R_CONVERT_PROGRAM_UNRECOGNIZED_PARAMETER_ERROR;
	}
	return sRESULT_SUCCESS;
    }


/*----------------------------------------------------------------------------*/

} // namespace boOX


/*----------------------------------------------------------------------------*/
// main program

int main(int argc, char **argv)
{
    sResult result;
    sCommandParameters command_parameters;

    print_IntroductoryMessage();

    if (argc >= 2 && argc <= 11)
    {
	for (int i = 1; i < argc; ++i)
	{
	    result = parse_CommandLineParameter(argv[i], command_parameters);
	    if (sFAILED(result))
	    {
		printf("Error: Cannot parse command line parameters (code = %d).\n", result);
		print_Help();

		return result;
	    }
	}
	result = convert_GridMap2RealMap(command_parameters);
	if (sFAILED(result))
	{
	    return result;
	}
    }
    else
    {
	print_Help();
    }
    return sRESULT_SUCCESS;
}

