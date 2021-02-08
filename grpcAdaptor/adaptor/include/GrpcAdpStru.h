#ifndef __GRPC_ADP_STRU_H
#define __GRPC_ADP_STRU_H

#include <stdbool.h>

typedef unsigned char           uint8_t;  
typedef unsigned short int      uint16_t;  
#ifndef __uint32_t_defined  
typedef unsigned int            uint32_t;  
# define __uint32_t_defined  
#endif  

#ifndef __uint64_t_defined  
typedef unsigned long            uint64_t;  
# define __uint64_t_defined  
#endif  

#define INVALID_ID_G 0xFFFFFFFF
typedef enum 
{
    IPV4_G = 0,
    IPV6_G = 1
}IpAddressTypeG;

#define IP_ADDR_LEN_G 20
typedef struct IpAddressG {
    IpAddressTypeG type;
    char addr[IP_ADDR_LEN_G];
}IpAddressG;

typedef struct pstG
{
    uint32_t  dstProcID;
    uint32_t  srcProcID;
    uint32_t  dstEnt;
    uint32_t  dstInst;
    uint32_t  srcEnt;
    uint32_t  srcInst;
    uint32_t  prior;
    uint32_t  route;
    uint32_t  event;
    uint32_t  region;
    uint32_t  pool;
    uint32_t  selector;
    uint32_t  interfaceVersion;
    uint32_t  cellID;
}PstG;

typedef struct cuCpIDG
{
	uint32_t cpId;
	uint32_t cellId;
	uint32_t cpUeId;
} CuCpIDG;

typedef struct cuUpIDG
{
	uint32_t upId;
	uint32_t cellId;
	uint64_t upUeId;
} CuUpIDG;

typedef struct suSpIDG
{
	uint32_t suId;
	uint32_t spId;
} SuSpIDG;

typedef struct cuCpUpHdrG
{
	PstG cuCpPst;
	CuCpIDG cuCpId;
	CuUpIDG cuUpId;
	SuSpIDG suSpId;
    uint32_t transId;
} CuCpUpHdrG;



typedef enum 
{
      WR_TNL_NORMAL_G = 0,
      WR_TNL_DL_DATFWD_G = 1,
      WR_TNL_UL_DATFWD_G = 2,
      WR_TNL_LIPA_G = 3,
      WR_TNL_ENDC_OFFLD_G = 4,
      WR_TNL_ENDC_MCGBR_OFFLD_G = 5,
      WR_TNL_ENDC_SCGBR_OFFLD_G = 6
}TunnelTypeG;

typedef enum {
    INTREFACE_NONE_G = 0,
    S1_G = 1,
    V1_G = 2,
    X2_G = 3
}InterfaceTypeG;

typedef enum {
    HO_TYPE_NONE_G = 0,
    S1_BASED_G = 1,
    X2_BASED_G = 2,
    INTRA_BBU_G = 3,
    INTRA_DU_BBU_G = 4,
    INTER_DU_BBU_G = 5
}HandoverTypeG;

#define CPJ_CIPHER_KEY_LEN_G 16

typedef struct SecurityConfigureG {
    uint32_t  algorithm_type;
    uint32_t encryption_key[CPJ_CIPHER_KEY_LEN_G] ;
}SecurityConfigureG;

typedef struct SecurityConfigReqG{
    bool is_ho;
    SecurityConfigureG security_config;
}SecurityConfigReqG;

typedef struct GtpTunnelIdG {
    uint32_t  cell_id;
    uint32_t  crnti;
    uint32_t  rb_id;
    TunnelTypeG  tnl_type;
    uint32_t  tnl_priority ;
    uint32_t  rb_keep_indicator;
}GtpTunnelIdG;

