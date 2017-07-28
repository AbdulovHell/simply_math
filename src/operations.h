#pragma once
#ifndef OPERATIONS_H
#define OPERATIONS_H
#include "classes.h"
namespace Project {
	namespace Core {
		using namespace std;
		class operations :
			public math_obj	{
		protected:
			math_obj* point_left;	//левый операнд
			math_obj* point_right;	//правый операнд
			//data_list* point_up;  //наследуетс€ (math_obj), по умолчанию не используетс€.
		public:		
			
			operations();	
			operations(math_obj* _pl);
			operations(math_obj* _pl, math_obj* _pr);
			virtual ~operations();
			
			//ћетод get. ” ј«ј“≈Ћ№ "левый рукав"
			math_obj* get_pl();
			//ћетод assing. ” ј«ј“≈Ћ№ "левый рукав"
			void assing_pl(math_obj* _pointer);

			//ћетод get. ” ј«ј“≈Ћ№ "правый рукав"
			math_obj* get_pr();
			//ћетод assing. ” ј«ј“≈Ћ№ "правый рукав"
			void assing_pr(math_obj* _pointer);

			virtual flags get_class_type();			
			
			//¬озвращает результат выполнени€ операции в ввиде мат. объекта. Nullptr при вызове непосредственно дл€ этого класса
			virtual math_obj* get_result();
		};

		class addition :
			public operations {
		protected:
			//math_obj* point_left;		//наследуетс€ (operations), левый операнд
			//math_obj* point_right;	//наследуетс€ (operations), правый операнд
			//data_list* point_up;		//наследуетс€ (math_obj), по умолчанию не используетс€.
		public:
			addition();
			addition(math_obj* _pl);
			addition(math_obj* _pl, math_obj* _pr);
			~addition();
			
			flags get_class_type();

			//ћетод get. ЁЋ≈ћ≈Ќ“
			virtual math_obj* get_this();

			math_obj* get_result();
		};

		class subtraction :
			public operations {
		protected:
			//math_obj* point_left;		//наследуетс€ (operations), левый операнд
			//math_obj* point_right;	//наследуетс€ (operations), правый операнд
			//data_list* point_up;		//наследуетс€ (math_obj), по умолчанию не используетс€.

		public:
			subtraction();
			subtraction(math_obj* _pl);
			subtraction(math_obj* _pl, math_obj* _pr);
			~subtraction();

			flags get_class_type();

			//ћетод get. ЁЋ≈ћ≈Ќ“
			virtual math_obj* get_this();

			math_obj* get_result();
		};

		class multiplication :
			public operations {
		protected:
			//math_obj* point_left;		//наследуетс€ (operations), левый операнд
			//math_obj* point_right;	//наследуетс€ (operations), правый операнд
			//data_list* point_up;		//наследуетс€ (math_obj), по умолчанию не используетс€.
		public:
			multiplication();
			multiplication(math_obj* _pl);
			multiplication(math_obj* _pl, math_obj* _pr);
			~multiplication();
						
			flags get_class_type();

			//ћетод get. ЁЋ≈ћ≈Ќ“
			virtual math_obj* get_this();

			math_obj* get_result();
		};		

		class division :
			public operations {
		protected:
			//math_obj* point_left;		//наследуетс€ (operations), левый операнд
			//math_obj* point_right;	//наследуетс€ (operations), правый операнд
			//data_list* point_up;		//наследуетс€ (math_obj), по умолчанию не используетс€.
		public:
			division();
			division(math_obj* _pl);
			division(math_obj* _pl, math_obj* _pr);
			~division();
			
			flags get_class_type();

			//ћетод get. ЁЋ≈ћ≈Ќ“
			virtual math_obj* get_this();

			math_obj* get_result();
		};	

		class power :
			public operations {
		protected:
			//math_obj* point_left;		//наследуетс€ (operations), левый операнд
			//math_obj* point_right;	//наследуетс€ (operations), правый операнд
			//data_list* point_up;		//наследуетс€ (math_obj), по умолчанию не используетс€.
		public:
			power();
			power(math_obj* _pl);
			power(math_obj* _pl, math_obj* _pr);
			~power();
			
			flags get_class_type();

			//ћетод get. ЁЋ≈ћ≈Ќ“
			virtual math_obj* get_this();

			math_obj* get_result();
		};
	}
}

#endif // !OPERATIONS_H




