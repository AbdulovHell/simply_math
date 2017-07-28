#pragma once
#ifndef CORE_H
#define CORE_H
///Macros
#define GET_FRAC(var) ((double)((double)var-(int)var))
#define GET_INTGR(var) ((int)var)
///

#include "common.h"

#include "error.h"
#include "IO.h"
#include "filters.h"

#include "math_builder.h"

namespace Project {
	namespace Core {

		using namespace std;
		using namespace Project;
		//using namespace Project::IO;
		using namespace Project::Core;

	//	void ClearGeneral();
		//Инициализация массива констант, переменных, функций и уравнений
		data_list* Init();
		
		/*Функция выполняет проверку/анализ результата заполнения дерева операций и запросов пользователя.
		Возвращает строку с результатом текущей итерации вычислений.*/
		wstring analizer(math_obj* CE);
		/*Функция является точкой входа в вычислительное ядро.
		Возвращает 0 в случае успешного выполнения.
		-1 -  ошибка.*/
		int input_to_analize(data_list* all_math_data);
	}
}
#endif //CORE_H