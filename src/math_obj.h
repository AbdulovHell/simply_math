#pragma once
#ifndef MATH_OBJ_H
#define MATH_OBJ_H

#include "common.h"
#include "MemWiz.h"
#include "smart_ptr.h"

namespace Project {
	namespace Core {
		using namespace std;
		using namespace Project;	
		using namespace MemoryControl;
		class math_obj;
		class tree_ptr //16b
		{
		private:
			MemoryControl::_ref* s_ref; //4-8b
			uint32_t last_ref;  //4b
			uint16_t virtual_ref;//2

			bool check_if_can_continue(size_t size_of_type, size_t _new_size_of_arr);

			void* get_real_ptr(uint32_t);

		public:

			class __leaf_ptr {
			private:
				MemoryControl::_ref* s_ref; //4-8b
				uint32_t relative_ref;  //4b				

				void* get_real_ptr();
			public:
				__leaf_ptr();
				__leaf_ptr(tree_ptr&);
				__leaf_ptr(const tree_ptr&);
				__leaf_ptr(tree_ptr&, math_obj*);
				__leaf_ptr(tree_ptr&, uint32_t);
				__leaf_ptr(const tree_ptr&, uint32_t);
				__leaf_ptr(__leaf_ptr&);
				~__leaf_ptr();

				bool is_null_ptr();				
				math_obj* get_ptr_unsafe();

				math_obj* operator->();
				__leaf_ptr& operator =(const __leaf_ptr&);
			};



			tree_ptr();			
			tree_ptr(math_obj* _obj);
			tree_ptr(math_obj*, size_t);
			tree_ptr(const tree_ptr&);
			~tree_ptr();

			void alloc(size_t);
			void realloc(size_t );
			void realloc_plus(size_t );

			bool is_null_ptr();			

			__leaf_ptr push_obj(math_obj*);

			math_obj* get_ptr_unsafe();

			math_obj* operator->();
			tree_ptr operator =(const tree_ptr& );
			operator __leaf_ptr();
			operator __leaf_ptr() const;
			
			/*bool operator== ( tree_ptr&);

			bool operator!=( tree_ptr&);*/
		};

		typedef tree_ptr::__leaf_ptr leaf_ptr;
				
		class math_obj {
		private:			
		protected:
			
		public:		

			//Нулевой конструктор
			math_obj() { };

			virtual ~math_obj() {};
			

			//Метод get. ТИП
			virtual flags get_class_type() {
				return flags::base;
			};

			//Метод get. РАЗМЕР
			virtual size_t get_sizeof() { return sizeof(*this); }

			//Метод get. ЭЛЕМЕНТ
			void* get_this_void() { return this; }

			virtual math_obj* get_this(){ return this; }

			//Метод get. ИМЯ
			virtual wstring get_name() { return wstring(); }
			//Метод assing. ИМЯ
			virtual void assing_name(wstring _name) {}

			//Метод get. ЧИСЛО
			virtual long double get_num() { return 0; }
			//Метод assing. ЧИСЛО
			virtual void assing_num(long double _num) {}

			//Метод get. УКАЗАТЕЛЬ "левый рукав"
			virtual leaf_ptr get_pl() { return leaf_ptr(); }
			//Метод assing. УКАЗАТЕЛЬ "левый рукав"
			virtual void assing_pl(leaf_ptr& _pointer) {}


			//Метод get. УКАЗАТЕЛЬ "правый рукав"
			virtual leaf_ptr get_pr() { return leaf_ptr(); }
			//Метод assing. УКАЗАТЕЛЬ "правый рукав"
			virtual void assing_pr(leaf_ptr _pointer) {}


			//Метод get. УКАЗАТЕЛЬ "воротник"
			virtual leaf_ptr get_pc() { return leaf_ptr(); }
			//Метод assing. УКАЗАТЕЛЬ "воротник"
			virtual void assing_pc(leaf_ptr _pointer) {}

			int get_priority() { return 0; }

			virtual uint16_t copy_to(void * _ptr) = 0;

			virtual math_obj* copy(math_obj* _original) = 0;
			
		};
				
	}
};
#endif //MATH_OBJ_H
