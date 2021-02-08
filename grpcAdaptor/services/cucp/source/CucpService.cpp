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
* File Name: CucpService.cpp
*
* Subsystem: Cucp
*
* Description: This file defines the service layer for Iwf
*
* Revision History:
* Date        Name         Description
* ----        ----         -----------
* 11/04/2020  JianZhang       Initial Release
*
**********************************************************************/
#include "SvcLog.h"
#include "CucpService.h"

CucpServer::~CucpServer()
{
}

void CucpServer::OnInit()
{
    ccLog("CucpServer:: OnInit");
    AddServiceHandler(SEC_CFG_CFM);
    AddServiceHandler(CRT_TUN_CFM);
    AddServiceHandler(MOD_TUN_CFM);
    AddServiceHandler(DEL_TUN_CFM);
    AddServiceHandler(ADD_RB_CFG_CFM);
    AddServiceHandler(MOD_RB_CFG_CFM);
    AddServiceHandler(DEL_RB_CFG_CFM);
    AddServiceHandler(PDCP_SDU_STAT_CFM);
    AddServiceHandler(ADD_PDCP_CFG_CFM);
    AddServiceHandler(MOD_PDCP_CFG_CFM);
}

grpc::Service* CucpServer::getService()
{
    return &mService;
}

ServiceServerCallData* CucpServer::allocCallData(int aServiceType)
{
    ccLog("CucpServer::allocCallData %d ", aServiceType);
    switch(aServiceType)
    {
    case SEC_CFG_CFM:
        ccLog("CucpServer::EbmSecCfgCfmCallData  ");
        return new EbmSecCfgCfmCallData();
    case CRT_TUN_CFM:
        ccLog("CucpServer::EbmCrtTunCfmCallData  ");
        return new EbmCrtTunCfmCallData();
    case MOD_TUN_CFM:
        ccLog("CucpServer::EbmModTunCfmCallData  ");
        return new EbmModTunCfmCallData();
    case DEL_TUN_CFM:
        ccLog("CucpServer::EbmDelTunCfmCallData  ");
        return new EbmDelTunCfmCallData();
    case ADD_RB_CFG_CFM:
        ccLog("CucpServer::EbmAddRbCfgCfmCallData  ");
        return new EbmAddRbCfgCfmCallData();
    case MOD_RB_CFG_CFM:
        ccLog("CucpServer::EbmModRbCfgCfmCallData  ");
        return new EbmModRbCfgCfmCallData();
    case DEL_RB_CFG_CFM:
        ccLog("CucpServer::EbmDelRbCfgCfmCallData  ");
        return new EbmDelRbCfgCfmCallData();
    case PDCP_SDU_STAT_CFM:
        ccLog("CucpServer::EbmPdcpSduStatCfmCallData  ");
        return new EbmPdcpSduStatusCfmCallData();
    case ADD_PDCP_CFG_CFM:
        ccLog("CucpServer::EbmAddPdcpCfgCfmCallData  ");
        return new EbmAddPdcpCfgCfmCallData();
    case MOD_PDCP_CFG_CFM:
        ccLog("CucpServer::EbmModPdcpCfgCfmCallData  ");
        return new EbmModPdcpCfgCfmCallData();
    default:
        ccLog("Unknown service type %d", aServiceType);
        return nullptr;
    }
}

void CucpServer::deallocCallData(ServiceServerCallData* aCD)
{
    delete aCD;
}

void CucpServer::OnMessage(ServiceServerCallData& aCD)
{
    switch(aCD.getRpcType())
    {
        case SEC_CFG_CFM:
            OnSecCfgCfm(dynamic_cast<SecurityConfigCfm&>(*aCD.getMessage()));
            break;
        case CRT_TUN_CFM:
            OnCrtTunCfm(dynamic_cast<CreateTunnelCfm&>(*aCD.getMessage()));
            break;

        case MOD_TUN_CFM:
            OnModTunCfm(dynamic_cast<ModifyTunnelCfm&>(*aCD.getMessage()));
            break;

        case DEL_TUN_CFM:
            OnDelTunCfm(dynamic_cast<DeleteTunnelCfm&>(*aCD.getMessage()));
            break;

        case ADD_RB_CFG_CFM:
            OnAddRbCfgCfm(dynamic_cast<AddRbConfigCfm&>(*aCD.getMessage()));
            break;

        case MOD_RB_CFG_CFM:
            OnModRbCfgCfm(dynamic_cast<ModifyRbConfigCfm&>(*aCD.getMessage()));
            break;

        case DEL_RB_CFG_CFM:
            OnDelRbCfgCfm(dynamic_cast<DeleteRbConfigCfm&>(*aCD.getMessage()));
            break;

        case PDCP_SDU_STAT_CFM:
            OnPdcpSduStatCfm(dynamic_cast<PDCPSduStatusCfm&>(*aCD.getMessage()));
            break;

        case ADD_PDCP_CFG_CFM:
            OnAddPdcpCfgCfm(dynamic_cast<AddPDCPConfigCfm&>(*aCD.getMessage()));
            break;

        case MOD_PDCP_CFG_CFM:
            OnModPdcpCfgCfm(dynamic_cast<ModifyPDCPConfigCfm&>(*aCD.getMessage()));
            break;

        default:
            ccWarning("Unknown service type %d", aCD.getRpcType());
            break;
    }
}
