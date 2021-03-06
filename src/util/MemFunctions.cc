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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>

#include "util/MemFunctions.h"
#include "internal/log.h"
#include "internal/profiler.h"
#include "internal/system.h"
#include "path/common/BaseDesc.h"
#include "ulm/ulm.h"

#ifdef _MIPS_SZLONG /* SGI MIPSpro compiler */

#if (_MIPS_SZLONG == 64)
#define _WORD_MASK_ 0x7
#elif (_MIPS_SZLONG == 32)
#define _WORD_MASK_ 0x3
#else
#define _WORD_MASK 0xFFFF
#endif

#elif defined(LONG_TYPE_SIZE) /* GNU C/C++ compilers, hopefully... */

#if (LONG_TYPE_SIZE == 64)
#define _WORD_MASK_ 0x7
#elif (LONG_TYPE_SIZE == 32)
#define _WORD_MASK_ 0x3
#else
#define _WORD_MASK 0xFFFF
#endif

#else /* and it's over the goalie's head, and ...  */

#ifndef _WORD_MASK_
#define _WORD_MASK_ 0x7
#endif

#endif


inline
bool wordaligned(const void *v)
{
    unsigned long vl = (unsigned long) v;

    if (vl & _WORD_MASK_) {
	return false;
    } else {
	return true;
    }
}

inline
bool wordaligned(const unsigned long v)
{
    if (v & _WORD_MASK_) {
	return false;
    } else {
	return true;
    }
}

inline
bool intaligned(const void *v)
{
    unsigned long vl = (unsigned long)v;

    if (vl & 3) {
        return false;
    }
    else {
        return true;
    }
}

inline
bool intaligned(const unsigned long v)
{
    if (v & 3) {
        return false;
    }
    else {
        return true;
    }
}

#ifdef _COUNT_ME_
static unsigned long _AA = 0;
static unsigned long _AU = 0;
static unsigned long _UA = 0;
static unsigned long _UU = 0;
#endif

/*
 * this version of bcopy_csum() looks a little too long, but it
 * handles cumulative checksumming for arbitrary lengths and address
 * alignments as best as it can; the contents of lastPartialLong and
 * lastPartialLength are updated to reflected the last partial word's
 * value and length (in bytes) -- this should allow proper handling of
 * checksumming contiguous or noncontiguous buffers via multiple calls
 * of bcopy_csum() - Mitch
 */

