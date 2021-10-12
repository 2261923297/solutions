#ifndef __FILE_RESOURCE__
#define __FILE_RESOURCE__

#include <iostream>
#include <memory>
#include <string>
#include <map>
#include <functional>
#include "Log.h"
namespace tt { 

namespace FileResource {

#define ENUM_CLASS_NAME Permission
#define ENUM_NAME Value
#define ENUM_BODY \
	ENUM_MESSAGE(Value, READ_ONLY, COM, SUP) \
//	
#define ENUM_VALUES \
	xx(Permission::Value::READ_ONLY) \
	xx(Permission::Value::COM) \
	xx(Permission::Value::SUP) \
//

#include "EnumReflection.h"

class Worker {
public:
	using ptr = std::shared_ptr<Worker>;
	virtual int 
	working(const std::string& url, std::string& ret) = 0;

private:

}; // class Worker

class AddWorker : public Worker {
public:
	int working(const std::string& url, std::string& ret) override;
};

class DelWorker : public Worker {
public:
	int working(const std::string& url, std::string& ret) override;
};

class GetWorker : public Worker {
public:
	int working(const std::string& url, std::string& ret) override;
};

class ListWorker : public Worker {
public:
	int working(const std::string& url, std::string& ret) override;
};

class UpdateWorker : public Worker {
public:
	int working(const std::string& url, std::string& ret) override;
};

class ErrorWorker : public Worker {
public:
	int working(const std::string& url, std::string& ret) override;
};

class Mapper{ 
public: 
	using ptr = std::shared_ptr<Mapper>;


	static const std::string 
	GetAbsPath(const std::string& fPath, const std::string& user_name) 
		{ return s_root_dir + user_name + fPath; }

public:
	static const std::string s_root_dir;
	static std::map<std::string, std::function<Worker*()> > s_worker_mapper;
}; // Mapper

#define ADD_WORKER_MAPPER(cmd, worker) \
	tt::FileResource::Mapper::s_worker_mapper.insert({#cmd, [](){ return new cmd##Worker; }});


class User {
public:
	using ptr = std::shared_ptr<User>;

	User(const std::string name) { reset_name(name); }
	void reset_name(const std::string& name); 

	const std::string& name() const { return m_name; }
	const std::string& password() const { return m_password; }
	Permission::Value permission() const { return m_permission; }
	
	bool is_sup() const { return m_permission == Permission::Value::SUP; }
	bool is_com() const { return m_permission == Permission::Value::COM; }
	bool is_read_only() const{ return m_permission == Permission::Value::READ_ONLY; }

private:
	std::string m_name;
	std::string m_password;

	Permission::Value m_permission;
}; // class User


// fnt <command> <-p> <url>
class Manager {
public:
	using ptr = std::shared_ptr<Manager>;
	Manager(const std::string user_name = "unknown");

	void schedul(const std::string& command);
	static void InitWorkerMapper();

private:
	void init_url(std::string& url);
	void mapping_worker(const std::string& cmd);

private:
	User::ptr		 m_user;
	Mapper::ptr		 m_mapper;
	Worker::ptr		 m_worker;
}; // class Manager

} // namespace FileResource

} // namespcae tt 

#endif // __FILE_RESOURCE__
