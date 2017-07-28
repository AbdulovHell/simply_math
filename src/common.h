
//Header для подключения стандартных библиотек и определения общих типов и функций
#pragma once
#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <wchar.h>
#include <stdio.h>
#include <math.h>



namespace Project {
	namespace Core {
		using namespace std;
		enum class flags : int16_t
		{
			//типы мат. объектов
			error = -10,
			math_dummy = -2,
			variable = -1,

			base = 0,

			number = 1,
			complex = 2,
			//qwauterion = 3

			constant = 10,
			constant_cmplx = 11,

			expression = 20,
			function = 21, //undef
			funct_defnd = 22,
			funct_arg_c = 23,
			funct_arg_v = 24,


			equation = 30,

			vector = 40,

			matrix = 50,

			operation = 60,

			addit = 61,
			minus = 62,
			mltpl = 63,
			divis = 64,
			power = 65,
		};
	}
}


#endif // !COMMON_H
