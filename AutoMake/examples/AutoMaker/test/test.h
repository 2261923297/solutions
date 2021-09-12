#pragma once

#include <iostream>
#include <memory>
#include <string>

namespace tt { 

class test{ 
public: 
	using ptr = std::shared_ptr<test>;
	test();
	~test();


protected:

}; // test

} // namespcae tt 


