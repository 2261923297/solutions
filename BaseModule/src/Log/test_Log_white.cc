#include "Log.h"
    
void test_shared_ptr() {
	
	ttlog::Logger::ptr logger = ttlog::Logger::ptr(new ttlog::Logger("root"));
	ttlog::Formatter::ptr f(new ttlog::Formatter("/l/tab/fn/s{::}/ln/s{>>}/m"));
	ttlog::Appandar::ptr ap = ttlog::Appandar::ptr(new ttlog::StdoutAppandar);

	logger->addAppandar(ap);
	logger->setFormatter(f);
//	LOG_DEBUG(logger) << "Hello, ttlog!";
//	LOG_DEBUG(logger) << "test_line_2";
	
	ttlog::Event::ptr e(new ttlog::Event(logger->getName(), LogLevel::Level::DEBUG, __LINE__, __FILE__));
//	ttlog::EventVar(logger, e).getSS() << "test log_msg";
//	logger->log(e);
//	LOG_LEVEL(logger, LogLevel::Level::DEBUG) << "test_log";
	LOG_DEBUG(logger) << "test_log";
}
    
int main() {
    std::cout << "Hello, test Log base!" << std::endl;
    test_shared_ptr();
    
    return 0;
}
    
