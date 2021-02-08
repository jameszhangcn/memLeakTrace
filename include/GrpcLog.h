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
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint16_t id;
    char     *desc;
}LOG_DESCRIBE_COMM_TYPE;

extern char * get_desc_by_id(LOG_DESCRIBE_COMM_TYPE *desc,unsigned id);

#define LOG_DESCRIBE_DECLARE_BEGIN(type) const static LOG_DESCRIBE_COMM_TYPE  LOG_describe_desc_##type[]={ 
#define LOG_DESCRIBE_DECLARE_END  {0xffff,"end"} };
#define LOG_DESCRIBE_ADD_DESC(id,str) {id,#str},  
//#define LOG_DESCRIBE(type,id) get_desc_by_id(LOG_describe_desc_##type,id)  
#define LOG_DESCRIBE(type,id) (id >= log_enum_butt)?"Unknown log id...":LOG_describe_desc_##type[id].desc  

typedef enum{
    server_inited,
    server_started,
    server_exited,
    client_inited,
    client_started,
    mutex_inited_failed,
    mutex_deinited_failed,
    client_mutex_lock_failed,
    client_mutex_unlock_failed,
    send_sec_cfg_req_in,
    send_sec_cfg_req_out,
    send_crt_tun_req_in,
    send_crt_tun_req_out,
    send_mod_tun_req_in,
    send_mod_tun_req_out,
    send_del_tun_req_in,
    send_del_tun_req_out,
    send_tun_state_change_req_in,
    send_tun_state_change_req_out,
    send_add_rb_req_in,
    send_add_rb_req_out,
    send_mod_rb_req_in,
    send_mod_rb_req_out,
    send_del_rb_req_in,
    send_del_rb_req_out,
    send_data_pause_resume_req_in,
    send_data_pause_resume_req_out,
    send_sdu_status_req_in,
    send_sdu_status_req_out,
    send_data_forward_req_in,
    send_data_forward_req_out,
    send_ue_inactive_timer_req_in,
    send_ue_inactive_timer_req_out,
    send_add_pdcp_req_in,
    send_add_pdcp_req_out,
    send_mod_pdcp_req_in,
    send_mod_pdcp_req_out,

    recv_sec_cfg_cfm_in,
    recv_sec_cfg_cfm_out,
    recv_crt_tun_cfm_in,
    recv_crt_tun_cfm_loop,
    recv_crt_tun_cfm_out,
    recv_mod_tun_cfm_in,
    recv_mod_tun_cfm_loop,
    recv_mod_tun_cfm_out,
    recv_del_tun_cfm_in,
    recv_del_tun_cfm_loop,
    recv_del_tun_cfm_out,
    recv_tun_state_change_cfm_in,
    recv_tun_state_change_cfm_out,
    recv_add_rb_cfm_in,
    recv_add_rb_cfm_loop,
    recv_add_rb_cfm_out,
    recv_mod_rb_cfm_in,
    recv_mod_rb_cfm_loop,
    recv_mod_rb_cfm_out,
    recv_del_rb_cfm_in,
    recv_del_rb_cfm_loop,
    recv_del_rb_cfm_out,
    recv_data_pause_resume_cfm_in,
    recv_data_pause_resume_cfm_out,
    recv_sdu_status_cfm_in,
    recv_sdu_status_cfm_out,
    recv_data_forward_cfm_in,
    recv_data_forward_cfm_out,
    recv_ue_inactive_timer_cfm_in,
    recv_ue_inactive_timer_cfm_out,
    recv_add_pdcp_cfm_in,
    recv_add_pdcp_cfm_out,
    recv_mod_pdcp_cfm_in,
    recv_mod_pdcp_cfm_out,

    log_enum_butt
}grpc_log_enum;

