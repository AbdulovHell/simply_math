
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
			wstring name;				//имя переменной
			//unsigned int num_var;		//наследуется (math_dummy). номер переменной в списке, 0 если переменная единственна.
			//math_obj* point_left;		//наследуется (math_dummy).Указатель на "левый" элемент списка
			//math_obj* point_right;	//наследуется (math_dummy).Указатель на "правый" элемент списка
			//math_obj* point_collar;	//наследуется (math_dummy).Указатель "наверх"
			//data_list* point_up;		//наследуется (math_obj).
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

			//Метод get. ТИП
			flags get_class_type();

			//Метод get. ЭЛЕМЕНТ
			math_obj* get_this();

			//Метод get. ИМЯ
			wstring get_name();
			//Метод assing. ИМЯ
			void assing_name(wstring _name);
			
		};
	}
}
#endif //!VARIABLE_H