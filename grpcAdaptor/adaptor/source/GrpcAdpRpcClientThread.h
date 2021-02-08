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
* File Name: CUUPLibRpcClientThread.h
*
* Subsystem: libCUUP
*
* Description: This file defines the RPC client for libCUUP
*
* Revision History:
* Date        Name         Description
* ----        ----         -----------
* 10/19/2020  Fisher Yu    Initial Release
*
**********************************************************************/
#pragma once

#include "CucpService.h"
#include "GrpcAdp.h"
#include "structure.pb.h"
#include "srvEbm.pb.h"
#include "srvCp.pb.h"

class GrpcAdpRpcClientThread : public EbmClient
{
public:
    GrpcAdpRpcClientThread()
        : mThreadInstId(0)
        , mThreadReady(false)
    {
        mEbmServerTarget = "127.0.0.1:9020";
    }
    void init(uint32_t mThreadInstId);
    virtual void on_init();
    virtual void on_exit();
    virtual void run();

    inline uint32_t get_thread_inst_id()
    {
        return mThreadInstId;
    }
    inline std::string& ebmServerTarget() {return mEbmServerTarget;}

    int SendSecurityCfgReq(CuCpUpHdrG* cp_up_hdr, SecurityConfigReqG *secCfg);

    int  SendCrtTunReq(CuCpUpHdrG* cp_up_hdr, CreateTunnelReqG *crtTunReq);
    int  SendModTunReq(CuCpUpHdrG* cp_up_hdr, ModifyTunnelReqG *modTunReq);
    int  SendDelTunReq(CuCpUpHdrG* cp_up_hdr, DeleteTunnelReqG *delTunReq);
    int  SendTunnelStateChangeReq(CuCpUpHdrG* cp_up_hdr, TunnelStateInfoG *tunnelStateInfo);

    int  SendAddRbCfgReq(CuCpUpHdrG* cp_up_hdr, AddRbConfigReqG *addRbCfg);
    int  SendModRbCfgReq(CuCpUpHdrG* cp_up_hdr, ModifyRbConfigReqG *modRbCfg);
    int  SendDelRbCfgReq(CuCpUpHdrG* cp_up_hdr, DeleteRbConfigReqG *delRbCfg);

    int  SendAddPdcpCfgReq(CuCpUpHdrG* cp_up_hdr,AddPDCPConfigReqG *pdcp_cfg);
    int  SendModPdcpCfgReq(CuCpUpHdrG* cp_up_hdr,ModPDCPConfigReqG *pdcp_cfg);

    int  SendPauseResumeDataReq(CuCpUpHdrG* cp_up_hdr, PauseResumeDataReqG *pauseResumeReq);
    int  SendPdcpSduStatusReq(CuCpUpHdrG* cp_up_hdr);

    int  SendDataForwardingReq(CuCpUpHdrG* cp_up_hdr, DataForwardingReqG* req);
    int  SendUeInactivityTimerReq(CuCpUpHdrG* cp_up_hdr);

    void ResponseHandler(ServiceClientCallData* aClientCallData);
private:
    uint32_t mThreadInstId;
    bool mThreadReady;
    std::string mEbmServerTarget;
    void SetMhdr(CuCpUpHdrG *cu_up_hdr, ::mhdr::MsgHdr *hdr);
    void SetCucpupHdr(CuCpUpHdrG *cu_up_hdr, ::structure::CpUpHdr *cpUpHdr);
    
    ::structure::TunnelType GrpcAdpRpcClientThread::GetTunType(TunnelTypeG tunType, InterfaceTypeG intfType);
    ::common::IpAddressType GrpcAdpRpcClientThread::GetIpType(IpAddressTypeG type);
    int FillAddPdcpCfg(::structure::PDCPAddCfgEnt *pdcpAddCfg, PDCPAddCfgEntG* pdcpCfgIn);
    int FillModPdcpCfg(::structure::PDCPModCfgEnt *pdcpCfgEnt, PDCPModCfgEntG* pdcpCfgIn);
    int FillDelPdcpCfg(::structure::PDCPDelCfgEnt *pdcpCfgEnt, PDCPDelCfgEntG* pdcpCfgIn);
    
};