typedef struct GtpTunnelInfoG {
    GtpTunnelIdG tnl_id;
    uint32_t dscp;
    TunnelTypeG tunnel_type;
    InterfaceTypeG interface_type;
    IpAddressG   dst_addr;
    IpAddressG   src_addr;
    uint32_t  rem_teid;
    uint32_t  lcl_teid;
    uint32_t  corr_id;
    bool    ho_src_enodeb;
    HandoverTypeG  ho_type;
    bool    is_fullCfg;
    uint32_t old_cell_id;
    uint32_t old_crnti;
    uint32_t  rlc_mode;
    uint32_t  qci;
    uint32_t  end_cbr_type;
}GtpTunnelInfoG;

#define MAX_TUN_LIST_SIZE_G (10)
typedef struct CreateTunnelReqG {
    uint32_t num_ent;
    GtpTunnelInfoG tnl_info_list[MAX_TUN_LIST_SIZE_G];
}CreateTunnelReqG;

typedef struct ModifyTunnelReqG {
    uint32_t num_ent;
    GtpTunnelInfoG tnl_info_list[MAX_TUN_LIST_SIZE_G];
}ModifyTunnelReqG;

typedef struct DeleteTunnelReqG {
    uint32_t num_ent;
    GtpTunnelInfoG tnl_info_list[MAX_TUN_LIST_SIZE_G];
}DeleteTunnelReqG;

typedef enum {
    WR_DAM_DRB_STATE_NORMAL_G = 1,
    WR_DAM_DRB_STATE_BUFFERIN_G = 2,
    WR_DAM_TNL_DL_FWDSTATE_NEW_G = 3
}TUNNEL_STATE_ENUM_G;

typedef struct TunnelStateInfoG {
    TUNNEL_STATE_ENUM_G state;
    uint32_t cause;
    uint32_t local_gtp_teid;
}TunnelStateInfoG;

typedef struct TunnelStateChangeReqG {
    TunnelStateInfoG state_info;
}TunnelStateChangeReqG;

typedef struct RlcInfoG {
    uint32_t  direction;
    uint32_t  mode;
    bool    status_rep_require;
    uint32_t  pdcp_sn_size;
}RlcInfoG;

#define CPJ_MAX_ROHC_PROF_LIST_G   9           /*!< ROHC profile list */
typedef struct HeaderCompressConfigureG {
    bool    hdr_comp_used;
    uint32_t  max_cid;
    uint32_t profile_list[CPJ_MAX_ROHC_PROF_LIST_G];
}HeaderCompressConfigureG;


typedef enum {
    CFG_ADD_G = 0,
    CFG_MODIFY_G = 1,
    CFG_DELETE_G = 2,
    REESTABLISH_G = 3,
    CFG_DELETE_UE_G = 4
}PDCPConfigTypeG;

typedef enum  {
    HO_NOT_PRSNT_G = 0,
    HO_UL_G = 1,
    HO_DL_G = 2
}PDCPHoPresG;

typedef enum {
    REAS_NONE_G = 0,
    ENT_CREAT_FAIL_G = 1,
    INVALID_RLC_MODE_G = 5,
    CIPHER_ALGO_INVALID_G = 8,
    ENT_DEL_FAIL_G = 9,
    UE_DEL_FAIL_G = 10,
    REEST_FAIL_G = 11,
    COMP_ALGO_INVALID_G = 12,
    SAME_UEID_G = 15,
    SAME_RBID_G = 16,
    INVALID_RBID_G = 17,
    REAS_BUTT_G = 18
}PDCPConfigFailureReasonG;

#define PDCP_HO_BIT_MAP_LEN (8)
typedef struct PDCPHoInfoG {
    PDCPHoPresG  is_ho_pres;
    uint32_t  sn_len;
    uint32_t  dl_count;
    uint32_t  ul_count;
    uint32_t  num_bits;
    uint32_t  bit_map[PDCP_HO_BIT_MAP_LEN];
}PDCPHoInfoG;

typedef struct PDCPDCBearerInfoG {
    uint32_t  sn_len;
    uint32_t  bearer_type;
    uint32_t  split_ratio;
}PDCPDCBearerInfoG;

