/*********************************************************************/
/*                                                                   */
/* Copyright (C) 2004-2020                                           */
/* Mavenir Systems, Inc.                                             */
/* Richardson, TX, USA                                               */
/*                                                                   */
/* ALL RIGHTS RESERVED                                               */
/*                                                                   */
/* Permission is hereby granted to licensees of Mavenir Systems,     */
/* Inc. products to use or abstract this computer program for the    */
/* sole purpose of implementing a product based on                   */
/* Mavenir Systems, Inc. products.  No other rights to reproduce,    */
/* use, or disseminate this computer program, whether in part or in  */
/* whole, are granted.                                               */
/*                                                                   */
/* Mavenir Systems, Inc. makes no representation or warranties       */
/* with respect to the performance of this computer program, and     */
/* specifically disclaims any responsibility for any damages,        */
/* special or consequential, connected with the use of this program. */
/*                                                                   */
/*********************************************************************/

/**********************************************************************
*
* File Name: CUUPLib.h
*
* Subsystem: libCUUP
*
* Description: This file defines the interface of libCUUP
*
* Revision History:
* Date        Name         Description
* ----        ----         -----------
* 10/19/2020  Fisher       Initial Release
*
**********************************************************************/
#ifndef __GRPC_ADP_H
#define __GRPC_ADP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "GrpcAdpStru.h"

extern void mav_init();
extern void start_grpc_server();
extern void join_grpc_server();
extern int  start_grpc_client(uint32_t cp_id, uint32_t up_id, IpAddressG *up_ip, uint32_t dst_port);

extern int  send_sec_cfg_req(CuCpUpHdrG* cucp_up_hdr, SecurityConfigReqG *secCfgReq);

extern int  send_crt_tun_req(CuCpUpHdrG* cucp_up_hdr, CreateTunnelReqG *crtTunReq);
extern int  send_mod_tun_req(CuCpUpHdrG* cucp_up_hdr, ModifyTunnelReqG *modTunReq);
extern int  send_del_tun_req(CuCpUpHdrG* cucp_up_hdr, DeleteTunnelReqG *delTunReq);

extern int  send_add_rb_cfg_req(CuCpUpHdrG* cucp_up_hdr, AddRbConfigReqG *addRbCfg);
extern int  send_mod_rb_cfg_req(CuCpUpHdrG* cucp_up_hdr, ModifyRbConfigReqG *modRbCfg);
extern int  send_del_rb_cfg_req(CuCpUpHdrG* cucp_up_hdr, DeleteRbConfigReqG *delRbCfg);


extern int  send_pause_resume_data_req(CuCpUpHdrG* cucp_up_hdr, PauseResumeDataReqG *pauseResumeReq);
extern int  send_pdcp_sdu_status_req(CuCpUpHdrG* cucp_up_hdr);
extern int  send_data_forwarding_req(CuCpUpHdrG* cucp_up_hdr, DataForwardingReqG* req);
extern int  send_ue_inactivity_timer_req(CuCpUpHdrG* cucp_up_hdr);

extern int  send_tunnel_state_change_req(CuCpUpHdrG* cucp_up_hdr, TunnelStateInfoG *tunnelStateInfo);

extern int  send_add_pdcp_cfg_req(CuCpUpHdrG* cucp_up_hdr, AddPDCPConfigReqG *pdcp_cfg);
extern int  send_mod_pdcp_cfg_req(CuCpUpHdrG* cucp_up_hdr, ModPDCPConfigReqG *pdcp_cfg);

#ifdef __cplusplus
}
#endif

#endif  // __GRPC_ADP_H
