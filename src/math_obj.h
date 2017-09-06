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
			MemoryControl::_ref* s_ref; //12b
			uint32_t last_ref;  //4b

			bool check_if_can_continue(size_t size_of_type, size_t _new_size_of_arr);

			void* get_real_ptr(uint32_t);

		public:

			class __leaf_ptr {
			private:
				MemoryControl::_ref* s_ref; //12b
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
			tree_ptr(uint32_t);
			tree_ptr(math_obj* _obj);
			tree_ptr(math_obj*, size_t);
			tree_ptr(const tree_ptr&);
			~tree_ptr();

			void alloc(size_t);
			void realloc(size_t);
			void realloc_plus(size_t);

			bool is_null_ptr();

			__leaf_ptr push_obj(math_obj*);

			math_obj* get_ptr_unsafe();

			math_obj* operator->();
			tree_ptr operator =(const tree_ptr&);
			operator __leaf_ptr();
			operator __leaf_ptr() const;

			/*bool operator== ( tree_ptr&);

			bool operator!=( tree_ptr&);*/
		};

		typedef tree_ptr::__leaf_ptr leaf_ptr;

		class math_obj {
		private:
			leaf_ptr point_left;	//����� �������
			leaf_ptr point_right;	//������ �������
			leaf_ptr point_collar; //��������� �� ���������� ��������	
		public:

			//������� �����������
			math_obj();
			math_obj(leaf_ptr _pl, leaf_ptr _pr, leaf_ptr _pc);

			~math_obj();

			//����� get. ���
			flags get_class_type();

			//����� get. ������
			size_t get_sizeof();

			//����� get. �������
			void* get_this();

			//����� get. ��������� "����� �����"
			leaf_ptr get_pl();
			//����� assing. ��������� "����� �����"
			void assing_pl(leaf_ptr _pointer);

			//����� get. ��������� "������ �����"
			leaf_ptr get_pr();
			//����� assing. ��������� "������ �����"
			void assing_pr(leaf_ptr _pointer);

			//����� get. ��������� "��������"
			leaf_ptr get_pc();
			//����� assing. ��������� "��������"
			void assing_pc(leaf_ptr _pointer);

			void copy_to(void * _ptr);

			virtual int get_priority() = 0;

			bool copy(math_obj* _original);
		};
	}
};
#endif //MATH_OBJ_H
