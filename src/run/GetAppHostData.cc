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



#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#ifdef ENABLE_BPROC
#include <sys/utsname.h>  // needed for uname
#endif
/* debug */
extern int h_errno;
/* end debug */

#include "internal/constants.h"
#include "internal/profiler.h"
#include "internal/types.h"
#include "internal/log.h"
#include "internal/new.h"
#include "run/Run.h"
#include "internal/new.h"
#include "run/globals.h"
#include "util/ParseString.h"
#include "util/Utility.h"
#include "run/Input.h"
#include "run/LSFResource.h"

/*
 * This routine simply sets NHosts to the user-supplied value.
 */
void GetAppHostCount(const char *InfoStream)
{
    int NSeparators = 2;
    char SeparatorList[] = { " , " };

    /* find HostList index in database */
    int OptionIndex =
        MatchOption("HostCount", ULMInputOptions, SizeOfInputOptionsDB);
    if (OptionIndex < 0) {
        ulm_err(("Error: Option HostCount not found in input parameter database\n"));
        Abort();
    }

    /* parse host list */
    ParseString HostData(ULMInputOptions[OptionIndex].InputData,
                         NSeparators, SeparatorList);

    /* set number of Host in job */
    RunParameters.NHosts = atoi(*HostData.begin());
    RunParameters.NHostsSet = true;
}




/*
 * get the primary name of this host from gethostname, if none is specified
 */
void GetMpirunHostnameNoInput(const char *InfoStream)
{
#ifdef ENABLE_BPROC
  /* gives a hostname of "n-1", which fails later */
    int node = bproc_currnode();
    sprintf(RunParameters.mpirunName, "n%d", node);


    // try to construct a dotted IP of the mpirun node:
    while (1) {
      struct hostent *hptr;
      struct utsname myname;
      if (uname(&myname)<0) break;
      if ( (hptr=gethostbyname(myname.nodename))==NULL) break;
      if (hptr->h_addrtype!=AF_INET) break;

      // multiple ip addresses:  h_addr_list[*].  Lets just take first one:
      if (NULL==hptr->h_addr_list[0]) break;
      in_addr *tin = (in_addr *) hptr->h_addr_list[0];

      // convert to dotted ip string:
      char *tmp = inet_ntoa( *tin );
      strncpy(RunParameters.mpirunName,tmp,ULM_MAX_HOSTNAME_LEN);
      break;
    }

#else
    HostName_t tmp;
    struct hostent *localHostInfo;
    tmp[ULM_MAX_HOSTNAME_LEN - 1] = '\0';
    if (gethostname(tmp, ULM_MAX_HOSTNAME_LEN - 1) == -1) {
        ulm_err(("Error: Can't find local hostname!\n"));
        Abort();
    }
    if ((localHostInfo = gethostbyname(tmp)) == NULL) {
        ulm_err(("Error: Hostname look-up failed for %s\n", tmp));
        Abort();
    }
    strncpy(RunParameters.mpirunName,
            (strlen(localHostInfo->h_name) >=
             ULM_MAX_HOSTNAME_LEN) ? tmp : localHostInfo->h_name,
            ULM_MAX_HOSTNAME_LEN);
#endif
}


/*
 * set the name of this host, trying a number of patterns
 */
