/**************************************************************************************************
*                                                                                                 *
* This file is part of HPIPM.                                                                     *
*                                                                                                 *
* HPIPM -- High-Performance Interior Point Method.                                                *
* Copyright (C) 2019 by Gianluca Frison.                                                          *
* Developed at IMTEK (University of Freiburg) under the supervision of Moritz Diehl.              *
* All rights reserved.                                                                            *
*                                                                                                 *
* The 2-Clause BSD License                                                                        *
*                                                                                                 *
* Redistribution and use in source and binary forms, with or without                              *
* modification, are permitted provided that the following conditions are met:                     *
*                                                                                                 *
* 1. Redistributions of source code must retain the above copyright notice, this                  *
*    list of conditions and the following disclaimer.                                             *
* 2. Redistributions in binary form must reproduce the above copyright notice,                    *
*    this list of conditions and the following disclaimer in the documentation                    *
*    and/or other materials provided with the distribution.                                       *
*                                                                                                 *
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND                 *
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED                   *
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE                          *
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR                 *
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES                  *
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;                    *
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND                     *
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT                      *
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS                   *
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                                    *
*                                                                                                 *
* Author: Gianluca Frison, gianluca.frison (at) imtek.uni-freiburg.de                             *
*                                                                                                 *
**************************************************************************************************/

#include <stdlib.h>
#include <stdio.h>

#include <blasfeo_target.h>
#include <blasfeo_common.h>
#include <blasfeo_d_blas.h>
#include <blasfeo_d_aux.h>

#include "../include/hpipm_d_ocp_qcqp_dim.h"
#include "../include/hpipm_d_ocp_qcqp.h"
#include "../include/hpipm_d_ocp_qcqp_sol.h"
#include "../include/hpipm_d_dense_qcqp_dim.h"
#include "../include/hpipm_d_dense_qcqp.h"
#include "../include/hpipm_d_dense_qcqp_sol.h"
#include "../include/hpipm_d_cond.h"
#include "../include/hpipm_d_cond_aux.h"
#include "../include/hpipm_d_cond_qcqp.h"
#include "../include/hpipm_aux_mem.h"



#define DOUBLE_PRECISION



#define COLAD blasfeo_dcolad
#define COND_DCTD d_cond_DCtd
#define COND_D d_cond_d
#define COND_B d_cond_b
#define COND_BABT d_cond_BAbt
#define COND_RQ d_cond_rq
#define COND_RSQRQ d_cond_RSQrq
#define COND_QCQP_ARG d_cond_qcqp_arg
#define COND_QCQP_WS d_cond_qcqp_ws
#define COND_QP_ARG d_cond_qp_arg
#define COND_QP_ARG_MEMSIZE d_cond_qp_arg_memsize
#define COND_QP_ARG_CREATE d_cond_qp_arg_create
#define COND_QP_WS d_cond_qp_ws
#define COND_QP_WS_MEMSIZE d_cond_qp_ws_memsize
#define COND_QP_WS_CREATE d_cond_qp_ws_create
#define CREATE_STRMAT blasfeo_create_dmat
#define CREATE_STRVEC blasfeo_create_dvec
#define DENSE_QCQP d_dense_qcqp
#define DENSE_QCQP_DIM d_dense_qcqp_dim
#define DENSE_QCQP_DIM_SET_NV d_dense_qcqp_dim_set_nv
#define DENSE_QCQP_DIM_SET_NE d_dense_qcqp_dim_set_ne
#define DENSE_QCQP_DIM_SET_NB d_dense_qcqp_dim_set_nb
#define DENSE_QCQP_DIM_SET_NG d_dense_qcqp_dim_set_ng
#define DENSE_QCQP_DIM_SET_NQ d_dense_qcqp_dim_set_nq
#define DENSE_QCQP_DIM_SET_NS d_dense_qcqp_dim_set_ns
#define DENSE_QCQP_DIM_SET_NSB d_dense_qcqp_dim_set_nsb
#define DENSE_QCQP_DIM_SET_NSG d_dense_qcqp_dim_set_nsg
#define DENSE_QCQP_DIM_SET_NSQ d_dense_qcqp_dim_set_nsq
#define DENSE_QCQP_SOL d_dense_qcqp_sol
#define DENSE_QP d_dense_qp
#define DENSE_QP_DIM d_dense_qp_dim
#define DENSE_QP_SOL d_dense_qp_sol
#define DOT blasfeo_ddot
#define EXPAND_SOL d_expand_sol
#define GEAD blasfeo_dgead
#define GECP blasfeo_dgecp
#define GEMM_NN blasfeo_dgemm_nn
#define GEMV_N blasfeo_dgemv_n
#define GESE blasfeo_dgese
#define OCP_QCQP d_ocp_qcqp
#define OCP_QCQP_DIM d_ocp_qcqp_dim
#define OCP_QCQP_SOL d_ocp_qcqp_sol
#define OCP_QP d_ocp_qp
#define OCP_QP_DIM d_ocp_qp_dim
#define OCP_QP_SOL d_ocp_qp_sol
#define REAL double
#define ROWEX blasfeo_drowex
#define SIZE_STRMAT blasfeo_memsize_dmat
#define SIZE_STRVEC blasfeo_memsize_dvec
#define STRMAT blasfeo_dmat
#define STRVEC blasfeo_dvec
#define SYMV_L blasfeo_dsymv_l
#define SYRK_LN blasfeo_dsyrk_ln
#define SYRK_LN_MN blasfeo_dsyrk_ln_mn
#define TRCP_L blasfeo_dtrcp_l
#define TRTR_L blasfeo_dtrtr_l

#define COND_QCQP_COMPUTE_DIM d_cond_qcqp_compute_dim
#define COND_QCQP_ARG_MEMSIZE d_cond_qcqp_arg_memsize
#define COND_QCQP_ARG_CREATE d_cond_qcqp_arg_create
#define COND_QCQP_ARG_SET_DEFAULT d_cond_qcqp_arg_set_default
#define COND_QCQP_ARG_SET_RIC_ALG d_cond_qcqp_arg_set_ric_alg
#define COND_QCQP_ARG_SET_COND_LAST_STAGE d_cond_qcqp_arg_set_cond_last_stage
#define COND_QCQP_WS_MEMSIZE d_cond_qcqp_ws_memsize
#define COND_QCQP_WS_CREATE d_cond_qcqp_ws_create
#define COND_QCQP_QC d_cond_qcqp_qc
#define COND_QCQP_QC_RHS d_cond_qcqp_qc_rhs
#define COND_QCQP_COND d_cond_qcqp_cond
#define COND_QCQP_COND_RHS d_cond_qcqp_cond_rhs
#define COND_QCQP_EXPAND_SOL d_cond_qcqp_expand_sol



#include "x_cond_qcqp.c"

