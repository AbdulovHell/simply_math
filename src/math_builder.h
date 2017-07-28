#pragma once
#ifndef MATH_BUILDER_H
#define MATH_BUILDER_H

#include "common.h"

#include "IO.h"
#include "error.h"
#include "data_list.h"
#include "function.h"
#include "constant.h"
#include "variable.h"
#include "number.h"
#include "operations.h"
#include "math_vector.h"


namespace Project {
	namespace Core {
		using namespace std;
		class math_builder  //данный класс предназначен дл€ построени€ дерева операций по строке ввода.
			//возможно разграничить об€занности: в этом классе строить объекты типа выражений, а в его потомках - функции/векторы.
		{
		protected:
			math_obj* object;
			data_list* data_reference;
		public:
			math_builder();
			math_builder(wchar_t* strPtr, wchar_t*endPtr, data_list* _up);
			~math_builder();
			/*ћетод строит дерево математических операций дл€ данного математического объекта.
			ƒанный метод в общем случае может быть рекурсивен.
			ћетод возвращает указатель на созданный экземпл€р класса math_obj, представл€ющий собой верхушку (корень) дерева, либо на копию уже созданного экземпл€ра.
			NULL в случае какой-либо ошибки, см. error.h.*/
			math_obj* build(wchar_t* strPtr, wchar_t*endPtr);
		};
	}
}


#endif // !MATH_BUILDER_H
