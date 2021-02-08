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
* File Name: CUUPLibRpcServerThread.cpp
*
* Subsystem: libCUUP
*
* Description: This file defines the RPC server for libCUUP
*
* Revision History:
* Date        Name         Description
* ----        ----         -----------
* 10/19/2020  Fisher       Initial Release
*
**********************************************************************/
#include "GrpcAdpRpcServerThread.h"
#include "GrpcAdpCntrl.h"
#include "EbmCfmCb.h"
#include "SvcLog.h"
#include "GrpcLog.h"

char grpcServerLogBuf[80];

void GrpcAdpRpcServerThread::init(uint32_t aThreadInstId)
{
    mThreadInstId = aThreadInstId;
    //CUUPLibRpcServerThread::Init(mIp + ":" + std::to_string(mPort));
    if(mServerAddr.size() > 0)
    {
        ccLog("init server %s", mServerAddr.c_str());
        CucpServer::Init(mServerAddr);
        mThreadReady = true;
    }
}

void GrpcAdpRpcServerThread::on_init()
{
    GRPC_LOG0(server_inited);
}

void GrpcAdpRpcServerThread::on_exit()
{
    GRPC_LOG0(server_exited);
    mThreadInstId = 0;
    mThreadReady = false;
}

void GrpcAdpRpcServerThread::run()
{
    StartThread();
}

std::string pdcpCfgFailureReasonArr[PDCPConfigFailureReasonG::REAS_BUTT_G] = {
    "REAS_NONE",
    "ENT_CREAT_FAIL",
    "INVALID_RLC_MODE",
    "CIPHER_ALGO_INVALID",
    "ENT_DEL_FAIL",
    "UE_DEL_FAIL",
    "REEST_FAIL",
    "COMP_ALGO_INVALID",
    "SAME_UEID",
    "SAME_RBID",
    "INVALID_RBID"
};

void GrpcAdpRpcServerThread::TransCpUpHdr(const ::structure::CpUpHdr &srcHdr, CuCpUpHdrG *dstHdr)
{
    dstHdr->transId = srcHdr.trans_id();
    dstHdr->cuCpPst.dstProcID = srcHdr.pst().dst_proc_id();
    dstHdr->cuCpPst.srcProcID = srcHdr.pst().src_proc_id();
    dstHdr->cuCpPst.dstEnt = srcHdr.pst().dst_ent();
    dstHdr->cuCpPst.dstInst = srcHdr.pst().dst_inst();
    dstHdr->cuCpPst.srcEnt = srcHdr.pst().src_ent();
    dstHdr->cuCpPst.srcInst = srcHdr.pst().src_inst();
    dstHdr->cuCpPst.prior = srcHdr.pst().prior();
    dstHdr->cuCpPst.route = srcHdr.pst().route();
    dstHdr->cuCpPst.event = srcHdr.pst().event();
    dstHdr->cuCpPst.region = srcHdr.pst().region();
    dstHdr->cuCpPst.pool = srcHdr.pst().pool();
    dstHdr->cuCpPst.selector = srcHdr.pst().selector();
    dstHdr->cuCpPst.interfaceVersion = srcHdr.pst().interface_version();
    dstHdr->cuCpPst.cellID = srcHdr.pst().cell_id();

    dstHdr->cuCpId.cpId = srcHdr.cu_cp_id().cp_id();
    dstHdr->cuCpId.cellId = srcHdr.cu_cp_id().cell_id();
    dstHdr->cuCpId.cpUeId = srcHdr.cu_cp_id().ue_id() & 0x0000FFFF;

    dstHdr->cuUpId.upId = srcHdr.cu_up_id().up_id();
    dstHdr->cuUpId.cellId = srcHdr.cu_up_id().cell_id();
    dstHdr->cuUpId.upUeId = srcHdr.cu_up_id().ue_id();

    dstHdr->suSpId.suId = srcHdr.su_sp_id().su_id();
    dstHdr->suSpId.spId = srcHdr.su_sp_id().sp_id();
}

