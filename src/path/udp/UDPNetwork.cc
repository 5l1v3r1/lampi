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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>		// for gethostbyname()
#include <unistd.h>

#include "client/adminMessage.h"
#include "client/daemon.h"
#include "internal/constants.h"
#include "internal/log.h"
#include "internal/log.h"
#include "internal/system.h"
#include "path/udp/UDPNetwork.h"
#include "os/atomic.h"
#include "ulm/ulm.h"


// Prototype for initializing descriptor lists. This function is local/private
// to UDP so it is not exported in a header file.
extern int initLocalUDPSetup();

UDPNetwork *UDPGlobals::UDPNet = 0;
bool UDPGlobals::checkLongMessageSocket = true;
Locks UDPGlobals::longMessageLock;

// executed by the client daemon process only

int UDPNetwork::initPreFork()
{
    return initLocalUDPSetup();       // initialize descriptor pools
}


int UDPNetwork::initPostFork(int ifCount, struct sockaddr_in* peerAddrs)
{
    UDPGlobals::UDPNet = new UDPNetwork(ifCount, peerAddrs);
    if (!UDPGlobals::UDPNet) {
        ulm_exit(("UDPNetwork::beginInitLocal - UDPNet not allocated!\n"));
    }
    return ULM_SUCCESS;
}


UDPNetwork::UDPNetwork(int ifCount, struct sockaddr_in* peerAddrs)
{
    nHosts = nhosts();
    nProcs = nprocs();

    hostAddrs = (struct sockaddr_in *)ulm_malloc(sizeof(struct sockaddr_in) * nProcs);
    if (!hostAddrs) {
        ulm_exit(("UDPNetwork::UDPNetwork error - unable to allocate "
                  "%d bytes for hostAddrs!\n",
                  sizeof(struct sockaddr_in) * nHosts));
    }

    int index=0;
    for (int i = 0; i < nProcs; i++) {
        hostAddrs[i] = peerAddrs[index];
        index += ifCount;
    }

    int numPorts = UDPGlobals::NPortsPerProc * nProcs;
    procPorts =
        (unsigned short *) ulm_malloc(sizeof(unsigned short) * numPorts);
    if (!procPorts) {
        ulm_exit(("UDPNetwork::UDPNetwork error - unable to allocate "
                  "%d bytes for procPorts!\n",
                  sizeof(unsigned short) * numPorts));
    }
    for (int i = 0; i < numPorts; i++)
        procPorts[i] = htons(0);
}

int UDPNetwork::initialize(int ProcID)
{
    struct sockaddr_in dynaddr;
    int receive_buffer = 0, send_buffer = 0;

    socklen_t addrlen = sizeof(struct sockaddr_in);
    socklen_t optlen;
    int j, fflags;

    for (j = 0; j < UDPGlobals::NPortsPerProc; j++) {
        if ((sockfd[j] = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
            ulm_err(("UDPNetwork::initialize: "
                     "can't open datagram socket\n"));
            return ULM_ERROR;
        }

        dynaddr.sin_family = AF_INET;
        dynaddr.sin_addr.s_addr = INADDR_ANY;
        dynaddr.sin_port = htons(0);
        // get a dynamically allocated port
        if (bind
            (sockfd[j], (struct sockaddr *) &dynaddr,
             sizeof(struct sockaddr_in)) < 0) {
            ulm_err(("UDPNetwork::initialize: "
                     "can't bind local address %d\n",
                     ntohs(dynaddr.sin_port)));
            return ULM_ERROR;
        }
        if (getsockname
            (sockfd[j], (struct sockaddr *) &dynaddr,
             (socklen_t *) &addrlen) < 0) {
            ulm_err(("UDPNetwork::initialize: " "can't getsockname!\n"));
            return ULM_ERROR;
        }
        // store port in network order
        portID[j] = dynaddr.sin_port;

        // try to set the socket's receive buffer as high as possible
        // or somewhere >= UDP_MIN_RECVBUF
        if (getsockopt
            (sockfd[j], SOL_SOCKET, SO_RCVBUF, &receive_buffer,
             (socklen_t *) &optlen) == 0) {
            /* if no buffer is currently allocated, allocate a small amount */
            if (receive_buffer == 0)
                receive_buffer = 1024;
            while (receive_buffer < UDP_MIN_RECVBUF) {
                receive_buffer += receive_buffer;
                if (setsockopt
                    (sockfd[j], SOL_SOCKET, SO_RCVBUF, &receive_buffer,
                     sizeof(int)) < 0)
                    break;
            }
        }
        // try to set the socket's receive buffer as high as possible
        // or somewhere >= UDP_MIN_SENDBUF
        if (getsockopt
            (sockfd[j], SOL_SOCKET, SO_SNDBUF, &send_buffer,
             (socklen_t *) &optlen) == 0) {
            /* if no buffer is currently allocated, allocate a small amount */
            if (send_buffer == 0)
                send_buffer = 1024;
            while (send_buffer < UDP_MIN_RECVBUF) {
                send_buffer += send_buffer;
                if (setsockopt
                    (sockfd[j], SOL_SOCKET, SO_SNDBUF, &send_buffer,
                     sizeof(int)) < 0)
                    break;
            }
        }
        // try to set the socket to nonblocking mode
        if ((fflags = fcntl(sockfd[j], F_GETFL, 0)) >= 0) {
            fflags |= O_NONBLOCK;
            if (fcntl(sockfd[j], F_SETFL, fflags) != -1) {
            } else {
                ulm_err(("UDPNetwork::initialize: fcntl(%d, F_SETFL, %d) of UDP socket to set O_NONBLOCK mode failed!\n", sockfd[j], fflags));
                return ULM_ERROR;
            }
        } else {
            ulm_err(("UDPNetwork::initialize: fcntl(%d, F_GETFL, 0) of UDP socket returned error %d!\n", sockfd[j], fflags));
            return ULM_ERROR;
        }

    }

    return ULM_SUCCESS;
}
