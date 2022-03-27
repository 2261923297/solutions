#include "Log.h"
    
void test_shared_ptr() {
	

//	LOG_DEBUG(logger) << "Hello, ttlog!";
//	LOG_DEBUG(logger) << "test_line_2";
	
	tt::system::Event::ptr e(
    new tt::system::Event(
        logger_system->getName()
        , LogLevel::Level::DEBUG
        , __LINE__
        , __FILE__
        , __FUNCTION__));
//	tt::system::EventVar(logger, e).getSS() << "test log_msg";
//	logger->log(e);
//	LOG_LEVEL(logger, LogLevel::Level::DEBUG) << "test_log";
	DEBUG_SYS << "test_log";
}
void test_in_run() {




}
    
int main() {
	DEBUG_SYS << "test tt_debug";
    std::cout << "Hello, test Log base!" << std::endl;
    test_shared_ptr();
    
    return 0;
}
    
