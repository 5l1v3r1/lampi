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
#include "path/common/path.h"

/*!
 * non blocking send
 *
 * \param buf		Data buffer
 * \param buf_size	Buffer size, in bytes, if dtype is NULL, or the
 *			number of ULMType_t's in buf, if dtype is not NULL
 * \param dtype		Datatype descriptor for non-contiguous data,
 *			NULL if data is contiguous
 * \param dest		Destination process
 * \param tag		Message tag
 * \param comm		Communicator ID
 * \param sendMode	Send mode - standard, buffered, synchronous, or ready
 * \return		ULM return code
 */
extern "C" int ulm_send(void *buf, size_t size, ULMType_t *dtype,
                        int dest, int tag, int comm, int sendMode)
{
    int returnCode;
    BaseSendDesc_t *SendDescriptor ;

    /* bind send descriptor to a given path....this can fail... */
    returnCode= communicators[comm]->pt2ptPathSelectionFunction
	    ((void **) (&SendDescriptor),comm,dest );
    if (returnCode != ULM_SUCCESS) {
	    return returnCode;
    }

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
    } else {
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

    // set the persistence flag

    // start send
    returnCode = communicators[comm]->isend_start(&SendDescriptor);
    if (returnCode != ULM_SUCCESS) {
	return returnCode;
    }
    // wait for completion
    returnCode = ulm_make_progress();
    if ((returnCode == ULM_ERR_OUT_OF_RESOURCE)
        || (returnCode == ULM_ERR_FATAL)
        || (returnCode == ULM_ERROR)) {
        return returnCode;
    }
    while ((SendDescriptor->messageDone == REQUEST_INCOMPLETE)) {
        returnCode = ulm_make_progress();
        if ((returnCode == ULM_ERR_OUT_OF_RESOURCE)
            || (returnCode == ULM_ERR_FATAL)
            || (returnCode == ULM_ERROR)) {
            return returnCode;
        }
    }

    /* mark the request free as called */
    SendDescriptor->freeCalled=true;

    return ULM_SUCCESS;
}
