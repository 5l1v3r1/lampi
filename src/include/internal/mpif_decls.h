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

#ifndef _ULM_MPIF_DECLS_H_
#define _ULM_MPIF_DECLS_H_

#include "internal/linkage.h"

CDECL_BEGIN

/*
 * This file contains only the function prototypes of the Fortran MPI bindings.
 * As of gcc 3.1.1, the compiler will not properly emit weak symbols for the
 * below listed functions unless they are also declared....Mitch, 9/20/02
 *
 * The exact form of the declaration does not matter as they will only be
 * called from Fortran.
 */

/************************ Function Prototypes ***************************/

/*
 * MPI prototypes (includes most of MPI2)
 */
void MPI_WTICK(void);
void mpi_wtick(void);
void mpi_wtick_(void);
void mpi_wtick__(void);
void MPI_WTIME(void);
void mpi_wtime(void);
void mpi_wtime_(void);
void mpi_wtime__(void);
void MPI_ABORT(void);
void mpi_abort(void);
void mpi_abort_(void);
void mpi_abort__(void);
void MPI_ADDRESS(void);
void mpi_address(void);
void mpi_address_(void);
void mpi_address__(void);
void MPI_ALLGATHER(void);
void mpi_allgather(void);
void mpi_allgather_(void);
void mpi_allgather__(void);
void MPI_ALLGATHERV(void);
void mpi_allgatherv(void);
void mpi_allgatherv_(void);
void mpi_allgatherv__(void);
void MPI_ALLREDUCE(void);
void mpi_allreduce(void);
void mpi_allreduce_(void);
void mpi_allreduce__(void);
void MPI_ALLTOALL(void);
void mpi_alltoall(void);
void mpi_alltoall_(void);
void mpi_alltoall__(void);
void MPI_ALLTOALLV(void);
void mpi_alltoallv(void);
void mpi_alltoallv_(void);
void mpi_alltoallv__(void);
void MPI_ALLTOALLW(void);
void mpi_alltoallw(void);
void mpi_alltoallw_(void);
void mpi_alltoallw__(void);
void MPI_ATTR_DELETE(void);
void mpi_attr_delete(void);
void mpi_attr_delete_(void);
void mpi_attr_delete__(void);
void MPI_ATTR_GET(void);
void mpi_attr_get(void);
void mpi_attr_get_(void);
void mpi_attr_get__(void);
void MPI_ATTR_PUT(void);
void mpi_attr_put(void);
void mpi_attr_put_(void);
void mpi_attr_put__(void);
void MPI_BARRIER(void);
void mpi_barrier(void);
void mpi_barrier_(void);
void mpi_barrier__(void);
void MPI_BCAST(void);
void mpi_bcast(void);
void mpi_bcast_(void);
void mpi_bcast__(void);
void MPI_BSEND(void);
void mpi_bsend(void);
void mpi_bsend_(void);
void mpi_bsend__(void);
void MPI_BSEND_INIT(void);
void mpi_bsend_init(void);
void mpi_bsend_init_(void);
void mpi_bsend_init__(void);
void MPI_BUFFER_ATTACH(void);
void mpi_buffer_attach(void);
void mpi_buffer_attach_(void);
void mpi_buffer_attach__(void);
void MPI_BUFFER_DETACH(void);
void mpi_buffer_detach(void);
void mpi_buffer_detach_(void);
void mpi_buffer_detach__(void);
void MPI_CANCEL(void);
void mpi_cancel(void);
void mpi_cancel_(void);
void mpi_cancel__(void);
void MPI_CART_COORDS(void);
void mpi_cart_coords(void);
void mpi_cart_coords_(void);
void mpi_cart_coords__(void);
void MPI_CART_CREATE(void);
void mpi_cart_create(void);
void mpi_cart_create_(void);
void mpi_cart_create__(void);
void MPI_CART_GET(void);
void mpi_cart_get(void);
void mpi_cart_get_(void);
void mpi_cart_get__(void);
void MPI_CART_MAP(void);
void mpi_cart_map(void);
void mpi_cart_map_(void);
void mpi_cart_map__(void);
void MPI_CART_RANK(void);
void mpi_cart_rank(void);
void mpi_cart_rank_(void);
void mpi_cart_rank__(void);
void MPI_CART_SHIFT(void);
void mpi_cart_shift(void);
void mpi_cart_shift_(void);
void mpi_cart_shift__(void);
void MPI_CART_SUB(void);
void mpi_cart_sub(void);
void mpi_cart_sub_(void);
void mpi_cart_sub__(void);
void MPI_CARTDIM_GET(void);
void mpi_cartdim_get(void);
void mpi_cartdim_get_(void);
void mpi_cartdim_get__(void);
void MPI_COMM_COMPARE(void);
void mpi_comm_compare(void);
void mpi_comm_compare_(void);
void mpi_comm_compare__(void);
void MPI_COMM_CREATE(void);
void mpi_comm_create(void);
void mpi_comm_create_(void);
void mpi_comm_create__(void);
void MPI_COMM_DUP(void);
void mpi_comm_dup(void);
void mpi_comm_dup_(void);
void mpi_comm_dup__(void);
void MPI_COMM_FREE(void);
void mpi_comm_free(void);
void mpi_comm_free_(void);
void mpi_comm_free__(void);
void MPI_COMM_GET_NAME(void);
void mpi_comm_get_name(void);
void mpi_comm_get_name_(void);
void mpi_comm_get_name__(void);
void MPI_COMM_GROUP(void);
void mpi_comm_group(void);
void mpi_comm_group_(void);
void mpi_comm_group__(void);
void MPI_COMM_RANK(void);
void mpi_comm_rank(void);
void mpi_comm_rank_(void);
void mpi_comm_rank__(void);
void MPI_COMM_REMOTE_GROUP(void);
void mpi_comm_remote_group(void);
void mpi_comm_remote_group_(void);
void mpi_comm_remote_group__(void);
void MPI_COMM_REMOTE_SIZE(void);
void mpi_comm_remote_size(void);
void mpi_comm_remote_size_(void);
void mpi_comm_remote_size__(void);
void MPI_COMM_SET_NAME(void);
void mpi_comm_set_name(void);
void mpi_comm_set_name_(void);
void mpi_comm_set_name__(void);
void MPI_COMM_SIZE(void);
void mpi_comm_size(void);
void mpi_comm_size_(void);
void mpi_comm_size__(void);
void MPI_COMM_SPLIT(void);
void mpi_comm_split(void);
void mpi_comm_split_(void);
void mpi_comm_split__(void);
void MPI_COMM_TEST_INTER(void);
void mpi_comm_test_inter(void);
void mpi_comm_test_inter_(void);
void mpi_comm_test_inter__(void);
void MPI_COMM_CREATE_ERRHANDLER(void);
void mpi_comm_create_errhandler(void);
void mpi_comm_create_errhandler_(void);
void mpi_comm_create_errhandler__(void);
void MPI_COMM_FREE_ERRHANDLER(void);
void mpi_comm_free_errhandler(void);
void mpi_comm_free_errhandler_(void);
void mpi_comm_free_errhandler__(void);
void MPI_COMM_GET_ERRHANDLER(void);
void mpi_comm_get_errhandler(void);
void mpi_comm_get_errhandler_(void);
void mpi_comm_get_errhandler__(void);
void MPI_COMM_SET_ERRHANDLER(void);
void mpi_comm_set_errhandler(void);
void mpi_comm_set_errhandler_(void);
void mpi_comm_set_errhandler__(void);
void MPI_DIMS_CREATE(void);
void mpi_dims_create(void);
void mpi_dims_create_(void);
void mpi_dims_create__(void);
void MPI_ERRHANDLER_CREATE(void);
void mpi_errhandler_create(void);
void mpi_errhandler_create_(void);
void mpi_errhandler_create__(void);
void MPI_ERRHANDLER_FREE(void);
void mpi_errhandler_free(void);
void mpi_errhandler_free_(void);
void mpi_errhandler_free__(void);
void MPI_ERRHANDLER_GET(void);
void mpi_errhandler_get(void);
void mpi_errhandler_get_(void);
void mpi_errhandler_get__(void);
void MPI_ERRHANDLER_SET(void);
void mpi_errhandler_set(void);
void mpi_errhandler_set_(void);
void mpi_errhandler_set__(void);
void MPI_ERROR_CLASS(void);
void mpi_error_class(void);
void mpi_error_class_(void);
void mpi_error_class__(void);
void MPI_ERROR_STRING(void);
void mpi_error_string(void);
void mpi_error_string_(void);
void mpi_error_string__(void);
void MPI_FINALIZE(void);
void mpi_finalize(void);
void mpi_finalize_(void);
void mpi_finalize__(void);
void MPI_FINALIZED(void);
void mpi_finalized(void);
void mpi_finalized_(void);
void mpi_finalized__(void);
void MPI_GATHER(void);
void mpi_gather(void);
void mpi_gather_(void);
void mpi_gather__(void);
void MPI_GATHERV(void);
void mpi_gatherv(void);
void mpi_gatherv_(void);
void mpi_gatherv__(void);
void MPI_GET_COUNT(void);
void mpi_get_count(void);
void mpi_get_count_(void);
void mpi_get_count__(void);
void MPI_GET_ELEMENTS(void);
void mpi_get_elements(void);
void mpi_get_elements_(void);
void mpi_get_elements__(void);
void MPI_GET_PROCESSOR_NAME(void);
void mpi_get_processor_name(void);
void mpi_get_processor_name_(void);
void mpi_get_processor_name__(void);
void MPI_GET_VERSION(void);
void mpi_get_version(void);
void mpi_get_version_(void);
void mpi_get_version__(void);
void MPI_GRAPH_CREATE(void);
void mpi_graph_create(void);
void mpi_graph_create_(void);
void mpi_graph_create__(void);
void MPI_GRAPH_GET(void);
void mpi_graph_get(void);
void mpi_graph_get_(void);
void mpi_graph_get__(void);
void MPI_GRAPH_MAP(void);
void mpi_graph_map(void);
void mpi_graph_map_(void);
void mpi_graph_map__(void);
void MPI_GRAPH_NEIGHBORS(void);
void mpi_graph_neighbors(void);
void mpi_graph_neighbors_(void);
void mpi_graph_neighbors__(void);
void MPI_GRAPH_NEIGHBORS_COUNT(void);
void mpi_graph_neighbors_count(void);
void mpi_graph_neighbors_count_(void);
void mpi_graph_neighbors_count__(void);
void MPI_GRAPHDIMS_GET(void);
void mpi_graphdims_get(void);
void mpi_graphdims_get_(void);
void mpi_graphdims_get__(void);
void MPI_GROUP_COMPARE(void);
void mpi_group_compare(void);
void mpi_group_compare_(void);
void mpi_group_compare__(void);
void MPI_GROUP_DIFFERENCE(void);
void mpi_group_difference(void);
void mpi_group_difference_(void);
void mpi_group_difference__(void);
void MPI_GROUP_EXCL(void);
void mpi_group_excl(void);
void mpi_group_excl_(void);
void mpi_group_excl__(void);
void MPI_GROUP_FREE(void);
void mpi_group_free(void);
void mpi_group_free_(void);
void mpi_group_free__(void);
void MPI_GROUP_INCL(void);
void mpi_group_incl(void);
void mpi_group_incl_(void);
void mpi_group_incl__(void);
void MPI_GROUP_INTERSECTION(void);
void mpi_group_intersection(void);
void mpi_group_intersection_(void);
void mpi_group_intersection__(void);
void MPI_GROUP_RANGE_EXCL(void);
void mpi_group_range_excl(void);
void mpi_group_range_excl_(void);
void mpi_group_range_excl__(void);
void MPI_GROUP_RANGE_INCL(void);
void mpi_group_range_incl(void);
void mpi_group_range_incl_(void);
void mpi_group_range_incl__(void);
void MPI_GROUP_RANK(void);
void mpi_group_rank(void);
void mpi_group_rank_(void);
void mpi_group_rank__(void);
void MPI_GROUP_SIZE(void);
void mpi_group_size(void);
void mpi_group_size_(void);
void mpi_group_size__(void);
void MPI_GROUP_TRANSLATE_RANKS(void);
void mpi_group_translate_ranks(void);
void mpi_group_translate_ranks_(void);
void mpi_group_translate_ranks__(void);
void MPI_GROUP_UNION(void);
void mpi_group_union(void);
void mpi_group_union_(void);
void mpi_group_union__(void);
void MPI_IBSEND(void);
void mpi_ibsend(void);
void mpi_ibsend_(void);
void mpi_ibsend__(void);
void MPI_INIT(void);
void mpi_init(void);
void mpi_init_(void);
void mpi_init__(void);
void MPI_INIT_THREAD(void);
void mpi_init_thread(void);
void mpi_init_thread_(void);
void mpi_init_thread__(void);
void MPI_INITIALIZED(void);
void mpi_initialized(void);
void mpi_initialized_(void);
void mpi_initialized__(void);
void MPI_INTERCOMM_CREATE(void);
void mpi_intercomm_create(void);
void mpi_intercomm_create_(void);
void mpi_intercomm_create__(void);
void MPI_INTERCOMM_MERGE(void);
void mpi_intercomm_merge(void);
void mpi_intercomm_merge_(void);
void mpi_intercomm_merge__(void);
void MPI_IPROBE(void);
void mpi_iprobe(void);
void mpi_iprobe_(void);
void mpi_iprobe__(void);
void MPI_IRECV(void);
void mpi_irecv(void);
void mpi_irecv_(void);
void mpi_irecv__(void);
void MPI_IRSEND(void);
void mpi_irsend(void);
void mpi_irsend_(void);
void mpi_irsend__(void);
void MPI_ISEND(void);
void mpi_isend(void);
void mpi_isend_(void);
void mpi_isend__(void);
void MPI_ISSEND(void);
void mpi_issend(void);
void mpi_issend_(void);
void mpi_issend__(void);
void MPI_KEYVAL_CREATE(void);
void mpi_keyval_create(void);
void mpi_keyval_create_(void);
void mpi_keyval_create__(void);
void MPI_KEYVAL_FREE(void);
void mpi_keyval_free(void);
void mpi_keyval_free_(void);
void mpi_keyval_free__(void);
void MPI_OP_CREATE(void);
void mpi_op_create(void);
void mpi_op_create_(void);
void mpi_op_create__(void);
void MPI_OP_FREE(void);
void mpi_op_free(void);
void mpi_op_free_(void);
void mpi_op_free__(void);
void MPI_PACK(void);
void mpi_pack(void);
void mpi_pack_(void);
void mpi_pack__(void);
void MPI_PACK_SIZE(void);
void mpi_pack_size(void);
void mpi_pack_size_(void);
void mpi_pack_size__(void);
void MPI_PCONTROL(void);
void mpi_pcontrol(void);
void mpi_pcontrol_(void);
void mpi_pcontrol__(void);
void MPI_PROBE(void);
void mpi_probe(void);
void mpi_probe_(void);
void mpi_probe__(void);
void MPI_RECV(void);
void mpi_recv(void);
void mpi_recv_(void);
void mpi_recv__(void);
void MPI_RECV_INIT(void);
void mpi_recv_init(void);
void mpi_recv_init_(void);
void mpi_recv_init__(void);
void MPI_REDUCE(void);
void mpi_reduce(void);
void mpi_reduce_(void);
void mpi_reduce__(void);
void MPI_REDUCE_SCATTER(void);
void mpi_reduce_scatter(void);
void mpi_reduce_scatter_(void);
void mpi_reduce_scatter__(void);
void MPI_REQUEST_FREE(void);
void mpi_request_free(void);
void mpi_request_free_(void);
void mpi_request_free__(void);
void MPI_RSEND(void);
void mpi_rsend(void);
void mpi_rsend_(void);
void mpi_rsend__(void);
void MPI_RSEND_INIT(void);
void mpi_rsend_init(void);
void mpi_rsend_init_(void);
void mpi_rsend_init__(void);
void MPI_SCAN(void);
void mpi_scan(void);
void mpi_scan_(void);
void mpi_scan__(void);
void MPI_SCATTER(void);
void mpi_scatter(void);
void mpi_scatter_(void);
void mpi_scatter__(void);
void MPI_SCATTERV(void);
void mpi_scatterv(void);
void mpi_scatterv_(void);
void mpi_scatterv__(void);
void MPI_SEND(void);
void mpi_send(void);
void mpi_send_(void);
void mpi_send__(void);
void MPI_SEND_INIT(void);
void mpi_send_init(void);
void mpi_send_init_(void);
void mpi_send_init__(void);
void MPI_SENDRECV(void);
void mpi_sendrecv(void);
void mpi_sendrecv_(void);
void mpi_sendrecv__(void);
void MPI_SENDRECV_REPLACE(void);
void mpi_sendrecv_replace(void);
void mpi_sendrecv_replace_(void);
void mpi_sendrecv_replace__(void);
void MPI_SSEND(void);
void mpi_ssend(void);
void mpi_ssend_(void);
void mpi_ssend__(void);
void MPI_SSEND_INIT(void);
void mpi_ssend_init(void);
void mpi_ssend_init_(void);
void mpi_ssend_init__(void);
void MPI_START(void);
void mpi_start(void);
void mpi_start_(void);
void mpi_start__(void);
void MPI_STARTALL(void);
void mpi_startall(void);
void mpi_startall_(void);
void mpi_startall__(void);
void MPI_STATUS_SET_CANCELLED(void);
void mpi_status_set_cancelled(void);
void mpi_status_set_cancelled_(void);
void mpi_status_set_cancelled__(void);
void MPI_STATUS_SET_ELEMENTS(void);
void mpi_status_set_elements(void);
void mpi_status_set_elements_(void);
void mpi_status_set_elements__(void);
void MPI_TEST(void);
void mpi_test(void);
void mpi_test_(void);
void mpi_test__(void);
void MPI_TEST_CANCELLED(void);
void mpi_test_cancelled(void);
void mpi_test_cancelled_(void);
void mpi_test_cancelled__(void);
void MPI_TESTALL(void);
void mpi_testall(void);
void mpi_testall_(void);
void mpi_testall__(void);
void MPI_TESTANY(void);
void mpi_testany(void);
void mpi_testany_(void);
void mpi_testany__(void);
void MPI_TESTSOME(void);
void mpi_testsome(void);
void mpi_testsome_(void);
void mpi_testsome__(void);
void MPI_TOPO_TEST(void);
void mpi_topo_test(void);
void mpi_topo_test_(void);
void mpi_topo_test__(void);
void MPI_TYPE_COMMIT(void);
void mpi_type_commit(void);
void mpi_type_commit_(void);
void mpi_type_commit__(void);
void MPI_TYPE_CONTIGUOUS(void);
void mpi_type_contiguous(void);
void mpi_type_contiguous_(void);
void mpi_type_contiguous__(void);
void MPI_TYPE_COUNT(void);
void mpi_type_count(void);
void mpi_type_count_(void);
void mpi_type_count__(void);
void MPI_TYPE_EXTENT(void);
void mpi_type_extent(void);
void mpi_type_extent_(void);
void mpi_type_extent__(void);
void MPI_TYPE_FREE(void);
void mpi_type_free(void);
void mpi_type_free_(void);
void mpi_type_free__(void);
void MPI_TYPE_GET_CONTENTS(void);
void mpi_type_get_contents(void);
void mpi_type_get_contents_(void);
void mpi_type_get_contents__(void);
void MPI_TYPE_GET_ENVELOPE(void);
void mpi_type_get_envelope(void);
void mpi_type_get_envelope_(void);
void mpi_type_get_envelope__(void);
void MPI_TYPE_GET_NAME(void);
void mpi_type_get_name(void);
void mpi_type_get_name_(void);
void mpi_type_get_name__(void);
void MPI_TYPE_HINDEXED(void);
void mpi_type_hindexed(void);
void mpi_type_hindexed_(void);
void mpi_type_hindexed__(void);
void MPI_TYPE_HVECTOR(void);
void mpi_type_hvector(void);
void mpi_type_hvector_(void);
void mpi_type_hvector__(void);
void MPI_TYPE_INDEXED(void);
void mpi_type_indexed(void);
void mpi_type_indexed_(void);
void mpi_type_indexed__(void);
void MPI_TYPE_LB(void);
void mpi_type_lb(void);
void mpi_type_lb_(void);
void mpi_type_lb__(void);
void MPI_TYPE_SET_NAME(void);
void mpi_type_set_name(void);
void mpi_type_set_name_(void);
void mpi_type_set_name__(void);
void MPI_TYPE_SIZE(void);
void mpi_type_size(void);
void mpi_type_size_(void);
void mpi_type_size__(void);
void MPI_TYPE_STRUCT(void);
void mpi_type_struct(void);
void mpi_type_struct_(void);
void mpi_type_struct__(void);
void MPI_TYPE_UB(void);
void mpi_type_ub(void);
void mpi_type_ub_(void);
void mpi_type_ub__(void);
void MPI_TYPE_VECTOR(void);
void mpi_type_vector(void);
void mpi_type_vector_(void);
void mpi_type_vector__(void);
void MPI_UNPACK(void);
void mpi_unpack(void);
void mpi_unpack_(void);
void mpi_unpack__(void);
void MPI_WAIT(void);
void mpi_wait(void);
void mpi_wait_(void);
void mpi_wait__(void);
void MPI_WAITALL(void);
void mpi_waitall(void);
void mpi_waitall_(void);
void mpi_waitall__(void);
void MPI_WAITANY(void);
void mpi_waitany(void);
void mpi_waitany_(void);
void mpi_waitany__(void);
void MPI_WAITSOME(void);
void mpi_waitsome(void);
void mpi_waitsome_(void);
void mpi_waitsome__(void);
void MPI_WIN_CREATE_ERRHANDLER(void);
void mpi_win_create_errhandler(void);
void mpi_win_create_errhandler_(void);
void mpi_win_create_errhandler__(void);
void MPI_WIN_FREE_ERRHANDLER(void);
void mpi_win_free_errhandler(void);
void mpi_win_free_errhandler_(void);
void mpi_win_free_errhandler__(void);
void MPI_WIN_GET_ERRHANDLER(void);
void mpi_win_get_errhandler(void);
void mpi_win_get_errhandler_(void);
void mpi_win_get_errhandler__(void);
void MPI_WIN_SET_ERRHANDLER(void);
void mpi_win_set_errhandler(void);
void mpi_win_set_errhandler_(void);
void mpi_win_set_errhandler__(void);

