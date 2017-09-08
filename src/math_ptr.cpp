#include "math_obj.h"


namespace Project {
	namespace Core {
		using namespace MemoryControl;

		const uint32_t max_size = 96;

		bool tree_ptr::check_if_can_continue(size_t size_of_type, size_t _new_size_of_arr)
		{
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

		void * tree_ptr::get_real_ptr(uint32_t _ref = 0)
		{
			return (this->is_null_ptr()) ? nullptr:((int8_t*)ALLOCATOR.get_block() + s_ref->shift + _ref) ;
		}

		tree_ptr::tree_ptr() //:_smart_ptr<expression>()
		{
			s_ref = nullptr;
			last_ref = 0;
			virtual_ref = 0;
		}
			

		tree_ptr::tree_ptr(math_obj *_obj)
		{
			s_ref = ALLOCATOR.allocate_mem(max_size);
			virtual_ref = _obj->copy_to(this->get_real_ptr());
			last_ref = max_size;
		}

		tree_ptr::tree_ptr(math_obj *_obj, size_t _num)
		{
			s_ref = ALLOCATOR.allocate_mem(max_size, _num);
			virtual_ref = _obj->copy_to(this->get_real_ptr());
			last_ref = max_size;
		}

		tree_ptr::tree_ptr(const tree_ptr& _ptr)
		{
			this->s_ref = _ptr.s_ref;
			this->last_ref = _ptr.last_ref;
			this->virtual_ref = _ptr.virtual_ref;
			if (!this->is_null_ptr())
				this->s_ref->ref_count++;
		}

		tree_ptr::~tree_ptr()
		{
			if (!this->is_null_ptr())
			if (s_ref->ref_count > 0) {
				s_ref->ref_count--;
			}
			else {//возможен какой-либо кос€к с учЄтом всех ссылок.

			}
		}

		void tree_ptr::alloc(size_t _size_of_arr)
		{
			if (s_ref != nullptr) return;
			//если пам€ть уже выделена - делать ничего не надо.
			s_ref = ALLOCATOR.allocate_mem(max_size, _size_of_arr);
		}

		void tree_ptr::realloc(size_t _new_size_of_arr)
		{
			if (s_ref == nullptr) return alloc(_new_size_of_arr);
			ref_iterator this_ref = ALLOCATOR.get_iterator_before(s_ref);
			this_ref++;
			ref_iterator next_ref = this_ref;
			next_ref++;
			size_t num_of_bytes = next_ref->shift - s_ref->shift;
			if (_new_size_of_arr*max_size < num_of_bytes)return;
			if (check_if_can_continue(max_size, _new_size_of_arr))	return;

			_ref *new_ref = ALLOCATOR.relocate_mem(max_size, _new_size_of_arr, ALLOCATOR.get_iterator_before(s_ref));
			if (new_ref != s_ref) {
				throw std::bad_alloc();
			}

		}

		void tree_ptr::realloc_plus(size_t _plus_new_size_of_arr = 1)
		{
			if (s_ref == nullptr) return alloc(_plus_new_size_of_arr);
			size_t num_of_bytes = 0;
			ref_iterator this_ref = ALLOCATOR.get_iterator_before(s_ref);
			this_ref++;
			ref_iterator next_ref = this_ref;
			next_ref++;
			num_of_bytes = next_ref->shift - s_ref->shift;
			if (check_if_can_continue(max_size, num_of_bytes / max_size + _plus_new_size_of_arr)) return;

			_ref *new_ref = ALLOCATOR.relocate_mem(max_size, num_of_bytes / max_size + _plus_new_size_of_arr, ALLOCATOR.get_iterator_before(s_ref));
			if (new_ref != s_ref) {
				throw std::bad_alloc();
			}
		}

		bool tree_ptr::is_null_ptr()
		{
			return s_ref == nullptr || !s_ref->ref_count;
		}

		tree_ptr::__leaf_ptr tree_ptr::push_obj(math_obj *_obj)
		{

			if (this->is_null_ptr()) return __leaf_ptr();
			size_t obj_size = _obj->get_sizeof();
			ref_iterator this_ref = ALLOCATOR.get_iterator_before(s_ref);
			this_ref++;
			ref_iterator next_ref = this_ref;
			next_ref++;
			if (next_ref->shift && (last_ref + obj_size >= next_ref->shift)) {
				this->realloc_plus();
			}
			uint16_t virt_ref = _obj->copy_to(this->get_real_ptr(last_ref));
			__leaf_ptr out = __leaf_ptr(*this,last_ref + virt_ref);
			last_ref += obj_size;
			return out;
		}

		math_obj * tree_ptr::get_ptr_unsafe()
		{
			uint8_t* temp = (uint8_t*)(this->get_real_ptr());
			temp += virtual_ref;
			math_obj* out = (math_obj*)temp;
			out = out->get_this();
			return out;
		}

		math_obj * tree_ptr::operator->()
		{			
			uint8_t* temp = (uint8_t*)(this->get_real_ptr());
			temp += virtual_ref;
			math_obj* out = (math_obj*)temp;
			out = out->get_this();
			return out;
		}

		tree_ptr tree_ptr::operator=(const tree_ptr& _right)
		{
			if (!this->is_null_ptr()) //≈сли при присвоении указател€ _right, этот указатель был инициализирован - уменьшить счЄтчик ссылок.
				this->s_ref->ref_count--;
			this->s_ref = _right.s_ref;
			this->last_ref = _right.last_ref;
			this->virtual_ref = _right.virtual_ref;
			if (this->is_null_ptr()) return *this;
			this->s_ref->ref_count++;
			return *this;
		}

		tree_ptr::operator __leaf_ptr() const
		{
			return __leaf_ptr(*this,(uint32_t)0);
		}

		tree_ptr::operator __leaf_ptr()
		{
			return __leaf_ptr(*this, (uint32_t)0);
		}

		/*bool tree_ptr::operator==(tree_ptr & _right)
		{
			return (this->s_ref == _right.s_ref) && (this->relative_ref == _right.relative_ref);
		}

		bool tree_ptr::operator!=(tree_ptr &_right)
		{
			return !(*this == _right);
		}*/


		void * tree_ptr::__leaf_ptr::get_real_ptr()
		{
			return (this->is_null_ptr()) ? nullptr : ((int8_t*)ALLOCATOR.get_block() + s_ref->shift + relative_ref);
		}

		tree_ptr::__leaf_ptr::__leaf_ptr() {
			s_ref = nullptr;
			relative_ref = 0;			
		}

		tree_ptr::__leaf_ptr::__leaf_ptr(tree_ptr &_tree)
		{
			s_ref = _tree.s_ref;			
			if (!this->is_null_ptr()) 
				s_ref->ref_count++;
			relative_ref = _tree.virtual_ref;
		}
		tree_ptr::__leaf_ptr::__leaf_ptr(const tree_ptr &_tree) {
			s_ref = _tree.s_ref;			
			if (!this->is_null_ptr())
				s_ref->ref_count++;
			relative_ref = _tree.virtual_ref;
		}

		tree_ptr::__leaf_ptr::__leaf_ptr(tree_ptr &_tree, math_obj *_obj)
		{
			*this = _tree.push_obj(_obj);
			//*this = t;

			/*s_ref = t.s_ref;
			relative_ref = t.relative_ref;
			if (!this->is_null_ptr()) {
				s_ref->ref_count++;				
			}*/
		}

		tree_ptr::__leaf_ptr::__leaf_ptr(tree_ptr &_tree, uint32_t _new_ref)
		{
			s_ref = _tree.s_ref;
			relative_ref = _new_ref;
			if (!this->is_null_ptr())
				s_ref->ref_count++;
		}
				
		tree_ptr::__leaf_ptr::__leaf_ptr(const tree_ptr &_tree, uint32_t _new_ref) {
			s_ref = _tree.s_ref;
			relative_ref = _new_ref;
			if (!this->is_null_ptr())
				s_ref->ref_count++;
		}

		tree_ptr::__leaf_ptr::__leaf_ptr(__leaf_ptr & _leaf)
		{
			s_ref = _leaf.s_ref;
			relative_ref = _leaf.relative_ref;
			if (!_leaf.is_null_ptr()) {
				s_ref->ref_count++;
			}
			
		}

		tree_ptr::__leaf_ptr::~__leaf_ptr()
		{
			if (!this->is_null_ptr()&&s_ref->ref_count > 0)
				s_ref->ref_count--; 
		}

		bool tree_ptr::__leaf_ptr::is_null_ptr()
		{
			return s_ref == nullptr;
		}

		math_obj * tree_ptr::__leaf_ptr::get_ptr_unsafe()
		{
			return (math_obj*)this->get_real_ptr();
		}

		math_obj * tree_ptr::__leaf_ptr::operator->()
		{
			/*uint8_t* temp = (uint8_t*)(this->get_real_ptr());
			math_obj* temp1 = (math_obj*)(temp);
			math_obj* temp2 = (math_obj*)(temp + 32);*/
			math_obj* out = (math_obj*)this->get_real_ptr();
			//temp = (uint8_t*)temp2->get_this_void();
			//out = temp1->get_this();
			return out;
		}

		tree_ptr::__leaf_ptr & tree_ptr::__leaf_ptr::operator=(const __leaf_ptr & _leaf)
		{
			if (!this->is_null_ptr()&& s_ref->ref_count>0) //≈сли при присвоении указател€ _right, этот указатель был инициализирован - уменьшить счЄтчик ссылок.
				this->s_ref->ref_count--;
			s_ref = _leaf.s_ref;
			relative_ref = _leaf.relative_ref;
			if (this->is_null_ptr()) return *this;
			s_ref->ref_count++;
			return *this;
		}




	}
}