unsigned long
bcopy_csum (
    const void * RESTRICT_MACRO source,
    void * RESTRICT_MACRO destination,
    unsigned long copylen,
    unsigned long csumlen,
    unsigned long * RESTRICT_MACRO lastPartialLong,
    unsigned long * RESTRICT_MACRO lastPartialLength
    )
{
    unsigned long * RESTRICT_MACRO src = (unsigned long *) source;
    unsigned long * RESTRICT_MACRO dest = (unsigned long *) destination;
    unsigned long csum = 0;
    ssize_t csumlenresidue;
    unsigned long i, temp;

    csumlenresidue = (csumlen > copylen) ? (csumlen - copylen) : 0;
    temp = *lastPartialLong;

    if (wordaligned(source) && wordaligned(dest)) {
#ifdef _COUNT_ME_
	_AA++;
#endif
	if (*lastPartialLength) {
	    // do we have enough data to fill out the partial word?
	    if (copylen >= (sizeof(unsigned long) - *lastPartialLength)) { // YES, we do...
		memcpy(((char *)&temp + *lastPartialLength), src,
		       (sizeof(unsigned long) - *lastPartialLength));
		memcpy(dest, ((char *)&temp + *lastPartialLength),
		       (sizeof(unsigned long) - *lastPartialLength));
		src = (unsigned long *)((char *)src + sizeof(unsigned long) - *lastPartialLength);
		dest = (unsigned long *)((char *)dest + sizeof(unsigned long) - *lastPartialLength);
		csum += (temp - *lastPartialLong);
		copylen -= sizeof(unsigned long) - *lastPartialLength;
		// now we have an unaligned source and an unaligned destination
		for( ;copylen >= sizeof(*src); copylen -= sizeof(*src)) {
		    memcpy(&temp, src, sizeof(temp));
		    src++;
		    csum += temp;
		    memcpy(dest, &temp, sizeof(temp));
		    dest++;
		}
		*lastPartialLength = 0;
		*lastPartialLong = 0;
	    }
	    else { // NO, we don't...
		memcpy(((char *)&temp + *lastPartialLength), src, copylen);
		memcpy(dest, ((char *)&temp + *lastPartialLength), copylen);
		src = (unsigned long *)((char *)src + copylen);
		dest = (unsigned long *)((char *)dest + copylen);
		csum += (temp - *lastPartialLong);
		*lastPartialLong = temp;
		*lastPartialLength += copylen;
		copylen = 0;
	    }
	}
	else { // fast path...
	    unsigned long numLongs = copylen/sizeof(unsigned long);
	    for(i = 0; i < numLongs; i++) {
		csum += *src;
		*dest++ = *src++;
	    }
	    *lastPartialLong = 0;
	    *lastPartialLength = 0;
	    if (wordaligned(copylen) && (csumlenresidue == 0)) {
		return(csum);
	    }
	    else {
		copylen -= i * sizeof(unsigned long);
	    }
	}
    } else if (wordaligned(source)) {
#ifdef _COUNT_ME_
	_AU++;
#endif
	if (*lastPartialLength) {
	    // do we have enough data to fill out the partial word?
	    if (copylen >= (sizeof(unsigned long) - *lastPartialLength)) { // YES, we do...
		memcpy(((char *)&temp + *lastPartialLength), src,
		       (sizeof(unsigned long) - *lastPartialLength));
		memcpy(dest, ((char *)&temp + *lastPartialLength),
		       (sizeof(unsigned long) - *lastPartialLength));
		src = (unsigned long *)((char *)src + sizeof(unsigned long) - *lastPartialLength);
		dest = (unsigned long *)((char *)dest + sizeof(unsigned long) - *lastPartialLength);
		csum += (temp - *lastPartialLong);
		copylen -= sizeof(unsigned long) - *lastPartialLength;
		// now we have an unaligned source and an unknown alignment for our destination
		if (wordaligned(dest)) {
		    unsigned long numLongs = copylen/sizeof(unsigned long);
		    for(i = 0; i < numLongs; i++) {
			memcpy(&temp, src, sizeof(temp));
			src++;
			csum += temp;
			*dest++ = temp;
		    }
		    copylen -= i * sizeof(unsigned long);
		}
		else {
		    for( ;copylen >= sizeof(*src); copylen -= sizeof(*src)) {
			memcpy(&temp, src, sizeof(temp));
			src++;
			csum += temp;
			memcpy(dest, &temp, sizeof(temp));
			dest++;
		    }
		}
		*lastPartialLong = 0;
		*lastPartialLength = 0;
	    }
	    else { // NO, we don't...
		memcpy(((char *)&temp + *lastPartialLength), src, copylen);
		memcpy(dest, ((char *)&temp + *lastPartialLength), copylen);
		src = (unsigned long *)((char *)src + copylen);
		dest = (unsigned long *)((char *)dest + copylen);
		csum += (temp - *lastPartialLong);
		*lastPartialLong = temp;
		*lastPartialLength += copylen;
		copylen = 0;
	    }
	}
	else {
	    for( ;copylen >= sizeof(*src); copylen -= sizeof(*src)) {
		temp = *src++;
		csum += temp;
		memcpy(dest, &temp, sizeof(temp));
		dest++;
	    }
	    *lastPartialLong = 0;
	    *lastPartialLength = 0;
	}
    } else if (wordaligned(dest)) {
#ifdef _COUNT_ME_
	_UA++;
#endif
	if (*lastPartialLength) {
	    // do we have enough data to fill out the partial word?
	    if (copylen >= (sizeof(unsigned long) - *lastPartialLength)) { // YES, we do...
		memcpy(((char *)&temp + *lastPartialLength), src,
		       (sizeof(unsigned long) - *lastPartialLength));
		memcpy(dest, ((char *)&temp + *lastPartialLength),
		       (sizeof(unsigned long) - *lastPartialLength));
		src = (unsigned long *)((char *)src + sizeof(unsigned long) - *lastPartialLength);
		dest = (unsigned long *)((char *)dest + sizeof(unsigned long) - *lastPartialLength);
		csum += (temp - *lastPartialLong);
		copylen -= sizeof(unsigned long) - *lastPartialLength;
		// now we have a source of unknown alignment and a unaligned destination
		if (wordaligned(src)) {
		    for( ;copylen >= sizeof(*src); copylen -= sizeof(*src)) {
			temp = *src++;
			csum += temp;
			memcpy(dest, &temp, sizeof(temp));
			dest++;
		    }
		    *lastPartialLong = 0;
		    *lastPartialLength = 0;
		}
		else {
		    for( ;copylen >= sizeof(*src); copylen -= sizeof(*src)) {
			memcpy(&temp, src, sizeof(temp));
			src++;
			csum += temp;
			memcpy(dest, &temp, sizeof(temp));
			dest++;
		    }
		    *lastPartialLength = 0;
		    *lastPartialLong = 0;
		}
	    }
	    else { // NO, we don't...
		memcpy(((char *)&temp + *lastPartialLength), src, copylen);
		memcpy(dest, ((char *)&temp + *lastPartialLength), copylen);
		src = (unsigned long *)((char *)src + copylen);
		dest = (unsigned long *)((char *)dest + copylen);
		csum += (temp - *lastPartialLong);
		*lastPartialLong = temp;
		*lastPartialLength += copylen;
		copylen = 0;
	    }
	}
	else {
	    for( ;copylen >= sizeof(*src); copylen -= sizeof(*src)) {
		memcpy(&temp, src, sizeof(temp));
		src++;
		csum += temp;
		*dest++ = temp;
	    }
	    *lastPartialLength = 0;
	    *lastPartialLong = 0;
	}
    } else {
#ifdef _COUNT_ME_
	_UU++;
#endif
	if (*lastPartialLength) {
	    // do we have enough data to fill out the partial word?
	    if (copylen >= (sizeof(unsigned long) - *lastPartialLength)) { // YES, we do...
		memcpy(((char *)&temp + *lastPartialLength), src,
		       (sizeof(unsigned long) - *lastPartialLength));
		memcpy(dest, ((char *)&temp + *lastPartialLength),
		       (sizeof(unsigned long) - *lastPartialLength));
		src = (unsigned long *)((char *)src + sizeof(unsigned long) - *lastPartialLength);
		dest = (unsigned long *)((char *)dest + sizeof(unsigned long) - *lastPartialLength);
		csum += (temp - *lastPartialLong);
		copylen -= sizeof(unsigned long) - *lastPartialLength;
		// now we have an unknown alignment for our source and destination
		if (wordaligned(src) && wordaligned(dest)) {
		    unsigned long numLongs = copylen/sizeof(unsigned long);
		    for(i = 0; i < numLongs; i++) {
			csum += *src;
			*dest++ = *src++;
		    }
		    copylen -= i * sizeof(unsigned long);
		}
		else { // safe but slower for all other alignments
		    for( ;copylen >= sizeof(*src); copylen -= sizeof(*src)) {
			memcpy(&temp, src, sizeof(temp));
			src++;
			csum += temp;
			memcpy(dest, &temp, sizeof(temp));
			dest++;
		    }
		}
		*lastPartialLong = 0;
		*lastPartialLength = 0;
	    }
	    else { // NO, we don't...
		memcpy(((char *)&temp + *lastPartialLength), src, copylen);
		memcpy(dest, ((char *)&temp + *lastPartialLength), copylen);
		src = (unsigned long *)((char *)src + copylen);
		dest = (unsigned long *)((char *)dest + copylen);
		csum += (temp - *lastPartialLong);
		*lastPartialLong = temp;
		*lastPartialLength += copylen;
		copylen = 0;
	    }
	}
	else {
	    for( ;copylen >= sizeof(*src); copylen -= sizeof(*src)) {
		memcpy(&temp, src, sizeof(temp));
		src++;
		csum += temp;
		memcpy(dest, &temp, sizeof(temp));
		dest++;
	    }
	    *lastPartialLength = 0;
	    *lastPartialLong = 0;
	}
    }

    /* if copylen is non-zero there was a bit left, less than an unsigned long's worth */

    if ((copylen != 0) && (csumlenresidue == 0)) {
	temp = *lastPartialLong;
	if (*lastPartialLength) {
	    if (copylen >= (sizeof(unsigned long) - *lastPartialLength)) {
		// copy all remaining bytes from src to dest
		unsigned long copytemp = 0;
		memcpy(&copytemp, src, copylen);
		memcpy(dest, &copytemp, copylen);
		// fill out rest of partial word and add to checksum
		memcpy(((char *)&temp + *lastPartialLength), src,
		       (sizeof(unsigned long) - *lastPartialLength));
		// avoid unsigned arithmetic overflow by subtracting the old partial
		// word from the new one before adding to the checksum...
		csum += (temp - *lastPartialLong);
		copylen -= sizeof(unsigned long) - *lastPartialLength;
		src = (unsigned long *)((char *)src + sizeof(unsigned long) - *lastPartialLength);
		*lastPartialLength = copylen;
		// reset temp, and calculate next partial word
		temp = 0;
		if (copylen) {
		    memcpy(&temp, src, copylen);
		}
		// add it to the the checksum
		csum += temp;
		*lastPartialLong = temp;
	    }
	    else {
		// copy all remaining bytes from src to dest
		unsigned long copytemp = 0;
		memcpy(&copytemp, src, copylen);
		memcpy(dest, &copytemp, copylen);
		// fill out rest of partial word and add to checksum
		memcpy(((char *)&temp + *lastPartialLength), src,
		       copylen);
		// avoid unsigned arithmetic overflow by subtracting the old partial
		// word from the new one before adding to the checksum...
		csum += temp - *lastPartialLong;
		*lastPartialLong = temp;
		*lastPartialLength += copylen;
	    }
	}
	else { // fast path...
	    // temp and *lastPartialLong are 0 if *lastPartialLength is 0...
	    memcpy(&temp, src, copylen);
	    csum += temp;
	    memcpy(dest, &temp, copylen);
	    *lastPartialLong = temp;
	    *lastPartialLength = copylen;
	    // done...return the checksum
	}
    }
    else if (csumlenresidue != 0) {
	if (copylen != 0) {
	    temp = 0;
	    memcpy(&temp, src, copylen);
	    memcpy(dest, &temp, copylen);
	}
	if (csumlenresidue < (ssize_t)(sizeof(unsigned long) - copylen - *lastPartialLength)) {
	    temp = *lastPartialLong;
	    memcpy(((char *)&temp + *lastPartialLength), src, (copylen + csumlenresidue));
	    // avoid unsigned arithmetic overflow by subtracting the old partial
	    // word from the new one before adding to the checksum...
	    csum += temp - *lastPartialLong;
	    src++;
	    *lastPartialLong = temp;
	    *lastPartialLength += copylen + csumlenresidue;
	    csumlenresidue = 0;
	}
	else {
	    // we have enough chksum data to fill out our last partial
	    // word
	    temp = *lastPartialLong;
	    memcpy(((char *)&temp + *lastPartialLength), src,
		   (sizeof(unsigned long) - *lastPartialLength));
	    // avoid unsigned arithmetic overflow by subtracting the old partial
	    // word from the new one before adding to the checksum...
	    csum += temp - *lastPartialLong;
	    src = (unsigned long *)((char *)src + sizeof(unsigned long) - *lastPartialLength);
	    csumlenresidue -= sizeof(unsigned long) - *lastPartialLength - copylen;
	    *lastPartialLength = 0;
	    *lastPartialLong = 0;
	}
	if (wordaligned(src)) {
	    for (i = 0; i < csumlenresidue/sizeof(unsigned long); i++) {
		csum += *src++;
	    }
	}
	else {
	    for (i = 0; i < csumlenresidue/sizeof(unsigned long); i++) {
		memcpy(&temp, src, sizeof(temp));
		csum += temp;
		src++;
	    }
	}
	csumlenresidue -= i * sizeof(unsigned long);
	if (csumlenresidue) {
	    temp = 0;
	    memcpy(&temp, src, csumlenresidue);
	    csum += temp;
	    *lastPartialLong = temp;
	    *lastPartialLength = csumlenresidue;
	}
    } /* end else if (csumlenresidue != 0) */

    return csum;
}