/*
 * profiling interface prototypes
 */
void PMPI_WTICK(void);
void pmpi_wtick(void);
void pmpi_wtick_(void);
void pmpi_wtick__(void);
void PMPI_WTIME(void);
void pmpi_wtime(void);
void pmpi_wtime_(void);
void pmpi_wtime__(void);
void PMPI_ABORT(void);
void pmpi_abort(void);
void pmpi_abort_(void);
void pmpi_abort__(void);
void PMPI_ADDRESS(void);
void pmpi_address(void);
void pmpi_address_(void);
void pmpi_address__(void);
void PMPI_ALLGATHER(void);
void pmpi_allgather(void);
void pmpi_allgather_(void);
void pmpi_allgather__(void);
void PMPI_ALLGATHERV(void);
void pmpi_allgatherv(void);
void pmpi_allgatherv_(void);
void pmpi_allgatherv__(void);
void PMPI_ALLREDUCE(void);
void pmpi_allreduce(void);
void pmpi_allreduce_(void);
void pmpi_allreduce__(void);
void PMPI_ALLTOALL(void);
void pmpi_alltoall(void);
void pmpi_alltoall_(void);
void pmpi_alltoall__(void);
void PMPI_ALLTOALLV(void);
void pmpi_alltoallv(void);
void pmpi_alltoallv_(void);
void pmpi_alltoallv__(void);
void PMPI_ALLTOALLW(void);
void pmpi_alltoallw(void);
void pmpi_alltoallw_(void);
void pmpi_alltoallw__(void);
void PMPI_ATTR_DELETE(void);
void pmpi_attr_delete(void);
void pmpi_attr_delete_(void);
void pmpi_attr_delete__(void);
void PMPI_ATTR_GET(void);
void pmpi_attr_get(void);
void pmpi_attr_get_(void);
void pmpi_attr_get__(void);
void PMPI_ATTR_PUT(void);
void pmpi_attr_put(void);
void pmpi_attr_put_(void);
void pmpi_attr_put__(void);
void PMPI_BARRIER(void);
void pmpi_barrier(void);
void pmpi_barrier_(void);
void pmpi_barrier__(void);
void PMPI_BCAST(void);
void pmpi_bcast(void);
void pmpi_bcast_(void);
void pmpi_bcast__(void);
void PMPI_BSEND(void);
void pmpi_bsend(void);
void pmpi_bsend_(void);
void pmpi_bsend__(void);
void PMPI_BSEND_INIT(void);
void pmpi_bsend_init(void);
void pmpi_bsend_init_(void);
void pmpi_bsend_init__(void);
void PMPI_BUFFER_ATTACH(void);
void pmpi_buffer_attach(void);
void pmpi_buffer_attach_(void);
void pmpi_buffer_attach__(void);
void PMPI_BUFFER_DETACH(void);
void pmpi_buffer_detach(void);
void pmpi_buffer_detach_(void);
void pmpi_buffer_detach__(void);
void PMPI_CANCEL(void);
void pmpi_cancel(void);
void pmpi_cancel_(void);
void pmpi_cancel__(void);
void PMPI_CART_COORDS(void);
void pmpi_cart_coords(void);
void pmpi_cart_coords_(void);
void pmpi_cart_coords__(void);
void PMPI_CART_CREATE(void);
void pmpi_cart_create(void);
void pmpi_cart_create_(void);
void pmpi_cart_create__(void);
void PMPI_CART_GET(void);
void pmpi_cart_get(void);
void pmpi_cart_get_(void);
void pmpi_cart_get__(void);
void PMPI_CART_MAP(void);
void pmpi_cart_map(void);
void pmpi_cart_map_(void);
void pmpi_cart_map__(void);
void PMPI_CART_RANK(void);
void pmpi_cart_rank(void);
void pmpi_cart_rank_(void);
void pmpi_cart_rank__(void);
void PMPI_CART_SHIFT(void);
void pmpi_cart_shift(void);
void pmpi_cart_shift_(void);
void pmpi_cart_shift__(void);
void PMPI_CART_SUB(void);
void pmpi_cart_sub(void);
void pmpi_cart_sub_(void);
void pmpi_cart_sub__(void);
void PMPI_CARTDIM_GET(void);
void pmpi_cartdim_get(void);
void pmpi_cartdim_get_(void);
void pmpi_cartdim_get__(void);
void PMPI_COMM_COMPARE(void);
void pmpi_comm_compare(void);
void pmpi_comm_compare_(void);
void pmpi_comm_compare__(void);
void PMPI_COMM_CREATE(void);
void pmpi_comm_create(void);
void pmpi_comm_create_(void);
void pmpi_comm_create__(void);
void PMPI_COMM_DUP(void);
void pmpi_comm_dup(void);
void pmpi_comm_dup_(void);
void pmpi_comm_dup__(void);
void PMPI_COMM_FREE(void);
void pmpi_comm_free(void);
void pmpi_comm_free_(void);
void pmpi_comm_free__(void);
void PMPI_COMM_GET_NAME(void);
void pmpi_comm_get_name(void);
void pmpi_comm_get_name_(void);
void pmpi_comm_get_name__(void);
void PMPI_COMM_GROUP(void);
void pmpi_comm_group(void);
void pmpi_comm_group_(void);
void pmpi_comm_group__(void);
void PMPI_COMM_RANK(void);
void pmpi_comm_rank(void);
void pmpi_comm_rank_(void);
void pmpi_comm_rank__(void);
void PMPI_COMM_REMOTE_GROUP(void);
void pmpi_comm_remote_group(void);
void pmpi_comm_remote_group_(void);
void pmpi_comm_remote_group__(void);
void PMPI_COMM_REMOTE_SIZE(void);
void pmpi_comm_remote_size(void);
void pmpi_comm_remote_size_(void);
void pmpi_comm_remote_size__(void);
void PMPI_COMM_SET_NAME(void);
void pmpi_comm_set_name(void);
void pmpi_comm_set_name_(void);
void pmpi_comm_set_name__(void);
void PMPI_COMM_SIZE(void);
void pmpi_comm_size(void);
void pmpi_comm_size_(void);
void pmpi_comm_size__(void);
void PMPI_COMM_SPLIT(void);
void pmpi_comm_split(void);
void pmpi_comm_split_(void);
void pmpi_comm_split__(void);
void PMPI_COMM_TEST_INTER(void);
void pmpi_comm_test_inter(void);
void pmpi_comm_test_inter_(void);
void pmpi_comm_test_inter__(void);
void PMPI_COMM_CREATE_ERRHANDLER(void);
void pmpi_comm_create_errhandler(void);
void pmpi_comm_create_errhandler_(void);
void pmpi_comm_create_errhandler__(void);
void PMPI_COMM_FREE_ERRHANDLER(void);
void pmpi_comm_free_errhandler(void);
void pmpi_comm_free_errhandler_(void);
void pmpi_comm_free_errhandler__(void);
void PMPI_COMM_GET_ERRHANDLER(void);
void pmpi_comm_get_errhandler(void);
void pmpi_comm_get_errhandler_(void);
void pmpi_comm_get_errhandler__(void);
void PMPI_COMM_SET_ERRHANDLER(void);
void pmpi_comm_set_errhandler(void);
void pmpi_comm_set_errhandler_(void);
void pmpi_comm_set_errhandler__(void);
void PMPI_DIMS_CREATE(void);
void pmpi_dims_create(void);
void pmpi_dims_create_(void);
void pmpi_dims_create__(void);
void PMPI_ERRHANDLER_CREATE(void);
void pmpi_errhandler_create(void);
void pmpi_errhandler_create_(void);
void pmpi_errhandler_create__(void);
void PMPI_ERRHANDLER_FREE(void);
void pmpi_errhandler_free(void);
void pmpi_errhandler_free_(void);
void pmpi_errhandler_free__(void);
void PMPI_ERRHANDLER_GET(void);
void pmpi_errhandler_get(void);
void pmpi_errhandler_get_(void);
void pmpi_errhandler_get__(void);
void PMPI_ERRHANDLER_SET(void);
void pmpi_errhandler_set(void);
void pmpi_errhandler_set_(void);
void pmpi_errhandler_set__(void);
void PMPI_ERROR_CLASS(void);
void pmpi_error_class(void);
void pmpi_error_class_(void);
void pmpi_error_class__(void);
void PMPI_ERROR_STRING(void);
void pmpi_error_string(void);
void pmpi_error_string_(void);
void pmpi_error_string__(void);
void PMPI_FINALIZE(void);
void pmpi_finalize(void);
void pmpi_finalize_(void);
void pmpi_finalize__(void);
void PMPI_FINALIZED(void);
void pmpi_finalized(void);
void pmpi_finalized_(void);
void pmpi_finalized__(void);
void PMPI_GATHER(void);
void pmpi_gather(void);
void pmpi_gather_(void);
void pmpi_gather__(void);
void PMPI_GATHERV(void);
void pmpi_gatherv(void);
void pmpi_gatherv_(void);
void pmpi_gatherv__(void);
void PMPI_GET_COUNT(void);
void pmpi_get_count(void);
void pmpi_get_count_(void);
void pmpi_get_count__(void);
void PMPI_GET_ELEMENTS(void);
void pmpi_get_elements(void);
void pmpi_get_elements_(void);
void pmpi_get_elements__(void);
void PMPI_GET_PROCESSOR_NAME(void);
void pmpi_get_processor_name(void);
void pmpi_get_processor_name_(void);
void pmpi_get_processor_name__(void);
void PMPI_GET_VERSION(void);
void pmpi_get_version(void);
void pmpi_get_version_(void);
void pmpi_get_version__(void);
void PMPI_GRAPH_CREATE(void);
void pmpi_graph_create(void);
void pmpi_graph_create_(void);
void pmpi_graph_create__(void);
void PMPI_GRAPH_GET(void);
void pmpi_graph_get(void);
void pmpi_graph_get_(void);
void pmpi_graph_get__(void);
void PMPI_GRAPH_MAP(void);
void pmpi_graph_map(void);
void pmpi_graph_map_(void);
void pmpi_graph_map__(void);
void PMPI_GRAPH_NEIGHBORS(void);
void pmpi_graph_neighbors(void);
void pmpi_graph_neighbors_(void);
void pmpi_graph_neighbors__(void);
void PMPI_GRAPH_NEIGHBORS_COUNT(void);
void pmpi_graph_neighbors_count(void);
void pmpi_graph_neighbors_count_(void);
void pmpi_graph_neighbors_count__(void);
void PMPI_GRAPHDIMS_GET(void);
void pmpi_graphdims_get(void);
void pmpi_graphdims_get_(void);
void pmpi_graphdims_get__(void);
void PMPI_GROUP_COMPARE(void);
void pmpi_group_compare(void);
void pmpi_group_compare_(void);
void pmpi_group_compare__(void);
void PMPI_GROUP_DIFFERENCE(void);
void pmpi_group_difference(void);
void pmpi_group_difference_(void);
void pmpi_group_difference__(void);
void PMPI_GROUP_EXCL(void);
void pmpi_group_excl(void);
void pmpi_group_excl_(void);
void pmpi_group_excl__(void);
void PMPI_GROUP_FREE(void);
void pmpi_group_free(void);
void pmpi_group_free_(void);
void pmpi_group_free__(void);
void PMPI_GROUP_INCL(void);
void pmpi_group_incl(void);
void pmpi_group_incl_(void);
void pmpi_group_incl__(void);
void PMPI_GROUP_INTERSECTION(void);
void pmpi_group_intersection(void);
void pmpi_group_intersection_(void);
void pmpi_group_intersection__(void);
void PMPI_GROUP_RANGE_EXCL(void);
void pmpi_group_range_excl(void);
void pmpi_group_range_excl_(void);
void pmpi_group_range_excl__(void);
void PMPI_GROUP_RANGE_INCL(void);
void pmpi_group_range_incl(void);
void pmpi_group_range_incl_(void);
void pmpi_group_range_incl__(void);
void PMPI_GROUP_RANK(void);
void pmpi_group_rank(void);
void pmpi_group_rank_(void);
void pmpi_group_rank__(void);
void PMPI_GROUP_SIZE(void);
void pmpi_group_size(void);
void pmpi_group_size_(void);
void pmpi_group_size__(void);
void PMPI_GROUP_TRANSLATE_RANKS(void);
void pmpi_group_translate_ranks(void);
void pmpi_group_translate_ranks_(void);
void pmpi_group_translate_ranks__(void);
void PMPI_GROUP_UNION(void);
void pmpi_group_union(void);
void pmpi_group_union_(void);
void pmpi_group_union__(void);
void PMPI_IBSEND(void);
void pmpi_ibsend(void);
void pmpi_ibsend_(void);
void pmpi_ibsend__(void);
void PMPI_INIT(void);
void pmpi_init(void);
void pmpi_init_(void);
void pmpi_init__(void);
void PMPI_INIT_THREAD(void);
void pmpi_init_thread(void);
void pmpi_init_thread_(void);
void pmpi_init_thread__(void);
void PMPI_INITIALIZED(void);
void pmpi_initialized(void);
void pmpi_initialized_(void);
void pmpi_initialized__(void);
void PMPI_INTERCOMM_CREATE(void);
void pmpi_intercomm_create(void);
void pmpi_intercomm_create_(void);
void pmpi_intercomm_create__(void);
void PMPI_INTERCOMM_MERGE(void);
void pmpi_intercomm_merge(void);
void pmpi_intercomm_merge_(void);
void pmpi_intercomm_merge__(void);
void PMPI_IPROBE(void);
void pmpi_iprobe(void);
void pmpi_iprobe_(void);
void pmpi_iprobe__(void);
void PMPI_IRECV(void);
void pmpi_irecv(void);
void pmpi_irecv_(void);
void pmpi_irecv__(void);
void PMPI_IRSEND(void);
void pmpi_irsend(void);
void pmpi_irsend_(void);
void pmpi_irsend__(void);
void PMPI_ISEND(void);
void pmpi_isend(void);
void pmpi_isend_(void);
void pmpi_isend__(void);
void PMPI_ISSEND(void);
void pmpi_issend(void);
void pmpi_issend_(void);
void pmpi_issend__(void);
void PMPI_KEYVAL_CREATE(void);
void pmpi_keyval_create(void);
void pmpi_keyval_create_(void);
void pmpi_keyval_create__(void);
void PMPI_KEYVAL_FREE(void);
void pmpi_keyval_free(void);
void pmpi_keyval_free_(void);
void pmpi_keyval_free__(void);
void PMPI_OP_CREATE(void);
void pmpi_op_create(void);
void pmpi_op_create_(void);
void pmpi_op_create__(void);
void PMPI_OP_FREE(void);
void pmpi_op_free(void);
void pmpi_op_free_(void);
void pmpi_op_free__(void);
void PMPI_PACK(void);
void pmpi_pack(void);
void pmpi_pack_(void);
void pmpi_pack__(void);
void PMPI_PACK_SIZE(void);
void pmpi_pack_size(void);
void pmpi_pack_size_(void);
void pmpi_pack_size__(void);
void PMPI_PCONTROL(void);
void pmpi_pcontrol(void);
void pmpi_pcontrol_(void);
void pmpi_pcontrol__(void);
void PMPI_PROBE(void);
void pmpi_probe(void);
void pmpi_probe_(void);
void pmpi_probe__(void);
void PMPI_RECV(void);
void pmpi_recv(void);
void pmpi_recv_(void);
void pmpi_recv__(void);
void PMPI_RECV_INIT(void);
void pmpi_recv_init(void);
void pmpi_recv_init_(void);
void pmpi_recv_init__(void);
void PMPI_REDUCE(void);
void pmpi_reduce(void);
void pmpi_reduce_(void);
void pmpi_reduce__(void);
void PMPI_REDUCE_SCATTER(void);
void pmpi_reduce_scatter(void);
void pmpi_reduce_scatter_(void);
void pmpi_reduce_scatter__(void);
void PMPI_REQUEST_FREE(void);
void pmpi_request_free(void);
void pmpi_request_free_(void);
void pmpi_request_free__(void);
void PMPI_RSEND(void);
void pmpi_rsend(void);
void pmpi_rsend_(void);
void pmpi_rsend__(void);
void PMPI_RSEND_INIT(void);
void pmpi_rsend_init(void);
void pmpi_rsend_init_(void);
void pmpi_rsend_init__(void);
void PMPI_SCAN(void);
void pmpi_scan(void);
void pmpi_scan_(void);
void pmpi_scan__(void);
void PMPI_SCATTER(void);
void pmpi_scatter(void);
void pmpi_scatter_(void);
void pmpi_scatter__(void);
void PMPI_SCATTERV(void);
void pmpi_scatterv(void);
void pmpi_scatterv_(void);
void pmpi_scatterv__(void);
void PMPI_SEND(void);
void pmpi_send(void);
void pmpi_send_(void);
void pmpi_send__(void);
void PMPI_SEND_INIT(void);
void pmpi_send_init(void);
void pmpi_send_init_(void);
void pmpi_send_init__(void);
void PMPI_SENDRECV(void);
void pmpi_sendrecv(void);
void pmpi_sendrecv_(void);
void pmpi_sendrecv__(void);
void PMPI_SENDRECV_REPLACE(void);
void pmpi_sendrecv_replace(void);
void pmpi_sendrecv_replace_(void);
void pmpi_sendrecv_replace__(void);
void PMPI_SSEND(void);
void pmpi_ssend(void);
void pmpi_ssend_(void);
void pmpi_ssend__(void);
void PMPI_SSEND_INIT(void);
void pmpi_ssend_init(void);
void pmpi_ssend_init_(void);
void pmpi_ssend_init__(void);
void PMPI_START(void);
void pmpi_start(void);
void pmpi_start_(void);
void pmpi_start__(void);
void PMPI_STARTALL(void);
void pmpi_startall(void);
void pmpi_startall_(void);
void pmpi_startall__(void);
void PMPI_STATUS_SET_CANCELLED(void);
void pmpi_status_set_cancelled(void);
void pmpi_status_set_cancelled_(void);
void pmpi_status_set_cancelled__(void);
void PMPI_STATUS_SET_ELEMENTS(void);
void pmpi_status_set_elements(void);
void pmpi_status_set_elements_(void);
void pmpi_status_set_elements__(void);
void PMPI_TEST(void);
void pmpi_test(void);
void pmpi_test_(void);
void pmpi_test__(void);
void PMPI_TEST_CANCELLED(void);
void pmpi_test_cancelled(void);
void pmpi_test_cancelled_(void);
void pmpi_test_cancelled__(void);
void PMPI_TESTALL(void);
void pmpi_testall(void);
void pmpi_testall_(void);
void pmpi_testall__(void);
void PMPI_TESTANY(void);
void pmpi_testany(void);
void pmpi_testany_(void);
void pmpi_testany__(void);
void PMPI_TESTSOME(void);
void pmpi_testsome(void);
void pmpi_testsome_(void);
void pmpi_testsome__(void);
void PMPI_TOPO_TEST(void);
void pmpi_topo_test(void);
void pmpi_topo_test_(void);
void pmpi_topo_test__(void);
void PMPI_TYPE_COMMIT(void);
void pmpi_type_commit(void);
void pmpi_type_commit_(void);
void pmpi_type_commit__(void);
void PMPI_TYPE_CONTIGUOUS(void);
void pmpi_type_contiguous(void);
void pmpi_type_contiguous_(void);
void pmpi_type_contiguous__(void);
void PMPI_TYPE_COUNT(void);
void pmpi_type_count(void);
void pmpi_type_count_(void);
void pmpi_type_count__(void);
void PMPI_TYPE_EXTENT(void);
void pmpi_type_extent(void);
void pmpi_type_extent_(void);
void pmpi_type_extent__(void);
void PMPI_TYPE_FREE(void);
void pmpi_type_free(void);
void pmpi_type_free_(void);
void pmpi_type_free__(void);
void PMPI_TYPE_GET_CONTENTS(void);
void pmpi_type_get_contents(void);
void pmpi_type_get_contents_(void);
void pmpi_type_get_contents__(void);
void PMPI_TYPE_GET_ENVELOPE(void);
void pmpi_type_get_envelope(void);
void pmpi_type_get_envelope_(void);
void pmpi_type_get_envelope__(void);
void PMPI_TYPE_GET_NAME(void);
void pmpi_type_get_name(void);
void pmpi_type_get_name_(void);
void pmpi_type_get_name__(void);
void PMPI_TYPE_HINDEXED(void);
void pmpi_type_hindexed(void);
void pmpi_type_hindexed_(void);
void pmpi_type_hindexed__(void);
void PMPI_TYPE_HVECTOR(void);
void pmpi_type_hvector(void);
void pmpi_type_hvector_(void);
void pmpi_type_hvector__(void);
void PMPI_TYPE_INDEXED(void);
void pmpi_type_indexed(void);
void pmpi_type_indexed_(void);
void pmpi_type_indexed__(void);
void PMPI_TYPE_LB(void);
void pmpi_type_lb(void);
void pmpi_type_lb_(void);
void pmpi_type_lb__(void);
void PMPI_TYPE_SET_NAME(void);
void pmpi_type_set_name(void);
void pmpi_type_set_name_(void);
void pmpi_type_set_name__(void);
void PMPI_TYPE_SIZE(void);
void pmpi_type_size(void);
void pmpi_type_size_(void);
void pmpi_type_size__(void);
void PMPI_TYPE_STRUCT(void);
void pmpi_type_struct(void);
void pmpi_type_struct_(void);
void pmpi_type_struct__(void);
void PMPI_TYPE_UB(void);
void pmpi_type_ub(void);
void pmpi_type_ub_(void);
void pmpi_type_ub__(void);
void PMPI_TYPE_VECTOR(void);
void pmpi_type_vector(void);
void pmpi_type_vector_(void);
void pmpi_type_vector__(void);
void PMPI_UNPACK(void);
void pmpi_unpack(void);
void pmpi_unpack_(void);
void pmpi_unpack__(void);
void PMPI_WAIT(void);
void pmpi_wait(void);
void pmpi_wait_(void);
void pmpi_wait__(void);
void PMPI_WAITALL(void);
void pmpi_waitall(void);
void pmpi_waitall_(void);
void pmpi_waitall__(void);
void PMPI_WAITANY(void);
void pmpi_waitany(void);
void pmpi_waitany_(void);
void pmpi_waitany__(void);
void PMPI_WAITSOME(void);
void pmpi_waitsome(void);
void pmpi_waitsome_(void);
void pmpi_waitsome__(void);
void PMPI_WIN_CREATE_ERRHANDLER(void);
void pmpi_win_create_errhandler(void);
void pmpi_win_create_errhandler_(void);
void pmpi_win_create_errhandler__(void);
void PMPI_WIN_FREE_ERRHANDLER(void);
void pmpi_win_free_errhandler(void);
void pmpi_win_free_errhandler_(void);
void pmpi_win_free_errhandler__(void);
void PMPI_WIN_GET_ERRHANDLER(void);
void pmpi_win_get_errhandler(void);
void pmpi_win_get_errhandler_(void);
void pmpi_win_get_errhandler__(void);
void PMPI_WIN_SET_ERRHANDLER(void);
void pmpi_win_set_errhandler(void);
void pmpi_win_set_errhandler_(void);
void pmpi_win_set_errhandler__(void);

/* default attribute functions */

#ifdef MPI_DUP_FN
#undef MPI_DUP_FN
#endif
#ifdef MPI_NULL_COPY_FN
#undef MPI_NULL_COPY_FN
#endif
#ifdef MPI_NULL_DELETE_FN
#undef MPI_NULL_DELETE_FN
#endif

void MPI_DUP_FN(void);
void mpi_dup_fn(void);
void mpi_dup_fn_(void);
void mpi_dup_fn__(void);
void MPI_NULL_COPY_FN(void);
void mpi_null_copy_fn(void);
void mpi_null_copy_fn_(void);
void mpi_null_copy_fn__(void);
void MPI_NULL_DELETE_FN(void);
void mpi_null_delete_fn(void);
void mpi_null_delete_fn_(void);
void mpi_null_delete_fn__(void);

CDECL_END

#endif				/* _ULM_MPIF_DECLS_H_ */
