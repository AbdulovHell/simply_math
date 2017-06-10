#pragma once
#ifndef ERROR_H
#define ERROR_H
#include <string>

namespace Project {
	using namespace std;
	namespace ProjectError {

		enum class ErrorCode : int {
			//Переопределение переменной в качестве константы
			VARBL_BLOCKED = -1,
			//Векторы не поддерживаются в полной мере.
			VECTORS_LIMITED = -3,
			//Уравнения не реализованы
			IS_EQUATION = -4,
			//Функция/уравнение нескольких переменных
			MULTIPLE_VARIABLES = -5,
			//Неверный указатель
			INTERNAL_POINTER_ERR = -6,
			//Пустые скобки ()
			EMTY_BRACKETS = -2,




			//Невозможная ошибка
			UNREAL_ERROR = 0,

			//Строка начинается с '='
			EQUALY_FIRST = 1,
			//В выражении отсутствует знак '='
			EQUALY_MISSING = 2,
			//Не закрыта открывающая скобка
			LBRACKET_NOT_CLOSED = 3,
			//Неопределенная функция
			UNDEFINED_FUNC = 4,
			//Использование переменной в качестве функции
			USING_VAR_AS_FUNC = 5,
			//Имя переменной/функции не может начинаться с цифры
			BAD_NAME = 6,
			//Неожиданное положение скобки в выражении
			UNEXPECTED_BRACKET = 7,
			//Неопределенная переменная
			UNDEFINED_VARIABLE = 8,
			//Неожиданная положение операции в выражении
			UNEXPECTED_OPERATION = 9,
			//Количество открывающих и закрывающих скобок не совпадает
			BRACKET_COUNT = 10,
			//Строка содержит более одного знака '='
			UNEXPECTED_EQUALY = 11,
			//Строка содержит служебные символы
			SERVICE_SYMBOL = 12,
			//Попытка получить результат булева выражения вне условной области
			BOOL_EXPRESSION = 13,
			//Попытка переопределить фундаментальную константу
			FUNDAMENTAL_CONST = 14,
			//После скобки стоит неизвестный символ
			UNEXPECTED_SYMBOL = 15,
			//Неверное количество переменных в вызове функции
			UNEQUAL_NUM_OF_VAR = 16,
			//При вычислении нашлась функция с переменными аргументами
			VARIABL_FUNCT = 17,
			//Размерность вектора не соответствует размерности вектор-функции
			VECTOR_DIMENSIONS = 18,
			//Делить на вектор нельзя!
			VECTOR_DIVISION = 19,
			//Вызов математической функции без аргументов
			FUNDAMENTAL_FUNCT = 20,
			//Присвоение переменной константного выражения
			USING_VAR_AS_CONST = 21
		};

		//Преобразование кода в текстовое представление
		wstring GetProjectError(ErrorCode);

		class _ErrorPresent {
			wstring _textPresent;
			ErrorCode _code;
		public:
			_ErrorPresent();
			_ErrorPresent(ErrorCode code);
			//Конструктор, цепляющий последнюю ошибку программы
			_ErrorPresent(bool last);
			//Представление кода как текст и сохранение в классе
			void TextPresent();
			//Возвращает текст ошибки
			wstring GetErrorWStr();
			//Копирование
			void copy(_ErrorPresent* orig);
		};

		//Передает в класс последнюю ошибку программы
		void GetProjectLastError(Project::ProjectError::_ErrorPresent* errprsnt);
		//Задает код текущей ошибки как последнюю
		void SetProjectLastError(Project::ProjectError::ErrorCode code);
		//Инициализация указателя последней ошибки, не обязательно
		void Init();
	};
}

#endif //ERROR_H
