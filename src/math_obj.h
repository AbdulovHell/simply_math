#pragma once
#ifndef MATH_OBJ_H
#define MATH_OBJ_H

#include "common.h"

namespace Project {
	namespace Core {
		using namespace std;
		using namespace Project;	
		
		class math_obj {
		private:			
		protected://������� ����� �� �������� ������
						
		public:		

			//������� �����������
			math_obj() {};

			virtual ~math_obj() {};

			//����� get. ���
			virtual flags get_class_type() {
				return flags::base;
			};

			//����� get. �������
			virtual math_obj* get_this() =0;

			//����� get. ���
			virtual wstring get_name() = 0;
			//����� assing. ���
			virtual void assing_name(wstring _name) = 0;

			//����� get. �����
			virtual long double get_num() = 0;
			//����� assing. �����
			virtual void assing_num(long double _num) = 0;

			//����� get. ��������� "����� �����"
			virtual math_obj* get_pl() = 0;
			//����� assing. ��������� "����� �����"
			virtual void assing_pl(math_obj* _pointer) = 0;


			//����� get. ��������� "������ �����"
			virtual math_obj* get_pr() = 0;
			//����� assing. ��������� "������ �����"
			virtual void assing_pr(math_obj* _pointer) = 0;


			//����� get. ��������� "��������"
			virtual math_obj* get_pc() = 0;
			//����� assing. ��������� "��������"
			virtual void assing_pc(math_obj* _pointer) = 0;

			
			
		};
				
	}
};
#endif //MATH_OBJ_H
