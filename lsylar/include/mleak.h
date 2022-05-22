

#pragma once



#include "File.h"
#include "Log.h"        // sp_file

#include <stdlib.h>
#include <malloc.h>
#include <string>
#include <sstream>

#ifndef __MALLOC_HOOK_ERRMSG
#define __MALLOC_HOOK_ERRMSG                \
    ((sp_file(__FILE__) + " > "             \
    + __FUNCTION__  + ":"                   \
    + std::to_string(__LINE__)).c_str())    \
//
#endif // __MALLOC_HOOK_ERRMSG


#ifndef __MEM_DIR
#define __MEM_DIR "memcheck/mleak/"
#endif // __MEM_DIR

#ifndef __PROJECT_NAME
#define __PROJECT_NAME "default"
#endif // __PROJECT_NAME

#define __MEM_LOG_DIR __MEM_DIR __PROJECT_NAME "/"

static void write_check_log(void* ptr, const char* desc, bool is_malloc)
{
    char mf_signal = is_malloc ? '+' : '-';
    if(ptr != nullptr) 
    {
        std::stringstream log;
        std::stringstream _f_name;
        _f_name << ptr << ".mem";

        log << ptr << " "
            << "[ " << mf_signal << " ] --> "
            << desc << std::endl;
        std::string mem_log_dir = __MEM_LOG_DIR;
        std::string log_file_name = mem_log_dir + _f_name.str();
        if(is_malloc) {
            tt::system::FileManager<tt::system::LinuxDirMaker>::mkdirs(mem_log_dir);
            tt::system::Entry fentry(log_file_name.c_str());
            fentry.reopen("w");
            fentry.write(log.str().c_str(), log.str().size());
            fentry.close();
        } else {
            if(0 != remove(log_file_name.c_str()))
            { 
                ERROR_SYS << "remove file: "
                        << (mem_log_dir + _f_name.str()).c_str()
                        << ", " << strerror(errno);
            }
        }
        TT_DEBUG << log.str();
    }
    else 
    {

    }
}

static void* malloc_hook(size_t size, const char* desc)
{
    void* rt = malloc(size);
    write_check_log(rt, desc, true);
    return rt;
}

static void free_hook(void* p, const char* desc)
{
    if(p != nullptr) {
        free(p);
        write_check_log(p, desc, false);
    }
}

#ifdef __ENABLE_MEMLEAK_CHECK__
    #define malloc(size) malloc_hook(size, __MALLOC_HOOK_ERRMSG)
    #define free(addr) free_hook(addr, __MALLOC_HOOK_ERRMSG)
#endif // __ENABLE_MEMLEAK_CHECK__





