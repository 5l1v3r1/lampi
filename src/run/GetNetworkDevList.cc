/*
 * Copyright 2002-2004. The Regents of the University of
 * California. This material was produced under U.S. Government
 * contract W-7405-ENG-36 for Los Alamos National Laboratory, which is
 * operated by the University of California for the U.S. Department of
 * Energy. The Government is granted for itself and others acting on
 * its behalf a paid-up, nonexclusive, irrevocable worldwide license
 * in this material to reproduce, prepare derivative works, and
 * perform publicly and display publicly. Beginning five (5) years
 * after October 10,2002 subject to additional five-year worldwide
 * renewals, the Government is granted for itself and others acting on
 * its behalf a paid-up, nonexclusive, irrevocable worldwide license
 * in this material to reproduce, prepare derivative works, distribute
 * copies to the public, perform publicly and display publicly, and to
 * permit others to do so. NEITHER THE UNITED STATES NOR THE UNITED
 * STATES DEPARTMENT OF ENERGY, NOR THE UNIVERSITY OF CALIFORNIA, NOR
 * ANY OF THEIR EMPLOYEES, MAKES ANY WARRANTY, EXPRESS OR IMPLIED, OR
 * ASSUMES ANY LEGAL LIABILITY OR RESPONSIBILITY FOR THE ACCURACY,
 * COMPLETENESS, OR USEFULNESS OF ANY INFORMATION, APPARATUS, PRODUCT,
 * OR PROCESS DISCLOSED, OR REPRESENTS THAT ITS USE WOULD NOT INFRINGE
 * PRIVATELY OWNED RIGHTS.

 * Additionally, this program is free software; you can distribute it
 * and/or modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or any later version.  Accordingly, this
 * program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 */
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/*
 *  This routine is used to get the list of "computational" network
 *    devices to be used on each host.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "internal/profiler.h"
#include "internal/constants.h"
#include "internal/types.h"
#include "internal/new.h"
#include "internal/log.h"
#include "run/Input.h"
#include "run/Run.h"
#include "run/RunParams.h"
#include "util/Utility.h"

struct NetWorkDevs {
    char InputName[ULM_MAX_LEN_NETWORK_DEVTYPE];
    PathType_t PathType;
} _ulm_NetworkDevsSupported[] = {
    {
    "gm", PATH_GM}, {
    "udp", PATH_UDP}, {
    "tcp", PATH_TCP}, {
    "quadrics", PATH_QUADRICS}, {
    "sharedmemory", PATH_SHAREDMEM}, {
    "ib", PATH_IB}
};

void GetNetworkDevListNoInput(const char *InfoStream)
{
    /* create default defive type count */
    int cnt = 0;
#if ENABLE_UDP
    cnt++;
#endif

#if ENABLE_TCP
    cnt++;
#endif

#if ENABLE_QSNET
    cnt++;
#endif

#if ENABLE_GM
    cnt++;
#endif

#if ENABLE_INFINIBAND
    cnt++;
#endif

    RunParams.NPathTypes = ulm_new(int, RunParams.NHosts);
    RunParams.ListPathTypes = ulm_new(int *, RunParams.NHosts);

    /* allocate memory to hold list */
    for (int i = 0; i < RunParams.NHosts; i++) {
        int DevCnt = 0;
        RunParams.NPathTypes[i] = cnt;
        if (cnt > 0) {
            RunParams.ListPathTypes[i] = ulm_new(int, cnt);
        } else {
            RunParams.ListPathTypes[i] = 0;
        }
        /* fill in list */
#if ENABLE_UDP
        RunParams.ListPathTypes[i][DevCnt++] = PATH_UDP;
#endif
#if ENABLE_TCP
        RunParams.ListPathTypes[i][DevCnt++] = PATH_TCP;
#endif
#if ENABLE_QSNET
        RunParams.ListPathTypes[i][DevCnt++] = PATH_QUADRICS;
        RunParams.Networks.UseQSW = 1;
#endif
#if ENABLE_GM
        RunParams.ListPathTypes[i][DevCnt++] = PATH_GM;
#endif
#if ENABLE_INFINIBAND
        RunParams.ListPathTypes[i][DevCnt++] = PATH_IB;
#endif
        if (DevCnt == 0) {
            ulm_dbg(("Warning: No network devices configured\n"));
        }
    }                           /* end i loop */
}