TunnelTypeG GrpcAdpRpcServerThread::RecoverTunType(::structure::TunnelType tunnelType, InterfaceTypeG interfaceType)
{
    switch (tunnelType) {
        case ::structure::DL:
        case ::structure::UL:
            return WR_TNL_NORMAL_G;
        case ::structure::Dl_DATA_FWD:
            return WR_TNL_DL_DATFWD_G;
        case ::structure::UL_DATA_FWD:
            return WR_TNL_UL_DATFWD_G;
        case ::structure::LIPA:
            return WR_TNL_LIPA_G;
        default:
            return WR_TNL_NORMAL_G;
    } 
}

void GrpcAdpRpcServerThread::OnSecCfgCfm(SecurityConfigCfm& aInd)
{
    //transfer the rsp to trillium
    GRPC_LOG0(recv_sec_cfg_cfm_in);
    SecurityConfigCfmG secCfgCfm;
    
    TransCpUpHdr(aInd.cp_up_hdr(), &(secCfgCfm.cp_up_hdr));

    strcpy(secCfgCfm.sec_cfm.reason, aInd.sec_cfm().reason().c_str());
    secCfgCfm.sec_cfm.status = aInd.sec_cfm().status();
    send_sec_cfg_cfm(&secCfgCfm);
 
    //update the route table for dummy_up
    update_route_table(&(secCfgCfm.cp_up_hdr));
    GRPC_LOG0(recv_sec_cfg_cfm_out);
}

void GrpcAdpRpcServerThread::OnCrtTunCfm(CreateTunnelCfm& aInd)
{
    //transfer the rsp to trillium
    uint32_t idx;
    GRPC_LOG0(recv_crt_tun_cfm_in);
    
    CreateTunnelCfmG crtTunCfm;
    TransCpUpHdr(aInd.cp_up_hdr(), &(crtTunCfm.cp_up_hdr));
    //crtTunCfm.interface_type =  (InterfaceTypeG)(uint32_t)aInd.interface_type();
    CreateTnlCfmEntG *crtTunCfmEntOut;
    for (idx = 0; (idx < aInd.num_ent()) && (idx < MAX_TUN_LIST_SIZE_G); idx++) {
        ::structure::CreateTnlCfmEnt crtTunCfmEntIn = aInd.tnl_cfm_list(idx);
        crtTunCfmEntOut = &(crtTunCfm.tnl_cfm_list[idx]);
        crtTunCfmEntOut->gtp_tunnel_id.cell_id = crtTunCfmEntIn.gtp_tunnel_id().cell_id();
        crtTunCfmEntOut->gtp_tunnel_id.crnti = crtTunCfmEntIn.gtp_tunnel_id().crnti();
        crtTunCfmEntOut->gtp_tunnel_id.rb_id = crtTunCfmEntIn.gtp_tunnel_id().rb_id();
        crtTunCfmEntOut->gtp_tunnel_id.tnl_type = RecoverTunType(crtTunCfmEntIn.gtp_tunnel_id().tnl_type(), (uint32_t)crtTunCfmEntIn.interface_type());
        crtTunCfmEntOut->gtp_tunnel_id.rb_keep_indicator = crtTunCfmEntIn.gtp_tunnel_id().rb_keep_indicator();
        crtTunCfmEntOut->gtp_tunnel_id.tnl_priority = crtTunCfmEntIn.gtp_tunnel_id().tnl_priority();
        crtTunCfmEntOut->interface_type = (uint32_t)crtTunCfmEntIn.interface_type();

        crtTunCfmEntOut->dst_addr.type = (uint32_t)crtTunCfmEntIn.dst_addr().type();
        strcpy(crtTunCfmEntOut->dst_addr.addr, crtTunCfmEntIn.dst_addr().addr().c_str());
        crtTunCfmEntOut->src_addr.type = (uint32_t)crtTunCfmEntIn.src_addr().type();
        strcpy(crtTunCfmEntOut->src_addr.addr, crtTunCfmEntIn.src_addr().addr().c_str());
        crtTunCfmEntOut->local_gtp_teid = crtTunCfmEntIn.local_gtp_teid();
        crtTunCfmEntOut->remote_gtp_teid = crtTunCfmEntIn.remote_gtp_teid();
        crtTunCfmEntOut->status = crtTunCfmEntIn.status();
        GRPC_LOG0(recv_crt_tun_cfm_loop);
    }
    crtTunCfm.num_ent = idx;

    send_dam_tun_crt_cfm(&crtTunCfm);

    GRPC_LOG0(recv_crt_tun_cfm_out);
}

