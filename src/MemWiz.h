#pragma once
#ifndef MEMWIZ_H
#define MEMWIZ_H
//#define _TEST
#include "common.h"
#define ALLOCATOR MemoryControl::mem_wiz


static size_t reference_amount = 512;

static size_t unit_memory = 32; //в  б

namespace MemoryControl {	

	struct _ref {
		uint32_t ref_count;
		uint32_t shift;//сдвиг относительно указател€ block в байтах

		_ref* get_ref_ptr();
	};

	typedef  std::forward_list<MemoryControl::_ref>::iterator ref_iterator;

	class _memory_interface {
	private:
		template<class T>
		friend class _smart_ptr;	
		

		std::forward_list<_ref> reference_table;
		void *block;		
		void *block_end;
	
	public:
		_ref* allocate_mem(size_t _size_of_type, size_t _size_of_arr = 1);
		_ref* relocate_mem(size_t _size_of_type, size_t _new_size_of_arr, ref_iterator _ref_before);

		
		ref_iterator get_iterator_before(_ref*);
	
		_memory_interface();
		~_memory_interface();

		std::forward_list<_ref>& get_ref_table();
		void* get_block();
		void* get_block_end();

	};
	extern _memory_interface mem_wiz;		

}





#endif //MEMWIZ_H