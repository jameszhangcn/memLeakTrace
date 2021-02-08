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
* File Name: CUUPLib.cpp
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

#include <cstdio>
#include <cstddef>
#include "SvcLog.h"
#include "GrpcAdpCntrl.h"
#include "GrpcAdp.h"
#include "GrpcLog.h"

void mav_init()
{
    ccTrace();
    mav::GrpcAdpCntrl::getInstance()->init();
}

void init_grpc_server_config()
{
    ccTrace();
    printf("Init grpc server config");
}
void start_grpc_server()
{
    ccTrace("start_grpc_server");
    mav::GrpcAdpCntrl::getInstance()->init();
    printf("testing jianzhag");
    //mav::GrpcAdpCntrl::getInstance()->join_threads();
}

void join_grpc_server()
{
    ccTrace("join_grpc_server");
    mav::GrpcAdpCntrl::getInstance()->join_threads();
}

int start_grpc_client(uint32_t cp_id, uint32_t up_id, IpAddressG *up_ip, uint32_t dst_port)
{
    mav::GrpcAdpCntrl::getInstance()->init_client(cp_id, up_id, up_ip, dst_port);
    return mav::GrpcAdpCntrl::getInstance()->start_client(cp_id, up_id);
}

int send_sec_cfg_req(CuCpUpHdrG* cucp_up_hdr, SecurityConfigReqG *secCfg)
{
    ccTrace("send_sec_cfg_req: ");
    mav::GrpcAdpCntrl::getInstance()->client_lock(0);
    int ret = mav::GrpcAdpCntrl::getInstance()->getGrpcClientThread().SendSecurityCfgReq(cucp_up_hdr,secCfg);
    mav::GrpcAdpCntrl::getInstance()->client_unlock(0);
    return ret;
}


int  send_crt_tun_req(CuCpUpHdrG* cucp_up_hdr, CreateTunnelReqG *crtTunReq)
{
    ccTrace("send_crt_tun_req: ");
    mav::GrpcAdpCntrl::getInstance()->client_lock(0);
    int ret = mav::GrpcAdpCntrl::getInstance()->getGrpcClientThread().SendCrtTunReq(cucp_up_hdr, crtTunReq);
    mav::GrpcAdpCntrl::getInstance()->client_unlock(0);
    return ret;
}


int  send_mod_tun_req(CuCpUpHdrG* cucp_up_hdr, ModifyTunnelReqG *modTunReq)
{
    ccTrace("send_mod_tun_req: ");
    mav::GrpcAdpCntrl::getInstance()->client_lock(0);
    int ret = mav::GrpcAdpCntrl::getInstance()->getGrpcClientThread().SendModTunReq(cucp_up_hdr, modTunReq);
    mav::GrpcAdpCntrl::getInstance()->client_unlock(0);
    return ret;
}



int  send_del_tun_req(CuCpUpHdrG* cucp_up_hdr, DeleteTunnelReqG *delTunReq)
{
    ccTrace("send_del_tun_req: ");
    mav::GrpcAdpCntrl::getInstance()->client_lock(0);
    int ret = mav::GrpcAdpCntrl::getInstance()->getGrpcClientThread().SendDelTunReq(cucp_up_hdr, delTunReq);
    mav::GrpcAdpCntrl::getInstance()->client_unlock(0);
    return ret;
}

int  send_tunnel_state_change_req(CuCpUpHdrG* cucp_up_hdr, TunnelStateInfoG *tunnelStateInfo)
{
    ccTrace("send_tunnel_state_change_req: ");
    mav::GrpcAdpCntrl::getInstance()->client_lock(0);
    int ret = mav::GrpcAdpCntrl::getInstance()->getGrpcClientThread().SendTunnelStateChangeReq(cucp_up_hdr, tunnelStateInfo);
    mav::GrpcAdpCntrl::getInstance()->client_unlock(0);
    return ret;
}

int  send_add_rb_cfg_req(CuCpUpHdrG* cucp_up_hdr, AddRbConfigReqG *addRbCfg)
{
    ccTrace("send_add_rb_cfg_req: ");
    mav::GrpcAdpCntrl::getInstance()->client_lock(0);
    int ret = mav::GrpcAdpCntrl::getInstance()->getGrpcClientThread().SendAddRbCfgReq(cucp_up_hdr, addRbCfg);
    mav::GrpcAdpCntrl::getInstance()->client_unlock(0);
    return ret;
}