/*
 * this version is appropriate for checksumming a buffer via one call
 * to bcopy_csum()
 */

unsigned long
bcopy_csum (
    const void * RESTRICT_MACRO source,
    void * RESTRICT_MACRO destination,
    unsigned long copylen,
    unsigned long csumlen
    )
{
    unsigned long plong = 0;
    unsigned long plength = 0;
    return bcopy_csum(source, destination, copylen, csumlen, &plong, &plength);
}

unsigned int
bcopy_uicsum (
    const void * RESTRICT_MACRO source,
    void * RESTRICT_MACRO destination,
    unsigned long copylen,
    unsigned long csumlen,
    unsigned int * RESTRICT_MACRO lastPartialInt,
    unsigned int * RESTRICT_MACRO lastPartialLength
    )
{
    unsigned int * RESTRICT_MACRO src = (unsigned int *) source;
    unsigned int * RESTRICT_MACRO dest = (unsigned int *) destination;
    unsigned int csum = 0;
    ssize_t csumlenresidue;
    unsigned long i;
    unsigned int temp;

    csumlenresidue = (csumlen > copylen) ? (csumlen - copylen) : 0;
    temp = *lastPartialInt;

    if (intaligned(source) && intaligned(dest)) {
#ifdef _COUNT_ME_
	_AA++;
#endif
	if (*lastPartialLength) {
	    // do we have enough data to fill out the partial word?
	    if (copylen >= (sizeof(unsigned int) - *lastPartialLength)) { // YES, we do...
		memcpy(((char *)&temp + *lastPartialLength), src,
		       (sizeof(unsigned int) - *lastPartialLength));
		memcpy(dest, ((char *)&temp + *lastPartialLength),
		       (sizeof(unsigned int) - *lastPartialLength));
		src = (unsigned int *)((char *)src + sizeof(unsigned int) - *lastPartialLength);
		dest = (unsigned int *)((char *)dest + sizeof(unsigned int) - *lastPartialLength);
		csum += (temp - *lastPartialInt);
		copylen -= sizeof(unsigned int) - *lastPartialLength;
		// now we have an unaligned source and an unaligned destination
		for( ;copylen >= sizeof(*src); copylen -= sizeof(*src)) {
		    memcpy(&temp, src, sizeof(temp));
		    src++;
		    csum += temp;
		    memcpy(dest, &temp, sizeof(temp));
		    dest++;
		}
		*lastPartialLength = 0;
		*lastPartialInt = 0;
	    }
	    else { // NO, we don't...
		memcpy(((char *)&temp + *lastPartialLength), src, copylen);
		memcpy(dest, ((char *)&temp + *lastPartialLength), copylen);
		src = (unsigned int *)((char *)src + copylen);
		dest = (unsigned int *)((char *)dest + copylen);
		csum += (temp - *lastPartialInt);
		*lastPartialInt = temp;
		*lastPartialLength += copylen;
		copylen = 0;
	    }
	}
	else { // fast path...
	    unsigned long numLongs = copylen/sizeof(unsigned int);
	    for(i = 0; i < numLongs; i++) {
		csum += *src;
		*dest++ = *src++;
	    }
	    *lastPartialInt = 0;
	    *lastPartialLength = 0;
	    if (intaligned(copylen) && (csumlenresidue == 0)) {
		return(csum);
	    }
	    else {
		copylen -= i * sizeof(unsigned int);
	    }
	}
    } else if (intaligned(source)) {
#ifdef _COUNT_ME_
	_AU++;
#endif
	if (*lastPartialLength) {
	    // do we have enough data to fill out the partial word?
	    if (copylen >= (sizeof(unsigned int) - *lastPartialLength)) { // YES, we do...
		memcpy(((char *)&temp + *lastPartialLength), src,
		       (sizeof(unsigned int) - *lastPartialLength));
		memcpy(dest, ((char *)&temp + *lastPartialLength),
		       (sizeof(unsigned int) - *lastPartialLength));
		src = (unsigned int *)((char *)src + sizeof(unsigned int) - *lastPartialLength);
		dest = (unsigned int *)((char *)dest + sizeof(unsigned int) - *lastPartialLength);
		csum += (temp - *lastPartialInt);
		copylen -= sizeof(unsigned int) - *lastPartialLength;
		// now we have an unaligned source and an unknown alignment for our destination
		if (intaligned(dest)) {
		    unsigned long numLongs = copylen/sizeof(unsigned int);
		    for(i = 0; i < numLongs; i++) {
			memcpy(&temp, src, sizeof(temp));
			src++;
			csum += temp;
			*dest++ = temp;
		    }
		    copylen -= i * sizeof(unsigned int);
		}
		else {
		    for( ;copylen >= sizeof(*src); copylen -= sizeof(*src)) {
			memcpy(&temp, src, sizeof(temp));
			src++;
			csum += temp;
			memcpy(dest, &temp, sizeof(temp));
			dest++;
		    }
		}
		*lastPartialInt = 0;
		*lastPartialLength = 0;
	    }
	    else { // NO, we don't...
		memcpy(((char *)&temp + *lastPartialLength), src, copylen);
		memcpy(dest, ((char *)&temp + *lastPartialLength), copylen);
		src = (unsigned int *)((char *)src + copylen);
		dest = (unsigned int *)((char *)dest + copylen);
		csum += (temp - *lastPartialInt);
		*lastPartialInt = temp;
		*lastPartialLength += copylen;
		copylen = 0;
	    }
	}
	else {
	    for( ;copylen >= sizeof(*src); copylen -= sizeof(*src)) {
		temp = *src++;
		csum += temp;
		memcpy(dest, &temp, sizeof(temp));
		dest++;
	    }
	    *lastPartialInt = 0;
	    *lastPartialLength = 0;
	}
    } else if (intaligned(dest)) {
#ifdef _COUNT_ME_
	_UA++;
#endif
	if (*lastPartialLength) {
	    // do we have enough data to fill out the partial word?
	    if (copylen >= (sizeof(unsigned int) - *lastPartialLength)) { // YES, we do...
		memcpy(((char *)&temp + *lastPartialLength), src,
		       (sizeof(unsigned int) - *lastPartialLength));
		memcpy(dest, ((char *)&temp + *lastPartialLength),
		       (sizeof(unsigned int) - *lastPartialLength));
		src = (unsigned int *)((char *)src + sizeof(unsigned int) - *lastPartialLength);
		dest = (unsigned int *)((char *)dest + sizeof(unsigned int) - *lastPartialLength);
		csum += (temp - *lastPartialInt);
		copylen -= sizeof(unsigned int) - *lastPartialLength;
		// now we have a source of unknown alignment and a unaligned destination
		if (intaligned(src)) {
		    for( ;copylen >= sizeof(*src); copylen -= sizeof(*src)) {
			temp = *src++;
			csum += temp;
			memcpy(dest, &temp, sizeof(temp));
			dest++;
		    }
		    *lastPartialInt = 0;
		    *lastPartialLength = 0;
		}
		else {
		    for( ;copylen >= sizeof(*src); copylen -= sizeof(*src)) {
			memcpy(&temp, src, sizeof(temp));
			src++;
			csum += temp;
			memcpy(dest, &temp, sizeof(temp));
			dest++;
		    }
		    *lastPartialLength = 0;
		    *lastPartialInt = 0;
		}
	    }
	    else { // NO, we don't...
		memcpy(((char *)&temp + *lastPartialLength), src, copylen);
		memcpy(dest, ((char *)&temp + *lastPartialLength), copylen);
		src = (unsigned int *)((char *)src + copylen);
		dest = (unsigned int *)((char *)dest + copylen);
		csum += (temp - *lastPartialInt);
		*lastPartialInt = temp;
		*lastPartialLength += copylen;
		copylen = 0;
	    }
	}
	else {
	    for( ;copylen >= sizeof(*src); copylen -= sizeof(*src)) {
		memcpy(&temp, src, sizeof(temp));
		src++;
		csum += temp;
		*dest++ = temp;
	    }
	    *lastPartialLength = 0;
	    *lastPartialInt = 0;
	}
    } else {
#ifdef _COUNT_ME_
	_UU++;
#endif
	if (*lastPartialLength) {
	    // do we have enough data to fill out the partial word?
	    if (copylen >= (sizeof(unsigned int) - *lastPartialLength)) { // YES, we do...
		memcpy(((char *)&temp + *lastPartialLength), src,
		       (sizeof(unsigned int) - *lastPartialLength));
		memcpy(dest, ((char *)&temp + *lastPartialLength),
		       (sizeof(unsigned int) - *lastPartialLength));
		src = (unsigned int *)((char *)src + sizeof(unsigned int) - *lastPartialLength);
		dest = (unsigned int *)((char *)dest + sizeof(unsigned int) - *lastPartialLength);
		csum += (temp - *lastPartialInt);
		copylen -= sizeof(unsigned int) - *lastPartialLength;
		// now we have an unknown alignment for our source and destination
		if (intaligned(src) && intaligned(dest)) {
		    unsigned long numLongs = copylen/sizeof(unsigned int);
		    for(i = 0; i < numLongs; i++) {
			csum += *src;
			*dest++ = *src++;
		    }
		    copylen -= i * sizeof(unsigned int);
		}
		else { // safe but slower for all other alignments
		    for( ;copylen >= sizeof(*src); copylen -= sizeof(*src)) {
			memcpy(&temp, src, sizeof(temp));
			src++;
			csum += temp;
			memcpy(dest, &temp, sizeof(temp));
			dest++;
		    }
		}
		*lastPartialInt = 0;
		*lastPartialLength = 0;
	    }
	    else { // NO, we don't...
		memcpy(((char *)&temp + *lastPartialLength), src, copylen);
		memcpy(dest, ((char *)&temp + *lastPartialLength), copylen);
		src = (unsigned int *)((char *)src + copylen);
		dest = (unsigned int *)((char *)dest + copylen);
		csum += (temp - *lastPartialInt);
		*lastPartialInt = temp;
		*lastPartialLength += copylen;
		copylen = 0;
	    }
	}
	else {
	    for( ;copylen >= sizeof(*src); copylen -= sizeof(*src)) {
		memcpy(&temp, src, sizeof(temp));
		src++;
		csum += temp;
		memcpy(dest, &temp, sizeof(temp));
		dest++;
	    }
	    *lastPartialLength = 0;
	    *lastPartialInt = 0;
	}
    }

    /* if copylen is non-zero there was a bit left, less than an unsigned int's worth */

    if ((copylen != 0) && (csumlenresidue == 0)) {
	temp = *lastPartialInt;
	if (*lastPartialLength) {
	    if (copylen >= (sizeof(unsigned int) - *lastPartialLength)) {
		// copy all remaining bytes from src to dest
		unsigned int copytemp = 0;
		memcpy(&copytemp, src, copylen);
		memcpy(dest, &copytemp, copylen);
		// fill out rest of partial word and add to checksum
		memcpy(((char *)&temp + *lastPartialLength), src,
		       (sizeof(unsigned int) - *lastPartialLength));
		// avoid unsigned arithmetic overflow by subtracting the old partial
		// word from the new one before adding to the checksum...
		csum += (temp - *lastPartialInt);
		copylen -= sizeof(unsigned int) - *lastPartialLength;
		src = (unsigned int *)((char *)src + sizeof(unsigned int) - *lastPartialLength);
		*lastPartialLength = copylen;
		// reset temp, and calculate next partial word
		temp = 0;
		if (copylen) {
		    memcpy(&temp, src, copylen);
		}
		// add it to the the checksum
		csum += temp;
		*lastPartialInt = temp;
	    }
	    else {
		// copy all remaining bytes from src to dest
		unsigned int copytemp = 0;
		memcpy(&copytemp, src, copylen);
		memcpy(dest, &copytemp, copylen);
		// fill out rest of partial word and add to checksum
		memcpy(((char *)&temp + *lastPartialLength), src,
		       copylen);
		// avoid unsigned arithmetic overflow by subtracting the old partial
		// word from the new one before adding to the checksum...
		csum += temp - *lastPartialInt;
		*lastPartialInt = temp;
		*lastPartialLength += copylen;
	    }
	}
	else { // fast path...
	    // temp and *lastPartialInt are 0 if *lastPartialLength is 0...
	    memcpy(&temp, src, copylen);
	    csum += temp;
	    memcpy(dest, &temp, copylen);
	    *lastPartialInt = temp;
	    *lastPartialLength = copylen;
	    // done...return the checksum
	}
    }
    else if (csumlenresidue != 0) {
	if (copylen != 0) {
	    temp = 0;
	    memcpy(&temp, src, copylen);
	    memcpy(dest, &temp, copylen);
	}
	if (csumlenresidue < (ssize_t)(sizeof(unsigned int) - copylen - *lastPartialLength)) {
	    temp = *lastPartialInt;
	    memcpy(((char *)&temp + *lastPartialLength), src, (copylen + csumlenresidue));
	    // avoid unsigned arithmetic overflow by subtracting the old partial
	    // word from the new one before adding to the checksum...
	    csum += temp - *lastPartialInt;
	    src++;
	    *lastPartialInt = temp;
	    *lastPartialLength += copylen + csumlenresidue;
	    csumlenresidue = 0;
	}
	else {
	    // we have enough chksum data to fill out our last partial
	    // word
	    temp = *lastPartialInt;
	    memcpy(((char *)&temp + *lastPartialLength), src,
		   (sizeof(unsigned int) - *lastPartialLength));
	    // avoid unsigned arithmetic overflow by subtracting the old partial
	    // word from the new one before adding to the checksum...
	    csum += temp - *lastPartialInt;
	    src = (unsigned int *)((char *)src + sizeof(unsigned int) - *lastPartialLength);
	    csumlenresidue -= sizeof(unsigned int) - *lastPartialLength - copylen;
	    *lastPartialLength = 0;
	    *lastPartialInt = 0;
	}
	if (intaligned(src)) {
	    for (i = 0; i < csumlenresidue/sizeof(unsigned int); i++) {
		csum += *src++;
	    }
	}
	else {
	    for (i = 0; i < csumlenresidue/sizeof(unsigned int); i++) {
		memcpy(&temp, src, sizeof(temp));
		csum += temp;
		src++;
	    }
	}
	csumlenresidue -= i * sizeof(unsigned int);
	if (csumlenresidue) {
	    temp = 0;
	    memcpy(&temp, src, csumlenresidue);
	    csum += temp;
	    *lastPartialInt = temp;
	    *lastPartialLength = csumlenresidue;
	}
    } /* end else if (csumlenresidue != 0) */

    return csum;
}

