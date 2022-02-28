#include "Mutex.h" 
namespace tt {
namespace system {

Mutex::Mutex() { 
	pthread_mutex_init(&m_mutex, nullptr);

}



Mutex::~Mutex() {
	pthread_mutex_destroy(&m_mutex);

}


} // namespace system
} // namespace tt
