#pragma once

#include <iostream>
#include <memory>
#include <string>

namespace tt { 

class Fiber{ 
public: 
	using ptr = std::shared_ptr<Fiber>;
	Fiber();
	~Fiber();


protected:

}; // Fiber

} // namespcae tt 


