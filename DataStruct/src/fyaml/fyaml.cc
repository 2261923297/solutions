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



/*
fyaml::fyaml() { 


}

fyaml::~fyaml() {


}
*/
} // namespace tt
