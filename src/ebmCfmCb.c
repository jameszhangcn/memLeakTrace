#include "../include/EbmCfmCb.h"
#include "stdio.h"
#include "../include/GrpcLog.h"


void grpc_log0(uint32_t log_id)
{
    printf(" \n GRPC %s", LOG_DESCRIBE(grpc_log,log_id));
}

void grpc_log1(uint32_t log_id, uint32_t para)
{
    printf("log1");
}

void grpc_log2(uint32_t log_id, uint32_t para_1, uint32_t para_2)
{
    printf("log2");
}

void grpc_log_str(unsigned char* logContent)
{
    printf("log2");
}

void update_route_table(CuCpUpHdrG* cpUpHdr)
{
    printf("log2");
}

void send_sec_cfg_cfm(SecurityConfigCfmG* secCfgCfm)
{
    printf("\n send_sec_cfg_cfm");
}
void send_dam_tun_crt_cfm(CreateTunnelCfmG* tunCrtCfm)
{
    printf("log2");
}

void send_v1u_tun_crt_cfm(CreateTunnelCfmG* tunCrtCfm)
{
    printf("log2");
}
void send_add_rb_cfg_cfm(AddRbConfigCfmG* AddRbCfgCfm)
{
    printf("log2");
}

void send_dam_tun_del_cfm(DeleteTunnelCfmG* tunDelCfm)
{
    printf("log2");
}

void send_dam_tun_mod_cfm(ModifyTunnelCfmG* tunModCfm)
{
    printf("log2");
}

void send_v1u_ue_cell_switch_cfm(ModifyTunnelCfmG* tunModCfm)
{
    printf("log2");
}

void send_v1u_tun_del_cfm(DeleteTunnelCfmG* tunDelCfm)
{
    printf("log2");
}

void send_mod_rb_cfg_cfm(ModifyRbConfigCfmG* AddRbCfgCfm)
{
    printf("log2");
}

void send_del_rb_cfg_cfm(DeleteRbConfigCfmG* DelRbCfgCfm)
{
    printf("log2");
}

void send_pdcp_sdu_stat_cfm(PDCPSduStatusCfmG* pdcpSduStatusCfm)
{
    printf("log2");
}

