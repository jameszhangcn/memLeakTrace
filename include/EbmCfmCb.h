#ifndef EBM_CFM_CB_H
#define EBM_CFM_CB_H


#ifdef __cplusplus
extern "C" {
#endif

#include "GrpcAdpStru.h"

#define EVTCVZTUNNELCREATEREQ_G       4 /*!< Tunnel create request */
#define EVTCVZTNLCELLSWITCHREQ_G      10

#define    EVTDAMTNLMODREQ_G          20  /* DAM  Tunnel Modify Req */
#define    EVTDAMTNLSWTCHREQ_G        26  /* DAM Switch TNL */


#define ENTVZU_G      0xef             /* V1-U Interface for V1AP */
#define ENTEU_G       0xd4            /* EGTP USER */


extern void grpc_log0(uint32_t log_id);

extern void grpc_log1(uint32_t log_id, uint32_t para);

extern void grpc_log2(uint32_t log_id, uint32_t para_1, uint32_t para_2);

extern void grpc_log_str(unsigned char* logContent);

extern void update_route_table(CuCpUpHdrG* cpUpHdr);

extern void send_sec_cfg_cfm(SecurityConfigCfmG* secCfgCfm);

extern void send_dam_tun_crt_cfm(CreateTunnelCfmG* tunCrtCfm);

extern void send_v1u_tun_crt_cfm(CreateTunnelCfmG* tunCrtCfm);

extern void send_add_rb_cfg_cfm(AddRbConfigCfmG* AddRbCfgCfm);

extern void send_dam_tun_del_cfm(DeleteTunnelCfmG* tunDelCfm);

extern void send_dam_tun_mod_cfm(ModifyTunnelCfmG* tunModCfm);

extern void send_v1u_ue_cell_switch_cfm(ModifyTunnelCfmG* tunModCfm);

extern void send_v1u_tun_del_cfm(DeleteTunnelCfmG* tunDelCfm);

extern void send_mod_rb_cfg_cfm(ModifyRbConfigCfmG* AddRbCfgCfm);

extern void send_del_rb_cfg_cfm(DeleteRbConfigCfmG* DelRbCfgCfm);

extern void send_pdcp_sdu_stat_cfm(PDCPSduStatusCfmG* pdcpSduStatusCfm);


#ifdef __cplusplus
}
#endif

#endif