#pragma once

#include <iostream>
#include <memory>
#include <string>
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

class Mapper{ 
public: 
	using ptr = std::shared_ptr<Mapper>;


	static const std::string 
	GetAbsPath(const std::string& fPath, const std::string& user_name) 
		{ return s_root_dir + user_name + fPath; }


protected:
	static const std::string s_root_dir;

}; // Mapper

class User {
public:
	using ptr = std::shared_ptr<User>;

	User(const std::string name) { reset_name(name); }

	void reset_name(const std::string& name); 

	const std::string& name() const { return m_name; }
	const std::string& password() const { return m_password; }
	Permission::Value permission() const { return m_permission; }
	
	bool is_sup() { return m_permission == Permission::Value::SUP; }
	bool is_com() { return m_permission == Permission::Value::COM; }
	bool is_read_only() { return m_permission == Permission::Value::READ_ONLY; }

private:
	std::string m_name;
	std::string m_password;

	Permission::Value m_permission;
}; // class User

class Manager {
public:
	using ptr = std::shared_ptr<Manager>;
	Manager(const std::string user_name = "unknown");

	void add(const std::string& url);
	void del(const std::string& url);
	void get(const std::string& url);
	void list(const std::string& url);
	void reset(const std::string& url);

private:
	User::ptr		 m_user;
	Mapper::ptr		 m_mapper;
	
}; // class Manager

} // namespace Resource

} // namespcae tt 

const tt::FileResource::Mapper::s_root_file = "/";