void GrpcAdpRpcServerThread::CopyModTunToCrtTun(ModifyTunnelCfmG *modTunCfm, CreateTunnelCfmG *crtTunCfm)
{
    uint32_t idx;
    CreateTnlCfmEntG *crtTunCfmEntOut;
    ModTnlCfmEntG *modTunCfmEntIn;

    for (idx = 0; idx < modTunCfm->num_ent; idx++) {
        modTunCfmEntIn = &(modTunCfm->tnl_cfm_list[idx]);
        crtTunCfmEntOut = &(crtTunCfm->tnl_cfm_list[idx]);

        crtTunCfmEntOut->gtp_tunnel_id.cell_id = modTunCfmEntIn->gtp_tunnel_id.cell_id;
        crtTunCfmEntOut->gtp_tunnel_id.crnti = modTunCfmEntIn->gtp_tunnel_id.crnti;
        crtTunCfmEntOut->gtp_tunnel_id.rb_id = modTunCfmEntIn->gtp_tunnel_id.rb_id;
        crtTunCfmEntOut->gtp_tunnel_id.tnl_type = modTunCfmEntIn->gtp_tunnel_id.tnl_type;
        crtTunCfmEntOut->gtp_tunnel_id.rb_keep_indicator = modTunCfmEntIn->gtp_tunnel_id.rb_keep_indicator;
        crtTunCfmEntOut->gtp_tunnel_id.tnl_priority = modTunCfmEntIn->gtp_tunnel_id.tnl_priority;
        if (modTunCfmEntIn->has_addr == true) {
            //crtTunCfmEntOut->dst_addr.type = modTunCfmEntIn->dst_addr.type;
            //strcpy(crtTunCfmEntOut->dst_addr.addr, modTunCfmEntIn->dst_addr.addr);

            crtTunCfmEntOut->src_addr.type = modTunCfmEntIn->src_addr.type;
            strcpy(crtTunCfmEntOut->src_addr.addr, modTunCfmEntIn->src_addr.addr);

            crtTunCfmEntOut->has_addr = true;
        }
        crtTunCfmEntOut->status = modTunCfmEntIn->status;
        crtTunCfmEntOut->local_gtp_teid = modTunCfmEntIn->local_gtp_teid;
        crtTunCfmEntOut->remote_gtp_teid = modTunCfmEntIn->remote_gtp_teid;

    }
    crtTunCfm->num_ent = modTunCfm->num_ent;
}

