
#pragma once

#include <iostream>
#include <sys/syscall.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <execinfo.h>
#include <sstream> 

namespace tt {

static pid_t
GetThreadId() { return syscall(SYS_gettid); }

static void 
Backtrace(std::vector<std::string>& bt, int size, int skip) {
    void** array = (void**)malloc((sizeof(void*) * size));
    size_t s = ::backtrace(array, size);

    char** strings = backtrace_symbols(array, s);
    if(strings == NULL) {
		std::cout << "backtrace_synbols error";
        return;
    }

    for(size_t i = skip; i < s; ++i) {
        bt.push_back(strings[i]);
    }

    free(strings);
    free(array);
}

static std::string 
BacktraceToString(int size, int skip, const std::string& prefix) {
    std::vector<std::string> bt;
    Backtrace(bt, size, skip);
    std::stringstream ss;
    for(size_t i = 0; i < bt.size(); ++i) {
        ss << prefix << bt[i] << std::endl;
    }
    return ss.str();
}

}