/*
 * this version is appropriate for checksumming a buffer via one call
 * to bcopy_uicsum()
 */

unsigned int
bcopy_uicsum (
    const void * RESTRICT_MACRO source,
    void * RESTRICT_MACRO destination,
    unsigned long copylen,
    unsigned long csumlen
    )
{
    unsigned int pint = 0;
    unsigned int plength = 0;
    return bcopy_uicsum(source, destination, copylen, csumlen, &pint, &plength);
}

#ifdef _COUNT_ME_
void
print_bcopy_csum_stats() {
    printf("\n*** begin statistics for bcopy_csum() ***\n");
    printf("        aligned/aligned: %10ld\n", _AA);
    printf("      aligned/unaligned: %10ld\n", _AU);
    printf("      unaligned/aligned: %10ld\n", _UA);
    printf("    unaligned/unaligned: %10ld\n", _UU);
    printf("            total calls: %10ld\n", _AA+_AU+_UA+_UU);
    printf("*** end statistics for bcopy_csum() ***\n\n");
}
#endif

/*
 * csum() generates a bcopy_csum() - compatible checksum that can be
 * called multiple times
 */

unsigned long
csum (
    const void * RESTRICT_MACRO source,
    unsigned long csumlen,
    unsigned long * RESTRICT_MACRO lastPartialLong,
    unsigned long * RESTRICT_MACRO lastPartialLength
    )
{
    unsigned long * RESTRICT_MACRO src = (unsigned long *) source;
    unsigned long csum = 0;
    unsigned long i, temp;



    temp = *lastPartialLong;

    if (wordaligned(source))  {
	if (*lastPartialLength) {
	    // do we have enough data to fill out the partial word?
	    if (csumlen >= (sizeof(unsigned long) - *lastPartialLength)) { // YES, we do...
		memcpy(((char *)&temp + *lastPartialLength), src,
		       (sizeof(unsigned long) - *lastPartialLength));
		src = (unsigned long *)((char *)src + sizeof(unsigned long) - *lastPartialLength);
		csum += (temp - *lastPartialLong);
		csumlen -= sizeof(unsigned long) - *lastPartialLength;
		// now we have an unaligned source
		for(i = 0; i < csumlen/sizeof(unsigned long); i++) {
		    memcpy(&temp, src, sizeof(temp));
		    csum += temp;
		    src++;
		}
		csumlen -= i * sizeof(unsigned long);
		*lastPartialLong = 0;
		*lastPartialLength = 0;
	    }
	    else { // NO, we don't...
		memcpy(((char *)&temp + *lastPartialLength), src, csumlen);
		src = (unsigned long *)((char *)src + csumlen);
		csum += (temp - *lastPartialLong);
		*lastPartialLong = temp;
		*lastPartialLength += csumlen;
		csumlen = 0;
	    }
	}
	else { // fast path...
	    unsigned long numLongs = csumlen/sizeof(unsigned long);
	    for(i = 0; i < numLongs; i++) {
		csum += *src++;
	    }
	    *lastPartialLong = 0;
	    *lastPartialLength = 0;
	    if (wordaligned(csumlen)) {
		return(csum);
	    }
	    else {
		csumlen -= i * sizeof(unsigned long);
	    }
	}
    } else {
	if (*lastPartialLength) {
	    // do we have enough data to fill out the partial word?
	    if (csumlen >= (sizeof(unsigned long) - *lastPartialLength)) { // YES, we do...
		memcpy(((char *)&temp + *lastPartialLength), src,
		       (sizeof(unsigned long) - *lastPartialLength));
		src = (unsigned long *)((char *)src + sizeof(unsigned long) - *lastPartialLength);
		csum += (temp - *lastPartialLong);
		csumlen -= sizeof(unsigned long) - *lastPartialLength;
		// now we have a source of unknown alignment
		if (wordaligned(src)) {
		    for(i = 0; i < csumlen/sizeof(unsigned long); i++) {
			csum += *src++;
		    }
		    csumlen -= i * sizeof(unsigned long);
		    *lastPartialLong = 0;
		    *lastPartialLength = 0;
		}
		else {
		    for(i = 0; i < csumlen/sizeof(unsigned long); i++) {
			memcpy(&temp, src, sizeof(temp));
			csum += temp;
			src++;
		    }
		    csumlen -= i * sizeof(unsigned long);
		    *lastPartialLong = 0;
		    *lastPartialLength = 0;
		}
	    }
	    else { // NO, we don't...
		memcpy(((char *)&temp + *lastPartialLength), src, csumlen);
		src = (unsigned long *)((char *)src + csumlen);
		csum += (temp - *lastPartialLong);
		*lastPartialLong = temp;
		*lastPartialLength += csumlen;
		csumlen = 0;
	    }
	}
	else {
	    for( ;csumlen >= sizeof(*src); csumlen -= sizeof(*src)) {
		memcpy(&temp, src, sizeof(temp));
		src++;
		csum += temp;
	    }
	    *lastPartialLength = 0;
	    *lastPartialLong = 0;
	}
    }

    /* if csumlen is non-zero there was a bit left, less than an unsigned long's worth */

    if (csumlen != 0) {
	temp = *lastPartialLong;
	if (*lastPartialLength) {
	    if (csumlen >= (sizeof(unsigned long) - *lastPartialLength)) {
		// fill out rest of partial word and add to checksum
		memcpy(((char *)&temp + *lastPartialLength), src,
		       (sizeof(unsigned long) - *lastPartialLength));
		csum += (temp - *lastPartialLong);
		csumlen -= sizeof(unsigned long) - *lastPartialLength;
		src = (unsigned long *)((char *)src + sizeof(unsigned long) - *lastPartialLength);
		*lastPartialLength = csumlen;
		// reset temp, and calculate next partial word
		temp = 0;
		if (csumlen) {
		    memcpy(&temp, src, csumlen);
		}
		// add it to the the checksum
		csum += temp;
		*lastPartialLong = temp;
	    }
	    else {
		// fill out rest of partial word and add to checksum
		memcpy(((char *)&temp + *lastPartialLength), src,
		       csumlen);
		csum += (temp - *lastPartialLong);
		*lastPartialLong = temp;
		*lastPartialLength += csumlen;
	    }
	}
	else { // fast path...
	    // temp and *lastPartialLong are 0 if *lastPartialLength is 0...
	    memcpy(&temp, src, csumlen);
	    csum += temp;
	    *lastPartialLong = temp;
	    *lastPartialLength = csumlen;
	    // done...return the checksum
	}
    }

    return csum;
}