void GrpcAdpRpcServerThread::OnModTunCfm(ModifyTunnelCfm& aInd)
{
    uint32_t idx;
    ModifyTunnelCfmG modTunCfm;
    TransCpUpHdr(aInd.cp_up_hdr(), &(modTunCfm.cp_up_hdr));
    GRPC_LOG0(recv_mod_tun_cfm_in);
    ModTnlCfmEntG *modTunCfmEntOut;
    for (idx = 0; (idx < aInd.num_ent()) && (idx < MAX_TUN_LIST_SIZE_G); idx++) {
        ::structure::ModTnlCfmEnt modTunCfmEntIn = aInd.tnl_cfm_list(idx);
        modTunCfmEntOut = &(modTunCfm.tnl_cfm_list[idx]);
        modTunCfmEntOut->interface_type =  (InterfaceTypeG)(uint32_t)modTunCfmEntIn.interface_type();
        modTunCfmEntOut->gtp_tunnel_id.cell_id = modTunCfmEntIn.gtp_tunnel_id().cell_id();
        modTunCfmEntOut->gtp_tunnel_id.crnti = modTunCfmEntIn.gtp_tunnel_id().crnti();
        modTunCfmEntOut->gtp_tunnel_id.rb_id = modTunCfmEntIn.gtp_tunnel_id().rb_id();
        modTunCfmEntOut->gtp_tunnel_id.tnl_type = RecoverTunType(modTunCfmEntIn.gtp_tunnel_id().tnl_type(), (uint32_t)modTunCfmEntIn.interface_type());
        modTunCfmEntOut->gtp_tunnel_id.rb_keep_indicator = modTunCfmEntIn.gtp_tunnel_id().rb_keep_indicator();
        modTunCfmEntOut->gtp_tunnel_id.tnl_priority = modTunCfmEntIn.gtp_tunnel_id().tnl_priority();

        modTunCfmEntOut->old_cell_id = modTunCfmEntIn.old_cell_id();
        modTunCfmEntOut->old_crnti = modTunCfmEntIn.old_crnti();

        if (modTunCfmEntIn.has_dst_addr()) {
            modTunCfmEntOut->dst_addr.type = (uint32_t)modTunCfmEntIn.dst_addr().type();
            strcpy(modTunCfmEntOut->dst_addr.addr, modTunCfmEntIn.dst_addr().addr().c_str());
        }

        if (modTunCfmEntIn.has_src_addr()) {
            modTunCfmEntOut->has_addr = true;
            modTunCfmEntOut->src_addr.type = (uint32_t)modTunCfmEntIn.src_addr().type();
            strcpy(modTunCfmEntOut->src_addr.addr, modTunCfmEntIn.src_addr().addr().c_str());
        }
        modTunCfmEntOut->local_gtp_teid = modTunCfmEntIn.local_gtp_teid();
        modTunCfmEntOut->remote_gtp_teid = modTunCfmEntIn.remote_gtp_teid();
        modTunCfmEntOut->status = modTunCfmEntIn.status();
    }
    modTunCfm.num_ent = idx;
    if (aInd.tnl_cfm_list(0).interface_type() == structure::S1) {
        if (modTunCfm.cp_up_hdr.cuCpPst.event == EVTDAMTNLSWTCHREQ_G){
            CreateTunnelCfmG crtTunCfm;
            CopyModTunToCrtTun(&modTunCfm, &crtTunCfm);
            TransCpUpHdr(aInd.cp_up_hdr(), &(crtTunCfm.cp_up_hdr));
            send_dam_tun_crt_cfm(&crtTunCfm);
        } else if (modTunCfm.cp_up_hdr.cuCpPst.event == EVTDAMTNLMODREQ_G){
            send_dam_tun_mod_cfm(&modTunCfm);
        }
    }
    if (aInd.tnl_cfm_list(0).interface_type() == structure::V1) {
        if (modTunCfm.cp_up_hdr.cuCpPst.event == EVTCVZTNLCELLSWITCHREQ_G) {
            send_v1u_ue_cell_switch_cfm(&modTunCfm);
        } else if (modTunCfm.cp_up_hdr.cuCpPst.event == EVTCVZTUNNELCREATEREQ_G) {
            CreateTunnelCfmG crtTunCfm;
            CopyModTunToCrtTun(&modTunCfm, &crtTunCfm);
            TransCpUpHdr(aInd.cp_up_hdr(), &(crtTunCfm.cp_up_hdr));
            send_v1u_tun_crt_cfm(&crtTunCfm);
        }
    }
    
    GRPC_LOG0(recv_mod_tun_cfm_out);
}

