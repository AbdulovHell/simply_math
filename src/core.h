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
		//
		math_obj* filling_vector(wchar_t* strPtr, wchar_t*ePtr, math_obj* c_e, int brakets);
		/*Функция выполняет проверку/анализ результата заполнения дерева операций и запросов пользователя.
		Возвращает строку с результатом текущей итерации вычислений.*/
		wstring analized_output(wchar_t* _pDest, wchar_t* _endPtr, math_obj* _current_element);
		/*Функция является точкой входа в вычислительное ядро.
		Возвращает строку с результатом текущей итерации вычислений.*/
		wstring input_to_analize(wchar_t* input);
	}
}
#endif //CORE_H