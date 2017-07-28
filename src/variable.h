
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
			//math_obj* point_left;		//����������� (math_dummy).��������� �� "�����" ������� ������
			//math_obj* point_right;	//����������� (math_dummy).��������� �� "������" ������� ������
			//math_obj* point_collar;	//����������� (math_dummy).��������� "������"
			//data_list* point_up;		//����������� (math_obj).
		public:
			variable();
			variable(wstring _name);
			variable(wstring _name, math_obj* _pl, math_obj* _pr, math_obj* _pc);
			variable(math_obj* _pl, math_obj* _pr, math_obj* _pc);
			variable(wstring _name, unsigned int _num, math_obj* _pl, math_obj* _pr, math_obj* _pc);
			variable(unsigned int _num, math_obj* _pl, math_obj* _pr, math_obj* _pc);
			variable(wstring _name, data_list * _data);
			variable(variable * _original);
			virtual ~variable();

			//����� get. ���
			flags get_class_type();

			//����� get. �������
			math_obj* get_this();

			//����� get. ���
			wstring get_name();
			//����� assing. ���
			void assing_name(wstring _name);
			
		};
	}
}
#endif //!VARIABLE_H