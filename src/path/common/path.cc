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

#include <assert.h>

#include "queue/globals.h"
#include "internal/options.h"
#include "path/common/path.h"
#include "path/common/InitSendDescriptors.h"

void BasePath_t::ReturnDesc(BaseSendDesc_t *message, int poolIndex)
{
    // sanity check - list must be empty
#ifndef _DEBUGQUEUES
    assert(message->FragsToSend.size() == 0);
    assert(message->FragsToAck.size() == 0);
#else
    if (message->FragsToSend.size() != 0L) {
        ulm_exit((-1, "sharedmemPath::ReturnDesc: message %p "
                  "FragsToSend.size() %ld numfrags %d numsent %d "
                  "numacked %d list %d\n", message, 
		  message->FragsToSend.size(),
                  message->numfrags, message->NumSent, message->NumAcked,
                  message->WhichQueue));
    }
    if (message->FragsToAck.size() != 0L) {
        ulm_exit((-1, "sharedmemPath::ReturnDesc: message %p "
                  "FragsToAck.size() %ld numfrags %d numsent %d "
                  "numacked %d list %d\n", message, message->FragsToAck.size(),
                  message->numfrags, message->NumSent, message->NumAcked,
                  message->WhichQueue));
    }
#endif                          // _DEBUGQUEUES

    // if this was a bsend (or aborted bsend), then decrement the reference
    // count for the appropriate buffer allocation
    if (message->sendType == ULM_SEND_BUFFERED) {
        if (message->posted_m.length_m > 0) {
            ulm_bsend_decrement_refcount(
			    (ULMRequestHandle_t) message,
			    message->bsendOffset);
        }
    }
    // mark descriptor as beeing in the free list
    message->WhichQueue = SENDDESCFREELIST;
    // return descriptor to pool -- always freed by allocating process!
    _ulm_SendDescriptors.returnElement(message, 0);
}