void GetMpirunHostname(const char *InfoStream)
{
    HostName_t tmp, myName, myDomain;
    struct hostent *localHostInfo;
    int NSeparators = 2, j, k, myNameLen;
    char SeparatorList[] = { " , " };
    bool success = false, foundDomain = false, isAddress = true;

    /* find HostList index in database */
    int OptionIndex = MatchOption("MpirunHostname", ULMInputOptions,
                                  SizeOfInputOptionsDB);
    if (OptionIndex < 0) {
        ulm_err(("Error: Option MpirunHostname not found in input parameter database\n"));
        Abort();
    }

    myName[ULM_MAX_HOSTNAME_LEN - 1] = '\0';
    if (gethostname(myName, ULM_MAX_HOSTNAME_LEN - 1) == -1) {
        ulm_err(("Error: Can't find local hostname!\n"));
        Abort();
    }

    myNameLen = strlen(myName);

    /* check string to see if it is an IP address - in which case
     * we skip the domain check
     */
    for (j = 0; j < myNameLen; j++) {
        if ((myName[j] != '.') && ((myName[j] < '0') || (myName[j] > '9'))) {
            isAddress = false;
            break;
        }
    }
    
    k = 0;
    if (!isAddress) {
        for (j = 0; j < myNameLen; j++) {
            if (!foundDomain && (myName[j] == '.')) {
                foundDomain = true;
            }
            if (foundDomain) {
                myDomain[k++] = myName[j];
                myName[j] = '\0';
            }
        }
    }
    myDomain[k] = '\0';

    /* parse host list */
    ParseString HostnameData(ULMInputOptions[OptionIndex].InputData,
                             NSeparators, SeparatorList);

    /* the name of this host for server/client info exchange and control */
    RunParameters.mpirunName[ULM_MAX_HOSTNAME_LEN - 1] = '\0';

    for (ParseString::iterator i = HostnameData.begin(); i != HostnameData.end(); i++) {
        sprintf(tmp, "%s", *i);
        if ((localHostInfo = gethostbyname(tmp)) != NULL) {
            strncpy(RunParameters.mpirunName, (strlen(localHostInfo->h_name) >= 
                ULM_MAX_HOSTNAME_LEN) ? tmp : localHostInfo->h_name,
                ULM_MAX_HOSTNAME_LEN - 1);
            success = true;
            break;
        }
        sprintf(tmp, "%s-%s%s", myName, *i, foundDomain ? myDomain : "");
        if ((localHostInfo = gethostbyname(tmp)) != NULL) {
            strncpy(RunParameters.mpirunName, (strlen(localHostInfo->h_name) >= 
                ULM_MAX_HOSTNAME_LEN) ? tmp : localHostInfo->h_name,
                ULM_MAX_HOSTNAME_LEN - 1);
            success = true;
            break;
        }
        sprintf(tmp, "%s%s%s", myName, *i, foundDomain ? myDomain : "");
        if ((localHostInfo = gethostbyname(tmp)) != NULL) {
            strncpy(RunParameters.mpirunName, (strlen(localHostInfo->h_name) >= 
                ULM_MAX_HOSTNAME_LEN) ? tmp : localHostInfo->h_name,
                ULM_MAX_HOSTNAME_LEN - 1);
            success = true;
            break;
        }
        sprintf(tmp, "%s-%s%s", *i, myName, foundDomain ? myDomain : "");
        if ((localHostInfo = gethostbyname(tmp)) != NULL) {
            strncpy(RunParameters.mpirunName, (strlen(localHostInfo->h_name) >= 
                ULM_MAX_HOSTNAME_LEN) ? tmp : localHostInfo->h_name,
                ULM_MAX_HOSTNAME_LEN - 1);
            success = true;
            break;
        }
        sprintf(tmp, "%s%s%s", *i, myName, foundDomain ? myDomain : "");
        if ((localHostInfo = gethostbyname(tmp)) != NULL) {
            strncpy(RunParameters.mpirunName, (strlen(localHostInfo->h_name) >= 
                ULM_MAX_HOSTNAME_LEN) ? tmp : localHostInfo->h_name,
                ULM_MAX_HOSTNAME_LEN - 1);
            success = true;
            break;
        }
    }

    if (!success) {
        sprintf(RunParameters.mpirunName, "%s%s", myName, foundDomain ? myDomain : "");
        ulm_warn(("mpirun: Warning: Option MpirunHostname (-s %s) did not find a valid hostname, using default, %s!\n",
            ULMInputOptions[OptionIndex].InputData, RunParameters.mpirunName));
    }
}


/*
 * This routine sets up the host information, when this data is
 * specified in one or more of the input sources.
 */