void GetNetworkDevList(const char *InfoStream)
{
    int NHostLists, cnt = 0, Flag;

    int _ulm_NumNetworkDevsSupported =
        sizeof(_ulm_NetworkDevsSupported) / sizeof(struct NetWorkDevs);

    /* Setup array to store all device types used in the job.
     *   This is needed for a scalable wireup.  We will add
     *   to a host's device list those devices that the devices
     *   for which the given host just passes the parameter through
     *   w/o doing anything with the data */
    int nTotalJobDevs = 0;
    int *totalJobDevList = ulm_new(int, _ulm_NumNetworkDevsSupported);

    /*  find HostList index in database */
    int OptionIndex = MatchOption("NetworkDevices");
    if (OptionIndex < 0) {
        ulm_err(("Error: Option HostList not found in Input parameter database\n"));
        Abort();
    }

    /* allocate memory for number of interface device types */
    RunParams.NPathTypes = ulm_new(int, RunParams.NHosts);
    RunParams.ListPathTypes = ulm_new(int *, RunParams.NHosts);

    /* determine if each host has its own data set, or 1 for all */
    ParseString HostDevData(Options[OptionIndex].InputData, 4, " ,\t\n");
    NHostLists = HostDevData.GetNSubStrings();
    if ((NHostLists != 1) && (NHostLists != RunParams.NHosts)) {
        ulm_err(("Error: Wrong number of arguments for NetworkDevices.\n"));
        ulm_err(("Found %d arguments, but should be either 1 or %d\n",
                 NHostLists, RunParams.NHosts));
        ulm_err(("Input String: %s\n", Options[OptionIndex].InputData));
        Abort();
    }

    /* process list */
    if (NHostLists == 1) {
        /* one device list for all hosts, e.g. all hosts will 
         *   use the same device types */
        ParseString DevList((*HostDevData.begin()), 1, ":");
        /* check list - and compute number of devices */
        cnt = 0;
        for (ParseString::iterator Dev = DevList.begin();
             Dev != DevList.end(); Dev++) {
            /* loop over supported devices */
            int len = strlen(*Dev);
            Flag = 0;
            for (int k = 0; k < _ulm_NumNetworkDevsSupported; k++) {
                if (((size_t) len ==
                     strlen(_ulm_NetworkDevsSupported[k].InputName))
                    &&
                    (strncmp
                     ((*Dev), _ulm_NetworkDevsSupported[k].InputName,
                      len) == 0)) {
                    Flag = 1;
                    /* check and see if we need to add this to 
                     *   totalJobDevList */
                    cnt++;
                }
            }
            if (!Flag) {
                ulm_err(("Error: Unrecognized device type : %s ", *Dev));
                Abort();
            }
        }
        /* allocate memory to hold list */
        for (int i = 0; i < RunParams.NHosts; i++) {
            RunParams.NPathTypes[i] = cnt;
            RunParams.ListPathTypes[i] = ulm_new(int, cnt);
            /* fill in list */
            int DevCnt = 0;
            for (ParseString::iterator Dev = DevList.begin();
                 Dev != DevList.end(); Dev++) {
                int len = strlen(*Dev);
                /* find device type */
                for (int k = 0; k < _ulm_NumNetworkDevsSupported; k++) {
                    if (((size_t) len ==
                         strlen(_ulm_NetworkDevsSupported[k].InputName))
                        &&
                        (strncmp
                         ((*Dev), _ulm_NetworkDevsSupported[k].InputName,
                          len) == 0)) {
                        RunParams.ListPathTypes[i][DevCnt] =
                            _ulm_NetworkDevsSupported[k].PathType;
                        DevCnt++;
                    }
                }               /* end k loop */
            }                   /* end loop over devices */
        }                       /* end i loop */

    } else {
        /* one network device list per host */
        /*  figure out how many device types will be used
         *  in the run */
        for (ParseString::iterator Host = HostDevData.begin();
             Host != HostDevData.end(); Host++) {
            ParseString DevList(*Host, 1, ":");
            /* check list - and compute number of devices */
            for (ParseString::iterator Dev = DevList.begin();
                 Dev != DevList.end(); Dev++) {
                /* loop over supported devices */
                int len = strlen(*Dev);
                Flag = 0;
                int devIndex = -1;
                for (int k = 0; k < _ulm_NumNetworkDevsSupported; k++) {
                    if (((size_t) len ==
                         strlen(_ulm_NetworkDevsSupported[k].InputName))
                        &&
                        (strncmp
                         ((*Dev), _ulm_NetworkDevsSupported[k].InputName,
                          len) == 0)) {
                        Flag = 1;
                        devIndex = k;
                        break;
                    }
                }
                if (Flag) {
                    int found = 0;
                    for (int dev = 0; dev < cnt; dev++) {
                        if (totalJobDevList[dev] ==
                            _ulm_NetworkDevsSupported[devIndex].PathType) {
                            found = 1;
                            break;
                        }
                    }

                    if (!found) {
                        totalJobDevList[nTotalJobDevs] =
                            _ulm_NetworkDevsSupported[devIndex].PathType;
                        nTotalJobDevs++;
                    }
                }               /* end if (Flag) */
                if (!Flag) {
                    ulm_err(("Error: Unrecognized device type : %s\n",
                             *Dev));
                    Abort();
                }
            }
        }                       /* end loop over hosts (i) */
        /* fill in the device list for this host */
        int HostCnt = 0;
        for (ParseString::iterator Host = HostDevData.begin();
             Host != HostDevData.end(); Host++) {
            ParseString DevList(*Host, 1, ":");
            /* allocate memory to hold list */
            RunParams.ListPathTypes[HostCnt] =
                ulm_new(int, _ulm_NumNetworkDevsSupported);
            /* fill in list */
            int DevCnt = 0;
            for (ParseString::iterator Dev = DevList.begin();
                 Dev != DevList.end(); Dev++) {
                int len = strlen(*Dev);
                for (int k = 0; k < _ulm_NumNetworkDevsSupported; k++) {        /* find device type */
                    if (((size_t) len ==
                         strlen(_ulm_NetworkDevsSupported[k].InputName))
                        &&
                        (strncmp
                         ((*Dev), _ulm_NetworkDevsSupported[k].InputName,
                          len) == 0)) {
                        RunParams.
                            ListPathTypes[HostCnt][DevCnt] =
                            _ulm_NetworkDevsSupported[k].PathType;
                        DevCnt++;
                    }
                }               /* end k loop */
            }                   /* end loop over devices */
            /* fill in the devices used by other hosts, but not
             *   this one */
            /* loop over list of all devices used */
            for (int totDevs = 0; totDevs < nTotalJobDevs; totDevs++) {
                int devID = totalJobDevList[totDevs];
                /* loop over devices in current host's list */
                int Found = 0;
                for (int hDevs = 0; hDevs < DevCnt; hDevs++) {
                    if (devID == RunParams.ListPathTypes[HostCnt][hDevs]) {
                        Found = 1;
                        break;
                    }
                }
                if (!Found) {
                    RunParams.ListPathTypes[HostCnt][DevCnt] = -devID;
                    DevCnt++;
                }
            }
            /* fill in device count */
            RunParams.NPathTypes[HostCnt] = DevCnt;

            HostCnt++;
        }                       /* end loop over hosts (i) */
    }

    /* free temprorary resources */
    ulm_delete(totalJobDevList);
}
