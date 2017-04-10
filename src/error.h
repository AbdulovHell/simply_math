namespace Project {
	using namespace std;

	namespace ProjectError {

	//public:
		
		enum class ErrorCode : int {
			COMPLEX_FUNC_NOT_REALIZED = -1,
			//UNDEFINED_FUNC = -2,
			IMPLICIT_FUNC =-3,
			IS_EQUATION =-4,
			MULTIPLE_VARIABLES=-5,
			INTERNAL_POINTER_ERR=-6,
			NEGATIVE_FUNC=-7,
			
			EQUALY_FIRST=1,
			EQUALY_MISSING=2,
			LBRACKET_NOT_CLOSED=3,
			UNDEFINED_FUNC=4,
			USING_VAR_AS_FUNC=5,
			BAD_NAME=6,
			UNEXPECTED_BRACKET=7,
			UNDEFINED_VARIABLE=8,
			UNEXPECTED_OPERATION=9
		};

		static wstring GetProjectError(ErrorCode code) {
			switch (code)
			{
			case ErrorCode::COMPLEX_FUNC_NOT_REALIZED:
				return L"Вложенные/сложные функции не реализованы.";
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
			void TextPresent() {
				_textPresent = GetProjectError(_code);
			}
			wstring GetErrorWStr() {
				_textPresent = GetProjectError(_code);
				return _textPresent;
			}
			void copy(_ErrorPresent* orig) {
				_code = orig->_code;
				_textPresent = orig->_textPresent;
			}
		};

	//protected:
		static Project::ProjectError::_ErrorPresent* last_err;
		static int NewLastError;
			
	//public:
		static void GetProjectLastError(Project::ProjectError::_ErrorPresent* errprsnt) {
			errprsnt->copy(last_err);
			NewLastError = false;
		}
		static void SetProjectLastError(Project::ProjectError::ErrorCode code) {
			if (last_err) delete last_err;
			last_err = new Project::ProjectError::_ErrorPresent(code);
			NewLastError = true;
		}
		static void Init() {
			Project::ProjectError::last_err = new Project::ProjectError::_ErrorPresent();
		}

	};

}

//void testExp() {
//	throw(Project::ProjectError::ErrorCode::ERROR2);
//}

//try {
//	testExp();
//}
//catch (Project::ProjectError::ErrorCode code) {
//	wstring str = Project::ProjectError::GetProjectError(code);
//}
