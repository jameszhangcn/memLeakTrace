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
* File Name: CUUPLibRpcClientThread.cpp
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
#include "GrpcAdpRpcClientThread.h"
#include "SvcLog.h"
#include "CucpService.h"
#include "GrpcLog.h"

enum {
    SERVICE_EBM
};

char grpcClientLogBuf[80];

void GrpcAdpRpcClientThread::init(uint32_t aThreadInstId)
{
    mThreadInstId = aThreadInstId;

    //ccLog("Connect to EBM %s", mEbmServerTarget);
    Connect(SERVICE_EBM, mEbmServerTarget);

    mThreadReady = true;
    secCfgClient = new CucpEbmSecCfgClient(getChannel(SERVICE_EBM), getCompletionQueue());
    crtTunClient = new CucpEbmCrtTunClient(getChannel(SERVICE_EBM), getCompletionQueue());
    modTunClient = new CucpEbmModTunClient(getChannel(SERVICE_EBM), getCompletionQueue());
    delTunClient = new CucpEbmDelTunClient(getChannel(SERVICE_EBM), getCompletionQueue());
    tunStateChgClient = new CucpEbmTunStateChgClient(getChannel(SERVICE_EBM), getCompletionQueue());

    addRbCfgClient = new CucpEbmAddRbCfgClient(getChannel(SERVICE_EBM), getCompletionQueue());
    modRbCfgClient = new CucpEbmModRbCfgClient(getChannel(SERVICE_EBM), getCompletionQueue());
    delRbCfgClient = new CucpEbmDelRbCfgClient(getChannel(SERVICE_EBM), getCompletionQueue());
    pauseResumeClient = new CucpEbmPauseResumeClient(getChannel(SERVICE_EBM), getCompletionQueue());
    pdcpSduStatusClient = new CucpEbmPdcpSduStatusClient(getChannel(SERVICE_EBM), getCompletionQueue());
    dataFwdClient = new CucpEbmDataFwdClient(getChannel(SERVICE_EBM), getCompletionQueue());
    ueInactTimerClient = new CucpEbmUeInactTimerClient(getChannel(SERVICE_EBM), getCompletionQueue());
    addPdcpCfgClient = new CucpEbmAddPdcpCfgClient(getChannel(SERVICE_EBM), getCompletionQueue());
    modPdcpCfgClient = new CucpEbmModPdcpCfgClient(getChannel(SERVICE_EBM), getCompletionQueue());
}

void GrpcAdpRpcClientThread::on_init()
{
    ccLog("");
}
void GrpcAdpRpcClientThread::SetMhdr(CuCpUpHdrG *cu_up_hdr, ::mhdr::MsgHdr *hdr)
{

    static uint32_t seq = 0;
    hdr->mutable_torid()->set_sn("EBM");
    hdr->mutable_fromrid()->set_sn("CUCP");
    hdr->mutable_fromloc()->set_address("127.0.0.1");
    hdr->mutable_fromloc()->set_port(9021);

    hdr->mutable_toloc()->set_address("127.0.0.1");
    hdr->mutable_toloc()->set_port(9027);

    hdr->set_transid(seq++);
    return;
}

void GrpcAdpRpcClientThread::SetCucpupHdr(CuCpUpHdrG *cu_up_hdr, ::structure::CpUpHdr *cpUpHdr)
{
    cpUpHdr->mutable_pst()->set_dst_proc_id(cu_up_hdr->cuCpPst.dstProcID);
    cpUpHdr->mutable_pst()->set_src_proc_id(cu_up_hdr->cuCpPst.srcProcID);
    cpUpHdr->mutable_pst()->set_dst_ent(cu_up_hdr->cuCpPst.dstEnt);
    cpUpHdr->mutable_pst()->set_dst_inst(cu_up_hdr->cuCpPst.dstInst);
    cpUpHdr->mutable_pst()->set_src_ent(cu_up_hdr->cuCpPst.srcEnt);
    cpUpHdr->mutable_pst()->set_src_inst(cu_up_hdr->cuCpPst.srcInst);

    cpUpHdr->mutable_pst()->set_prior(cu_up_hdr->cuCpPst.prior);
    cpUpHdr->mutable_pst()->set_route(cu_up_hdr->cuCpPst.route);
    cpUpHdr->mutable_pst()->set_event(cu_up_hdr->cuCpPst.event);
    cpUpHdr->mutable_pst()->set_region(cu_up_hdr->cuCpPst.region);
    cpUpHdr->mutable_pst()->set_pool(cu_up_hdr->cuCpPst.pool);
    cpUpHdr->mutable_pst()->set_selector(cu_up_hdr->cuCpPst.selector);
    cpUpHdr->mutable_pst()->set_interface_version(cu_up_hdr->cuCpPst.interfaceVersion);
    cpUpHdr->mutable_pst()->set_cell_id(cu_up_hdr->cuCpPst.cellID);

    cpUpHdr->mutable_cu_cp_id()->set_cp_id(cu_up_hdr->cuCpId.cpId);
    cpUpHdr->mutable_cu_cp_id()->set_cell_id(cu_up_hdr->cuCpId.cellId);
    //cpUpHdr->mutable_cu_cp_id()->set_ue_id(cu_up_hdr->cuCpId.cpUeId);
    uint32_t cuCpUeId = (((cu_up_hdr->cuCpId.cellId << 16) & 0xFFFF0000) | cu_up_hdr->cuCpId.cpUeId);
    cpUpHdr->mutable_cu_cp_id()->set_ue_id(cuCpUeId);

    cpUpHdr->mutable_cu_up_id()->set_up_id(cu_up_hdr->cuUpId.upId);
    cpUpHdr->mutable_cu_up_id()->set_cell_id(cu_up_hdr->cuUpId.cellId);
    //EBM think 0 is a invalid value
    if (INVALID_ID_G == cu_up_hdr->cuUpId.upUeId) {
        cpUpHdr->mutable_cu_up_id()->set_ue_id(0);
    } else {  
        cpUpHdr->mutable_cu_up_id()->set_ue_id(cu_up_hdr->cuUpId.upUeId);
    }

    cpUpHdr->mutable_su_sp_id()->set_su_id(cu_up_hdr->suSpId.suId);
    cpUpHdr->mutable_su_sp_id()->set_sp_id(cu_up_hdr->suSpId.spId);

    cpUpHdr->set_trans_id(cu_up_hdr->transId);
    return;
}

