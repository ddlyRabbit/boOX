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
/* perm_solver_main.h / 0-279_zenon                                           */
/*----------------------------------------------------------------------------*/
//
// Token Permutation Problem Solver - main program.
//
// A CBS-based solver for token permutation problem (swaps including).
//
/*----------------------------------------------------------------------------*/


#ifndef __PERM_SOLVER_MAIN_H__
#define __PERM_SOLVER_MAIN_H__

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
	enum Algorithm
	{
	    ALGORITHM_CBS,
	    ALGORITHM_CBS_PLUS,
	    ALGORITHM_CBS_PLUS_PLUS,
	    ALGORITHM_SMTCBS,
	    ALGORITHM_SMTCBS_PLUS
	};

	sCommandParameters();
        /*--------------------------------*/

	sInt_32 m_cost_limit;
	Algorithm m_algorithm;	

	sString m_input_filename;		
	sString m_output_filename;

	sDouble m_timeout;
    };


/*----------------------------------------------------------------------------*/

    void print_IntroductoryMessage(void);
    void print_ConcludingMessage(void);
    void print_Help(void);
    
    sResult parse_CommandLineParameter(const sString &parameter, sCommandParameters &parameters);
    sResult solve_TokenPermutationInstance(const sCommandParameters &parameters);


/*----------------------------------------------------------------------------*/

} // namespace boOX


#endif /* __PERM_SOLVER_MAIN_H__ */
