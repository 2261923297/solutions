#include "Log.h"
#include <string>


ttlog::Log::Log() {
    
}
    
ttlog::Log::~Log() {
    
}


void Formatter::initFormatItem()  {
	static std::map<std::string, 
		std::function<FormatItem::ptr(const std::string& str)>> fItemMapper = {
#define xx(str, C) \
		{ #str, [](const std::string& fmt) {return FormatItem::ptr(new C(fmt));} }
		xx(l, LevelFormatItem)
	}
	
	std::string pure_format = "";
	for(int i = 0; i < m_format.size(); i++) {
		if(m_format[i] == ' ' 
			|| m_format[i] == '\t' 
			|| m_format[i] == '\n') {
			
			continue;
		}
		pure_format.push_back(m_format[i]);
	}

	std::cout << "pure_format:" << pure_format << std::endl;
	
	std::vector<std::string> patterns;
	//0123456
	//-abcd-
	size_t beg_ins = 1, end_ins = 0;
	while(beg_ins < m_foramt.size()) {
		end_ins = m_format.find_first_of('-', beg_ins);
		patterns.push_back(m_format.substr(beg_ins, end_ins - beg_ins));
		std::cout << "add_pattern: " << patterns[patterns.size() - 1] << std::endl;
		
		beg_ins = end_ins + 1;
	}
	std::string desc = "", format_str = "";
	size_t desc_beg = 0;
	for(auto pattern : patterns) {
		desc_beg = pattern.find_first_of('{');
		if(desc_beg != std::string::npos) {
			desc_beg + 1;
			desc = pattern.substr(desc_beg, pattern.size() - 1 - desc_begin);
		} else {
			desc_beg = pattern.size();
		}
		format_str = pattern.substr(desc_beg);
		auto fItem = fItemMapper.find(format_str);
		if(fTem == fItemMapper.end()) {
			std::cout << "error format: " << format_str << std::endl;
			continue;
		}
		m_items.push(fItem->second(desc));
	}
}
