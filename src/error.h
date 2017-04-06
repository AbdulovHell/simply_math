namespace Project {
	using namespace std;

    static class ProjectError {

	public:
		enum class ErrorCode : int {
			ERROR1 = 1,
			ERROR2 = 2
		};

		static wstring GetProjectError(ErrorCode code) {
			switch (code)
			{
			case ErrorCode::ERROR1:
				return L"Error1";
			case ErrorCode::ERROR2:
				return L"Error2";
			default:
				return L"Unresolved error";
			}
		}

        class _ErrorPresent {
            wstring _textPresent;
            ErrorCode _code;
public:
            _ErrorPresent(){}
            _ErrorPresent(ErrorCode code){
                _code=code;
            }
            void TextPresent(){
                _textPresent=GetProjectError(_code);
            }
            wstring GetErrorWStr(){
                _textPresent=GetProjectError(_code);
                return _textPresent;
            }
            void copy(_ErrorPresent* orig){
                _code=orig->_code;
                _textPresent=orig->_textPresent;
            }
		};

    private:
    Project::ProjectError::_ErrorPresent* last_err;
    bool NewLastError;

    public:
    void GetProjectLastError(Project::ProjectError::_ErrorPresent* errprsnt){
        errprsnt->copy(last_err);
        NewLastError=false;
    }
    void SetProjectLastError(Project::ProjectError::ErrorCode code){
        if(last_err) delete last_err;
        last_err=new Project::ProjectError::_ErrorPresent(code);
        NewLastError=true;
    }
    void Init(){
        Project::ProjectError::last_err=new Project::ProjectError::_ErrorPresent();
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
