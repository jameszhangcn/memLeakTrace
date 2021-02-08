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
* File Name: CUUPLibCntrl.cpp
*
* Subsystem: libCUUP
*
* Description: This file defines the RPC client for libCUUP
*
* Revision History:
* Date        Name         Description
* ----        ----         -----------
* 10/19/2020  Fisher       Initial Release
*
**********************************************************************/
#include "GrpcAdpCntrl.h"
#include "GrpcAdpThreads.h"
#include "SvcLog.h"
#include "GrpcLog.h"
#include "ServiceCommonType.h"

char grpcCntrlLogBuf[80];

namespace mav
{
    GrpcAdpCntrl* GrpcAdpCntrl::mInstance = nullptr;
    int GrpcAdpCntrl::mInstId = 0;

    GrpcAdpCntrl::GrpcAdpCntrl():
        mThreadNum(1),
        mThreadInstanceIdGen(1)
    {
        
    }
   
    GrpcAdpCntrl* GrpcAdpCntrl::getInstance()
    {
        if (!mInstance)
        {
            mInstance = new GrpcAdpCntrl();
        }
        return mInstance;
    }
   
    int GrpcAdpCntrl::initGrpcServerConfig()
    {
        //get the grpc port config from  
        std::string cucpGrpcIP = "0.0.0.0";
        std::string cucpGrpcPort = "9021";
        getGrpcServerThread().serverAddr() = cucpGrpcIP + ":" + cucpGrpcPort;
        GRPC_LOG0(server_inited);
        return 0;

    }

    int GrpcAdpCntrl::initGrpcClientConfig()
    {
        //get the grpc port config from  
        std::string ebmGrpcIP = "0.0.0.0";
        std::string ebmGrpcPort = "9027";
        getGrpcClientThread().ebmServerTarget() = ebmGrpcIP + ":" + ebmGrpcPort;

        //init the mutex
        if (pthread_mutex_init(&this->mClientMutex, NULL) != 0){
            GRPC_LOG0(mutex_inited_failed);
        }

        return 0;

    }

    int GrpcAdpCntrl::startGrpcServerThread(unsigned char& aThreadInstanceId)
    {
        unsigned char threadInstanceId = mThreadInstanceIdGen++;
        
        mRpcServerThread.init(threadInstanceId);
        aThreadInstanceId = threadInstanceId;
        mRpcServerThread.run();
        GRPC_LOG0(server_started);
        return 0;
    }


    int GrpcAdpCntrl::startGrpcClientThread(unsigned char& aThreadInstanceId)
    {
        unsigned char threadInstanceId = mThreadInstanceIdGen++;

        mRpcClientThread.init(threadInstanceId);
        aThreadInstanceId = threadInstanceId;
        mRpcClientThread.run();
        GRPC_LOG0(client_started);
        return 0;
    }

    int GrpcAdpCntrl::createWorkerThreads()
    {
        #if 0
        int ret;
        pthread_t th;
        int arg = 10;
        mWorkerInstance.push_back(nullptr);
        for(int i = 1; i <= 2; i++)
        {
            unsigned char threadInstancedId = mThreadInstanceIdGen;
            mThreadInstanceIdGen++;
            GrpcWorker *theWorker = new GrpcWorker(threadInstancedId);

            ret = pthread_create( &th, NULL, grpcServer, &arg);
            if (ret != 0){
                GRPC_LOG0(client_started);
                return -1;
            }
        }
        #endif
        return 0;
    }

    bool GrpcAdpCntrl::init()
    {
        unsigned char threadNum = mThreadInstanceIdGen;
        
        createWorkerThreads();
        initGrpcServerConfig();
        startGrpcServerThread(threadNum);
        initGrpcClientConfig();
        startGrpcClientThread(threadNum);
        return true;
    }

    void GrpcAdpCntrl::client_lock(int clientId)
    {
        if (pthread_mutex_lock(&this->mClientMutex) != 0){
            GRPC_LOG0(client_mutex_lock_failed);
        }
        return;
    }

    void GrpcAdpCntrl::client_unlock(int clientId)
    {
        if (pthread_mutex_unlock(&this->mClientMutex) != 0){
            GRPC_LOG0(client_mutex_unlock_failed);
        }
        return;
    }


    bool GrpcAdpCntrl::init_client(uint32_t up_id, uint32_t cp_id, IpAddressG* addr, uint32_t port)
    {
        //TODO: multi client instance
        GRPC_LOG0(client_inited);
        return initGrpcClientConfig();
    }

    bool GrpcAdpCntrl::start_client(uint32_t up_id, uint32_t cp_id)
    {
        unsigned char threadNum;
        return startGrpcClientThread(threadNum);
    }

    bool GrpcAdpCntrl::deinit()
    {
        GrpcAdpThreads::getInstance()->killRpcServerThread();
        GrpcAdpThreads::getInstance()->killRpcClientThread();
         if (pthread_mutex_destroy(&this->mClientMutex) != 0){
            GRPC_LOG0(mutex_deinited_failed);
        }
        return true;
    }

    void GrpcAdpCntrl::join_threads()
    {
        GrpcAdpThreads::getInstance()->joinRpcServerThread();
    }
} // namespace mav