int GrpcAdpRpcClientThread::SendSecurityCfgReq(CuCpUpHdrG* cp_up_hdr, SecurityConfigReqG *secCfgIn)
{
    int i;
    srvEbm::SecurityConfigReq aMsg;

    ::mhdr::MsgHdr * hdr = aMsg.mutable_hdr();
    ::structure::CpUpHdr *cpUpHdr = aMsg.mutable_cp_up_hdr();
    SetMhdr(cp_up_hdr, hdr);
    hdr->set_method(srvEbm::MethodSubmitSecurityConfigReq);
    hdr->set_methodname("MethodSubmitSecurityConfigReq");
    GRPC_LOG0(send_sec_cfg_req_in);

    SetCucpupHdr(cp_up_hdr, cpUpHdr);

    aMsg.mutable_security_config()->set_algorithm_type(secCfgIn->security_config.algorithm_type);
    
    ::structure::SecurityConfig *secCfgOut = aMsg.mutable_security_config();

    for (int i=0; i < CPJ_CIPHER_KEY_LEN_G; i++) {
        secCfgOut->add_encryption_key(secCfgIn->security_config.encryption_key[i]);
    }

    //CucpEbmSecCfgClient client(getChannel(SERVICE_EBM), getCompletionQueue());

    secCfgClient->Send(aMsg);
 
    GRPC_LOG0(send_sec_cfg_req_out);
    return 0; // may return other failure code if needed
}

::structure::TunnelType GrpcAdpRpcClientThread::GetTunType(TunnelTypeG tunnelType, InterfaceTypeG interfaceType)
{
    switch (tunnelType) {
        case WR_TNL_NORMAL_G:
            if (S1_G == interfaceType) {
                return ::structure::DL;
            }
            return ::structure::UL;
        case WR_TNL_DL_DATFWD_G:
            return ::structure::Dl_DATA_FWD;
        case WR_TNL_UL_DATFWD_G:
            return ::structure::UL_DATA_FWD;
        case WR_TNL_LIPA_G:
            return ::structure::LIPA;
        default:
            return ::structure::DL;
    } 
}

::common::IpAddressType GrpcAdpRpcClientThread::GetIpType(IpAddressTypeG type)
{
    switch (type) {
      case IPV4_G:
            return ::common::IPV4;
        case IPV6_G:
            return ::common::IPV6;
        default:
            return ::common::IPV4;
    } 
}