/* wrapper for single csum() call */

unsigned long csum(const void * RESTRICT_MACRO source, unsigned long csumlen)
{
    unsigned long lastPartialLong = 0;
    unsigned long lastPartialLength = 0;
    return csum(source, csumlen, &lastPartialLong, &lastPartialLength);
}

unsigned int
uicsum (
    const void * RESTRICT_MACRO source,
    unsigned long csumlen,
    unsigned int * RESTRICT_MACRO lastPartialInt,
    unsigned int * RESTRICT_MACRO lastPartialLength
    )
{
    unsigned int * RESTRICT_MACRO src = (unsigned int *) source;
    unsigned int csum = 0;
    unsigned int temp;
    unsigned long i;


    temp = *lastPartialInt;

    if (intaligned(source))  {
	if (*lastPartialLength) {
	    // do we have enough data to fill out the partial word?
	    if (csumlen >= (sizeof(unsigned int) - *lastPartialLength)) { // YES, we do...
		memcpy(((char *)&temp + *lastPartialLength), src,
		       (sizeof(unsigned int) - *lastPartialLength));
		src = (unsigned int *)((char *)src + sizeof(unsigned int) - *lastPartialLength);
		csum += (temp - *lastPartialInt);
		csumlen -= sizeof(unsigned int) - *lastPartialLength;
		// now we have an unaligned source
		for(i = 0; i < csumlen/sizeof(unsigned int); i++) {
		    memcpy(&temp, src, sizeof(temp));
		    csum += temp;
		    src++;
		}
		csumlen -= i * sizeof(unsigned int);
		*lastPartialInt = 0;
		*lastPartialLength = 0;
	    }
	    else { // NO, we don't...
		memcpy(((char *)&temp + *lastPartialLength), src, csumlen);
		src = (unsigned int *)((char *)src + csumlen);
		csum += (temp - *lastPartialInt);
		*lastPartialInt = temp;
		*lastPartialLength += csumlen;
		csumlen = 0;
	    }
	}
	else { // fast path...
	    unsigned long numLongs = csumlen/sizeof(unsigned int);
	    for(i = 0; i < numLongs; i++) {
		csum += *src++;
	    }
	    *lastPartialInt = 0;
	    *lastPartialLength = 0;
	    if (intaligned(csumlen)) {
		return(csum);
	    }
	    else {
		csumlen -= i * sizeof(unsigned int);
	    }
	}
    } else {
	if (*lastPartialLength) {
	    // do we have enough data to fill out the partial word?
	    if (csumlen >= (sizeof(unsigned int) - *lastPartialLength)) { // YES, we do...
		memcpy(((char *)&temp + *lastPartialLength), src,
		       (sizeof(unsigned int) - *lastPartialLength));
		src = (unsigned int *)((char *)src + sizeof(unsigned int) - *lastPartialLength);
		csum += (temp - *lastPartialInt);
		csumlen -= sizeof(unsigned int) - *lastPartialLength;
		// now we have a source of unknown alignment
		if (intaligned(src)) {
		    for(i = 0; i < csumlen/sizeof(unsigned int); i++) {
			csum += *src++;
		    }
		    csumlen -= i * sizeof(unsigned int);
		    *lastPartialInt = 0;
		    *lastPartialLength = 0;
		}
		else {
		    for(i = 0; i < csumlen/sizeof(unsigned int); i++) {
			memcpy(&temp, src, sizeof(temp));
			csum += temp;
			src++;
		    }
		    csumlen -= i * sizeof(unsigned int);
		    *lastPartialInt = 0;
		    *lastPartialLength = 0;
		}
	    }
	    else { // NO, we don't...
		memcpy(((char *)&temp + *lastPartialLength), src, csumlen);
		src = (unsigned int *)((char *)src + csumlen);
		csum += (temp - *lastPartialInt);
		*lastPartialInt = temp;
		*lastPartialLength += csumlen;
		csumlen = 0;
	    }
	}
	else {
	    for( ;csumlen >= sizeof(*src); csumlen -= sizeof(*src)) {
		memcpy(&temp, src, sizeof(temp));
		src++;
		csum += temp;
	    }
	    *lastPartialLength = 0;
	    *lastPartialInt = 0;
	}
    }

    /* if csumlen is non-zero there was a bit left, less than an unsigned int's worth */

    if (csumlen != 0) {
	temp = *lastPartialInt;
	if (*lastPartialLength) {
	    if (csumlen >= (sizeof(unsigned int) - *lastPartialLength)) {
		// fill out rest of partial word and add to checksum
		memcpy(((char *)&temp + *lastPartialLength), src,
		       (sizeof(unsigned int) - *lastPartialLength));
		csum += (temp - *lastPartialInt);
		csumlen -= sizeof(unsigned int) - *lastPartialLength;
		src = (unsigned int *)((char *)src + sizeof(unsigned int) - *lastPartialLength);
		*lastPartialLength = csumlen;
		// reset temp, and calculate next partial word
		temp = 0;
		if (csumlen) {
		    memcpy(&temp, src, csumlen);
		}
		// add it to the the checksum
		csum += temp;
		*lastPartialInt = temp;
	    }
	    else {
		// fill out rest of partial word and add to checksum
		memcpy(((char *)&temp + *lastPartialLength), src,
		       csumlen);
		csum += (temp - *lastPartialInt);
		*lastPartialInt = temp;
		*lastPartialLength += csumlen;
	    }
	}
	else { // fast path...
	    // temp and *lastPartialInt are 0 if *lastPartialLength is 0...
	    memcpy(&temp, src, csumlen);
	    csum += temp;
	    *lastPartialInt = temp;
	    *lastPartialLength = csumlen;
	    // done...return the checksum
	}
    }

    return csum;
}

