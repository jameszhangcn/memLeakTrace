#include "pthread.h"
#include <stdio.h>
#include <unistd.h>
#include <sched.h>

#include "../include/GrpcAdp.h"
#include "../include/EbmCfmCb.h"

#include <unordered_map>
#include <string>

std::unordered_map <std::string,std::string> ebmMap;

void add_ebm_map(char* ebmID, char* ipPort)
{
    ebmMap[ebmID] = ipPort;
}

char* get_ebm_ip_port(char* ebmID)
{
    return const_cast<char*>(ebmMap[ebmID].data());
}

void *grpcClient(void *arg){
    int ret;
    pid_t pid;
    pthread_t tid;
    printf("This is grpc client thread!");
    *(int*)arg = 0;
    SecurityConfigReqG cfg_req;
    CuCpUpHdrG cp_up_hdr;

    cfg_req.is_ho = false;
    cfg_req.security_config.algorithm_type = 1234;
    for (int i=0; i < CPJ_CIPHER_KEY_LEN_G; i++) {
        cfg_req.security_config.encryption_key[i] = i;

    }

    pid = getpid();
    tid = pthread_self();

    while(1){
        sleep(1);
        //printf("In client thread pid %x tid %x \n", pid, tid);
        //send msg to EBM
        ret =  send_sec_cfg_req(&cp_up_hdr, &cfg_req);
        //ret = send_sec_cfg_req_m("CP001", tid, "EBM001", &cp_up_hdr, &secCfg);
        //printf("send to EBM ret %d pid %x tid %x \n", ret, pid, tid);
    }
    return arg;
}

#define MAX_CLIENT_THREAD_NUM (5)

int main(int argc, char *argv[])
{
    pthread_t th[MAX_CLIENT_THREAD_NUM];
    int ret;
    int idx;
    int arg = 10;
    int *thread_ret[MAX_CLIENT_THREAD_NUM] = {NULL};

    int cpu_num;

    cpu_num = sysconf(_SC_NPROCESSORS_ONLN);
    printf("cpu_num %d \n", cpu_num);

    cpu_set_t mask;

    CPU_ZERO(&mask);
    CPU_SET(cpu_num-1, &mask);

    if (pthread_setaffinity_np(pthread_self(),sizeof(mask), &mask) < 0) {
        printf("set affinity failed 0");
    }

    printf("This is the main process.\n");
    start_grpc_server();

    for (idx = 0; idx < MAX_CLIENT_THREAD_NUM; idx++) {
        ret = pthread_create( &th[idx], NULL, grpcClient, &arg);
        if (ret != 0){
            printf("Create grpcClient thread error!\n");
            return -1;
        }
    }

    for (idx = 0; idx < MAX_CLIENT_THREAD_NUM; idx++) {
        pthread_join(th[idx], (void**)&thread_ret[idx]);
        printf("thread_ret = %d.\n", * thread_ret[idx]);
    }


    return 0;
}
