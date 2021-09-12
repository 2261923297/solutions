#pragma once

#include <iostream>
#include <memory>
#include <string>

namespace tt {
class Hook{ 
public: 
	using ptr = std::shared_ptr<Hook>;
	Hook();
	~Hook();


protected:

}; // Hook

}