int  send_mod_rb_cfg_req(CuCpUpHdrG* cucp_up_hdr, ModifyRbConfigReqG *modRbCfg)
{
    ccTrace("send_mod_rb_cfg_req: ");
    mav::GrpcAdpCntrl::getInstance()->client_lock(0);
    int ret = mav::GrpcAdpCntrl::getInstance()->getGrpcClientThread().SendModRbCfgReq(cucp_up_hdr, modRbCfg);
    mav::GrpcAdpCntrl::getInstance()->client_unlock(0);
    return ret;
}

int  send_del_rb_cfg_req(CuCpUpHdrG* cucp_up_hdr, DeleteRbConfigReqG *delRbCfg)
{
    ccTrace("send_del_rb_cfg_req: ");
    mav::GrpcAdpCntrl::getInstance()->client_lock(0);
    int ret = mav::GrpcAdpCntrl::getInstance()->getGrpcClientThread().SendDelRbCfgReq(cucp_up_hdr, delRbCfg);
    mav::GrpcAdpCntrl::getInstance()->client_unlock(0);
    return ret;
}

int  send_pause_resume_data_req(CuCpUpHdrG* cucp_up_hdr, PauseResumeDataReqG *pauseResumeReq)
{
    mav::GrpcAdpCntrl::getInstance()->client_lock(0);
    int ret = mav::GrpcAdpCntrl::getInstance()->getGrpcClientThread().SendPauseResumeDataReq(cucp_up_hdr, pauseResumeReq);
    mav::GrpcAdpCntrl::getInstance()->client_unlock(0);
    return ret;
}

int  send_pdcp_sdu_status_req(CuCpUpHdrG* cucp_up_hdr)
{
    mav::GrpcAdpCntrl::getInstance()->client_lock(0);
    int ret = mav::GrpcAdpCntrl::getInstance()->getGrpcClientThread().SendPdcpSduStatusReq(cucp_up_hdr);
    mav::GrpcAdpCntrl::getInstance()->client_unlock(0);
    return ret;
}

int  send_data_forwarding_req(CuCpUpHdrG* cucp_up_hdr, DataForwardingReqG* req)
{
    mav::GrpcAdpCntrl::getInstance()->client_lock(0);
    int ret = mav::GrpcAdpCntrl::getInstance()->getGrpcClientThread().SendDataForwardingReq(cucp_up_hdr, req);
    mav::GrpcAdpCntrl::getInstance()->client_unlock(0);
    return ret;
}

int  send_ue_inactivity_timer_req(CuCpUpHdrG* cucp_up_hdr)
{
    mav::GrpcAdpCntrl::getInstance()->client_lock(0);
    int ret = mav::GrpcAdpCntrl::getInstance()->getGrpcClientThread().SendUeInactivityTimerReq(cucp_up_hdr);
    mav::GrpcAdpCntrl::getInstance()->client_unlock(0);
    return ret;
}

int  send_add_pdcp_cfg_req(CuCpUpHdrG* cucp_up_hdr, AddPDCPConfigReqG *pdcp_cfg)
{
    mav::GrpcAdpCntrl::getInstance()->client_lock(0);
    int ret = mav::GrpcAdpCntrl::getInstance()->getGrpcClientThread().SendAddPdcpCfgReq(cucp_up_hdr, pdcp_cfg);
    mav::GrpcAdpCntrl::getInstance()->client_unlock(0);
    return ret;
}

int  send_mod_pdcp_cfg_req(CuCpUpHdrG* cucp_up_hdr, ModPDCPConfigReqG *pdcp_cfg)
{
    mav::GrpcAdpCntrl::getInstance()->client_lock(0);
    int ret = mav::GrpcAdpCntrl::getInstance()->getGrpcClientThread().SendModPdcpCfgReq(cucp_up_hdr, pdcp_cfg);
    mav::GrpcAdpCntrl::getInstance()->client_unlock(0);
    return ret;
}

char * get_desc_by_id(LOG_DESCRIBE_COMM_TYPE *desc,unsigned id)
{
   int i=0;
   static char  no_match[10]={0};
   while(desc[i].id < 0xffff)
   {
      if(desc[i].id==id)
          return desc[i].desc;
      i++;
   }
   
   return "can't find the id";
}