int GrpcAdpRpcClientThread::SendCrtTunReq(CuCpUpHdrG* cp_up_hdr, CreateTunnelReqG *crtTunReq)
{
    srvEbm::CreateTunnelReq aMsg;
    uint32_t idx;
    std::string str;

    ::mhdr::MsgHdr * hdr = aMsg.mutable_hdr();
    ::structure::CpUpHdr *cpUpHdr = aMsg.mutable_cp_up_hdr();
    SetMhdr(cp_up_hdr, hdr);
    SetCucpupHdr(cp_up_hdr, cpUpHdr);
    hdr->set_method(srvEbm::MethodSubmitCreateTunnelReq);
    hdr->set_methodname("MethodSubmitCreateTunnelReq");

    GRPC_LOG0(send_crt_tun_req_in);

    aMsg.set_num_ent(crtTunReq->num_ent);
    GtpTunnelInfoG * gtpTunInfoIn;
    for (idx = 0; idx < crtTunReq->num_ent; idx++) {
        ::structure::GtpTunnelInfo *gtpTunInfo = aMsg.add_tnl_info_list();
        ::structure::GtpTunnelId *gtpTunID = gtpTunInfo->mutable_tnl_id();
        gtpTunInfoIn = &(crtTunReq->tnl_info_list[idx]);

        gtpTunID->set_cell_id(gtpTunInfoIn->tnl_id.cell_id);
        gtpTunID->set_crnti(gtpTunInfoIn->tnl_id.crnti);
        gtpTunID->set_rb_id(gtpTunInfoIn->tnl_id.rb_id);
        gtpTunID->set_tnl_type(GetTunType(gtpTunInfoIn->tnl_id.tnl_type, gtpTunInfoIn->interface_type));
        gtpTunID->set_tnl_priority(gtpTunInfoIn->tnl_id.tnl_priority);
        gtpTunID->set_rb_keep_indicator(gtpTunInfoIn->tnl_id.rb_keep_indicator);

        gtpTunInfo->set_rem_teid(gtpTunInfoIn->rem_teid);
        gtpTunInfo->set_dscp(gtpTunInfoIn->dscp);
        gtpTunInfo->set_tunnel_type(GetTunType(gtpTunInfoIn->tnl_id.tnl_type, gtpTunInfoIn->interface_type));
        gtpTunInfo->set_interface_type((uint32_t)gtpTunInfoIn->interface_type);
        //set addr
        gtpTunInfo->mutable_dst_addr()->set_type((uint32_t)gtpTunInfoIn->dst_addr.type);
        if (gtpTunInfoIn->dst_addr.type == IPV4_G) {
            str = gtpTunInfoIn->dst_addr.addr;
            gtpTunInfo->mutable_dst_addr()->set_addr(str);
        } else {
            gtpTunInfo->mutable_dst_addr()->set_addr("f020:172:17:32::112");
        }

        gtpTunInfo->mutable_src_addr()->set_type((uint32_t)gtpTunInfoIn->src_addr.type);
        str = gtpTunInfoIn->src_addr.addr;
        gtpTunInfo->mutable_src_addr()->set_addr(str);
        //gtpTunInfo->mutable_src_addr()->set_addr("172.16.36.118");
        if(V1_G == gtpTunInfoIn->interface_type) {
            gtpTunInfo->mutable_src_addr()->set_addr("");
            gtpTunInfo->mutable_dst_addr()->set_addr("");
        }

        gtpTunInfo->set_rem_teid(gtpTunInfoIn->rem_teid);
        gtpTunInfo->set_corr_id(gtpTunInfoIn->corr_id);
        gtpTunInfo->set_ho_src_enodeb(gtpTunInfoIn->ho_src_enodeb);
        gtpTunInfo->set_ho_type(0);
        gtpTunInfo->set_is_fullcfg(gtpTunInfoIn->is_fullCfg);
        gtpTunInfo->set_old_cell_id(gtpTunInfoIn->old_cell_id);
        gtpTunInfo->set_old_crnti(gtpTunInfoIn->old_crnti);

        gtpTunInfo->set_rlc_mode(gtpTunInfoIn->rlc_mode);
        gtpTunInfo->set_qci(gtpTunInfoIn->qci);
        gtpTunInfo->set_end_cbr_type(gtpTunInfoIn->end_cbr_type);
    }

    //CucpEbmCrtTunClient client(getChannel(SERVICE_EBM), getCompletionQueue());

    crtTunClient->Send(aMsg);
    GRPC_LOG0(send_crt_tun_req_out);
    return 0; // may return other failure code if needed
}



int GrpcAdpRpcClientThread::SendModTunReq(CuCpUpHdrG* cp_up_hdr, ModifyTunnelReqG *modTunReq)
{
    srvEbm::ModifyTunnelReq aMsg;
    uint32_t idx;

    ::mhdr::MsgHdr * hdr = aMsg.mutable_hdr();
    ::structure::CpUpHdr *cpUpHdr = aMsg.mutable_cp_up_hdr();
    SetMhdr(cp_up_hdr, hdr);
    SetCucpupHdr(cp_up_hdr, cpUpHdr);
    hdr->set_method(srvEbm::MethodSubmitModifyTunnelReq);
    hdr->set_methodname("MethodSubmitModifyTunnelReq");

    GRPC_LOG0(send_mod_tun_req_in);

    aMsg.set_num_ent(modTunReq->num_ent);
    GtpTunnelInfoG * gtpTunInfoIn;
    for (idx = 0; idx < modTunReq->num_ent; idx++) {
        ::structure::GtpTunnelInfo *gtpTunInfo = aMsg.add_tnl_info_list();
        ::structure::GtpTunnelId *gtpTunID = gtpTunInfo->mutable_tnl_id();
        gtpTunInfoIn = &(modTunReq->tnl_info_list[idx]);

        gtpTunID->set_cell_id(gtpTunInfoIn->tnl_id.cell_id);
        gtpTunID->set_crnti(gtpTunInfoIn->tnl_id.crnti);
        gtpTunID->set_rb_id(gtpTunInfoIn->tnl_id.rb_id);
        gtpTunID->set_tnl_type(GetTunType(gtpTunInfoIn->tnl_id.tnl_type, gtpTunInfoIn->interface_type));
        gtpTunID->set_tnl_priority(gtpTunInfoIn->tnl_id.tnl_priority);
        gtpTunID->set_rb_keep_indicator(gtpTunInfoIn->tnl_id.rb_keep_indicator);

        gtpTunInfo->set_interface_type((uint32_t)gtpTunInfoIn->interface_type);
        gtpTunInfo->set_rem_teid(gtpTunInfoIn->rem_teid);
        gtpTunInfo->set_dscp(gtpTunInfoIn->dscp);
        gtpTunInfo->set_interface_type((uint32_t)gtpTunInfoIn->interface_type);
        gtpTunInfo->set_tunnel_type(GetTunType(gtpTunInfoIn->tnl_id.tnl_type, gtpTunInfoIn->interface_type));
        //set addr
        gtpTunInfo->mutable_dst_addr()->set_type((uint32_t)gtpTunInfoIn->dst_addr.type);
        gtpTunInfo->mutable_dst_addr()->set_addr(gtpTunInfoIn->dst_addr.addr);

        
        gtpTunInfo->mutable_src_addr()->set_type((uint32_t)gtpTunInfoIn->src_addr.type);
        gtpTunInfo->mutable_src_addr()->set_addr(gtpTunInfoIn->src_addr.addr);


        gtpTunInfo->set_rem_teid(gtpTunInfoIn->rem_teid);
        gtpTunInfo->set_corr_id(gtpTunInfoIn->corr_id);
        gtpTunInfo->set_ho_src_enodeb(gtpTunInfoIn->ho_src_enodeb);
        gtpTunInfo->set_ho_type(0);
        gtpTunInfo->set_is_fullcfg(gtpTunInfoIn->is_fullCfg);
        gtpTunInfo->set_old_cell_id(gtpTunInfoIn->old_cell_id);
        gtpTunInfo->set_old_crnti(gtpTunInfoIn->old_crnti);

        gtpTunInfo->set_rlc_mode(gtpTunInfoIn->rlc_mode);
        gtpTunInfo->set_qci(gtpTunInfoIn->qci);
        gtpTunInfo->set_end_cbr_type(gtpTunInfoIn->end_cbr_type);
    }

    //CucpEbmModTunClient client(getChannel(SERVICE_EBM), getCompletionQueue());

    modTunClient->Send(aMsg);
    GRPC_LOG0(send_mod_tun_req_out);
    return 0; // may return other failure code if needed
}


