#include "fyaml.h" 

namespace tt {  


fyaml_data::fyaml_data() {
	m_name = "";
	m_location[0] = m_location[1] = 0;
	m_type = fyaml_type::type::ERROR;
	m_level = -1;
}

fyaml_data::~fyaml_data() { }


void
fyaml_data::load(const std::vector<int>& levels, const std::vector<std::string>& strs, int beg, int nLine) {
	m_location[0] = beg;							//location
	m_location[1] = nLine;
	m_level = levels[beg];						//level
	int name_end_pos = strs[beg].find_first_of(":");
	if(name_end_pos == std::string::npos) { 
		m_type = fyaml_type::type::SCALA;				// type::scala
		m_name = strs[beg];								// name
		return;
	}
	m_name = strs[beg].substr(0, name_end_pos);   // name

	if(nLine == 1) {
		if (std::string::npos == strs[beg].find_first_of("[")) { 
			m_type = fyaml_type::type::QUEUE; }			// type::queue : [vals]
		else { m_type == fyaml_type::type::SCALA; }		// type::scala : val
	}
	int l_count = 0;
	for(int i = 1; i < nLine; i++) {
		if(levels[beg + i] == m_level + 1) { l_count++; }
		if(l_count == 2) { 
			m_type = fyaml_type::type::QUEUE;			// type::queue : \n vals
			break;		
		}
		if(l_count == 1) {
			m_type = fyaml_type::type::MAP;				// type::map : \n
		} 
	}
}



void 
fyaml_data::show_data() {
	std::stringstream ss;
	ss  << "-fyaml_data:"
		<< "\n\tname:       " << m_name
		<< "\n\tlevel:      " << m_level
		<< "\n\ttype:       " << m_type
		<< "\n\tlocation: [ " << m_location[0] << ", " << m_location[1] << " ]"
		<< "\n\tsons:     [ "; 
	for(auto son : m_sons) ss << son << ", ";
	ss << " ]" << std::endl;
	std::cout << ss.str();
}
/*
fyaml::fyaml() { 


}

fyaml::~fyaml() {


}
*/
} // namespace tt
