/*
 * Copyright 2002-2003.  The Regents of the University of
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

#ifndef IB_STATE_H
#define IB_STATE_H

#include <vapi.h>

#define LAMPI_MAX_IB_HCA_PORTS  2
#define LAMPI_MAX_IB_HCAS       4

/* no dynamic sizing to help performance -- or at least
 * not unintentionally hurt it... 
 */

typedef struct {
    VAPI_qp_hndl_t handle;
    VAPI_qp_prop_t prop;
    bool receive_multicast;
} ib_qp_state_t;

typedef struct {
    bool usable;
    int num_active_ports;
    int active_ports[LAMPI_MAX_IB_HCA_PORTS];
    VAPI_hca_hndl_t handle;
    VAPI_hca_vendor_t vendor;
    VAPI_hca_cap_t cap;
    VAPI_hca_port_t ports[LAMPI_MAX_IB_HCA_PORTS];
    VAPI_pd_hndl_t pd;
    VAPI_cq_hndl_t recv_cq;
    VAPI_cq_hndl_t send_cq;
    VAPI_cqe_num_t recv_cq_size;
    VAPI_cqe_num_t send_cq_size;
    ib_qp_state_t ud;
} ib_hca_state_t;

typedef struct {
    u_int32_t num_hcas;
    int num_active_hcas;
    int active_hcas[LAMPI_MAX_IB_HCAS];
    VAPI_hca_id_t hca_ids[LAMPI_MAX_IB_HCAS];
    ib_hca_state_t hca[LAMPI_MAX_IB_HCAS];
    VAPI_qkey_t qkey;
} ib_state_t;

extern ib_state_t ib_state;

#endif
