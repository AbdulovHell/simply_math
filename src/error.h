namespace Project {
	using namespace std;

	namespace ProjectError {

		enum class ErrorCode : int {
			//Вложенные/сложные функции не реализованы
			COMPLEX_FUNC_NOT_REALIZED = -1,
			//Неявное вложение функции
			IMPLICIT_FUNC = -3,
			//Уравнения не реализованы
			IS_EQUATION = -4,
			//Функция/уравнение нескольких переменных
			MULTIPLE_VARIABLES = -5,
			//Неверный указатель
			INTERNAL_POINTER_ERR = -6,
			//Наличие '-' перед функцией
			NEGATIVE_FUNC = -7,
			

			

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
		};

		//Преобразование кода в текстовое представление
		static wstring GetProjectError(ErrorCode code) {
			switch (code)
			{
			case ErrorCode::COMPLEX_FUNC_NOT_REALIZED:
				return L"Вложенные/сложные функции не реализованы";
			case ErrorCode::UNDEFINED_FUNC:
				return L"Неопределенная функция";
			case ErrorCode::IMPLICIT_FUNC:
				return L"Неявное вложение функции";
			case ErrorCode::IS_EQUATION:
				return L"Уравнения не реализованы";
			case ErrorCode::MULTIPLE_VARIABLES:
				return L"Функция/уравнение нескольких переменных";
			case ErrorCode::INTERNAL_POINTER_ERR:
				return L"Неверный указатель";
			case ErrorCode::NEGATIVE_FUNC:
				return L"Наличие '-' перед функцией";
			case ErrorCode::EQUALY_FIRST:
				return L"Строка начинается с '='";
			case ErrorCode::EQUALY_MISSING:
				return L"В выражении отсутствует знак '='";
			case ErrorCode::LBRACKET_NOT_CLOSED:
				return L"Не закрыта открывающая скобка";
			case ErrorCode::USING_VAR_AS_FUNC:
				return L"Использование переменной в качестве функции";
			case ErrorCode::BAD_NAME:
				return L"Имя переменной/функции не может начинаться с цифры";
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
			case ErrorCode::UNREAL_ERROR:
				return L"Невозможная ошибка!";
			default:
				return L"Unresolved error";
			}
		}

		class _ErrorPresent {
			wstring _textPresent;
			ErrorCode _code;
		public:
			_ErrorPresent() {}
			_ErrorPresent(ErrorCode code) {
				_code = code;
			}
			//Конструктор, цепляющий последнюю ошибку программы
			/*_ErrorPresent(bool last) {
				if (last) {
					ProjectError::_ErrorPresent* err = new ProjectError::_ErrorPresent();
					ProjectError::GetProjectLastError(err);
					_textPresent= err->_textPresent;
					_code = err->_code;
					delete err;
				}
			}*/
			//Представление кода как текст и сохранение в классе
			void TextPresent() {
				_textPresent = GetProjectError(_code);
			}
			//Возвращает текст ошибки
			wstring GetErrorWStr() {
				_textPresent = GetProjectError(_code);
				return _textPresent;
			}
			//Копирование
			void copy(_In_ _ErrorPresent* orig) {
				_code = orig->_code;
				_textPresent = orig->_textPresent;
			}
		};

		//protected:
		static Project::ProjectError::_ErrorPresent* last_err;
		static int NewLastError;

		//Передает в класс последнюю ошибку программы
		static void GetProjectLastError(_Out_ Project::ProjectError::_ErrorPresent* errprsnt) {
			errprsnt->copy(last_err);
			NewLastError = false;
		}
		//Задает код текущей ошибки как последнюю
		static void SetProjectLastError(_In_ Project::ProjectError::ErrorCode code) {
			if (last_err) delete last_err;
			last_err = new Project::ProjectError::_ErrorPresent(code);
			NewLastError = true;
		}
		//Инициализация указателя последней ошибки, не обязательно
		static void Init() {
			Project::ProjectError::last_err = new Project::ProjectError::_ErrorPresent();
		}

	};

}

#pragma region Пример 1

//void testExp() {
//	throw(Project::ProjectError::ErrorCode::ERROR2);
//}

//try {
//	testExp();
//}
//catch (Project::ProjectError::ErrorCode code) {
//	wstring str = Project::ProjectError::GetProjectError(code);
//}

#pragma endregion
