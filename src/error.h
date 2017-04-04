namespace Project {
	using namespace std;

	public class ProjectError {

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

	private:
		struct _ErrorPresent {
			wstring textPresent;
			ErrorCode code;
		};

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