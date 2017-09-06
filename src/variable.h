
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
			//uint32_t point_left;		//наследуется (math_dummy).Указатель на "левый" элемент списка
			//uint32_t point_right;	//наследуется (math_dummy).Указатель на "правый" элемент списка
			//uint32_t point_collar;	//наследуется (math_dummy).Указатель "наверх"

		public:
			variable() :math_dummy() {};
			variable(wstring _name) :math_dummy() { name = _name; };
			variable(wstring _name, leaf_ptr _pl, leaf_ptr _pr, leaf_ptr _pc) : math_dummy(_pl, _pr, _pc) { name = _name; };
			variable(leaf_ptr _pl, leaf_ptr _pr, leaf_ptr _pc) :math_dummy(_pl, _pr, _pc) {};
			variable(wstring _name, uint32_t _num, leaf_ptr _pl, leaf_ptr _pr, leaf_ptr _pc) :math_dummy(_num, _pl, _pr, _pc) { name = _name; };
			variable(uint32_t _num, leaf_ptr _pl, leaf_ptr _pr, leaf_ptr _pc) :math_dummy(_num, _pl, _pr, _pc) {};
			variable(variable * _original);
			
			//Метод get. ТИП
			flags get_class_type();

			//Метод get. ЭЛЕМЕНТ
			void* get_this();

			//Метод get. ИМЯ
			wstring get_name();
			//Метод assing. ИМЯ
			void assing_name(wstring _name);

			int get_priority();
			
			void copy_to(void * _ptr);

			bool copy(variable* _original);

		};
	}
}
#endif //!VARIABLE_H