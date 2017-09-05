
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
			variable();
			variable(wstring _name);
			variable(wstring _name, leaf_ptr _pl, leaf_ptr _pr, leaf_ptr _pc);
			variable(leaf_ptr _pl, leaf_ptr _pr, leaf_ptr _pc);
			variable(wstring _name, uint32_t _num, leaf_ptr _pl, leaf_ptr _pr, leaf_ptr _pc);
			variable(uint32_t _num, leaf_ptr _pl, leaf_ptr _pr, leaf_ptr _pc);
			variable(variable * _original);
			virtual ~variable();

			//Метод get. ТИП
			flags get_class_type();

			//Метод get. ЭЛЕМЕНТ
			void* get_this();

			//Метод get. ИМЯ
			wstring get_name();
			//Метод assing. ИМЯ
			void assing_name(wstring _name);

			virtual int get_priority();

			virtual void copy_to(void * _ptr);

			virtual math_obj* copy(math_obj* _original);
			
		};
	}
}
#endif //!VARIABLE_H