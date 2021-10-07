#pragma once

#include <iostream>
#include <memory>
#include <stdlib.h>
#include <string>
#include <stdio.h>
#include <malloc.h>
#include "../../include/Log.h"

namespace tt { 

class ContainerStrategies {
public:
	using ptr = std::shared_ptr<ContainerStrategies>;

	ContainerStrategies(size_t add_size = 64, size_t mul_base = 1.5) : m_add_base(add_size), m_mul_base(mul_base) {}
	virtual ~ContainerStrategies() {}

	size_t get_new_size_add(size_t& size) const { return size += m_add_base; }

	float  get_new_size_mul(size_t& size) const { return size += m_mul_base; }

protected:
	size_t m_add_base;
	float  m_mul_base;
}; // class ContainerStrategies 

template<class NodeType>
class structrue { 
public: 
	using node_t = NodeType;
	using node_p = NodeType*;
	using node_r = NodeType&;

	using ptr = std::shared_ptr<structrue>;
	structrue(size_t size = 0) { 
		reset(size); 
		m_strategies = ContainerStrategies::ptr(new ContainerStrategies); 
	}	
	
	virtual ~structrue() { clean(); }

	virtual bool
	add(const node_t& val, int pos) = 0;

	virtual bool
	del(node_p pos) = 0;

	virtual node_p
	find(const node_t& val, int pos) = 0;

	virtual node_p
	find(const node_t& val, int pos, int len) = 0;

	virtual bool
	alter(node_p pos, const node_t& to_val) = 0;

	virtual size_t 
	clean() = 0;

	bool reset(size_t size)  { 
		clean();
		if(size != 0) {
			m_head = (node_p*)malloc(sizeof(node_t) * size);
			if(m_head == nullptr) {
				TT_DEBUG << "no memory! "; 
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
	size_t m_capacity;
	ContainerStrategies::ptr m_strategies;
}; // structrue


template<class NodeType>
class Array : public structrue<NodeType> {
public:
	using ptr = std::shared_ptr<Array>;

	using node_t = NodeType;
	using node_p = NodeType*;
	using node_r = NodeType&;


	Array(size_t size = 0) : structrue<node_t>(size) {}
	~Array() { free(this->m_head); }
	bool add(const node_t& val, int pos) override {
		if(this->m_capacity - this->m_size > 0) {
			this->m_head[this->m_size] = val;
			this->m_size++;
			return true;
		}
		relloc();
	}

	bool del(node_p pos) override {
		node_p re_pos = pos;
		int pos_ins = ((int)pos - (int)this->m_head) / sizeof(node_t);
		if(!(pos >= 0 && pos < this->m_size)) {
			TT_DEBUG << "Array::del(pos): error pos!";
			return false;
		}

		for(size_t i = pos_ins + 1; i < this->m_size; i++) {
			this->m_head[i - 1] = this->m_head[i];
		}
		this->m_size--;
	}

	node_p find(const node_t& val, size_t ins) override {
		for(size_t i = ins; i < this->m_size; i++) {
			if(this->m_head[i] == val) { return this->m_head + i; }
		}
		return nullptr;
	}

	node_p find(const node_t& val, int ins, int len) override {
		len = this->m_size - ins > len ? len : this->m_size - len;
		for(int i = 0; i < len; i++) {
			if(this->m_head[i + ins] == val) { return this->m_head + i + ins;}
		}
		return nullptr;
	}

	bool alter(node_p pos, const node_t& to_val) override {
		uint64_t int_pos = uint64_t(pos);
		uint64_t int_head = uint64_t(this->m_head);
		if(int_pos < int_head || int_pos > this->m_head + sizeof(node_t) * this->m_size) {
			TT_DEBUG << "Array::alter: error pos";
			return false;
		}
		*pos = to_val;
		return true;
	}
private:
	bool relloc() {
		node_t* mid_array = nullptr;
		mid_array = (node_t*)malloc(sizeof(node_t) * this->m_size);

		if(mid_array == nullptr) {
			TT_DEBUG << "Array::relloc: no memory!";
			return false;
		}
		for(int i = 0; i < this->m_size; i++) {
			mid_array[i] = this->m_head[i];
		}

		this->reset(this->m_strategies->get_new_size_add(this->m_capacity));
		for(int i = 0; i < this->m_size; i++) {
			this->m_head[i] = mid_array[i];
		}
		free(mid_array);
	}


protected:



}; // Array

} // namespcae tt 


