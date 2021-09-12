#include "util.h"
#include "Log.h"
#include <assert.h>


#define TT_DEBUG LOG_DEBUG(g_logger)
#define TT_ASSERT(x)  \
	if(!(x)) {      \
		TT_DEBUG << "ASSERTION:" << #x \
		<< "\nbacktrace:\n" \
		<< tt::BacktraceToString(100, 2, "    "); \
		assert(x); \
	}				 \
//



