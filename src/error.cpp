#include "error.h"

namespace Project {
	using namespace std;

	namespace ProjectError {

		Project::ProjectError::_ErrorPresent* last_err;
		int NewLastError;
		//Преобразование кода в текстовое представление
		wstring GetProjectError(ErrorCode code) {
			switch (code)
			{
			case ErrorCode::MATH_ERROR:
				return L"Ошибка мат. вычислений";
			case ErrorCode::UNDEFINED_FUNC:
				return L"Неопределенная функция";
			case ErrorCode::VECTORS_LIMITED:
				return L"Часть математических вычислений не поддерживаются в полной мере.";
			case ErrorCode::IS_EQUATION:
				return L"Уравнения не реализованы";
			case ErrorCode::MULTIPLE_VARIABLES:
				return L"Функция/уравнение нескольких переменных";
			case ErrorCode::INTERNAL_POINTER_ERR:
				return L"Неверный указатель";
			case ErrorCode::EMTY_BRACKETS:
				return L"Пустые скобки ()";
			case ErrorCode::SING_OF_EQUALITY_FIRST:
				return L"Строка начинается с '='";
			case ErrorCode::SING_OF_EQUALITY_MISSING:
				return L"В выражении отсутствует знак '='";
			case ErrorCode::LBRACKET_NOT_CLOSED:
				return L"Не закрыта открывающая скобка";
			case ErrorCode::USING_VAR_AS_FUNC:
				return L"Использование переменной в качестве функции";
			case ErrorCode::OPERATIONS_CONSTRUCT_FAILED:
				return L"Конструктор операции завершился ошибкой";
			case ErrorCode::UNEXPECTED_BRACKET:
				return L"Неожиданное положение скобки в выражении";
			case ErrorCode::UNDEFINED_VARIABLE:
				return L"Неопределенная переменная";
			case ErrorCode::UNEXPECTED_OPERATION:
				return L"Неожиданная положение операции в выражении";
			case ErrorCode::BRACKET_COUNT:
				return L"Количество открывающих и закрывающих скобок не совпадает";
			case ErrorCode::UNEXPECTED_EQUALY:
				return L"Строка содержит более одного знака '='";
			case ErrorCode::SERVICE_SYMBOL:
				return L"Строка содержит служебные символы";
			case ErrorCode::BOOL_EXPRESSION:
				return L"Попытка получить результат булева выражения вне условной области";
			case ErrorCode::FUNDAMENTAL_CONST:
				return L"Попытка переопределить фундаментальную константу";
			case ErrorCode::UNEXPECTED_SYMBOL:
				return L"Неизвестный символ";
			case ErrorCode::UNEQUAL_NUM_OF_VAR:
				return L"Неверное количество переменных в вызове функции";
			case ErrorCode::VARIABL_FUNCT:
				return L"При вычислении нашлась функция с переменными аргументами";
			case ErrorCode::VECTOR_DIMENSIONS:
				return L"Размерность вектора не соответствует размерности вектор-функции";
			case ErrorCode::VECTOR_DIVISION:
				return L"Делить на вектор нельзя! Возводить в векторную степень нельзя!!";
			case ErrorCode::FUNDAMENTAL_FUNCT:
				return L"Вызов математической функции без аргументов";
			case ErrorCode::USING_VAR_AS_CONST:
				return L"Присвоение переменной константного выражения";
			case ErrorCode::UNREAL_ERROR:
				return L"Не удалось создать математический объект по строке ввода.";
			default:
				return L"Unresolved error";
			}
		}

		_ErrorPresent::_ErrorPresent() {}
		_ErrorPresent::_ErrorPresent(ErrorCode code) {
			_code = code;
		}
		//Конструктор, цепляющий последнюю ошибку программы
		_ErrorPresent::_ErrorPresent(bool last) {
			if (last) {
				ProjectError::_ErrorPresent* err = new ProjectError::_ErrorPresent();
				ProjectError::GetProjectLastError(err);
				_textPresent = err->_textPresent;
				_code = err->_code;
				delete err;
			}
		}
		//Представление кода как текст и сохранение в классе
		void _ErrorPresent::TextPresent() {
			_textPresent = GetProjectError(_code);
		}
		//Возвращает текст ошибки
		wstring _ErrorPresent::GetErrorWStr() {
			_textPresent = GetProjectError(_code);
			return _textPresent;
		}
		//Копирование
		void _ErrorPresent::copy(_ErrorPresent* orig) {
			_code = orig->_code;
			_textPresent = orig->_textPresent;
		}

		ErrorCode _ErrorPresent::get_code()
		{
			return _code;
		}

		//Передает в класс последнюю ошибку программы
		void GetProjectLastError(Project::ProjectError::_ErrorPresent* errprsnt) {
			errprsnt->copy(last_err);
			NewLastError = false;
		}
		//Задает код текущей ошибки как последнюю
		void SetProjectLastError(Project::ProjectError::ErrorCode code) {
			if (last_err) delete last_err;
			last_err = new Project::ProjectError::_ErrorPresent(code);
			NewLastError = true;
		}
		//Инициализация указателя последней ошибки, не обязательно
		void Init() {
			Project::ProjectError::last_err = new Project::ProjectError::_ErrorPresent();
		}
	};
}