typedef struct PDCPAddCfgEntG {
    uint32_t  rb_id;
    uint32_t  qci;
    RlcInfoG rlc_info;
    uint32_t  discard_timer;
    uint32_t  re_ord_tmr_val;
    HeaderCompressConfigureG    hdr_cmp_cfg;
}PDCPAddCfgEntG;

typedef struct PDCPModCfgEntG {
    uint32_t  rb_id;
    uint32_t  bit_flag;
    HeaderCompressConfigureG    hdr_cmp_cfg;
    PDCPHoInfoG  ho_info;
    uint32_t  re_ord_tmr_val;
    PDCPDCBearerInfoG    dc_bearer_info;
}PDCPModCfgEntG;

typedef struct PDCPDelCfgEntG {
    uint32_t rb_id;
}PDCPDelCfgEntG;

#define CPJ_MAX_CFG_G    10                    /*!< Maximum PDCP entity to be configured */ 

typedef struct AddRbConfigReqG {
    uint32_t num_ent;
    uint32_t is_ho;
    uint32_t rlc_ul_sap_id;
    uint32_t rlc_dl_sap_id;
    PDCPAddCfgEntG config_list[CPJ_MAX_CFG_G];
}AddRbConfigReqG;

typedef struct ModifyRbConfigReqG {
    uint32_t num_ent;
    PDCPModCfgEntG config_list[CPJ_MAX_CFG_G];
}ModifyRbConfigReqG;


typedef struct DeleteRbConfigReqG {
    uint32_t num_ent;
    PDCPDelCfgEntG config_list[CPJ_MAX_CFG_G];
}DeleteRbConfigReqG;



typedef struct PauseResumeDataReqG {
    uint32_t tgt_cell_id;
    uint32_t new_rnti;
    bool is_pause;
    HandoverTypeG ho_type;
}PauseResumeDataReqG;

typedef struct PDCPSduStatusReqG {
}PDCPSduStatusReqG;

typedef struct DataForwardingReqG {
    bool is_ul_data_fwd_Req;
    HandoverTypeG handover_type;
}DataForwardingReqG;

typedef struct AddPDCPConfigReqG {
    bool is_ho;
    SecurityConfigureG secCfg;
    uint32_t num_ent;
    uint32_t rlc_ul_sap_id;
    uint32_t rlc_dl_sap_id;
    PDCPAddCfgEntG config_list[CPJ_MAX_CFG_G];
}AddPDCPConfigReqG;

typedef struct ModPDCPConfigReqG {
    bool is_ho;
    SecurityConfigureG secCfg;
    uint32_t num_ent;
    uint32_t new_cp_ue_id;
    uint32_t new_cell_id;
    PDCPModCfgEntG config_list[CPJ_MAX_CFG_G];
}ModPDCPConfigReqG;

//below for the cfm

#define MAX_REASON_LEN_G (16)

typedef struct SecurityConfigCfmInfoG {
    bool status;
    char reason[MAX_REASON_LEN_G];
}SecurityConfigCfmInfoG;

typedef struct SecurityConfigCfmG {
    CuCpUpHdrG cp_up_hdr;
    SecurityConfigCfmInfoG sec_cfm;
}SecurityConfigureCfmG;

typedef struct CreateTnlCfmEntG {
    InterfaceTypeG interface_type;
    GtpTunnelIdG gtp_tunnel_id;
    bool         has_addr;
    IpAddressG   dst_addr;
    IpAddressG   src_addr;
    uint32_t  local_gtp_teid;
    uint32_t  remote_gtp_teid;
    uint32_t  old_cell_id;
    uint32_t  old_crnti;
    bool status;
}CreateTnlCfmEntG;

typedef struct CreateTunnelCfmG {
    CuCpUpHdrG cp_up_hdr;
    uint32_t num_ent;
    CreateTnlCfmEntG tnl_cfm_list[MAX_TUN_LIST_SIZE_G];
}CreateTunnelCfmG;

