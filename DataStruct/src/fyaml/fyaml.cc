#include "fyaml.h" 
#include "../../include/File.h"
namespace tt {  

const std::string& 
fyaml_data::s_name_follows = "abcdefghijklmnopqrstuvwxyz_";


fyaml_data::fyaml_data() {
	m_name = "root";
	m_location[0] = m_location[1] = 0;
	m_type = fyaml_type::type::ERROR;
	m_level = 0;
}

fyaml_data::~fyaml_data() { }

fyaml_data::fyaml_data(const std::string& name, int location, int level) {
	m_name = name;
	m_location[0] = location;
	m_location[1] = 1;
	m_level = level;
	m_type = fyaml_type::type::SCALA;
}

void 
fyaml_data::load_line(const std::string& line_str, int location, int level) {
	m_level = level;
	m_location[0] = location;
	m_location[1] = 1;

	int sp_pos = line_str.find(":");
	if(sp_pos == std::string::npos) { // SCALA
		sp_pos = line_str.find_first_not_of(s_name_follows);
		m_type = fyaml_type::type::SCALA;
	} else {
		int que_pos = line_str.find("[");
		if(std::string::npos == que_pos) {		// MAP
			m_type = fyaml_type::type::MAP;
			int sca_beg = 0, sca_end = 0;
			sca_beg = line_str.find_first_of(s_name_follows, sp_pos);
			sca_end = line_str.find_first_not_of(s_name_follows, sca_beg);
			std::string sca_name = line_str.substr(sca_beg, sca_end - sca_beg);
			m_sons.push_back(sca_name);
			fyaml_data::ptr son(new fyaml_data(sca_name, location, level + 1));
			add_son(sca_name, son);
		} else {								// MAP
			m_type = fyaml_type::type::QUEUE;
			size_t beg = que_pos + 2, end = line_str.find_first_not_of(s_name_follows, beg);
			std::string que_name = "";
			for(;;) {
				if(',' == line_str[end]) {		// SCALA
					end = line_str.find_first_not_of(s_name_follows, beg);

					que_name = line_str.substr(beg, end - beg);
					m_sons.push_back(que_name);
					fyaml_data::ptr son(new fyaml_data(que_name, location, level + 1));
					add_son(que_name, son);
					beg = end + 2;
				} else {
					break;
				}
			}
		}
	}
	m_name = line_str.substr(0, sp_pos);
}

void
fyaml_data::load(const std::vector<int>& levels, const std::vector<std::string>& strs, int beg, int nLine) {
		
	if(nLine == 1) {
		load_line(strs[beg], beg, levels[beg]);
	}


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

	int l_count = 0, name_end = 0;
	for(int i = 1; i < nLine; i++) {
		if(levels[beg + i] == m_level + 1) { 
			l_count++; 
			name_end = strs[beg + i].find_first_of(":");
			name_end = name_end == std::string::npos ? strs[beg + i].size() : name_end;
			m_sons.push_back(strs[beg + i].substr(0, name_end));
			
		} else {
			continue;
		}
	}
	if(l_count >= 2) { 
		m_type = fyaml_type::type::QUEUE;			// type::queue : \n vals
	}
	if(l_count == 1) {
		m_type = fyaml_type::type::MAP;				// type::map : \n
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
	std::string str = "";
	for(int i = 0; i < m_sons.size(); i++) {
		str = i == m_sons.size() - 1 ? m_sons[i] : (m_sons[i] + ", ");
		ss << str;
	}
	ss << " ]" << std::endl;
	for(auto son : m_son_nodes) {
//		son.second->show_data();
	}
	std::cout << ss.str();
}	

void 
fyaml_loader::load_from_file(const std::string& file_name)  {
	tt::File::Entry::ptr fentry(new tt::File::Entry(file_name));	
	fentry->reopen();
	const size_t buffer_size = 2048;
	char buffer[buffer_size + 1] = { 0 };
	fentry->read(buffer, buffer_size);
//	std::cout << buffer << std::endl;

	// split line
	int line = 0;
	m_levels.push_back(0);
	m_confs.push_back("");
	for(int i = 0; i < buffer_size; i++) {
		if(buffer[i] == '\t') {
			m_levels[line]++;
			continue;
		} else if(buffer[i] == '\n') {
			if(buffer[i - 1] == '\n') {continue; }
			line ++;
			m_levels.push_back(0);
			m_confs.push_back("");
			continue;
		}
		m_confs[line] += buffer[i];
	}
	int max_level = m_levels[0];
	for(size_t i = 0; i < m_levels.size(); i++) {
		m_levels[i]++;
		max_level = max_level > m_levels[i] ? max_level : m_levels[i];
	}
	std::cout << "max_level = " << max_level << std::endl;
	m_mapper.resize(max_level + 1);
	fyaml_data::ptr root(new fyaml_data);
	add(root);
}

void
fyaml_loader::parent(fyaml_data::ptr& ans, int line) {
	int cur_level = m_levels[line];
	for(int i = line - 1; i >= 0; i--) {
		if(m_levels[i] > cur_level) {
			std::string name = m_confs[i].substr(0, m_confs[i].find(":"));
			std::cout << "line: " << line << "parent_name: " << name << std::endl;
			
			ans = m_mapper[cur_level - 1][name];
			return;
		}
	}
	ans.reset();
}
void 
fyaml_loader::auto_make_data() {
	for(int i = 0; i < m_levels.size(); i++) {
		int level = m_levels[i];
		fyaml_data::ptr& d_parent = (*((m_mapper[level - 1].end())--)).second;
		for(int j = i; j < m_levels.size(); j++) {
			if(m_levels[j] >= level) {

				fyaml_data::ptr new_data(new fyaml_data);
				new_data->load(m_levels, m_confs, i, j - i);
				d_parent->add_son(new_data->name(), new_data);
			}
		}
	}		
}

/*
fyaml::fyaml() { 


}

fyaml::~fyaml() {


}
*/
} // namespace tt