/* wrapper for single uicsum() call */

unsigned int uicsum(const void * RESTRICT_MACRO source, unsigned long csumlen)
{
    unsigned int lastPartialInt = 0;
    unsigned int lastPartialLength = 0;
    return uicsum(source, csumlen, &lastPartialInt, &lastPartialLength);
}

/* globals for CRC32 bcopy and calculation routines */

static bool _ulm_crc_table_initialized = false;
static unsigned int _ulm_crc_table[256];

/* CRC32 table generation routine - thanks to Charles Michael Heard for his
 * optimized CRC32 code...
 */

void ulm_initialize_crc_table(void)
{
    register int i,j;
    register unsigned int crc_accum;

    for (i = 0; i < 256; i++) {
        crc_accum = (i << 24);
        for (j = 0; j < 8; j++) {
            if (crc_accum & 0x80000000)
                crc_accum = (crc_accum << 1) ^ CRC_POLYNOMIAL;
            else
                crc_accum = (crc_accum << 1);
        }
        _ulm_crc_table[i] = crc_accum;
    }

    /* set global bool to true to do this work once! */
    _ulm_crc_table_initialized = true;
    return;
}

unsigned int bcopy_uicrc(const void * RESTRICT_MACRO source, void * RESTRICT_MACRO destination,
                         unsigned long copylen, unsigned long crclen, unsigned int partial_crc)
{
    unsigned long crclenresidue = (crclen > copylen) ? (crclen - copylen) : 0;
    register int i, j;
    register unsigned int tmp;
    register unsigned char t;

    if (!_ulm_crc_table_initialized) {
        ulm_initialize_crc_table();
    }

    if (intaligned(source) && intaligned(destination)) {
        register unsigned int * RESTRICT_MACRO src = (unsigned int *)source;
        register unsigned int * RESTRICT_MACRO dst = (unsigned int *)destination;
        register unsigned char *ts, *td;
        /* copy whole integers */
        while (copylen >= sizeof(unsigned int)) {
            tmp = *src++;
            *dst++ = tmp;
            ts = (unsigned char *)&tmp;
            for (j = 0; j < (int)sizeof(unsigned int); j++) {
                i = ((partial_crc >> 24) ^ *ts++) & 0xff;
                partial_crc = (partial_crc << 8) ^ _ulm_crc_table[i];
            }
            copylen -= sizeof(unsigned int);
        }
        ts = (unsigned char *)src;
        td = (unsigned char *)dst;
        /* copy partial integer */
        while (copylen--) {
            t = *ts++;
            *td++ = t;
            i = ((partial_crc >> 24) ^ t) & 0xff;
            partial_crc = (partial_crc << 8) ^ _ulm_crc_table[i];
        }
        /* calculate CRC over remaining bytes... */
        while (crclenresidue--) {
            i = ((partial_crc >> 24) ^ *ts++) & 0xff;
            partial_crc = (partial_crc << 8) ^ _ulm_crc_table[i];
        }
    }
    else {
        register unsigned char * RESTRICT_MACRO src = (unsigned char *)source;
        register unsigned char * RESTRICT_MACRO dst = (unsigned char *)destination;
        while (copylen--) {
            t = *src++;
            *dst++ = t;
            i = ((partial_crc >> 24) ^ t) & 0xff;
            partial_crc = (partial_crc << 8) ^ _ulm_crc_table[i];
        }
        while (crclenresidue--) {
            i = ((partial_crc >> 24) ^ *src++) & 0xff;
            partial_crc = (partial_crc << 8) ^ _ulm_crc_table[i];
        }
    }

    return partial_crc;
}

