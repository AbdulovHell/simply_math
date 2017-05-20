#pragma once
#ifndef CORE_H
#define CORE_H
///Macros
#define GET_FRAC(var) ((double)((double)var-(int)var))
#define GET_INTGR(var) ((int)var)
///
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <vector>

#include "error.h"
#include "IO.h"
#include "filters.h"
#include "classes.h"

namespace Project {
	namespace Core {

		using namespace std;
		using namespace Project;
		//using namespace Project::IO;
		using namespace Project::Core;

		void ClearGeneral();
		//Инициализация массива констант, переменных, функций и уравнений
		void Init();
		//функция проходит по вектору general_var_const и ищет объект с нужным именем
		math_obj* run_vector(wstring _pv);
		/*Функция создаёт математический объект - вектор - по строке символов типа wchar_t, имеющей начало begin и конец end.
		Функция возвращает указатель на созданный экземпляр класса math_obj. Функция вызывает в качестве внутренней функции, а так же рекурсивно, основную функцию заполнения build_tree.
		NULL в случае какой-либо ошибки, см. error.h.*/
		math_obj *make_vector(int size, wchar_t*begin, wchar_t*end);		

		/*Функция строит математические объекты из других мат объектов в виде структуры типа дерева по строке символов типа wchar_t, имеющей начало strPtr и конец ePtr.
		Функция возвращает указатель на созданный экземпляр класса math_obj, представляющий собой верхушку (корень) дерева, либо на копию уже созданного экземпляра.
		NULL в случае какой-либо ошибки, см. error.h. Типы мат объектов описаны в classes.h*/
		math_obj* build_tree(wchar_t* strPtr, wchar_t*ePtr, math_obj* c_e, int brakets);
		/*Функция выполняет проверку/анализ результата заполнения дерева операций и запросов пользователя.
		Возвращает строку с результатом текущей итерации вычислений.*/
		wstring analized_output(wchar_t* _pDest, wchar_t* _endPtr, math_obj* _current_element);
		/*Функция является точкой входа в вычислительное ядро.
		Возвращает строку с результатом текущей итерации вычислений.*/
		wstring input_to_analize(wchar_t* input);
	}
}
#endif //CORE_H