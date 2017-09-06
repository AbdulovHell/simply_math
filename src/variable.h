
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
			variable() :math_dummy() {};
			variable(wstring _name) :math_dummy() { name = _name; };
			variable(wstring _name, leaf_ptr _pl, leaf_ptr _pr, leaf_ptr _pc) : math_dummy(_pl, _pr, _pc) { name = _name; };
			variable(leaf_ptr _pl, leaf_ptr _pr, leaf_ptr _pc) :math_dummy(_pl, _pr, _pc) {};
			variable(wstring _name, uint32_t _num, leaf_ptr _pl, leaf_ptr _pr, leaf_ptr _pc) :math_dummy(_num, _pl, _pr, _pc) { name = _name; };
			variable(uint32_t _num, leaf_ptr _pl, leaf_ptr _pr, leaf_ptr _pc) :math_dummy(_num, _pl, _pr, _pc) {};
			variable(variable * _original);
			
			//����� get. ���
			flags get_class_type();

			//����� get. �������
			void* get_this();

			//����� get. ���
			wstring get_name();
			//����� assing. ���
			void assing_name(wstring _name);

			int get_priority();
			
			void copy_to(void * _ptr);

			bool copy(variable* _original);

		};
	}
}
#endif //!VARIABLE_H