void GetAppHostData(const char *InfoStream)
{
    struct hostent *NetEntry;
    int NSeparators = 2, cnt = 0;
    int first, last, scan_cnt;
    char SeparatorList[] = { " , " };
    size_t len;
    ParseString::iterator i;

    /* find HostList index in database */
    int OptionIndex =
        MatchOption("HostList", ULMInputOptions, SizeOfInputOptionsDB);
    if (OptionIndex < 0) {
        ulm_err(("Error: Option HostList not found in input parameter database\n"));
        Abort();
    }

    /* parse host list */
    ParseString HostData(ULMInputOptions[OptionIndex].InputData,
                         NSeparators, SeparatorList);

    /* count the number of hosts if Bproc is being used */
    if (RunParameters.UseBproc) {
       for (i = HostData.begin(); i != HostData.end(); i++) {
           scan_cnt = sscanf(*i, "%d - %d", &first, &last);
           switch (scan_cnt) {
               case 1:
                   cnt++;
                   break;
               case 2:
                   cnt += (last >= first) ? ((last - first) + 1) : 0;
                   break;
               default:
                   break;
           }
       } 
       if (cnt == 0) {
           ulm_err(("Error: No hostnames for BPROC nodes parsed from %d -H substrings\n",
               HostData.GetNSubStrings()));
           Abort();
       }
    }

    /* set number of Host in job */
    RunParameters.HostListSize = (cnt == 0) ? HostData.GetNSubStrings() : cnt;

    /* allocate memory for List of Hosts */
    RunParameters.HostList = ulm_new(HostName_t,  RunParameters.HostListSize);

    cnt = 0;
    /* fill in host data */
    for (i = HostData.begin(); i != HostData.end(); i++) {
        if (RunParameters.UseBproc) {
           scan_cnt = sscanf(*i, "%d - %d", &first, &last);
           switch (scan_cnt) {
               case 1:
                   sprintf(RunParameters.HostList[cnt++], "%d", first);
                   break;
               case 2:
                   if (last >= first) {
                       for (scan_cnt = first ; scan_cnt <= last; scan_cnt++) {
                           sprintf(RunParameters.HostList[cnt++], "%d", scan_cnt);
                       }
                   }
                   break;
               default:
                   break;
           }
        }
        else {
            NetEntry = gethostbyname(*i);
            if (NetEntry == NULL) {
                ulm_err(("Error: Hostname look-up failed for %s\n", *i));
                Abort();
            }
            len = strlen(NetEntry->h_name);
            if (len >= ULM_MAX_HOSTNAME_LEN) {
                ulm_err(("Error: Hostname too long for library buffer, length = %ld\n", len));
                Abort();
            }
	    bzero(RunParameters.HostList[cnt],len+1);
            strncpy((char *) RunParameters.HostList[cnt], NetEntry->h_name,
                len);
            cnt++;
        }
    }
}

/*
 * This routine sets up the host information, when this data is
 * specified in one or more of the input sources.
 */
