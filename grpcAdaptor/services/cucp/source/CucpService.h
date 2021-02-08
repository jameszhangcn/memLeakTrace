/*********************************************************************/
/*                                                                   */
/* Copyright (C) 2004-2019                                           */
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
* File Name: CucpService.h
*
* Subsystem: Cucp
*
* Description: This file defines the service layer for Iwf
*
* Revision History:
* Date        Name         Description
* ----        ----         -----------
* 11/03/2020  JianZhang       Initial Release
*
**********************************************************************/
#pragma once

#include <unistd.h>
#include <grpcpp/grpcpp.h>
#include "structure.grpc.pb.h"
#include "srvEbm.grpc.pb.h"
#include "srvCp.grpc.pb.h"
#include "ServiceImpl.h"
#include "SvcLog.h"
#include "GrpcAdpStru.h"

using namespace srvEbm;
using namespace srvCp;

enum RpcType {

    // CUCP -> EBM
    SEC_CFG_REQ,
    CRT_TUN_REQ,
    MOD_TUN_REQ,
    DEL_TUN_REQ,
    ADD_RB_CFG_REQ,
    MOD_RB_CFG_REQ,
    DEL_RB_CFG_REQ,
    PAU_REQ_DATA_REQ,
    PDCP_SDU_STAT_REQ,
    DATA_FWD_REQ,
    UE_INACT_TM_REQ,
    TUN_STATE_CHG_REQ,
    ADD_PDCP_CFG_REQ,
    MOD_PDCP_CFG_REQ,
    
    // EBM -> CUCP
    SEC_CFG_CFM,
    CRT_TUN_CFM,
    MOD_TUN_CFM,
    DEL_TUN_CFM,
    ADD_RB_CFG_CFM,
    MOD_RB_CFG_CFM,
    DEL_RB_CFG_CFM,
    PDCP_SDU_STAT_CFM,
    ADD_PDCP_CFG_CFM,
    MOD_PDCP_CFG_CFM
};

DEF_RPC_CLIENT_OBJ(CucpEbmSecCfgClient, SEC_CFG_REQ, Ebm, SubmitSecurityConfigReq, SecurityConfigReq, ReqReply);
DEF_RPC_CLIENT_OBJ(CucpEbmCrtTunClient, CRT_TUN_REQ, Ebm, SubmitCreateTunnelReq, CreateTunnelReq, ReqReply);
DEF_RPC_CLIENT_OBJ(CucpEbmModTunClient, MOD_TUN_REQ, Ebm, SubmitModifyTunnelReq, ModifyTunnelReq, ReqReply);
DEF_RPC_CLIENT_OBJ(CucpEbmDelTunClient, DEL_TUN_REQ, Ebm, SubmitDeleteTunnelReq, DeleteTunnelReq, ReqReply);
DEF_RPC_CLIENT_OBJ(CucpEbmTunStateChgClient, TUN_STATE_CHG_REQ, Ebm, SubmitTunnelStateChangeReq, TunnelStateChangeReq, ReqReply);

DEF_RPC_CLIENT_OBJ(CucpEbmAddRbCfgClient, ADD_RB_CFG_REQ, Ebm, SubmitAddRbConfigReq, AddRbConfigReq, ReqReply);
DEF_RPC_CLIENT_OBJ(CucpEbmModRbCfgClient,  MOD_RB_CFG_REQ, Ebm, SubmitModifyRbConfigReq, ModifyRbConfigReq, ReqReply);
DEF_RPC_CLIENT_OBJ(CucpEbmDelRbCfgClient,  DEL_RB_CFG_REQ, Ebm, SubmitDeleteRbConfigReq, DeleteRbConfigReq, ReqReply);
DEF_RPC_CLIENT_OBJ(CucpEbmPauseResumeClient,  PAU_REQ_DATA_REQ, Ebm, SubmitPauseResumeDataReq, PauseResumeDataReq, ReqReply);
DEF_RPC_CLIENT_OBJ(CucpEbmPdcpSduStatusClient,  PDCP_SDU_STAT_REQ, Ebm, SubmitPDCPSduStatusReq, PDCPSduStatusReq, ReqReply);
DEF_RPC_CLIENT_OBJ(CucpEbmDataFwdClient,  DATA_FWD_REQ, Ebm, SubmitDataForwardingReq, DataForwardingReq, ReqReply);
DEF_RPC_CLIENT_OBJ(CucpEbmUeInactTimerClient,  UE_INACT_TM_REQ, Ebm, SubmitUeInactivityTimerReq, UeInactivityTimerReq, ReqReply);
DEF_RPC_CLIENT_OBJ(CucpEbmAddPdcpCfgClient,  ADD_PDCP_CFG_REQ, Ebm, SubmitAddPDCPConfigReq, AddPDCPConfigReq, ReqReply);
DEF_RPC_CLIENT_OBJ(CucpEbmModPdcpCfgClient,  MOD_PDCP_CFG_REQ, Ebm, SubmitModifyPDCPConfigReq, ModifyPDCPConfigReq, ReqReply);


