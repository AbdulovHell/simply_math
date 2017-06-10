#pragma once
#ifndef CORE_H
#define CORE_H
///Macros
#define GET_FRAC(var) ((double)((double)var-(int)var))
#define GET_INTGR(var) ((int)var)
///

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
		
		/*Функция выполняет проверку/анализ результата заполнения дерева операций и запросов пользователя.
		Возвращает строку с результатом текущей итерации вычислений.*/
		wstring analized_output(wchar_t* _pDest, wchar_t* _endPtr, math_obj* _current_element);
		/*Функция является точкой входа в вычислительное ядро.
		Возвращает строку с результатом текущей итерации вычислений.*/
		int input_to_analize(data_list* all_math_data);
	}
}
#endif //CORE_H