int GrpcAdpRpcClientThread::SendDelTunReq(CuCpUpHdrG* cp_up_hdr, DeleteTunnelReqG *delTunReq)
{
    srvEbm::DeleteTunnelReq aMsg;
    uint32_t idx;

    ::mhdr::MsgHdr * hdr = aMsg.mutable_hdr();
    ::structure::CpUpHdr *cpUpHdr = aMsg.mutable_cp_up_hdr();
    SetMhdr(cp_up_hdr, hdr);
    SetCucpupHdr(cp_up_hdr, cpUpHdr);
    hdr->set_method(srvEbm::MethodSubmitDeleteTunnelReq);
    hdr->set_methodname("MethodSubmitDeleteTunnelReq");
    GRPC_LOG0(send_del_tun_req_in);
    
    aMsg.set_num_ent(delTunReq->num_ent);
    GtpTunnelInfoG * gtpTunInfoIn;
    for (idx = 0; idx < delTunReq->num_ent; idx++) {
        ::structure::GtpTunnelInfo *gtpTunInfo = aMsg.add_tnl_info_list();
        ::structure::GtpTunnelId *gtpTunID = gtpTunInfo->mutable_tnl_id();
        gtpTunInfoIn = &(delTunReq->tnl_info_list[idx]);
        
        gtpTunID->set_cell_id(gtpTunInfoIn->tnl_id.cell_id);
        gtpTunID->set_crnti(gtpTunInfoIn->tnl_id.crnti);
        gtpTunID->set_rb_id(gtpTunInfoIn->tnl_id.rb_id);
        gtpTunID->set_tnl_type(GetTunType(gtpTunInfoIn->tnl_id.tnl_type, gtpTunInfoIn->interface_type));
        gtpTunID->set_tnl_priority(gtpTunInfoIn->tnl_id.tnl_priority);
        gtpTunID->set_rb_keep_indicator(gtpTunInfoIn->tnl_id.rb_keep_indicator);

        gtpTunInfo->set_rem_teid(gtpTunInfoIn->rem_teid);
        gtpTunInfo->set_dscp(gtpTunInfoIn->dscp);
        gtpTunInfo->set_tunnel_type(GetTunType(gtpTunInfoIn->tnl_id.tnl_type, gtpTunInfoIn->interface_type));
        gtpTunInfo->set_interface_type((uint32_t)gtpTunInfoIn->interface_type);
        //set addr
        gtpTunInfo->mutable_dst_addr()->set_type((uint32_t)gtpTunInfoIn->dst_addr.type);
        gtpTunInfo->mutable_dst_addr()->set_addr(gtpTunInfoIn->dst_addr.addr);

        
        gtpTunInfo->mutable_src_addr()->set_type((uint32_t)gtpTunInfoIn->src_addr.type);
        gtpTunInfo->mutable_src_addr()->set_addr(gtpTunInfoIn->src_addr.addr);


        gtpTunInfo->set_rem_teid(gtpTunInfoIn->rem_teid);
        gtpTunInfo->set_corr_id(gtpTunInfoIn->corr_id);
        gtpTunInfo->set_ho_src_enodeb(gtpTunInfoIn->ho_src_enodeb);
        gtpTunInfo->set_ho_type(0);
        gtpTunInfo->set_is_fullcfg(gtpTunInfoIn->is_fullCfg);
        gtpTunInfo->set_old_cell_id(gtpTunInfoIn->old_cell_id);
        gtpTunInfo->set_old_crnti(gtpTunInfoIn->old_crnti);

        gtpTunInfo->set_rlc_mode(gtpTunInfoIn->rlc_mode);
        gtpTunInfo->set_qci(gtpTunInfoIn->qci);
        gtpTunInfo->set_end_cbr_type(gtpTunInfoIn->end_cbr_type);
    }

    //CucpEbmDelTunClient client(getChannel(SERVICE_EBM), getCompletionQueue());

    delTunClient->Send(aMsg);
    GRPC_LOG0(send_del_tun_req_out);
    return 0; // may return other failure code if needed
}

