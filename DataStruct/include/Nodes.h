#include <iostream>
#include <vector>
#include <string>
#include <list>

namespace tt {

template<class DataType>
class Node { 
public:
	using data_t = DataType;
	Node(const data_t& val = data_t()) : m_data(val) { } 
	virtual ~Node() { }

	void
	set_data(const data_t& val) { m_data = val; }

	const data_t&
	get_data() const { return m_data; }

	data_t&
	get_real_data() { return m_data; }
protected:
	data_t m_data;

}; // class Node

template<class DataType>
class ltree_node : public Node<DataType> {
public:
	using data_t = DataType;	
	using ptr = std::shared_ptr<ltree_node<data_t> >;
	using link_t = std::list<ptr>;

	ltree_node(const std::string& str)
	  { data_t& d = this->get_real_data();
		  d = data_t(str); } 

	ltree_node() { }
	~ltree_node() 
	  {}

	void
	add_same(ptr& val) { m_same_level_nodes.push_back(val); }

	void 
	add_low(ptr& val) { m_low_level_nodes.push_back(val); }

	


protected:
	link_t m_same_level_nodes;
	link_t m_low_level_nodes;
}; // ltree_node

}