/* wrapper for single bcopy_crc() call */

unsigned int bcopy_uicrc(const void * RESTRICT_MACRO source, void * RESTRICT_MACRO destination,
    unsigned long copylen, unsigned long crclen)
{
    return bcopy_uicrc(source, destination, copylen, crclen, CRC_INITIAL_REGISTER);
}

unsigned int uicrc(const void * RESTRICT_MACRO source, unsigned long crclen, unsigned int partial_crc) 
{
    register int i, j;
    register unsigned int tmp;
    register unsigned char * t;

    if (!_ulm_crc_table_initialized) {
        ulm_initialize_crc_table();
    }
    
    if (intaligned(source)) {
        register unsigned int * RESTRICT_MACRO src = (unsigned int *)source;
        while (crclen >= sizeof(unsigned int)) {
            tmp = *src++;
            t = (unsigned char *)&tmp;
            for (j = 0; j < (int)sizeof(unsigned int); j++) {
                i = ((partial_crc >> 24) ^ *t++) & 0xff;
                partial_crc = (partial_crc << 8) ^ _ulm_crc_table[i];
            }
            crclen -= sizeof(unsigned int);
        }
        t = (unsigned char *)src;
        while (crclen--) {
            i = ((partial_crc >> 24) ^ *t++) & 0xff;
            partial_crc = (partial_crc << 8) ^ _ulm_crc_table[i];
        }
    }
    else {
        register unsigned char * RESTRICT_MACRO src = (unsigned char *)source;
        while (crclen--) {
            i = ((partial_crc >> 24) ^ *src++) & 0xff;
            partial_crc = (partial_crc << 8) ^ _ulm_crc_table[i];
        }
    }
    
    return partial_crc;
}

/* wrapper for single crc() call */

unsigned int uicrc(const void * RESTRICT_MACRO source, unsigned long crclen)
{
    return uicrc(source, crclen, CRC_INITIAL_REGISTER);
}

/*
 * This routine is used to write a pattern to memory
 */

void poisonMemory(void *ptr, ssize_t lenInBytes, int pattern)

{
    // get length of region to poison
    int lenInInts=lenInBytes/sizeof(int);

    int *intPtr=(int *)ptr;

    // poison memory
    for(int i=0 ; i < lenInInts ; i++ ) {
	*intPtr=pattern;
	intPtr++;
    }

    return;
}
