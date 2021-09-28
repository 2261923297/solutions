#pragma once 
#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <map>

namespace tt {

namespace fyaml {

class data {
public:
	using ptr = std::shared_ptr<data>;
	using type_t = tt::fyaml::type;

	const std::string& name() const { return m_name; }
	const type_t&  type() const { return m_type; }
	const int  level() const { return m_level; }
	data::ptr get_son(const std::string& son_name) { return m_sons[son_name]; }
	void add_son(data::ptr data) { m_sons.insert({data->name(), data}); }

	data(const std::string& name, int level, int beg, int len, type_t type)
	 : m_name(name), m_level(level), m_type(type) 
	 { m_location[0] = beg; m_location[1] = len; }

private:
	data(data&);
	data(data&&);
	
protected:
	std::string m_name;
	int         m_level;
	int         m_location[2];  //beg, len
	type_t      m_type;
	std::map<std::string, data::ptr> m_sons;

} // class data


} // namespace fyaml
} // namespace tt
