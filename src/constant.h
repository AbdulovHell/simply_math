#pragma once
#ifndef CONSTANT_H
#define CONSTANT_H
#include "number.h"
#include "expression.h"
namespace Project {
	namespace Core {
		using namespace std;
		class constant :
			virtual public number, virtual public expression
		{
		protected:
			wstring name;//содержит имя константы			
			//long double num;		//наследуется (number).
		public:
			constant() : expression() { num = 0; }
			constant(leaf_ptr _pl) : expression(_pl) { num = 0; }
			constant(long double _num) : expression() { num = _num; }
			constant(wstring _name) : expression() { name = _name; }
			constant(leaf_ptr _pl, long double _num, wstring _name) : expression(_pl) { num = _num; name = _name; }

			//Метод get. ТИП
			flags get_class_type();

			//Метод get. РАЗМЕР
			size_t get_sizeof();

			//Метод get. ЭЛЕМЕНТ
			void* get_this();

			//Метод get. ИМЯ
			wstring get_name();
			//Метод assing. ИМЯ
			void assing_name(wstring _name);

			int get_priority();

			void copy_to(void * _ptr);

			bool copy(constant* _original);
		};
	}
}
#endif // !CONSTANT_H
