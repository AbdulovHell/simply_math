#pragma once
#ifndef OPERATIONS_H
#define OPERATIONS_H
#include "math_obj.h"
#include "number.h"
#include "error.h"
namespace Project {
	namespace Core {
		using namespace std;
		class operations :
			public math_obj	{
		protected:
			math_obj* point_left;	//левый операнд
			math_obj* point_right;	//правый операнд
			math_obj* point_collar; //указатель на предыдущую операцию	

			operations* prioritize_rec(int* _p);



		public:		
			
			operations();	
			operations(math_obj* _pl);
			operations(math_obj * _pl, math_obj * _pr);			
			virtual ~operations();
			
			//ћетод get. “»ѕ
			virtual flags get_class_type();

			//ћетод get. ЁЋ≈ћ≈Ќ“
			virtual math_obj* get_this();

			//ћетод get. »ћя
			virtual wstring get_name();
			//ћетод assing. »ћя
			virtual void assing_name(wstring _name);

			//ћетод get. „»—Ћќ
			virtual long double get_num();
			//ћетод assing. „»—Ћќ
			virtual void assing_num(long double _num);

			//ћетод get. ” ј«ј“≈Ћ№ "левый рукав"
			virtual math_obj* get_pl();
			//ћетод assing. ” ј«ј“≈Ћ№ "левый рукав"
			virtual void assing_pl(math_obj* _pointer);


			//ћетод get. ” ј«ј“≈Ћ№ "правый рукав"
			virtual math_obj* get_pr();
			//ћетод assing. ” ј«ј“≈Ћ№ "правый рукав"
			virtual void assing_pr(math_obj* _pointer);


			//ћетод get. ” ј«ј“≈Ћ№ "воротник"
			virtual math_obj* get_pc();
			//ћетод assing. ” ј«ј“≈Ћ№ "воротник"
			virtual void assing_pc(math_obj* _pointer);
			
			//¬озвращает результат выполнени€ операции в виде мат. объекта. 
			virtual math_obj* get_result()=0;

			
			bool define_operation(operations* _high, math_obj* _low, math_obj* _obj);

			operations* prioritize(int _priority);			

			virtual int get_priority() = 0;
		};

		class addition :
			public operations {
		protected:
			//math_obj* point_left;		//наследуетс€ (operations), левый операнд
			//math_obj* point_right;	//наследуетс€ (operations), правый операнд
			//math_obj* point_collar;	//наследуетс€ (operations), указатель на предыдущую операцию
		public:
			addition();
			addition(math_obj* _pl);
			addition(math_obj * _pl, math_obj * _pr);
			addition(operations * _high, math_obj * _low, math_obj * _obj) throw (ProjectError::ErrorCode);

			~addition();
			
			flags get_class_type(); 

			//ћетод get. ЁЋ≈ћ≈Ќ“
			math_obj* get_this();

			//ћетод get. »ћя (символ операции)
			wstring get_name();

			math_obj* get_result();			

			int get_priority();

		};

		class subtraction :
			public operations {
		protected:
			//math_obj* point_left;		//наследуетс€ (operations), левый операнд
			//math_obj* point_right;	//наследуетс€ (operations), правый операнд
			//math_obj* point_collar;	//наследуетс€ (operations), указатель на предыдущую операцию

		public:
			subtraction();
			subtraction(math_obj* _pl);
			subtraction(math_obj * _pl, math_obj * _pr);
			subtraction(operations * _high, math_obj * _low, math_obj * _obj)throw (ProjectError::ErrorCode);
			~subtraction();

			flags get_class_type();

			//ћетод get. ЁЋ≈ћ≈Ќ“
			virtual math_obj* get_this();

			//ћетод get. »ћя (символ операции)
			wstring get_name();

			math_obj* get_result();			

			int get_priority();
		};

		class multiplication :
			public operations {
		protected:
			//math_obj* point_left;		//наследуетс€ (operations), левый операнд
			//math_obj* point_right;	//наследуетс€ (operations), правый операнд
			//math_obj* point_collar;	//наследуетс€ (operations), указатель на предыдущую операцию
		public:
			multiplication();
			multiplication(math_obj* _pl);
			multiplication(math_obj * _pl, math_obj * _pr);
			multiplication(operations * _high, math_obj * _low, math_obj * _obj)throw (ProjectError::ErrorCode);
			~multiplication();
						
			flags get_class_type();

			//ћетод get. ЁЋ≈ћ≈Ќ“
			virtual math_obj* get_this();

			//ћетод get. »ћя (символ операции)
			wstring get_name();

			math_obj* get_result();

			int get_priority();
		};		

		class division :
			public operations {
		protected:
			//math_obj* point_left;		//наследуетс€ (operations), левый операнд
			//math_obj* point_right;	//наследуетс€ (operations), правый операнд
			//math_obj* point_collar;	//наследуетс€ (operations), указатель на предыдущую операцию
		public:
			division();
			division(math_obj* _pl);
			division(math_obj * _pl, math_obj * _pr);
			division(operations * _high, math_obj * _low, math_obj * _obj)throw (ProjectError::ErrorCode);
			~division();
			
			flags get_class_type();

			//ћетод get. ЁЋ≈ћ≈Ќ“
			virtual math_obj* get_this();

			//ћетод get. »ћя (символ операции)
			wstring get_name();

			math_obj* get_result();

			int get_priority();
		};	

		class power :
			public operations {
		protected:
			//math_obj* point_left;		//наследуетс€ (operations), левый операнд
			//math_obj* point_right;	//наследуетс€ (operations), правый операнд
			//math_obj* point_collar;	//наследуетс€ (operations), указатель на предыдущую операцию
		public:
			power();
			power(math_obj* _pl);
			power(math_obj * _pl, math_obj * _pr);
			power(operations * _high, math_obj * _low, math_obj * _obj)throw (ProjectError::ErrorCode);
			~power();
			
			flags get_class_type();

			//ћетод get. ЁЋ≈ћ≈Ќ“
			virtual math_obj* get_this();

			//ћетод get. »ћя (символ операции)
			wstring get_name();

			math_obj* get_result();

			int get_priority();
		};
	}
}

#endif // !OPERATIONS_H




