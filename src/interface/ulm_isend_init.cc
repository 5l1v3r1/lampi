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

#include "queue/globals.h"
#include "client/ULMClient.h"
#include "internal/options.h"
#include "internal/profiler.h"
#include "internal/state.h"
#include "internal/log.h"
#include "ulm/ulm.h"

/*!
 * initialize send descriptor
 * \param buf           Data buffer
 * \param size          Buffer size in bytes if dtype is NULL,
 *                      else number of buftype descriptors in buf
 * \param dtype         Datatype descriptor
 * \param dest          Destination process
 * \param tag           Message tag
 * \param comm          Communicator ID
 * \param request       ULM request
 * \param sendMode      type of send
 * \param persistent     flag indicating if the request is persistent
 * \return              ULM return code
 *
 * The descriptor is allocated from the library's internal pools and
 * then filled in.  A request object is also allocated.
 */
extern "C" int ulm_isend_init(void *buf, size_t size, ULMType_t *dtype,
                              int dest, int tag, int comm,
                              ULMRequestHandle_t *request, int sendMode,
                              int persistent)
{
    int errorCode;
    BaseSendDesc_t *SendDescriptor;

    /* bind send descriptor to a given path....this can fail... */
    errorCode= communicators[comm]->pt2ptPathSelectionFunction
	    ((void **) (&SendDescriptor),comm,dest );
    if (errorCode != ULM_SUCCESS) {
        return errorCode;
    }

    assert(SendDescriptor->WhichQueue == SENDDESCFREELIST);

    SendDescriptor->WhichQueue = REQUESTINUSE;

    // set value of pointer
    *request = (ULMRequestHandle_t) SendDescriptor;

    // set messageDone
    // this flag is used by test/wait to
    // when the request is done..

    SendDescriptor->messageDone = REQUEST_INCOMPLETE;

    // set message type in ReturnHandle
    SendDescriptor->requestType = REQUEST_TYPE_SEND;

    // set pointer to datatype
    SendDescriptor->datatype = dtype;

    // set buf type, posted size
    if (dtype == NULL) {
        SendDescriptor->posted_m.length_m = size;
    } else {
        SendDescriptor->posted_m.length_m = dtype->packed_size * size;
    }

    // set send address
    if ((dtype != NULL) && (dtype->layout == CONTIGUOUS) && (dtype->num_pairs != 0)) {
        SendDescriptor->AppAddr = (void *)((char *)buf + dtype->type_map[0].offset);
    }
    else {
        SendDescriptor->AppAddr = buf;
    }

    // set destination process (need to check for completion)
    SendDescriptor->posted_m.proc.destination_m = dest;

    // set user_tag
    SendDescriptor->posted_m.UserTag_m = tag;

    // set communicator
    SendDescriptor->ctx_m = comm;

    // set request state to inactive
    SendDescriptor->status = ULM_STATUS_INITED;

    // set the send mode
    SendDescriptor->sendType = sendMode;

    // set the persestence flag
    if (persistent) {
        SendDescriptor->persistent = true;
        // increment requestRefCount
        communicators[comm]->refCounLock.lock();
        (communicators[comm]->requestRefCount)++;
        communicators[comm]->refCounLock.unlock();
    } else {
        SendDescriptor->persistent = false;
    }

    return ULM_SUCCESS;
}
