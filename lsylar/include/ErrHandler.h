#pragma once

#include <string>
#include <memory>
#include <functional>

class ErrHandler {
public:
    using ptr = std::shared_ptr<ErrHandler>;
    ErrHandler(void* caller) {
        m_caller = caller;
    }
    virtual ~ErrHandler() { }

    void handle_error(
              const std::string& desc
            , std::function<void()> do_err = [](){}
            , bool need_handle = false) {
        std::cout << desc << std::endl;
        if(need_handle) {
            do_err();
        }
    }
private:
    void* m_caller;
}; // class ErrHandler


