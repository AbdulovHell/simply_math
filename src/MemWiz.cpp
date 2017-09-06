

#include "MemWiz.h"

using namespace MemoryControl;


namespace MemoryControl {
	_memory_interface mem_wiz = _memory_interface();

	MemoryControl::_ref* MemoryControl::_memory_interface::allocate_mem(size_t _size_of_type, size_t _size_of_arr)
	{
		ref_iterator temp_ref = this->reference_table.before_begin();
		ref_iterator next_ref;
		//void *end = (int8_t*)mem_wiz.block + unit_memory * 1024 - 8;
		size_t size = _size_of_type*_size_of_arr;
		while (temp_ref != this->reference_table.end()) {
			if (this->reference_table.empty()) {
				temp_ref = this->reference_table.emplace_after(temp_ref, _ref());
				next_ref = this->reference_table.emplace_after(temp_ref, _ref());
			}
			else {
				temp_ref++;
				next_ref = temp_ref;
				next_ref++;
			}
			if (!temp_ref->ref_count) {//счётчик ссылок равен нулю				
				if (temp_ref->shift || (temp_ref == this->reference_table.begin() && next_ref->shift)) {//если ранее память была использована/инициализирован указатель на конец предыдущего блока					
					if (next_ref == this->reference_table.end()) {//адреса кончились

						if ((int8_t*)this->block + (temp_ref->shift + size) <= this->block_end) {
							temp_ref->ref_count += 1;
							next_ref = this->reference_table.emplace_after(temp_ref, _ref());
							next_ref->shift = temp_ref->shift + size;
							return temp_ref->get_ref_ptr();
						}
						else {//TODO: выделение доп блока памяти.

						}
					}
					else if (!next_ref->shift) {
						if ((int8_t*)this->block + (temp_ref->shift + size) <= this->block_end) {
							temp_ref->ref_count += 1;
							next_ref->shift = temp_ref->shift + size; //задать конец куска памяти/начало следующего.
							return temp_ref->get_ref_ptr();
						}
						else {// выделение доп блока памяти.

						}
					}
					else {//next_ref->shift != 0
						if (temp_ref->shift + size <= next_ref->shift) {//достаточно ли места
							temp_ref->ref_count += 1;
							if (!next_ref->ref_count) {//если на следующий блок памяти ничего не указывает - передвинуть указатель поближе (ради более плотной упаковки)
								next_ref->shift = temp_ref->shift + size;
							}
							return temp_ref->get_ref_ptr();
						}
					}
				}
				else {//первая запись в таблице
					temp_ref->shift = 0;
					temp_ref->ref_count += 1;
					next_ref->shift = temp_ref->shift + size;
					return temp_ref->get_ref_ptr();
				}
			}
		}
		return nullptr;//на всякий случай.
	}

	_ref * _memory_interface::relocate_mem(size_t _size_of_type, size_t _new_size_of_arr, ref_iterator _ref_before)
	{		

		size_t size = _size_of_type*_new_size_of_arr;		
		uint32_t last_shift_buf = 0;
		ref_iterator before_ref = _ref_before;
		ref_iterator temp_ref = _ref_before;
		temp_ref++;
		_ref buf = *(_ref_before->get_ref_ptr());
		ref_iterator next_ref = temp_ref;
		next_ref++;
		size_t size_before = next_ref->shift - temp_ref->shift;
		while (next_ref != this->reference_table.end()) {
			if (!temp_ref->ref_count)
				if (temp_ref->shift + size <= next_ref->shift)break;
			before_ref++;
			temp_ref++;
			next_ref++;
		}
		last_shift_buf = temp_ref->shift;
		_ref buf_n = *(_ref_before->get_ref_ptr());
		this->reference_table.erase_after(before_ref);
		this->reference_table.erase_after(_ref_before);
		temp_ref = this->reference_table.emplace_after(before_ref, _ref());
		before_ref = this->reference_table.emplace_after(_ref_before, buf);
		temp_ref->shift = last_shift_buf;
		temp_ref->ref_count = buf.ref_count;
		before_ref->ref_count = 0;
		if (next_ref == this->reference_table.end()) {
			if ((int8_t*)this->block + (temp_ref->shift + size) <= this->block_end) {
				next_ref = this->reference_table.emplace_after(temp_ref, _ref());
				
			}
			else {//TODO: выделение доп блока памяти.

			}
		}	
		if (!next_ref->shift) 
			next_ref->shift = temp_ref->shift + size;
		void *src = ((int8_t*)this->block + before_ref->shift);
		void *dest = ((int8_t*)this->block + temp_ref->shift);
		std::memcpy(dest, src, size_before);
		
		return temp_ref->get_ref_ptr();
	}

	MemoryControl::ref_iterator _memory_interface::get_iterator_before(_ref * _ref)
	{
		ref_iterator out = this->reference_table.before_begin();
		ref_iterator next_ref = out;
		next_ref++;
		while (next_ref->get_ref_ptr() != _ref) {
			out = next_ref;
			next_ref++;
		}
		return out;
	}

	MemoryControl::_memory_interface::_memory_interface()
	{
		//reference_table; //уже инициализирован конструктором по умолчанию
		block = nullptr;
		block_end = nullptr;
		uint32_t try_count = 0;

		//WriteProcessMemory(hDF, (void*)Inject_point, modsrc, 13, NULL);
		//ReadProcessMemory(hDF, (void*)PauseStateAddr, &PauseState, 1, NULL);
		do {
			//block = calloc(unit_memory, 1024);
			block = VirtualAllocEx(GetCurrentProcess(), NULL, unit_memory * 1024, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
			try_count++;
		} while (!block&&try_count < 5);
		/*if (!block) { //err
			GetLastError();
		}*/
		block_end = (void*)((int8_t*)block + unit_memory * 1024);
	}


	MemoryControl::_memory_interface::~_memory_interface()
	{
		//TODO:проверка счётчика ссылок. При завершении работы программы в принципе не требуется. но возможно потребуется переинициализация пула памяти в течение работы, например если реализовать сохранение/загрузку программ
		ref_iterator iter = this->reference_table.begin();
		//free(block);
		VirtualFreeEx(GetCurrentProcess(), this->block, unit_memory * 1024, MEM_RELEASE);

		this->reference_table.~forward_list();
	}
	std::forward_list<_ref>& _memory_interface::get_ref_table()
	{
		return this->reference_table;
	}
	void * _memory_interface::get_block()
	{
		return this->block;
	}
	void * _memory_interface::get_block_end()
	{
		return this->block_end;
	}
	_ref * _ref::get_ref_ptr()
	{
		return this;
	}
}


