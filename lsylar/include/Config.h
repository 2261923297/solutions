#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <mutex>
#include <map>
#include <functional>
#include <sstream>

namespace tt { 
namespace system {

class ConfigVarBase {
public:
	using ptr = std::shared_ptr<ConfigVarBase>;

	ConfigVarBase(const std::string& name, const std::string& desc) :
		m_name(name), m_desc(desc) { }
	~ConfigVarBase() { }

	const std::string& get_name() const { return m_name; }
	const std::string& get_desc() const { return m_desc; }

	void set_name(const std::string& name) { m_name = name; }
	void set_desc(const std::string& desc) { m_desc = desc; }

	virtual std::string to_string() const = 0;
	virtual bool from_string(const std::string& str) = 0;
protected:
	std::string m_name;
	std::string m_desc;

}; // class ConfigVarBase 

template<class From, class To>
class ChangeClass {
public:
	To operator()(const From& f) { return To(f); }
};

template<class T>
class ChangeClass<T, std::string> {
public:
	std::string operator() (const T& t) {
		std::stringstream ss;
		ss << t;
		return ss.str();
	}
private:

}; // class ChangeClass

template<class T>
class ChangeClass<std::string, T> {
public:
	T operator() (const std::string& str) {
		return (*(T*)str.c_str());
	}
private:

}; // class ChangeClass

template<class T
	, class FromStr = ChangeClass<std::string, T>
	, class ToStr = ChangeClass<T, std::string> >
class ConfigVar : public ConfigVarBase {
public:
	// sylar 锁的实现的好处, 锁的类型可以晚绑定
	using ptr = std::shared_ptr<ConfigVar>;
	using MutexType = std::unique_lock<std::mutex>;
	using ChangeCB_t = std::function<void(const T&, const T&)>;
	
	ConfigVar(const std::string& name
	, const T& val
	, const std::string& desc = " no desc ") : ConfigVarBase(name, desc) 
		{ m_val = val; }
	~ConfigVar() { }

	const T& get_val() const { return m_val; }
	void set_val(const T& val) {
		MutexType lock(m_mutex);
		m_cb(m_val, val);
		m_val = val;
	}

	std::string to_string() const override {
		return ToStr()(m_val);
	 }
	bool from_string(const std::string& str) override {
		m_val = FromStr()(str);
		return true;
	 }


private:
	T m_val;
	ChangeCB_t m_cb;
	std::mutex m_mutex;

}; // class ConfigVar


// 连接YAML 和 底层
class Config{ 
public: 
	using mutex_t = std::unique_ptr<std::mutex>;
	using ptr = std::shared_ptr<Config>;
	using config_map_t = std::map<std::string, ConfigVarBase::ptr>;
	Config() { }
	~Config() { }

	template<class T>
	static ConfigVarBase::ptr LookUp(const std::string& name
	, const T& default_value
	, const std::string& desc) {
		auto it = s_datas.find(name);
		if(s_datas.end() != it) {
			std::cout << name << " is exit, value = " << it->second->to_string() << std::endl; 
			return it->second;
		} else {
			s_datas.insert({ name
			, ConfigVarBase::ptr(new ConfigVar<T>(name
				, default_value, "for test")) });
			return s_datas.find(name)->second;
		}
		return nullptr;
	}


protected:
	static config_map_t s_datas;
	static mutex_t s_mutex;
}; // Config
} // namespace system
} // namespcae tt 