void GrpcAdpRpcServerThread::OnDelTunCfm(DeleteTunnelCfm& aInd)
{
    //transfer the rsp to trillium
    uint32_t idx;
    DeleteTunnelCfmG delTunCfm;
    TransCpUpHdr(aInd.cp_up_hdr(), &(delTunCfm.cp_up_hdr));
    GRPC_LOG0(recv_del_tun_cfm_in);
    DelTnlCfmEntG *delTunCfmEntOut;
    if (aInd.num_ent() > 0) {
        for (idx = 0; (idx < aInd.num_ent()) && (idx < MAX_TUN_LIST_SIZE_G); idx++) {
            ::structure::DelTnlCfmEnt delTunCfmEntIn = aInd.tnl_cfm_list(idx);
            delTunCfmEntOut = &(delTunCfm.tnl_cfm_list[idx]);

            delTunCfmEntOut->interface_type = (InterfaceTypeG)(uint32_t)delTunCfmEntIn.interface_type();
            delTunCfmEntOut->gtp_tunnel_id.cell_id = delTunCfmEntIn.gtp_tunnel_id().cell_id();
            delTunCfmEntOut->gtp_tunnel_id.crnti = delTunCfmEntIn.gtp_tunnel_id().crnti();
            delTunCfmEntOut->gtp_tunnel_id.rb_id = delTunCfmEntIn.gtp_tunnel_id().rb_id();
            delTunCfmEntOut->gtp_tunnel_id.tnl_type = RecoverTunType(delTunCfmEntIn.gtp_tunnel_id().tnl_type(), (uint32_t)delTunCfmEntIn.interface_type());
            delTunCfmEntOut->gtp_tunnel_id.rb_keep_indicator = delTunCfmEntIn.gtp_tunnel_id().rb_keep_indicator();
            delTunCfmEntOut->gtp_tunnel_id.tnl_priority = delTunCfmEntIn.gtp_tunnel_id().tnl_priority();

            delTunCfmEntOut->dst_addr.type = (uint32_t)delTunCfmEntIn.dst_addr().type();
            strcpy(delTunCfmEntOut->dst_addr.addr, delTunCfmEntIn.dst_addr().addr().c_str());
            delTunCfmEntOut->src_addr.type = (uint32_t)delTunCfmEntIn.src_addr().type();
            strcpy(delTunCfmEntOut->src_addr.addr, delTunCfmEntIn.src_addr().addr().c_str());
            delTunCfmEntOut->local_gtp_teid = delTunCfmEntIn.local_gtp_teid();
            delTunCfmEntOut->remote_gtp_teid = delTunCfmEntIn.remote_gtp_teid();
            delTunCfmEntOut->status = delTunCfmEntIn.status();
        }
        delTunCfm.num_ent = idx;
    } else {
        delTunCfm.num_ent = 0;
    }

    if ((ENTEU_G == delTunCfm.cp_up_hdr.cuCpPst.dstEnt) && (delTunCfm.num_ent == 0)) {
        send_dam_tun_del_cfm(&delTunCfm);
    } else {
        if (aInd.tnl_cfm_list(0).interface_type() == structure::S1) {
            send_dam_tun_del_cfm(&delTunCfm);
        }
        if (aInd.tnl_cfm_list(0).interface_type() == structure::V1) {
            send_v1u_tun_del_cfm(&delTunCfm);
        }
    }
    GRPC_LOG0(recv_del_tun_cfm_out);
}

void GrpcAdpRpcServerThread::OnAddRbCfgCfm(AddRbConfigCfm& aInd)
{
    GRPC_LOG0(recv_add_rb_cfm_in);
    //transfer the rsp to trillium
    uint32_t idx;
    AddRbConfigCfmG addRbCfgCfm;
    TransCpUpHdr(aInd.cp_up_hdr(), &(addRbCfgCfm.cp_up_hdr));
    PDCPAddCfgCfmEntG *addCfgCfmEntOut;
    for (idx = 0; (idx < aInd.num_ent()) && (idx < MAX_TUN_LIST_SIZE_G); idx++) {
        ::structure::PDCPAddCfgCfmEnt addCfgCfmEntIn = aInd.config_cfm_ent(idx);
        addCfgCfmEntOut = &(addRbCfgCfm.config_cfm_ent[idx]);
        addCfgCfmEntOut->rb_id = addCfgCfmEntIn.rb_id();
        addCfgCfmEntOut->status = addCfgCfmEntIn.status();
        strcpy(addCfgCfmEntOut->reason,pdcpCfgFailureReasonArr[addCfgCfmEntIn.reason()].c_str());
    }
    addRbCfgCfm.num_ent = idx;

    send_add_rb_cfg_cfm(&addRbCfgCfm);
    //update the route table for dummy_up, only update the table in sec cfg cfm
    //update_route_table(&(addRbCfgCfm.cp_up_hdr));
    GRPC_LOG0(recv_add_rb_cfm_out);
}


