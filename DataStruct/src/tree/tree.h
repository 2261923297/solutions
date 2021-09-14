#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <tuple>

#include "../../include/Nodes.h"

namespace tt { 

class f_yaml_data {
public:
	f_yaml_data() { m_name = "undef"; }

	f_yaml_data(const std::string& str, int beg, int end)  {
		m_name = str;	
		m_location[0] = beg;
		m_location[1] = end;
	}


	~f_yaml_data() { }
public:
	static std::vector<std::pair<int, std::string> > s_ori_strs;

protected:
	int type = 0;
	std::string m_name = "";
	int  m_location[2] = { 0, 0 };
	std::vector<std::string> m_sons = { "" };

}; // f_yaml_data

class tree{ 
public: 
	using ptr = std::shared_ptr<tree>;
	tree();
	~tree();


protected:

}; // tree

} // namespcae tt 


