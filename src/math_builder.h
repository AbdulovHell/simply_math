#pragma once
#ifndef MATH_BUILDER_H
#define MATH_BUILDER_H
#include "classes.h"

namespace Project {
	namespace Core {
		using namespace std;
		class math_builder
		{
		protected:
			math_obj* object;
		public:
			math_builder();
			~math_builder();
			/*ћетод строит дерево математических операций дл€ данного математического объекта.
			ƒанный метод в общем случае может быть рекурсивен.
			ћетод возвращает указатель на созданный экземпл€р класса math_obj, представл€ющий собой верхушку (корень) дерева, либо на копию уже созданного экземпл€ра.
			NULL в случае какой-либо ошибки, см. error.h.*/
			math_obj* build_tree(wchar_t* strPtr, wchar_t*endPtr);
		};
	}
}


#endif // !MATH_BUILDER_H
