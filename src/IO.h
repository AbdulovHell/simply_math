#pragma once
#ifndef IO_H
#define IO_H

#include "common.h"
#include "math_obj.h"
#include "error.h"
///Macros
#define GET_FRAC(var) ((double)((double)var-(int)var))
#define GET_INTGR(var) ((int)var)
///

namespace Project {
	namespace IO {
		using namespace std;
		//using namespace Project::Core;

		//сокращение дроби рекурсивно
		void reduce(int* u, int* d);
		//проверка на "целое"
		bool isInteger(double var);
		//проверка на переодичность дроби
		int isntPeriodical(double num);

		enum class var_type : char {
			//Дробное
			FRACTIONAL = 1,
			//Целое округленное
			INTEGER_ROUND = 2,
			//Целая часть
			INTEGER = 3,
			//Дробь с числителем и знаменателем
			TOWER = 4
		};

		//удаление пробелов
		wchar_t* wXchar(wchar_t*);
		//проверка входящей строки на служебные символы, простые ошибки со скобками и знаки равно. удаляет пробелы.
		bool VerifyInput(wchar_t*);
		//возвращает строку с var, представленную как type, с количеством знаков после запятой decimals
		wstring to_string(double, var_type, int);
		//наверное, устрело, см. to_string
		wchar_t* to_char_string(double, var_type, int);
	}
}
#endif //IO_H
