#include "Log.h"
#include "sv_rpcapi.h"
#include <unistd.h>

#include <thread>

int main() 
{

	tt::profession::ServerRpcApi sra;
	sra.GetReqpool();
	sra.run();
	while(1)
	{
		sleep(1);
	}
	return 0;
}