DEF_RPC_SERVER_OBJ(EbmSecCfgCfmCallData, SEC_CFG_CFM, SubmitSecurityConfigCfm, Cp, SecurityConfigCfm, CfmReply);
DEF_RPC_SERVER_OBJ(EbmCrtTunCfmCallData, CRT_TUN_CFM, SubmitCreateTunnelCfm, Cp, CreateTunnelCfm, CfmReply);
DEF_RPC_SERVER_OBJ(EbmModTunCfmCallData, MOD_TUN_CFM, SubmitModifyTunnelCfm, Cp, ModifyTunnelCfm, CfmReply);
DEF_RPC_SERVER_OBJ(EbmDelTunCfmCallData, DEL_TUN_CFM, SubmitDeleteTunnelCfm, Cp, DeleteTunnelCfm, CfmReply);
DEF_RPC_SERVER_OBJ(EbmAddRbCfgCfmCallData, ADD_RB_CFG_CFM, SubmitAddRbConfigCfm, Cp, AddRbConfigCfm, CfmReply);
DEF_RPC_SERVER_OBJ(EbmModRbCfgCfmCallData, MOD_RB_CFG_CFM, SubmitModifyRbConfigCfm, Cp, ModifyRbConfigCfm, CfmReply);
DEF_RPC_SERVER_OBJ(EbmDelRbCfgCfmCallData, DEL_RB_CFG_CFM, SubmitDeleteRbConfigCfm, Cp, DeleteRbConfigCfm, CfmReply);
DEF_RPC_SERVER_OBJ(EbmPdcpSduStatusCfmCallData, PDCP_SDU_STAT_CFM, SubmitPDCPSduStatusCfm, Cp, PDCPSduStatusCfm, CfmReply);
DEF_RPC_SERVER_OBJ(EbmAddPdcpCfgCfmCallData, ADD_PDCP_CFG_CFM, SubmitAddPDCPConfigCfm, Cp, AddPDCPConfigCfm, CfmReply);
DEF_RPC_SERVER_OBJ(EbmModPdcpCfgCfmCallData, MOD_PDCP_CFG_CFM, SubmitModifyPDCPConfigCfm, Cp, ModifyPDCPConfigCfm, CfmReply);


class CucpServer : public ServiceServerImpl
{
public:
    ~CucpServer();

    virtual void OnInit();
    virtual grpc::Service* getService();
    virtual ServiceServerCallData* allocCallData(int aServiceType);
    virtual void deallocCallData(ServiceServerCallData* aCD);
    virtual void OnMessage(ServiceServerCallData& aCD);

private:
    virtual void TransCpUpHdr(const ::structure::CpUpHdr &srcHdr, CuCpUpHdrG *dstHdr)
    {

    }
    virtual void OnSecCfgCfm(SecurityConfigCfm& aInd)
    {

    }
    virtual void OnCrtTunCfm(CreateTunnelCfm& aInd)
    {

    }
    virtual void OnModTunCfm(ModifyTunnelCfm& aInd)
    {

    }
    virtual void OnDelTunCfm(DeleteTunnelCfm& aInd)
    {

    }
    virtual void OnAddRbCfgCfm(AddRbConfigCfm& aInd)
    {

    }
    virtual void OnModRbCfgCfm(ModifyRbConfigCfm& aInd)
    {
        
    }
    virtual void OnDelRbCfgCfm(DeleteRbConfigCfm& aInd)
    {

    }
    virtual void OnPdcpSduStatCfm(PDCPSduStatusCfm& aInd)
    {

    }
    virtual void OnAddPdcpCfgCfm(AddPDCPConfigCfm& aInd)
    {

    }
    virtual void OnModPdcpCfgCfm(ModifyPDCPConfigCfm& aInd)
    {

    }
    Cp::AsyncService mService;
};

class EbmClient : public ServiceClientImpl
{
};
