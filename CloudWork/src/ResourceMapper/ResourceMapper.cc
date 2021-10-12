#include "ResourceMapper.h" 

namespace tt {  

namespace  FileResource {

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

void  Manager::add(const std::string& url) {
	TT_DEBUG << "add";	
}
void  Manager::del(const std::string& url) {
	TT_DEBUG << "del";	

}
void  Manager::get(const std::string& url) {
	TT_DEBUG << "del";	

}
void  Manager::list(const std::string& url) {
	TT_DEBUG << "del";	

}
void  Manager::reset(const std::string& url) {
	TT_DEBUG << "del";	

}

Manager::Manager(const std::string user_name) {
	m_user.reset(new User("unknow"));
	m_mapper.reset(new Mapper);
}


}

} // namespace tt
