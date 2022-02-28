#include <iostream>

class ENUM_CLASS_NAME {
public:
#undef ENUM_MESSAGE
#define ENUM_MESSAGE(name, ...) \
	enum name { __VA_ARGS__ };

	ENUM_BODY
};

static 
std::ostream& 
operator<<(std::ostream& os, ENUM_CLASS_NAME::ENUM_NAME _enum_val) { 
	std::string val_str;
	std::string sub_str;
	switch(_enum_val) {	
		
#undef xx
/*
#define xx(enum_val) \
		case enum_val : \
			 os << #enum_val;	\
		break;	\
//
*/

#define xx(enum_val) \
		case enum_val : \
			val_str = #enum_val; \
			sub_str = val_str.substr(val_str.find_last_of(':') + 1); \
			os << sub_str;	\
		break;	\
//
		ENUM_VALUES

	}
	return os;
}

#undef ENUM_CLASS_NAME
#undef ENUM_NAME
#undef ENUM_BODY
#undef ENUM_VALUES