int GrpcAdpRpcClientThread::SendTunnelStateChangeReq(CuCpUpHdrG* cp_up_hdr, TunnelStateInfoG *tunnelStateInfo)
{
    srvEbm::TunnelStateChangeReq aMsg;
    uint32_t idx;

    ::mhdr::MsgHdr * hdr = aMsg.mutable_hdr();
    ::structure::CpUpHdr *cpUpHdr = aMsg.mutable_cp_up_hdr();
    SetMhdr(cp_up_hdr, hdr);
    SetCucpupHdr(cp_up_hdr, cpUpHdr);
    hdr->set_method(srvEbm::MethodSubmitTunnelStateChangeReq);
    hdr->set_methodname("MethodSubmitTunnelStateChangeReq");
    GRPC_LOG0(send_tun_state_change_req_in);

    ::structure::TunnelStateInfo *tunnelStatInfoOut = aMsg.mutable_tunnel_state_info();

    tunnelStatInfoOut->set_state(uint32_t(tunnelStateInfo->state));
    tunnelStatInfoOut->set_cause(tunnelStateInfo->cause);
    tunnelStatInfoOut->set_local_gtp_teid(tunnelStateInfo->local_gtp_teid);
        
    //CucpEbmTunStateChgClient client(getChannel(SERVICE_EBM), getCompletionQueue());

    tunStateChgClient->Send(aMsg);
    GRPC_LOG0(send_tun_state_change_req_out);
    return 0; // may return other failure code if needed
}

int GrpcAdpRpcClientThread::FillAddPdcpCfg(::structure::PDCPAddCfgEnt *pdcpCfgEnt, PDCPAddCfgEntG* pdcpCfgIn)
{
    pdcpCfgEnt->set_rb_id(pdcpCfgIn->rb_id);
    pdcpCfgEnt->set_qci(pdcpCfgIn->qci);
    
    ::structure::RlcInfo * rlcInfo = pdcpCfgEnt->mutable_rlc_info();
    ::structure::HeaderCompressionConfig *headerCms = pdcpCfgEnt->mutable_hdr_cmp_cfg();
    
    rlcInfo->set_direction(pdcpCfgIn->rlc_info.direction);
    rlcInfo->set_mode(pdcpCfgIn->rlc_info.mode);
    rlcInfo->set_status_rep_require(pdcpCfgIn->rlc_info.status_rep_require);
    rlcInfo->set_pdcp_sn_size(pdcpCfgIn->rlc_info.pdcp_sn_size);


    pdcpCfgEnt->set_discard_timer(pdcpCfgIn->discard_timer);
    pdcpCfgEnt->set_re_ord_tmr_val(pdcpCfgIn->re_ord_tmr_val);

    headerCms->set_hdr_comp_used(pdcpCfgIn->hdr_cmp_cfg.hdr_comp_used);
    headerCms->set_max_cid(pdcpCfgIn->hdr_cmp_cfg.max_cid);
    for (int i = 0; i < CPJ_MAX_ROHC_PROF_LIST_G; i++) {
        headerCms->add_profile_list(pdcpCfgIn->hdr_cmp_cfg.profile_list[i]);
    }
}

int GrpcAdpRpcClientThread::SendAddRbCfgReq(CuCpUpHdrG* cp_up_hdr, AddRbConfigReqG *addRbCfg)
{
    srvEbm::AddRbConfigReq aMsg;
    uint32_t idx;

    ::mhdr::MsgHdr * hdr = aMsg.mutable_hdr();
    ::structure::CpUpHdr *cpUpHdr = aMsg.mutable_cp_up_hdr();
    SetMhdr(cp_up_hdr, hdr);
    SetCucpupHdr(cp_up_hdr, cpUpHdr);
    hdr->set_method(srvEbm::MethodSubmitAddRbConfigReq);
    hdr->set_methodname("MethodSubmitAddRbConfigReq");
    GRPC_LOG0(send_add_rb_req_in);

    aMsg.set_is_ho(addRbCfg->is_ho);
    aMsg.set_rlc_ul_sap_id(addRbCfg->rlc_ul_sap_id);
    aMsg.set_rlc_dl_sap_id(addRbCfg->rlc_dl_sap_id);
    
    ::structure::PDCPAddCfgEnt *addCfgEnt;

    PDCPAddCfgEntG* pdcpCfgIn;
    for (idx = 0; idx < addRbCfg->num_ent; idx++) {
        pdcpCfgIn = &(addRbCfg->config_list[idx]);
        addCfgEnt = aMsg.add_config_list();
        FillAddPdcpCfg(addCfgEnt, pdcpCfgIn);
    }
    aMsg.set_num_ent(idx);

    //CucpEbmAddRbCfgClient client(getChannel(SERVICE_EBM), getCompletionQueue());

    addRbCfgClient->Send(aMsg);
    GRPC_LOG0(send_add_rb_req_out);
    return 0; // may return other failure code if needed
}


