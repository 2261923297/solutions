#pragma once

#include <iostream>
#include <memory>
#include <string>
#include "../../include/Log.h"

namespace tt { 

template<class NodeType>
class structrue { 
public: 
	using node_t = NodeType;
	using node_p = NodeType*;
	using node_r = NodeType&;

	using ptr = std::shared_ptr<structrue>;
	structrue(size_t size = 0) { reset(size); }	
	
	virtual ~structrue() { clean(); }

	virtual bool
	add(const node_t& val, int pos) = 0;

	virtual bool
	del(node_p pos) = 0;

	virtual node_p
	find(const node_t& val, int pos) = 0;

	virtual node_p
	find(const code_t& val, int pos, int len) = 0;

	virtual bool
	alter(node_p pos, const node_t& to_val) = 0;

	virtual size_t 
	clean() = 0;

	bool reset(size_t size)  { 
		clean();
		if(size != 0) {
			m_head = (node_p*)malloc(sizeof(node_t) * size);
			if(m_head == nullptr) {
				DEBUG << "no memory! "; 
				return false;
			}
		}
		m_size = m_capacity = size;
		return m_size;
	}

 
	size_t size() { return m_size; }
	size_t capacity() { return m_capacity; }

protected:
	node_p m_head;
	size_t m_size;
	size_t m_capcacity;
}; // structrue

template<class NodeType>
class Array : structrue<NodeType> {i
public:
	using node_t = NodeType;
	using node_p = NodeType*;
	using node_r = NodeType&;

	Array(size_t size = 0) : structure(size) {}
	~Array() { free(m_head); }
	bool add(const node_t& val, int pos) override;

	bool del(node_p pos) override;

	node_p find(const node_t& val, int pos) override;

	node_p find(const code_t& val, int pos, int len) override;

	bool alter(node_p pos, const node_t& to_val) override;



protected:



}; // Array

} // namespcae tt 


