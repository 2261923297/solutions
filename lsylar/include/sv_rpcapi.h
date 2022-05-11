#pragma once
#include "servers.h"
#include "reqpool.h"

#include <mutex>

#define LOCAL_IP    "192.168.90.1"
#define LISTEN_IP	LOCAL_IP
#define LISTEN_PORT_START 9900
#define LISTEN_PORT_NUM	  10

#define REQ_IP		LOCAL_IP
#define REQ_PORT	(9999)	

#define CONNECT_MAX		  1024 * 1024 
#define SR_DEBUG_OPEN 0

namespace tt
{
namespace profession
{
class ServerRpcApi
{
public:

	ServerRpcApi();
	~ServerRpcApi() { this->destory(); }

	bool init();
	bool destory();
	bool run();

	static asynreq_context* GetReqpool();
public:
	static asynreq_context* s_reqpool;
private:
	reactor_t* m_reactor;

}; // class ServerRcpcApi

/* 类没有申请内存一次，那么类的静态成员不会申请内存*/
asynreq_context* ServerRpcApi::s_reqpool
	= (asynreq_context*)malloc(sizeof(asynreq_context));
} // namespace profession
} // namespace tt