int GrpcAdpRpcClientThread::FillModPdcpCfg(::structure::PDCPModCfgEnt *pdcpCfgEnt, PDCPModCfgEntG* pdcpCfgIn)
{
    ::structure::HeaderCompressionConfig *headerCms = pdcpCfgEnt->mutable_hdr_cmp_cfg();
    ::structure::PDCPHoInfo * hoInfo = pdcpCfgEnt->mutable_ho_info();
    ::structure::PDCPDCBearerInfo *bearerInfo = pdcpCfgEnt->mutable_dc_bearer_info();

    pdcpCfgEnt->set_rb_id(pdcpCfgIn->rb_id);
    pdcpCfgEnt->set_bit_flag(pdcpCfgIn->bit_flag);
    pdcpCfgEnt->set_re_ord_tmr_val(pdcpCfgIn->re_ord_tmr_val);

    headerCms->set_hdr_comp_used(pdcpCfgIn->hdr_cmp_cfg.hdr_comp_used);
    headerCms->set_max_cid(pdcpCfgIn->hdr_cmp_cfg.max_cid);
    for (int i = 0; i < CPJ_MAX_ROHC_PROF_LIST_G; i++) {
        headerCms->add_profile_list(pdcpCfgIn->hdr_cmp_cfg.profile_list[i]);
    }

    hoInfo->set_is_ho_pres((uint32_t)pdcpCfgIn->ho_info.is_ho_pres);
    hoInfo->set_sn_len(pdcpCfgIn->ho_info.sn_len);
    hoInfo->set_dl_count(pdcpCfgIn->ho_info.dl_count);
    hoInfo->set_ul_count(pdcpCfgIn->ho_info.ul_count);
    hoInfo->set_num_bits(pdcpCfgIn->ho_info.num_bits);
    for (int i = 0; i < PDCP_HO_BIT_MAP_LEN; i++) {
        hoInfo->add_bit_map(pdcpCfgIn->ho_info.bit_map[i]);
    }

    bearerInfo->set_sn_len(pdcpCfgIn->dc_bearer_info.sn_len);
    bearerInfo->set_bearer_type(pdcpCfgIn->dc_bearer_info.bearer_type);
    bearerInfo->set_split_ratio(pdcpCfgIn->dc_bearer_info.split_ratio);
}

int GrpcAdpRpcClientThread::SendModRbCfgReq(CuCpUpHdrG* cp_up_hdr, ModifyRbConfigReqG *modRbCfg)
{
    srvEbm::ModifyRbConfigReq aMsg;
    uint32_t idx;

    ::mhdr::MsgHdr * hdr = aMsg.mutable_hdr();
    ::structure::CpUpHdr *cpUpHdr = aMsg.mutable_cp_up_hdr();
    SetMhdr(cp_up_hdr, hdr);
    SetCucpupHdr(cp_up_hdr, cpUpHdr);
    hdr->set_method(srvEbm::MethodSubmitModifyRbConfigReq);
    hdr->set_methodname("MethodSubmitModifyRbConfigReq");
    GRPC_LOG0(send_mod_rb_req_in);
    
    ::structure::PDCPModCfgEnt *modCfgEnt;

    PDCPModCfgEntG* pdcpCfgIn;
    for (idx = 0; idx < modRbCfg->num_ent; idx++) {
        pdcpCfgIn = &(modRbCfg->config_list[idx]);
        modCfgEnt = aMsg.add_config_list();
        FillModPdcpCfg(modCfgEnt, pdcpCfgIn);
    }
    aMsg.set_num_ent(idx);

    //CucpEbmModRbCfgClient client(getChannel(SERVICE_EBM), getCompletionQueue());

    modRbCfgClient->Send(aMsg);
    GRPC_LOG0(send_mod_rb_req_out);

    return 0; // may return other failure code if needed
}

int GrpcAdpRpcClientThread::FillDelPdcpCfg(::structure::PDCPDelCfgEnt *pdcpCfgEnt, PDCPDelCfgEntG* pdcpCfgIn)
{
    pdcpCfgEnt->set_rb_id(pdcpCfgIn->rb_id);
}

int GrpcAdpRpcClientThread::SendDelRbCfgReq(CuCpUpHdrG* cp_up_hdr, DeleteRbConfigReqG *delRbCfg)
{
    srvEbm::DeleteRbConfigReq aMsg;
    uint32_t idx;

    ::mhdr::MsgHdr * hdr = aMsg.mutable_hdr();
    ::structure::CpUpHdr *cpUpHdr = aMsg.mutable_cp_up_hdr();
    SetMhdr(cp_up_hdr, hdr);
    SetCucpupHdr(cp_up_hdr, cpUpHdr);
    hdr->set_method(srvEbm::MethodSubmitDeleteRbConfigReq);
    hdr->set_methodname("MethodSubmitDeleteRbConfigReq");
    GRPC_LOG0(send_del_rb_req_in);
    
    ::structure::PDCPDelCfgEnt *delCfgEnt;

    PDCPDelCfgEntG* pdcpCfgIn;
    for (idx = 0; idx < delRbCfg->num_ent; idx++) {
        pdcpCfgIn = &(delRbCfg->config_list[idx]);
        delCfgEnt = aMsg.add_config_list();
        FillDelPdcpCfg(delCfgEnt, pdcpCfgIn);
    }
    aMsg.set_num_ent(idx);

    //CucpEbmDelRbCfgClient client(getChannel(SERVICE_EBM), getCompletionQueue());

    delRbCfgClient->Send(aMsg);
    GRPC_LOG0(send_del_rb_req_out);
    return 0; // may return other failure code if needed
}