void GetAppHostDataFromMachineFile(const char *InfoStream)
{
    /* find HostList index in database */
    int OptionIndex =
        MatchOption("MachineFile", ULMInputOptions, SizeOfInputOptionsDB);
    if (OptionIndex < 0) {
        ulm_err(("Error: Option MachineFile not found in input parameter database\n"));
        Abort();
    }

    FILE *fp = fopen(ULMInputOptions[OptionIndex].InputData, "r+");
    if(fp == NULL) {
        ulm_err(("error: unable to open %s for input\n", ULMInputOptions[OptionIndex].InputData));
        Abort();
    }

    int allocated = 128;
    RunParameters.HostList = (char (*)[ULM_MAX_HOSTNAME_LEN])
        malloc(allocated * ULM_MAX_HOSTNAME_LEN);
    int cnt = 0;
    char buff[ULM_MAX_CONF_FILELINE_LEN];
    while(fgets(buff, sizeof(buff)-1, fp) != NULL) {

        /* strip off any leading whitespace */
        char *hostname = buff;
        while(*hostname && isspace(*hostname))
            hostname++;

        /* strip off any trailing whitespace */
        size_t len = strlen(hostname);
        char *ptr = hostname + len - 1;
        while(ptr > hostname && isspace(*ptr)) {
            *ptr = '\0';
            ptr--;
        }

        /* if index exceeds allocated space grow array */
        if(cnt >= allocated) {
            allocated <<= 1;
            RunParameters.HostList = (char (*)[ULM_MAX_HOSTNAME_LEN])
                realloc(RunParameters.HostList, allocated * ULM_MAX_HOSTNAME_LEN);
        }

        /* resolve hostname */
        if (RunParameters.UseBproc) {
           int first,last;
           int scan_cnt = sscanf(hostname, "%d - %d", &first, &last);
           switch (scan_cnt) {
               case 1:
                   sprintf(RunParameters.HostList[cnt++], "%d", first);
                   break;
               case 2:
                   if (last >= first) {
                       for (scan_cnt = first ; scan_cnt <= last; scan_cnt++) {
                           sprintf(RunParameters.HostList[cnt++], "%d", scan_cnt);
                       }
                   }
                   break;
               default:
                   break;
           }
        }
        else {
            struct hostent *host = gethostbyname(hostname);
            if (host == NULL) {
                ulm_err(("error: hostname look-up failed for %s\n", hostname));
                Abort();
            }
            size_t len = strlen(host->h_name);
            if (len >= ULM_MAX_HOSTNAME_LEN) {
                ulm_err(("error: hostname too long for library buffer, length = %ld\n", len));
                Abort();
            }
	    bzero(RunParameters.HostList[cnt],len+1);
            strncpy((char *) RunParameters.HostList[cnt], host->h_name, len);
            cnt++;
        }
    }
    RunParameters.HostListSize = cnt;
    fclose(fp);
}



void GetAppHostDataNoInputRSH(const char *InfoStream)
{
    int RetVal;
    struct hostent *NetEntry;
    char LocalHostName[ULM_MAX_HOSTNAME_LEN];
    size_t len;

    RunParameters.HostListSize = 1;

    /* allocate memory for List of Hosts */
    RunParameters.HostList = ulm_new(HostName_t,  RunParameters.HostListSize);

    /* fill in host information */
    RetVal = gethostname(LocalHostName, ULM_MAX_HOSTNAME_LEN);
    if (RetVal < 0) {
        ulm_err(("Error: Can't find local hostname!\n"));
        Abort();
    }
    /* debug */
    int failureCount=0;
TRY_AGN:
    /* end debug */
    if ((NetEntry = gethostbyname(LocalHostName)) == NULL) {;
        ulm_err(("Error: Hostname look-up failed for %s\n", LocalHostName));
	/* debug */
	if( h_errno == NO_RECOVERY )
	       	ulm_err(("Error: NO_RECOVERY :: h_errno %d\n",h_errno));
	else if( h_errno == NO_ADDRESS )
	       	ulm_err(("Error: NO_ADDRESS :: h_errno %d\n",h_errno));
	else if( h_errno == NO_DATA )
	       	ulm_err(("Error: NO_DATA :: h_errno %d\n",h_errno));
	else if( h_errno == HOST_NOT_FOUND )
	       	ulm_err(("Error: HOST_NOT_FOUND :: h_errno %d\n",h_errno));
	else if( h_errno == TRY_AGAIN ){
		failureCount++;
	       	ulm_err(("Error: TRY_AGAIN :: h_errno %d\n",h_errno));
		if( failureCount < 5 )
			goto TRY_AGN;
	}
	/* debug */
        Abort();
    }
    len = strlen(NetEntry->h_name);
    if (len > ULM_MAX_HOSTNAME_LEN) {
        ulm_err(("Error: Host name too long for library buffer, length = %ld\n", len));
        Abort();
    }
    sprintf((char *) RunParameters.HostList[0], NetEntry->h_name, len);
    RunParameters.HostList[0][len] = '\0';
}