void GrpcAdpRpcServerThread::OnModRbCfgCfm(ModifyRbConfigCfm& aInd)
{
    GRPC_LOG0(recv_mod_rb_cfm_in);
    //transfer the rsp to trillium
    uint32_t idx;
    ModifyRbConfigCfmG modRbCfgCfm;
    TransCpUpHdr(aInd.cp_up_hdr(), &(modRbCfgCfm.cp_up_hdr));

    PDCPModCfgCfmEntG *modCfgCfmEntOut;
    for (idx = 0; (idx < aInd.num_ent()) && (idx < MAX_TUN_LIST_SIZE_G); idx++) {
        ::structure::PDCPModCfgCfmEnt modCfgCfmEntIn = aInd.config_cfm_ent(idx);
        modCfgCfmEntOut = &(modRbCfgCfm.config_cfm_ent[idx]);
        modCfgCfmEntOut->rb_id = modCfgCfmEntIn.rb_id();
        modCfgCfmEntOut->status = modCfgCfmEntIn.status();
        strcpy(modCfgCfmEntOut->reason,pdcpCfgFailureReasonArr[modCfgCfmEntIn.reason()].c_str());
    }
    modRbCfgCfm.num_ent = idx;

    send_mod_rb_cfg_cfm(&modRbCfgCfm);
    GRPC_LOG0(recv_mod_rb_cfm_out);
}

void GrpcAdpRpcServerThread::OnDelRbCfgCfm(DeleteRbConfigCfm& aInd)
{
    GRPC_LOG0(recv_del_rb_cfm_in);
    //transfer the rsp to trillium
    uint32_t idx;
    DeleteRbConfigCfmG delRbCfgCfm;
    TransCpUpHdr(aInd.cp_up_hdr(), &(delRbCfgCfm.cp_up_hdr));
    PDCPDelCfgCfmEntG *delCfgCfmEntOut;
    for (idx = 0; (idx < aInd.num_ent()) && (idx < MAX_TUN_LIST_SIZE_G); idx++) {
        ::structure::PDCPDelCfgCfmEnt delCfgCfmEntIn = aInd.config_cfm_ent(idx);
        delCfgCfmEntOut = &(delRbCfgCfm.config_cfm_ent[idx]);
        delCfgCfmEntOut->rb_id = delCfgCfmEntIn.rb_id();
        delCfgCfmEntOut->status = delCfgCfmEntIn.status();
        strcpy(delCfgCfmEntOut->reason,pdcpCfgFailureReasonArr[delCfgCfmEntIn.reason()].c_str());
    }
    delRbCfgCfm.num_ent = idx;

    send_del_rb_cfg_cfm(&delRbCfgCfm);
    GRPC_LOG0(recv_del_rb_cfm_out);
}

void GrpcAdpRpcServerThread::OnPdcpSduStatCfm(PDCPSduStatusCfm& aInd)
{
    GRPC_LOG0(recv_sdu_status_cfm_in);
    PDCPSduStatusCfmG pdcpSduStatusCfm;
    SduStatusInfoG *sduStatusOut;
    int idx;
    TransCpUpHdr(aInd.cp_up_hdr(), &(pdcpSduStatusCfm.cp_up_hdr));

    //workaround for EBM can't return the real sdu status
    for (idx = 0; (idx < aInd.num_rb()) && (idx < CPJ_MAX_DRB_G); idx++) {
        ::structure::SduStatusInfo sduStatusIn = aInd.sdu_status_info(idx);
        sduStatusOut = &(pdcpSduStatusCfm.sdu_status_info[idx]);
        sduStatusOut->rbId = sduStatusIn.rbid();
        sduStatusOut->dir = sduStatusIn.dir();
        sduStatusOut->snLen = sduStatusIn.snlen();
        sduStatusOut->ulSduStaInfo.numBits = sduStatusIn.ulsdustainfo().numbits();
        sduStatusOut->ulSduStaInfo.count = sduStatusIn.ulsdustainfo().count();
        sduStatusOut->dlSduStaInfo.count = sduStatusIn.dlsdustainfo().count();
        GRPC_PRT_STR_ARG(grpcServerLogBuf, "OnPdcpSduStatCfm rb[%d] dir[%d]", sduStatusOut->rbId, sduStatusOut->dir);
    }
    pdcpSduStatusCfm.num_rb = idx;

    pdcpSduStatusCfm.status = aInd.status();
    pdcpSduStatusCfm.reason = (uint32_t)aInd.reason();

    send_pdcp_sdu_stat_cfm(&pdcpSduStatusCfm);
    GRPC_LOG0(recv_sdu_status_cfm_out);
}

