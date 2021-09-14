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
fyaml_data::load(const std::vector<std::pair<int, std::string> >& lines
		int beg, int end) {
	

}

void
fyaml_data::load(const std::vecto<int>& levels, const std::vector<std::string>a& strs, 
	 int beg, int nLine) {
	m_location[0] = beg; 
	m_location[1] = nLine;
	m_level = levels[begin];
	int name_end_pos = strs[beg].find_first_of(":");
	if(name_end_pos == std::string::npos) { 
		m_type = fyaml_type::type::SCALA; 
		m_name = strs[beg];
		return;
	}
	m_name = strs[begin].substr(0, name_end_pos);

	if(nLine == 1) {
		if (std::npos == strs[begin].find_first_of("[")) { m_type = fyaml_type::type::QUEUE; }
		else { m_type == fyaml_type::type::SCALA; }
	}
	int l_count = 0;
	for(int i = 1, i < nLine; i++) {
		if(levels[begin + i] == m_level + 1) { 
			l_count++; 
		}
		if(l_count == 2) { 
			m_type = fyaml_type::type::QUEUE; 
		}
		if(l_count == 1) 
	}
}



/*
fyaml::fyaml() { 


}

fyaml::~fyaml() {


}
*/
} // namespace tt
