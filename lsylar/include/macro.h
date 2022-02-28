#include "util.h"
#include "Log.h"
#include <assert.h>


#define TT_ASSERT(x)  \
	if(!(x)) {      \
		TT_DEBUG << "ASSERTION:" << #x \
		<< "\nbacktrace:\n" \
		<< tt::system::BacktraceToString(100, 2, "    "); \
		assert(x); \
	}				 \
//



