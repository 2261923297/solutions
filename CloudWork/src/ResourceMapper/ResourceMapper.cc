#include "ResourceMapper.h" 

namespace tt {  

namespace  FileResource {

std::map<std::string, std::function<Worker*()> > Mapper::s_worker_mapper;
void User::reset_name(const std::string& name) { 
	m_name = name; 

	if(m_name == "root") {
		m_permission = Permission::Value::SUP;
	} else if(m_name == "unknown") {
		m_permission = Permission::Value::READ_ONLY;
	} else {
		m_permission = Permission::Value::COM;
	}

}

#undef xx
#define xx(worker) \
	int worker::working(const std::string& url, std::string& ret) { \
		TT_DEBUG << #worker;												 \
		ret = "Add " + url;													 \
		return 0;															 \
	}																		 \
//
xx(AddWorker)
xx(DelWorker)
xx(UpdateWorker)
xx(ListWorker)
xx(GetWorker)
xx(ErrorWorker)


#undef xx // xx(worker)
void Manager::schedul(const std::string& command) {
	size_t cmd_end =  command.find(" ");
	std::string cmd = command.substr(0, cmd_end);  
	std::string url = command.substr(cmd_end + 1, command.size() - cmd_end - 1);

	init_url(url);
	std::string work_result = "cant^t mapping working";
	mapping_worker(cmd);

	m_worker->working(url, work_result);
	
	TT_DEBUG << "cmd: " << cmd 
		<< ", url: " << url 
		<< ", ret: " << work_result 
		<< std::endl;
	m_worker.reset();
}

void Manager::init_url(std::string& url) {
	url = m_mapper->s_root_dir + m_user->name() + url;
}

void Manager::mapping_worker(const std::string& cmd) {
	TT_DEBUG << "mapping " << cmd << "!";
	auto it = m_mapper->s_worker_mapper.find(cmd);
	Worker* worker = nullptr;
	if(it != m_mapper->s_worker_mapper.end()) {
		worker = it->second();
	}
	TT_DEBUG << "end finding!";

	Worker::ptr w_ptr(nullptr);
	if(worker != nullptr)
		w_ptr = Worker::ptr(worker);
	else {
		w_ptr = Worker::ptr(new ErrorWorker);
		TT_DEBUG << "error_cmd";
	}

	m_worker = w_ptr;
}


Manager::Manager(const std::string user_name) {
	m_user.reset(new User("unknow"));
	m_mapper.reset(new Mapper);
	m_worker.reset();
}

void Manager::InitWorkerMapper() {
	ADD_WORKER_MAPPER(Add, Worker)
	ADD_WORKER_MAPPER(Del, Worker)
	ADD_WORKER_MAPPER(Get, Worker)
	ADD_WORKER_MAPPER(List, Worker)
	ADD_WORKER_MAPPER(Update, Worker)
	std::string ret = "/test";
	for(auto mapper : Mapper::s_worker_mapper) {
		TT_DEBUG << mapper.first;
		Worker::ptr worker(mapper.second());
		worker->working("test", ret);
	}
}
} // namespace FileResource
} // namespace tt
