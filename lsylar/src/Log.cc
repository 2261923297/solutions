#include "Log.h"
#include <string>

ttlog::Event::Event(const std::string& loggerName, LogLevel::Level level
	, uint64_t line, std::string fileName
	)
:m_loggerName(loggerName), m_line(line), m_file(fileName), m_level(level) 
{

}
ttlog::Event::~Event() {
	
}

void 
ttlog::Event::debugOut()  {
	std::cout << "[" << m_level << "] "
		<< m_file << ": " << m_line
		<< "\t" << m_ss.str() << std::endl;
}



void 
ttlog::Formatter::resetFormat(const std::string& format) {
	setFormat(format);
	initFormatItem();
}

std::string 
ttlog::Formatter::generate(Event::ptr e) {
	std::stringstream ss;
	for(auto item : m_items) {
		item->format(e, ss);
	}
	return ss.str();
}

void 
ttlog::Formatter::initFormatItem() {
	
	static std::map<std::string, std::function<FormatItem::ptr(const std::string)> > s_str_item_mapper = {
#define xx(str, C) \
		{ #str, [](const std::string& fmt) { return FormatItem::ptr(new C(fmt)); }}
		
		xx(l, LevelFormatItem)
		, xx(tab, TabFormatItem)
		, xx(nl, NewLineFormatItem)
		, xx(m,  MessageFormatItem)
		, xx(fn, FileNameFormatItem)
		, xx(s,  StringFormatItem)
		, xx(ln, LineFormatItem)
		, xx(lm, LoggerNameFormatItem)
	};
#undef xx
	std::string pure_format = "";
	std::vector<int> split_ins;
	std::vector<std::string> patterns;

	for(auto c : m_format) {
		if(c != ' ' && c != '\t' && c != '\n' && c != '\r') {
			pure_format.push_back(c);
			if(c == m_split) {
				split_ins.push_back(pure_format.size() - 1);
			}
			continue;
		}

	}
//	std::cout << "PURE_FORMAT: " << pure_format << std::endl;

	split_ins.push_back(pure_format.size());  //相当于最后一个 'split' 

	int beg = split_ins[0] + 1, end = 0;
	for(size_t i = 1; i < split_ins.size(); i++) {
		end = split_ins[i];
		patterns.push_back(pure_format.substr(beg, end - beg));
		beg = end + 1;
//		std::cout << "patterns add<" << patterns.back() << ">" << std::endl;
	}

	for(auto ori_str : patterns) {
		size_t fmt_beg = ori_str.find_first_of('{'), fmt_end = ori_str.find_first_of('}');
		std::string fmt = "", str = ori_str;
		if(fmt_beg != std::string::npos && fmt_end != std::string::npos) {
			fmt_beg++;
			fmt = ori_str.substr(fmt_beg, fmt_end - fmt_beg);
			str = ori_str.substr(0, fmt_beg - 1);
		}
			
		auto mapper = s_str_item_mapper.find(str);
		if(mapper == s_str_item_mapper.end()) {
			std::cout << "error pattern: " << str << std::endl;
		} else {
			m_items.push_back(mapper->second(fmt));
		}
	}


}

void
ttlog::Logger::log(ttlog::Event::ptr e)  {
	if(m_appandars.size() == 0) {
		m_def_appandar << m_def_formatter->generate(e);
		return;
	}
	for(auto appanda : m_appandars) {
		if(appanda->getLevel() >= e->getLevel()) {
			appanda << m_def_formatter->generate(e);
		}
	}
}