void GrpcAdpRpcServerThread::OnAddPdcpCfgCfm(AddPDCPConfigCfm& aInd)
{
    GRPC_LOG0(recv_add_pdcp_cfm_in);
    SecurityConfigCfmG secCfgCfm;
    int idx;
    
    TransCpUpHdr(aInd.cp_up_hdr(), &(secCfgCfm.cp_up_hdr));

    strcpy(secCfgCfm.sec_cfm.reason, aInd.sec_cfm().reason().c_str());
    secCfgCfm.sec_cfm.status = aInd.sec_cfm().status();
    send_sec_cfg_cfm(&secCfgCfm);
    //update the route table for dummy_up
    update_route_table(&(secCfgCfm.cp_up_hdr));

    AddRbConfigCfmG addRbCfgCfm;
    TransCpUpHdr(aInd.cp_up_hdr(), &(addRbCfgCfm.cp_up_hdr));

    PDCPAddCfgCfmEntG *addCfgCfmEntOut;
    for (idx = 0; (idx < aInd.num_ent()) && (idx < MAX_TUN_LIST_SIZE_G); idx++) {
        ::structure::PDCPAddCfgCfmEnt addCfgCfmEntIn = aInd.config_cfm_ent(idx);
        addCfgCfmEntOut = &(addRbCfgCfm.config_cfm_ent[idx]);
        addCfgCfmEntOut->rb_id = addCfgCfmEntIn.rb_id();
        addCfgCfmEntOut->status = addCfgCfmEntIn.status();
        strcpy(addCfgCfmEntOut->reason,pdcpCfgFailureReasonArr[addCfgCfmEntIn.reason()].c_str());
    }
    addRbCfgCfm.num_ent = idx;

    send_add_rb_cfg_cfm(&addRbCfgCfm);
    
    GRPC_LOG0(recv_add_pdcp_cfm_out);
}


void GrpcAdpRpcServerThread::OnModPdcpCfgCfm(ModifyPDCPConfigCfm& aInd)
{
    GRPC_LOG0(recv_mod_pdcp_cfm_in);
    SecurityConfigCfmG secCfgCfm;
    int idx;
    
    TransCpUpHdr(aInd.cp_up_hdr(), &(secCfgCfm.cp_up_hdr));

    strcpy(secCfgCfm.sec_cfm.reason, aInd.sec_cfm().reason().c_str());
    secCfgCfm.sec_cfm.status = aInd.sec_cfm().status();
    send_sec_cfg_cfm(&secCfgCfm);

    ModifyRbConfigCfmG modRbCfgCfm;
    TransCpUpHdr(aInd.cp_up_hdr(), &(modRbCfgCfm.cp_up_hdr));

    PDCPModCfgCfmEntG *modCfgCfmEntOut;
    for (idx = 0; (idx < aInd.num_ent()) && (idx < MAX_TUN_LIST_SIZE_G); idx++) {
        ::structure::PDCPModCfgCfmEnt modCfgCfmEntIn = aInd.config_cfm_ent(idx);
        modCfgCfmEntOut = &(modRbCfgCfm.config_cfm_ent[idx]);
        modCfgCfmEntOut->rb_id = modCfgCfmEntIn.rb_id();
        modCfgCfmEntOut->status = modCfgCfmEntIn.status();
        strcpy(modCfgCfmEntOut->reason,pdcpCfgFailureReasonArr[modCfgCfmEntIn.reason()].c_str());
    }
    modRbCfgCfm.num_ent = idx;

    send_mod_rb_cfg_cfm(&modRbCfgCfm);
    
    GRPC_LOG0(recv_mod_pdcp_cfm_out);
}