/*
 * Copyright 2002-2003. The Regents of the University of California. This material 
 * was produced under U.S. Government contract W-7405-ENG-36 for Los Alamos 
 * National Laboratory, which is operated by the University of California for 
 * the U.S. Department of Energy. The Government is granted for itself and 
 * others acting on its behalf a paid-up, nonexclusive, irrevocable worldwide 
 * license in this material to reproduce, prepare derivative works, and 
 * perform publicly and display publicly. Beginning five (5) years after 
 * October 10,2002 subject to additional five-year worldwide renewals, the 
 * Government is granted for itself and others acting on its behalf a paid-up, 
 * nonexclusive, irrevocable worldwide license in this material to reproduce, 
 * prepare derivative works, distribute copies to the public, perform publicly 
 * and display publicly, and to permit others to do so. NEITHER THE UNITED 
 * STATES NOR THE UNITED STATES DEPARTMENT OF ENERGY, NOR THE UNIVERSITY OF 
 * CALIFORNIA, NOR ANY OF THEIR EMPLOYEES, MAKES ANY WARRANTY, EXPRESS OR 
 * IMPLIED, OR ASSUMES ANY LEGAL LIABILITY OR RESPONSIBILITY FOR THE ACCURACY, 
 * COMPLETENESS, OR USEFULNESS OF ANY INFORMATION, APPARATUS, PRODUCT, OR 
 * PROCESS DISCLOSED, OR REPRESENTS THAT ITS USE WOULD NOT INFRINGE PRIVATELY 
 * OWNED RIGHTS.

 * Additionally, this program is free software; you can distribute it and/or 
 * modify it under the terms of the GNU Lesser General Public License as 
 * published by the Free Software Foundation; either version 2 of the License, 
 * or any later version.  Accordingly, this program is distributed in the hope 
 * that it will be useful, but WITHOUT ANY WARRANTY; without even the implied 
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * GNU Lesser General Public License for more details.
 */
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/



/***
 *** GetClientCpus.cc
 ***
 *** Sung-Eun Choi (sungeun@lanl.gov)
 *** 4 October 2000
 ***
 *** Parse the list of cpus named by the user
 ***
 *** For now (and maybe forever), the argument list is a comma
 ***  separated list of cpu numbers (0..127 on nirvana) to use on
 ***  each host.  Allowing the use of host specific cpus would be
 ***  nice, but could get messy and confusing.  For example:
 ***
 ***          % mpirun -H n01,n01 -cpulist n01:1,n01:2
 ***
 *** Also, it would be nice if this could deduce the number of cpus
 ***  to use based on the -cpulist argument.
 ***
 ***/

#ifdef __mips
#include "internal/profiler.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "internal/constants.h"
#include "internal/new.h"
#include "internal/types.h"
#include "run/Run.h"
#include "run/globals.h"
#include "internal/new.h"
#include "run/Input.h"
#include "util/ParseString.h"
#endif                          //ifdef __mips

#include "internal/log.h"

#ifdef __mips
void GetClientCpus(const char *InfoStream)
{
    ParseString::iterator pi;
    int i, len;
    char slist[] = { ", " };
    int OptionIndex = MatchOption("CpuList", ULMInputOptions,
                                  SizeOfInputOptionsDB);
    ParseString cpulist(ULMInputOptions[OptionIndex].InputData, 1, slist);

    len = cpulist.GetNSubStrings();
    RunParameters.CpuListLen = len;
    RunParameters.CpuList = ulm_new(int, len);

    for (pi = cpulist.begin(), i = 0; pi != cpulist.end(); pi++, i++) {
        RunParameters.CpuList[i] = atoi(*pi);
        ulm_info(("adding cpu %d\n", RunParameters.CpuList[i]));
    }
}

#else

#include <stdio.h>
void GetClientCpus(const char *InfoStream)
{
    ulm_warn(("Warning: '-cpulist' not implemented for this platform.\n"));
}

#endif
