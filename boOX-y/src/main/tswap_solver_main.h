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
/* tswap_solver_main.h / 0-279_zenon                                          */
/*----------------------------------------------------------------------------*/
//
// Token Swapping Problem Solver - main program.
//
// A CBS-based solver for token swapping problem.
//
/*----------------------------------------------------------------------------*/


#ifndef __TSWAP_SOLVER_MAIN_H__
#define __TSWAP_SOLVER_MAIN_H__

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

	sInt_32 m_cost_limit;

	sString m_input_filename;		
	sString m_output_filename;
    };


/*----------------------------------------------------------------------------*/

    void print_IntroductoryMessage(void);
    void print_ConcludingMessage(void);
    void print_Help(void);
    
    sResult parse_CommandLineParameter(const sString &parameter, sCommandParameters &parameters);
    sResult solve_TokenSwappingInstance(const sCommandParameters &parameters);


/*----------------------------------------------------------------------------*/

} // namespace boOX


#endif /* __TSWAP_SOLVER_MAIN_H__ */