int  GrpcAdpRpcClientThread::SendPauseResumeDataReq(CuCpUpHdrG* cp_up_hdr, PauseResumeDataReqG *pauseResumeReq)
{
    srvEbm::PauseResumeDataReq aMsg;

    ::mhdr::MsgHdr * hdr = aMsg.mutable_hdr();
    ::structure::CpUpHdr *cpUpHdr = aMsg.mutable_cp_up_hdr();
    SetMhdr(cp_up_hdr, hdr);
    SetCucpupHdr(cp_up_hdr, cpUpHdr);
    hdr->set_method(srvEbm::MethodSubmitPauseResumeDataReq);
    hdr->set_methodname("MethodSubmitPauseResumeDataReq");
    GRPC_LOG0(send_data_pause_resume_req_in);
    
    aMsg.set_tgt_cell_id(pauseResumeReq->tgt_cell_id);
    aMsg.set_new_rnti(pauseResumeReq->new_rnti);
    aMsg.set_is_pause(pauseResumeReq->is_pause);
    aMsg.set_ho_type((uint32_t)pauseResumeReq->ho_type);

    //CucpEbmPauseResumeClient client(getChannel(SERVICE_EBM), getCompletionQueue());

    pauseResumeClient->Send(aMsg);
    GRPC_LOG0(send_data_pause_resume_req_out);
    return 0; // may return other failure code if needed
}

int  GrpcAdpRpcClientThread::SendPdcpSduStatusReq(CuCpUpHdrG* cp_up_hdr)
{
    srvEbm::PDCPSduStatusReq aMsg;

    ::mhdr::MsgHdr * hdr = aMsg.mutable_hdr();
    ::structure::CpUpHdr *cpUpHdr = aMsg.mutable_cp_up_hdr();
    SetMhdr(cp_up_hdr, hdr);
    SetCucpupHdr(cp_up_hdr, cpUpHdr);
    hdr->set_method(srvEbm::MethodSubmitPDCPSduStatusReq);
    hdr->set_methodname("MethodSubmitPDCPSduStatusReq");
    GRPC_LOG0(send_sdu_status_req_in);

    //CucpEbmPdcpSduStatusClient client(getChannel(SERVICE_EBM), getCompletionQueue());

    pdcpSduStatusClient->Send(aMsg);
    GRPC_LOG0(send_sdu_status_req_out);
    return 0; // may return other failure code if needed
}

int  GrpcAdpRpcClientThread::SendDataForwardingReq(CuCpUpHdrG* cp_up_hdr, DataForwardingReqG* req)
{
    srvEbm::DataForwardingReq aMsg;

    ::mhdr::MsgHdr * hdr = aMsg.mutable_hdr();
    ::structure::CpUpHdr *cpUpHdr = aMsg.mutable_cp_up_hdr();
    SetMhdr(cp_up_hdr, hdr);
    SetCucpupHdr(cp_up_hdr, cpUpHdr);
    hdr->set_method(srvEbm::MethodSubmitDataForwardingReq);
    hdr->set_methodname("MethodSubmitDataForwardingReq");
    GRPC_LOG0(send_data_forward_req_in);
    
    aMsg.set_is_ul_data_fwd_req(req->is_ul_data_fwd_Req);
    aMsg.set_handover_type((uint32_t)req->handover_type);

    //CucpEbmDataFwdClient client(getChannel(SERVICE_EBM), getCompletionQueue());

    dataFwdClient->Send(aMsg);
    GRPC_LOG0(send_data_forward_req_out);
    return 0; // may return other failure code if needed
}

int  GrpcAdpRpcClientThread::SendUeInactivityTimerReq(CuCpUpHdrG* cp_up_hdr)
{
    srvEbm::UeInactivityTimerReq aMsg;

    ::mhdr::MsgHdr * hdr = aMsg.mutable_hdr();
    ::structure::CpUpHdr *cpUpHdr = aMsg.mutable_cp_up_hdr();
    SetMhdr(cp_up_hdr, hdr);
    SetCucpupHdr(cp_up_hdr, cpUpHdr);
    hdr->set_method(srvEbm::MethodSubmitUeInactivityTimerReq);
    hdr->set_methodname("MethodSubmitUeInactivityTimerReq");
    GRPC_LOG0(send_ue_inactive_timer_req_in);

    //CucpEbmUeInactTimerClient client(getChannel(SERVICE_EBM), getCompletionQueue());

    ueInactTimerClient->Send(aMsg);
    GRPC_LOG0(send_ue_inactive_timer_req_out);
    return 0; // may return other failure code if needed
}

