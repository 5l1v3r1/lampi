/*
 * Copyright 2002-2003. The Regents of the University of
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

#include <string.h>
#include <strings.h>

#include "queue/globals.h"
#include "util/DblLinkList.h"
#include "util/MemFunctions.h"
#include "util/Utility.h"
#include "util/inline_copy_functions.h"
#include "internal/buffer.h"
#include "internal/constants.h" // for CACHE_ALIGNMENT
#include "internal/log.h"
#include "internal/state.h"
#include "path/common/BaseDesc.h"
#include "path/common/InitSendDescriptors.h"
#include "ulm/ulm.h"

#ifdef ENABLE_SHARED_MEMORY
# include "path/sharedmem/SMPSharedMemGlobals.h"
#endif                          // SHARED_MEMORY

#ifdef DEBUG_DESCRIPTORS
#include "util/dclock.h"

extern double times [8];
extern int sendCount;
extern bool startCount;
extern double tt0;
#endif

// copied frag data to processor address space using a non-contiguous
// datatype as a format
int non_contiguous_copy(BaseRecvFragDesc_t *frag,
                        ULMType_t *datatype,
                        void *dest,
                        ssize_t *length, unsigned int *checkSum)
{
    size_t frag_seq_off = frag->seqOffset_m;
    size_t message_len = frag->msgLength_m;
    size_t frag_len = frag->length_m;
    void *src = frag->addr_m;
    int init_cnt = frag_seq_off / datatype->packed_size;
    int tot_cnt = message_len / datatype->packed_size;
    int tmap_init, ti, dtype_cnt;
    size_t len_to_copy;
    unsigned int lp_int = 0;
    unsigned int lp_len = 0;
    ULMTypeMapElt_t *tmap = datatype->type_map;
    int i;
    void *src_addr;
    void *dest_addr;
    size_t len_copied = 0;

    // find starting typemap index
    tmap_init = datatype->num_pairs - 1;
    for (i = 0; i < datatype->num_pairs; i++) {
        if (init_cnt * datatype->packed_size + tmap[i].seq_offset ==
            frag_seq_off) {
            tmap_init = i;
            break;
        } else if (init_cnt * datatype->packed_size + tmap[i].seq_offset >
                   frag_seq_off) {
            tmap_init = i - 1;
            break;
        }
    }

    // handle initial typemap pair
    src_addr = (void *) ((char *) src);
    dest_addr = (void *) ((char *) dest
                          + init_cnt * datatype->extent
                          + tmap[tmap_init].offset
                          - init_cnt * datatype->packed_size
                          - tmap[tmap_init].seq_offset + frag_seq_off);

    if (frag_len == 0) {
        len_to_copy = 0;
    } else {
        len_to_copy = tmap[tmap_init].size
            + init_cnt * datatype->packed_size
            + tmap[tmap_init].seq_offset - frag_seq_off;
        len_to_copy = (len_to_copy > frag_len) ? frag_len : len_to_copy;
    }

    if (checkSum)
        *checkSum = 0;
    len_copied =
        frag->nonContigCopyFunction(dest_addr, src_addr, len_to_copy,
                                    len_to_copy, checkSum, &lp_int,
                                    &lp_len, true,
                                    (frag_len - len_copied - len_to_copy ==
                                     0) ? true : false);

    if (frag_len - len_copied) {
        tmap_init++;
        for (dtype_cnt = init_cnt; dtype_cnt < tot_cnt; dtype_cnt++) {
            for (ti = tmap_init; ti < datatype->num_pairs; ti++) {
                src_addr = (void *) ((char *) src + len_copied);
                dest_addr = (void *) ((char *) dest
                                      + dtype_cnt * datatype->extent
                                      + tmap[ti].offset);

                len_to_copy = (frag_len - len_copied >= tmap[ti].size) ?
                    tmap[ti].size : frag_len - len_copied;
                if (len_to_copy == 0) {
                    *length = len_copied;
                    return ULM_SUCCESS;
                }

                bool lastCall = (frag_len - len_copied - len_to_copy == 0)
                    || ((ti == (datatype->num_pairs - 1))
                        && (dtype_cnt == (tot_cnt - 1)));
                len_copied +=
                    frag->nonContigCopyFunction(dest_addr, src_addr,
                                                len_to_copy, len_to_copy,
                                                checkSum, &lp_int, &lp_len,
                                                false, lastCall);
            }
            tmap_init = 0;
        }
    }
    *length = len_copied;
    return ULM_SUCCESS;
}

void SendDesc_t::shallowCopyTo(RequestDesc_t *request)
{
    SendDesc_t	*sendDesc = (SendDesc_t *)request;

    RequestDesc_t::shallowCopyTo(request);
    sendDesc->bsendBufferSize = bsendBufferSize;
    sendDesc->sendType = sendType;
    sendDesc->addr_m = addr_m;
    sendDesc->posted_m = posted_m;

    ulm_type_retain(sendDesc->bsendDatatype);
}


// return number of uncorrupted bytes copied, or -1 if the frag's data
// is corrupt
ssize_t RecvDesc_t::CopyToApp(void *FrgDesc, bool * recvDone)
{
    bool dataNotCorrupted = false;
    Communicator *Comm;
    ULMType_t *datatype;
    unsigned int checkSum;
    ssize_t bytesIntoBitBucket;

    // frag pointer
    BaseRecvFragDesc_t *FragDesc = (BaseRecvFragDesc_t *) FrgDesc;

    // compute frag offset
    unsigned long Offset = FragDesc->dataOffset();

    // destination buffer
    void *Destination = (void *) ((char *) addr_m + Offset);

    // length to copy
    ssize_t lengthToCopy = FragDesc->length_m;

    // make sure we don't overflow app buffer
    ssize_t AppBufferLen = posted_m.length_m - Offset;

    datatype = this->datatype;

    if (AppBufferLen <= 0) {
        FragDesc->MarkDataOK(true);
        // mark recv as complete
        lengthToCopy=0;
        bytesIntoBitBucket=FragDesc->length_m;
    } else {
        if (AppBufferLen < lengthToCopy) 
            lengthToCopy=AppBufferLen;
        bytesIntoBitBucket=FragDesc->length_m -lengthToCopy;
        if (datatype == NULL || datatype->layout == CONTIGUOUS) {
            // copy the data
            checkSum =
                FragDesc->CopyFunction(FragDesc->addr_m, 
                                       Destination, lengthToCopy);
        } else {                    
            // Non-contiguous case
            non_contiguous_copy(FragDesc, datatype, addr_m, 
                                &lengthToCopy, &checkSum);
        }

        // check to see if data arrived ok
        dataNotCorrupted = FragDesc->CheckData(checkSum, lengthToCopy);

        // return number of bytes copied
        if (!dataNotCorrupted) {
            return (-1);
        }
    }

    // update byte count
    DataReceived += lengthToCopy;
    DataInBitBucket += bytesIntoBitBucket;

    // check to see if receive is complete, and if so mark the request object
    //   as such
    if ((DataReceived + DataInBitBucket) >= reslts_m.length_m) {

        // mark recv as complete
        messageDone = REQUEST_COMPLETE;
        if (recvDone)
            *recvDone = true;
        wmb();

        // remove recv desc from list
        Comm = communicators[ctx_m];
        if (WhichQueue == MATCHEDIRECV) {
            Comm->privateQueues.MatchedRecv[reslts_m.peer_m]
                ->RemoveLink(this);
        }

        // if ulm_request_free has already been called, then 
        // we free the recv/request obj. here
        if (freeCalled)
            requestFree();
    }

    return lengthToCopy;
}


// return number of uncorrupted bytes copied, or -1 if the frag's
//   data is corrupt - recv descriptor is locked for atomic update of
//   counters
ssize_t RecvDesc_t::CopyToAppLock(void *FrgDesc, bool * recvDone)
{
    bool dataNotCorrupted = false;
    Communicator *Comm;
    ULMType_t *datatype;
    unsigned int checkSum;
    ssize_t bytesIntoBitBucket;

    // frag pointer
    BaseRecvFragDesc_t *FragDesc = (BaseRecvFragDesc_t *) FrgDesc;

    // compute frag offset
    unsigned long Offset = FragDesc->dataOffset();

    // destination buffer
    void *Destination = (void *) ((char *) addr_m + Offset);

    // length to copy
    ssize_t lengthToCopy = FragDesc->length_m;

    // make sure we don't overflow app buffer
    ssize_t AppBufferLen = posted_m.length_m - Offset;

    //Contiguous Data
    datatype = this->datatype;

    if (AppBufferLen <= 0) {
        FragDesc->MarkDataOK(true);
        // mark recv as complete
        lengthToCopy=0;
        bytesIntoBitBucket=FragDesc->length_m;
    } else {
        if (AppBufferLen < lengthToCopy) 
            lengthToCopy=AppBufferLen;
        bytesIntoBitBucket=FragDesc->length_m -lengthToCopy;
        if (datatype == NULL || datatype->layout == CONTIGUOUS) {
            // copy the data
            checkSum =
                FragDesc->CopyFunction(FragDesc->addr_m, 
                                       Destination, lengthToCopy);
        } else {                    
            // Non-contiguous case
            non_contiguous_copy(FragDesc, datatype, addr_m, 
                                &lengthToCopy, &checkSum);
        }

        // check to see if data arrived ok
        dataNotCorrupted = FragDesc->CheckData(checkSum, lengthToCopy);
        // return number of bytes copied
        if (!dataNotCorrupted) {
            return (-1);
        }
    }

    // update byte count
    if (usethreads()) {
        Lock.lock();
        DataReceived += lengthToCopy;
        DataInBitBucket += bytesIntoBitBucket;
        Lock.unlock();
    } else {
        DataReceived += lengthToCopy;
        DataInBitBucket += bytesIntoBitBucket;
    }

    // check to see if receive is complete, and if so mark the request
    // object as such
    if ((DataReceived + DataInBitBucket) >= reslts_m.length_m) {
        // mark recv as complete - the request descriptor will be marked
	// later on.  This is to avoid a race condition with another thread
	// waiting to complete a recv, completing, and try to free the
	// communicator before the current thread is done referencing
	// this communicator
        if (recvDone)
            *recvDone = true;
        wmb();

        // remove receive descriptor from list
        Comm = communicators[ctx_m];
        Comm->privateQueues.MatchedRecv[reslts_m.peer_m]->RemoveLink(this);

        // if ulm_request_free() has already been called, then we
        // free the recv/request object here...
        if (usethreads()) 
            Lock.lock();
        if (freeCalled)
            requestFree();
        if (usethreads())
            Lock.unlock();
    }

    return lengthToCopy;
}

#ifdef ENABLE_SHARED_MEMORY

int RecvDesc_t::SMPCopyToApp(unsigned long sequentialOffset,
                                   unsigned long fragLen, void *fragAddr,
                                   unsigned long sendMessageLength,
                                   int *recvDone)
{
    int returnValue = ULM_SUCCESS;
    ssize_t length;
    ssize_t AppBufferLen;
    void *Destination;
    ULMType_t *datatype;

    *recvDone = 0;

    datatype = this->datatype;

    if (datatype == NULL || datatype->layout == CONTIGUOUS) {
        // compute frag offset
        length = fragLen;
        // make sure we don't overflow buffer
        AppBufferLen = posted_m.length_m - sequentialOffset;
        // if AppBufferLen is negative or zero, then there is nothing to
        // copy, so get out of here as soon as possible...
        if (AppBufferLen <= 0) {

            DataInBitBucket += fragLen;
            return ULM_SUCCESS;
        } else if (AppBufferLen < length) {
            length = AppBufferLen;
        }

        Destination = (void *) ((char *) addr_m + sequentialOffset);   // Destination buffer
        MEMCOPY_FUNC(fragAddr, Destination, length);
    } else {
        contiguous_to_noncontiguous_copy(datatype, addr_m, &length,
                                         sequentialOffset, fragLen,
                                         fragAddr);
    }

    // update recv counters
    DataReceived += length;
    DataInBitBucket += (fragLen - length);

    return returnValue;
}

#endif                          // SHARED_MEMORY

void RecvDesc_t::requestFree(void) 
{
    Communicator *commPtr = communicators[ctx_m];

    // decrement request reference count
    if (persistent) {
        commPtr->refCounLock.lock();
        (commPtr->requestRefCount)--;
        commPtr->refCounLock.unlock();
    }

    // reset request parameters
    WhichQueue = REQUESTFREELIST;
    status = ULM_STATUS_INVALID;
    freeCalled = false;
    
    // return request to free list
    if (usethreads()) {
        IrecvDescPool.returnElement(this);
    } else {
        IrecvDescPool.returnElementNoLock(this);
    }

    return;
}
