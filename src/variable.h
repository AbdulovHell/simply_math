
#ifndef VARIABLE_H
#define VARIABLE_H
#include "math_dummy.h"
namespace Project {
	namespace Core {
		using namespace std;
		class variable :
			public math_dummy
		{
		protected:
			wstring name;				//��� ����������
			//unsigned int num_var;		//����������� (math_dummy). ����� ���������� � ������, 0 ���� ���������� �����������.
			//uint32_t point_left;		//����������� (math_dummy).��������� �� "�����" ������� ������
			//uint32_t point_right;	//����������� (math_dummy).��������� �� "������" ������� ������
			//uint32_t point_collar;	//����������� (math_dummy).��������� "������"
			
		public:
			variable();
			variable(wstring _name);
			variable(wstring _name, leaf_ptr _pl, leaf_ptr _pr, leaf_ptr _pc);
			variable(leaf_ptr _pl, leaf_ptr _pr, leaf_ptr _pc);
			variable(wstring _name, uint32_t _num, leaf_ptr _pl, leaf_ptr _pr, leaf_ptr _pc);
			variable(uint32_t _num, leaf_ptr _pl, leaf_ptr _pr, leaf_ptr _pc);
			variable(variable * _original);
			virtual ~variable();

			//����� get. ���
			flags get_class_type();

			//����� get. �������
			void* get_this();

			//����� get. ���
			wstring get_name();
			//����� assing. ���
			void assing_name(wstring _name);

			virtual int get_priority();

			virtual void copy_to(void * _ptr);

			virtual math_obj* copy(math_obj* _original);
			
		};
	}
}
#endif //!VARIABLE_H