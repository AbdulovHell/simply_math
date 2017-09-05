#pragma once
#ifndef SMART_PTR_H
#define SMART_PTR_H
#include "MemWiz.h"

namespace MemoryControl {

	template<class T>
	class _smart_ptr {
	protected:
		_ref* s_ref;

		bool check_if_can_continue(size_t size_of_type, size_t _new_size_of_arr) {
			ref_iterator this_ref = ALLOCATOR.get_iterator_before(s_ref);
			this_ref++;
			ref_iterator next_ref = this_ref;
			next_ref++;
			ref_iterator next_next_ref = next_ref;
			next_next_ref++;
			if (!next_ref->ref_count) {//далее записывать имеет смысл только если на следующую запись ничего не указывает
				if (next_next_ref != ALLOCATOR.get_ref_table().end()) {
					if (s_ref->shift + _new_size_of_arr*size_of_type < next_next_ref->shift) {
						next_ref->shift = s_ref->shift + _new_size_of_arr*size_of_type;
						return true;
					}
					if (s_ref->shift + _new_size_of_arr*size_of_type == next_next_ref->shift) {
						next_ref = ALLOCATOR.get_ref_table().erase_after(this_ref);
						return true;
					}
				}
				else {//next_next_ref == ALLOCATOR.reference_table.end()
					if ((int8_t*)ALLOCATOR.get_block() + (s_ref->shift + _new_size_of_arr*size_of_type) < ALLOCATOR.get_block_end()) {//есть место в блоке пам€ти
						next_ref->shift = s_ref->shift + _new_size_of_arr*size_of_type;
						return true;
					}
				}
			}
			return false;
		}
	public:
		_smart_ptr() {
			s_ref = nullptr;
		}


		_smart_ptr(T _obj) {
			s_ref = ALLOCATOR.allocate_mem(sizeof(T));
			T *temp_ptr = (T*)((int8_t*)ALLOCATOR.block + (s_ref->shift));
			*temp_ptr = T();
			*temp_ptr = _obj;
		}


		_smart_ptr(T _obj, size_t _size_of_array) {
			s_ref = ALLOCATOR.allocate_mem(sizeof(T), _size_of_array);
			T *temp_ptr = (T*)((int8_t*)ALLOCATOR.block + (s_ref->shift));
			for (int count = 0; count < _size_of_array; count++) {
				*(temp_ptr + count) = T();
			}
			*temp_ptr = _obj;
		}

		_smart_ptr(const _smart_ptr& _ptr) {
			this->s_ref = _ptr.s_ref;
			if (!this->is_null_ptr())
				this->s_ref->ref_count++;
		}

		_smart_ptr(const _smart_ptr&& _ptr) {
			this->s_ref = _ptr.s_ref;
			_ptr.s_ref = nullptr;
		}


		~_smart_ptr() {
			if (!this->is_null_ptr()) {
				if (s_ref->ref_count > 0) {
					s_ref->ref_count--;
				}
				else {//возможен какой-либо кос€к с учЄтом всех ссылок.

				}
			}
		}

		//Ќебезопасно! Ќельз€ удал€ть ("забывать") сам smart_pointer! Ќельз€ использовать delete!
		T* get_ptr_unsafe() {
			if (s_ref == nullptr) return nullptr;
			return (T*)((int8_t*)ALLOCATOR.block + (s_ref->shift));
		}

		T at(size_t _place) {
			return this[_place];
		}

		void alloc(T _obj, size_t _size_of_arr = 1) {
			if (s_ref != nullptr) return;
			//если пам€ть уже выделена - делать ничего не надо.
			s_ref = ALLOCATOR.allocate_mem(sizeof(_obj), _size_of_arr);

		}
		void realloc(size_t _new_size_of_arr) {
			if (s_ref == nullptr) return alloc(T(), _new_size_of_arr);
			if (check_if_can_continue(sizeof(T), _new_size_of_arr)) {//если можно доп пам€ть выделить дальше - значит всЄ уже готово
				return;
			}
			else {
				_ref *new_ref = ALLOCATOR.relocate_mem(max_size, _new_size_of_arr, ALLOCATOR.get_iterator_before(s_ref));
				/*if (new_ref != s_ref) {
					throw std::bad_alloc();
				}	*/
			}
		}

		//true если указатель не null.
		bool is_null_ptr()
		{
			return s_ref == nullptr || !s_ref->ref_count;
		}


		T* operator->() {
			T zero = T();
			if (this->is_null_ptr()) return &zero;//ради безопасного вызова возвращаетс€ ссылка на пустой экземпл€р T при нулевых указател€х
																								 //TODO: ¬озможно имеет смысл так же провер€ть чтобы нужный указатель ссылалс€ во внутрь пула пам€ти (а не за его пределы)
			return (T*)((int8_t*)ALLOCATOR.block + (s_ref->shift));
		}
		T& operator* () {
			T zero = T();
			if (this->is_null_ptr()) return zero;
			return *((T*)((int8_t*)ALLOCATOR.block + (s_ref->shift)));
		}

		T& operator[](size_t _place) {
			//Ќќ данный оператор сработает в т.ч. и не дл€ массивов. ѕри любом отличном от нул€ значении _place он выдаст результат нулевого конструктора, но выдаст.

			T zero = T();
			T* out;
			if (this->is_null_ptr()) return zero;//нулевые указатели
			if ((void*)((int8_t*)ALLOCATOR.block + (s_ref->shift + _place * sizeof(T))) >= ALLOCATOR.block_end) return zero; //выход за пределы глобальной пам€ти
			ref_iterator next_ref = s_ref;
			next_ref++;

			if (s_ref->shift + _place * sizeof(T) >= next_ref->shift) return zero;

			out = (T*)((int8_t*)ALLOCATOR.block + (s_ref->shift + _place * sizeof(T)));
			return *out;
		}

		_smart_ptr<T>& operator=(const _smart_ptr<T>& _right) {
			if (!this->is_null_ptr()) //≈сли при присвоении указател€ _right, этот указатель был инициализирован - уменьшить счЄтчик ссылок.
				this->s_ref->ref_count--;
			this->s_ref = _right.s_ref;
			if (this->is_null_ptr()) return *this;
			this->s_ref->ref_count++;
			return *this;
		}
	};

}



#endif //SMART_PTR_H