typedef struct ModTnlCfmEntG {
    uint32_t  old_cell_id;
    uint32_t  old_crnti;
    InterfaceTypeG interface_type;
    GtpTunnelIdG gtp_tunnel_id;
    bool         has_addr;
    IpAddressG   dst_addr;
    IpAddressG   src_addr;
    uint32_t  local_gtp_teid;
    uint32_t  remote_gtp_teid;
    bool status;
}ModTnlCfmEntG;

typedef struct ModifyTunnelCfmG {
    CuCpUpHdrG cp_up_hdr;
    InterfaceTypeG interface_type;
    uint32_t num_ent;
    bool is_tunnel_sw;
    ModTnlCfmEntG tnl_cfm_list[MAX_TUN_LIST_SIZE_G];
}ModifyTunnelCfmG;

typedef struct DelTnlCfmEntG {
    InterfaceTypeG interface_type;
    GtpTunnelIdG gtp_tunnel_id;
    IpAddressG   dst_addr;
    IpAddressG   src_addr;
    uint32_t  local_gtp_teid;
    uint32_t  remote_gtp_teid;
    bool status;
}DelTnlCfmEntG;

typedef struct DeleteTunnelCfmG {
    CuCpUpHdrG cp_up_hdr;
    uint32_t num_ent;
    DelTnlCfmEntG tnl_cfm_list[MAX_TUN_LIST_SIZE_G];
}DeleteTunnelCfmG;

#define CPJ_MAX_CFM_G (10)

typedef struct PDCPAddCfgCfmEntG {
    uint32_t rb_id;
    bool status;
    char reason[MAX_REASON_LEN_G];
}PDCPAddCfgCfmEntG;

typedef struct AddRbConfigCfmG {
    CuCpUpHdrG cp_up_hdr;
    uint32_t num_ent;
    PDCPAddCfgCfmEntG config_cfm_ent[CPJ_MAX_CFG_G];
}AddRbConfigCfmG;

typedef struct PDCPCfgCfmG {
    CuCpUpHdrG cp_up_hdr;
    SecurityConfigCfmInfoG sec_cfm;
    uint32_t num_ent;
    PDCPAddCfgCfmEntG config_cfm_ent[CPJ_MAX_CFG_G];
}PDCPCfgCfmG;

typedef struct PDCPModCfgCfmEntG {
    uint32_t rb_id;
    bool status;
    char reason[MAX_REASON_LEN_G];
}PDCPModCfgCfmEntG;

typedef struct ModifyRbConfigCfmG {
    CuCpUpHdrG cp_up_hdr;
    uint32_t num_ent;
    PDCPModCfgCfmEntG config_cfm_ent[CPJ_MAX_CFG_G];
}ModifyRbConfigCfmG;

typedef struct PDCPDelCfgCfmEntG {
    uint32_t rb_id;
    bool status;
    char reason[MAX_REASON_LEN_G];
}PDCPDelCfgCfmEntG;

typedef struct DeleteRbConfigCfmG {
    CuCpUpHdrG cp_up_hdr;
    uint32_t num_ent;
    PDCPDelCfgCfmEntG config_cfm_ent[CPJ_MAX_CFG_G];
}DeleteRbConfigCfmG;

typedef struct UlSduStaInfoG {
    uint32_t numBits;
    //U8 *ulBitMap;
    uint32_t count;
}UlSduStaInfoG;

typedef struct DlSduStaInfoG {
    uint32_t count;
}DlSduStaInfoG;

typedef struct SduStatusInfoG {
    uint32_t rbId;
    uint32_t dir;
    uint32_t snLen;
    UlSduStaInfoG ulSduStaInfo;
    DlSduStaInfoG dlSduStaInfo;
}SduStatusInfoG;

#define CPJ_MAX_DRB_G (10)

typedef struct PDCPSduStatusCfmG {
    CuCpUpHdrG cp_up_hdr;
    uint32_t  num_rb;
    SduStatusInfoG sdu_status_info[CPJ_MAX_DRB_G];
    bool    status;
    PDCPConfigFailureReasonG  reason;
}PDCPSduStatusCfmG;

#endif
