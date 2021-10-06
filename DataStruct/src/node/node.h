#pragma once

#include <iostream>
#include <memory>
#include <string>

namespace tt { 

class node{ 
public: 
	using ptr = std::shared_ptr<node>;
	node();
	~node();


protected:

}; // node

} // namespcae tt 


