#pragma once
#include "servers.h"
#include "reqpool.h"

#include <mutex>


namespace tt
{
namespace profession
{
class ServerRpcApi
{
public:
	ServerRpcApi();
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