int  GrpcAdpRpcClientThread::SendAddPdcpCfgReq(CuCpUpHdrG* cp_up_hdr,AddPDCPConfigReqG *pdcp_cfg)
{
    srvEbm::AddPDCPConfigReq aMsg;

    ::mhdr::MsgHdr * hdr = aMsg.mutable_hdr();
    ::structure::CpUpHdr *cpUpHdr = aMsg.mutable_cp_up_hdr();
    SetMhdr(cp_up_hdr, hdr);
    SetCucpupHdr(cp_up_hdr, cpUpHdr);
    hdr->set_method(srvEbm::MethodSubmitAddPDCPConfigReq);
    hdr->set_methodname("MethodSubmitAddPDCPConfigReq");
    GRPC_LOG0(send_add_pdcp_req_in);

    aMsg.set_is_ho(pdcp_cfg->is_ho);
    
    ::structure::SecurityConfig *secCfgOut = aMsg.mutable_security_config();
    secCfgOut->set_algorithm_type(pdcp_cfg->secCfg.algorithm_type);
    for (int i=0; i < CPJ_CIPHER_KEY_LEN_G; i++) {
        secCfgOut->add_encryption_key(pdcp_cfg->secCfg.encryption_key[i]);
    }

    aMsg.set_num_ent(pdcp_cfg->num_ent);
    aMsg.set_rlc_ul_sap_id(pdcp_cfg->rlc_ul_sap_id);
    aMsg.set_rlc_dl_sap_id(pdcp_cfg->rlc_dl_sap_id);

    ::structure::PDCPAddCfgEnt *pdcpAddEnt;
    PDCPAddCfgEntG* pdcpCfgIn;

    for (int i=0; i < pdcp_cfg->num_ent; i++) {
        pdcpCfgIn = &(pdcp_cfg->config_list[i]);
        pdcpAddEnt = aMsg.add_config_list();
        FillAddPdcpCfg(pdcpAddEnt, pdcpCfgIn);
    }

    CucpEbmAddPdcpCfgClient client(getChannel(SERVICE_EBM), getCompletionQueue());

    addPdcpCfgClient->Send(aMsg);
    GRPC_LOG0(send_add_pdcp_req_out);
    return 0; // may return other failure code if needed
}

int  GrpcAdpRpcClientThread::SendModPdcpCfgReq(CuCpUpHdrG* cp_up_hdr, ModPDCPConfigReqG *pdcp_cfg)
{
    srvEbm::ModifyPDCPConfigReq aMsg;

    ::mhdr::MsgHdr * hdr = aMsg.mutable_hdr();
    ::structure::CpUpHdr *cpUpHdr = aMsg.mutable_cp_up_hdr();
    SetMhdr(cp_up_hdr, hdr);
    SetCucpupHdr(cp_up_hdr, cpUpHdr);
    hdr->set_method(srvEbm::MethodSubmitModifyPDCPConfigReq);
    hdr->set_methodname("MethodSubmitModifyPDCPConfigReq");
    GRPC_LOG0(send_mod_pdcp_req_in);

    aMsg.set_is_ho(pdcp_cfg->is_ho);
    
    ::structure::SecurityConfig *secCfgOut = aMsg.mutable_security_config();
    secCfgOut->set_algorithm_type(pdcp_cfg->secCfg.algorithm_type);
    for (int i=0; i < CPJ_CIPHER_KEY_LEN_G; i++) {
        secCfgOut->add_encryption_key(pdcp_cfg->secCfg.encryption_key[i]);
    }

    aMsg.set_num_ent(pdcp_cfg->num_ent);
    aMsg.set_new_cp_ue_id(pdcp_cfg->new_cp_ue_id);
    aMsg.set_new_cell_id(pdcp_cfg->new_cell_id);

    ::structure::PDCPModCfgEnt *pdcpModEnt;
    PDCPModCfgEntG* pdcpCfgIn;

    for (int i=0; i < pdcp_cfg->num_ent; i++) {
        pdcpCfgIn = &(pdcp_cfg->config_list[i]);
        pdcpModEnt = aMsg.add_config_list();
        FillModPdcpCfg(pdcpModEnt, pdcpCfgIn);
    }

    CucpEbmModPdcpCfgClient client(getChannel(SERVICE_EBM), getCompletionQueue());

    modPdcpCfgClient->Send(aMsg);
    GRPC_LOG0(send_mod_pdcp_req_out);
    return 0; // may return other failure code if needed
}

void GrpcAdpRpcClientThread::on_exit()
{
    ccLog("");
    mThreadInstId = 0;
    mThreadReady = false;
}

void GrpcAdpRpcClientThread::run()
{
    StartThread();
}

void GrpcAdpRpcClientThread::ResponseHandler(ServiceClientCallData* aClientCallData)
{
    ccLog("Rpc response handler");
    switch (aClientCallData->getRpcType())
    {
        case SEC_CFG_REQ: 
        {
            //TODO
            ccLog("get response, %d", aClientCallData->getRpcType());
        }
        break;
        default:
            ccLog("Unsupported type by now, %d", aClientCallData->getRpcType());
    }
}


