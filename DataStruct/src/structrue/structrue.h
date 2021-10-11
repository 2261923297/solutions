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
	structrue() { 
		m_size = m_capacity = 0;
		m_head = nullptr;
		m_strategies = ContainerStrategies::ptr(new ContainerStrategies); 
	}	
	
	virtual ~structrue() { }

	virtual bool
	add(const node_t& val, size_t pos = -1) = 0;

	virtual bool
	del(node_p pos) = 0;

	virtual node_p
	find(const node_t& val, size_t pos) = 0;

	virtual node_p
	find(const node_t& val, size_t pos, int len) = 0;

	virtual bool
	alter(node_p pos, const node_t& to_val) = 0;

	virtual size_t 
	clean() = 0;

	virtual bool reset(size_t size) = 0;

 
	size_t size() { return m_size; }
	size_t capacity() { return m_capacity; }

	virtual node_r operator[](int ins) = 0; 
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


	Array(size_t size = 8) {
		reset(size);
		TT_DEBUG << "array structbeg";
	}
	virtual ~Array() { clean(); }
	bool add(const node_t& val, size_t pos = -1) override {
		if(pos == -1) { pos = this->m_size; }
		if(this->m_capacity - this->m_size <= 0) {
			if(false == relloc()) { 
				return false;
				TT_DEBUG << "relloc error";
			}
		}
		
		for(size_t i = this->m_size; i > pos; i--) {
			this->m_head[i] = this->m_head[i - 1];
		}
		this->m_head[pos] = val;
		this->m_size++;
		return true;
	}

	bool del(node_p pos) override {
		if(!(pos >= this->m_head && pos < this->m_head + this->m_size)) {
			TT_DEBUG << "Array::del(pos): error pos!";
			return false;
		}

		
		for(; pos < this->m_head + this->m_size - 1; pos++) {
			*(pos) = *(pos + 1);
		}
		this->m_size--;
		return true;
	}

	node_p find(const node_t& val, size_t ins) override {
		for(size_t i = ins; i < this->m_size; i++) {
			if(this->m_head[i] == val) { return this->m_head + i; }
		}
		return nullptr;
	}

	node_p find(const node_t& val, size_t ins, int len) override {
		len = this->m_size - ins > len ? len : this->m_size - len;
		for(int i = 0; i < len; i++) {
			if(this->m_head[i + ins] == val) { return this->m_head + i + ins;}
		}
		return nullptr;
	}

	bool alter(node_p pos, const node_t& to_val) override {
		if(pos < this->m_head || pos > this->m_head + this->m_size) {
			TT_DEBUG << "Array::alter: error pos";
			return false;
		}
		*pos = to_val;
		return true;
	}

	size_t clean() override { 
		if(this->m_head != nullptr) {
			free(this->m_head);
			this->m_head = nullptr;
		}
		this->m_size = 0;
		this->m_capacity = 0;
		return this->m_size;
	}

	bool reset(size_t size) override   { 
		clean();
		if(size != 0) {
			this->m_head = (node_p)malloc(sizeof(node_t) * size);
			if(this->m_head == nullptr) {
				TT_DEBUG << "no memory! "; 
				return false;
			}
		}
		this->m_capacity = size;
		return this->m_size;
	}

	node_p get_pos_from_ins(size_t ins) { return this->m_head + ins; }
	node_r operator[](int ins) override { return this->m_head[ins]; }
private:
	bool relloc() {
		size_t size = this->m_size;
		if(this->m_capacity == 0) { this->m_capacity = 8; }

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
		for(int i = 0; i < size; i++) {
			this->m_head[i] = mid_array[i];
			this->m_size++;
		}
		free(mid_array);

		return true;
	}


protected:



}; // Array
} // namespcae tt 