LOG_DESCRIBE_DECLARE_BEGIN(grpc_log)
LOG_DESCRIBE_ADD_DESC(server_inited,          	        "server_inited"          	       )
LOG_DESCRIBE_ADD_DESC(server_started, 				    "server_started" 				   )
LOG_DESCRIBE_ADD_DESC(server_exited, 				    "server_exited"				       )
LOG_DESCRIBE_ADD_DESC(client_inited, 				    "client_inited" 				   )
LOG_DESCRIBE_ADD_DESC(client_started,                   "client_started"                   )
LOG_DESCRIBE_ADD_DESC(mutex_inited_failed, 			    "mutex_inited_failed" 			   )
LOG_DESCRIBE_ADD_DESC(mutex_deinited_failed, 		    "mutex_deinited_failed" 		   )
LOG_DESCRIBE_ADD_DESC(client_mutex_lock_failed,	        "client_mutex_lock_failed"	       )
LOG_DESCRIBE_ADD_DESC(client_mutex_unlock_failed,       "client_mutex_unlock_failed"       )
LOG_DESCRIBE_ADD_DESC(send_sec_cfg_req_in,              "send_sec_cfg_req_in"              )
LOG_DESCRIBE_ADD_DESC(send_sec_cfg_req_out,             "send_sec_cfg_req_out"             )
LOG_DESCRIBE_ADD_DESC(send_crt_tun_req_in,              "send_crt_tun_req_in"              )
LOG_DESCRIBE_ADD_DESC(send_crt_tun_req_out,             "send_crt_tun_req_out"             )
LOG_DESCRIBE_ADD_DESC(send_mod_tun_req_in,              "send_mod_tun_req_in"              )
LOG_DESCRIBE_ADD_DESC(send_mod_tun_req_out,             "send_mod_tun_req_out"             )
LOG_DESCRIBE_ADD_DESC(send_del_tun_req_in,              "send_del_tun_req_in"              )
LOG_DESCRIBE_ADD_DESC(send_del_tun_req_out,             "send_del_tun_req_out"             )
LOG_DESCRIBE_ADD_DESC(send_tun_state_change_req_in,     "send_tun_state_change_req_in"     )
LOG_DESCRIBE_ADD_DESC(send_tun_state_change_req_out,    "send_tun_state_change_req_out"    )
LOG_DESCRIBE_ADD_DESC(send_add_rb_req_in,               "send_add_rb_req_in"               )
LOG_DESCRIBE_ADD_DESC(send_add_rb_req_out,              "send_add_rb_req_out"              )
LOG_DESCRIBE_ADD_DESC(send_mod_rb_req_in,               "send_mod_rb_req_in"               )
LOG_DESCRIBE_ADD_DESC(send_mod_rb_req_out,              "send_mod_rb_req_out"              )
LOG_DESCRIBE_ADD_DESC(send_del_rb_req_in,               "send_del_rb_req_in"               )
LOG_DESCRIBE_ADD_DESC(send_del_rb_req_out,              "send_del_rb_req_out"              )
LOG_DESCRIBE_ADD_DESC(send_data_pause_resume_req_in,    "send_data_pause_resume_req_in"    )
LOG_DESCRIBE_ADD_DESC(send_data_pause_resume_req_out,   "send_data_pause_resume_req_out"   )
LOG_DESCRIBE_ADD_DESC(send_sdu_status_req_in,           "send_sdu_status_req_in"           )
LOG_DESCRIBE_ADD_DESC(send_sdu_status_req_out,          "send_sdu_status_req_out"          )
LOG_DESCRIBE_ADD_DESC(send_data_forward_req_in,         "send_data_forward_req_in"         )
LOG_DESCRIBE_ADD_DESC(send_data_forward_req_out,        "send_data_forward_req_out"        )
LOG_DESCRIBE_ADD_DESC(send_ue_inactive_timer_req_in,    "send_ue_inactive_timer_req_in"    )
LOG_DESCRIBE_ADD_DESC(send_ue_inactive_timer_req_out,   "send_ue_inactive_timer_req_out"   )
LOG_DESCRIBE_ADD_DESC(send_add_pdcp_req_in,             "send_add_pdcp_req_in"             )
LOG_DESCRIBE_ADD_DESC(send_add_pdcp_req_out,            "send_add_pdcp_req_out"            )
LOG_DESCRIBE_ADD_DESC(send_mod_pdcp_req_in,             "send_mod_pdcp_req_in"             )
LOG_DESCRIBE_ADD_DESC(send_mod_pdcp_req_out,            "send_mod_pdcp_req_out"            )
LOG_DESCRIBE_ADD_DESC(recv_sec_cfg_cfm_in,              "recv_sec_cfg_cfm_in"              )
LOG_DESCRIBE_ADD_DESC(recv_sec_cfg_cfm_out,             "recv_sec_cfg_cfm_out"             )
LOG_DESCRIBE_ADD_DESC(recv_crt_tun_cfm_in,              "recv_crt_tun_cfm_in"              )
LOG_DESCRIBE_ADD_DESC(recv_crt_tun_cfm_loop,            "recv_crt_tun_cfm_loop"            )
LOG_DESCRIBE_ADD_DESC(recv_crt_tun_cfm_out,             "recv_crt_tun_cfm_out"             )
LOG_DESCRIBE_ADD_DESC(recv_mod_tun_cfm_in,              "recv_mod_tun_cfm_in"              )
LOG_DESCRIBE_ADD_DESC(recv_mod_tun_cfm_loop,            "recv_mod_tun_cfm_loop"            )
LOG_DESCRIBE_ADD_DESC(recv_mod_tun_cfm_out,             "recv_mod_tun_cfm_out"             )
LOG_DESCRIBE_ADD_DESC(recv_del_tun_cfm_in,              "recv_del_tun_cfm_in"              )
LOG_DESCRIBE_ADD_DESC(recv_del_tun_cfm_loop,            "recv_del_tun_cfm_loop"            )
LOG_DESCRIBE_ADD_DESC(recv_del_tun_cfm_out,             "recv_del_tun_cfm_out"             )
LOG_DESCRIBE_ADD_DESC(recv_tun_state_change_cfm_in,     "recv_tun_state_change_cfm_in"     )
LOG_DESCRIBE_ADD_DESC(recv_tun_state_change_cfm_out,    "recv_tun_state_change_cfm_out"    )
LOG_DESCRIBE_ADD_DESC(recv_add_rb_cfm_in,               "recv_add_rb_cfm_in"               )
LOG_DESCRIBE_ADD_DESC(recv_add_rb_cfm_loop,             "recv_add_rb_cfm_loop"             )
LOG_DESCRIBE_ADD_DESC(recv_add_rb_cfm_out,              "recv_add_rb_cfm_out"              )
LOG_DESCRIBE_ADD_DESC(recv_mod_rb_cfm_in,               "recv_mod_rb_cfm_in"               )
LOG_DESCRIBE_ADD_DESC(recv_mod_rb_cfm_loop,             "recv_mod_rb_cfm_loop"             )
LOG_DESCRIBE_ADD_DESC(recv_mod_rb_cfm_out,              "recv_mod_rb_cfm_out"              )
LOG_DESCRIBE_ADD_DESC(recv_del_rb_cfm_in,               "recv_del_rb_cfm_in"               )
LOG_DESCRIBE_ADD_DESC(recv_del_rb_cfm_loop,             "recv_del_rb_cfm_loop"             )
LOG_DESCRIBE_ADD_DESC(recv_del_rb_cfm_out,              "recv_del_rb_cfm_out"              )
LOG_DESCRIBE_ADD_DESC(recv_data_pause_resume_cfm_in,    "recv_data_pause_resume_cfm_in"    )
LOG_DESCRIBE_ADD_DESC(recv_data_pause_resume_cfm_out,   "recv_data_pause_resume_cfm_out"   )
LOG_DESCRIBE_ADD_DESC(recv_sdu_status_cfm_in,           "recv_sdu_status_cfm_in"           )
LOG_DESCRIBE_ADD_DESC(recv_sdu_status_cfm_out,          "recv_sdu_status_cfm_out"          )
LOG_DESCRIBE_ADD_DESC(recv_data_forward_cfm_in,         "recv_data_forward_cfm_in"         )
LOG_DESCRIBE_ADD_DESC(recv_data_forward_cfm_out,        "recv_data_forward_cfm_out"        )
LOG_DESCRIBE_ADD_DESC(recv_ue_inactive_timer_cfm_in,    "recv_ue_inactive_timer_cfm_in"    )
LOG_DESCRIBE_ADD_DESC(recv_ue_inactive_timer_cfm_out,   "recv_ue_inactive_timer_cfm_out"   )
LOG_DESCRIBE_ADD_DESC(recv_add_pdcp_cfm_in,             "recv_add_pdcp_cfm_in"             )
LOG_DESCRIBE_ADD_DESC(recv_add_pdcp_cfm_out,            "recv_add_pdcp_cfm_out"            )
LOG_DESCRIBE_ADD_DESC(recv_mod_pdcp_cfm_in,             "recv_mod_pdcp_cfm_in"             )
LOG_DESCRIBE_ADD_DESC(recv_mod_pdcp_cfm_out,            "recv_mod_pdcp_cfm_out"            )

LOG_DESCRIBE_DECLARE_END

#ifdef __cplusplus
}
#endif