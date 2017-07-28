#pragma once
#ifndef OPERATIONS_H
#define OPERATIONS_H
#include "math_obj.h"
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
			operations(math_obj* _high, math_obj* _low);
			virtual ~operations();
			
			//ћетод get. “»ѕ
			flags get_class_type();

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
			addition(math_obj* _high, math_obj* _low);
			~addition();
			
			//flags get_class_type(); //— точки зрени€ построени€ дерева операций не имеет значени€ кака€ именно операци€ находитс€ в узле, только сам факт того, что это операци€

			//ћетод get. ЁЋ≈ћ≈Ќ“
			math_obj* get_this();

			//ћетод get. »ћя (символ операции)
			wstring get_name();

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
			subtraction(math_obj* _high, math_obj* _low);
			~subtraction();

			//flags get_class_type();

			//ћетод get. ЁЋ≈ћ≈Ќ“
			virtual math_obj* get_this();

			//ћетод get. »ћя (символ операции)
			wstring get_name();

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
			multiplication(math_obj* _high, math_obj* _low);
			~multiplication();
						
			//flags get_class_type();

			//ћетод get. ЁЋ≈ћ≈Ќ“
			virtual math_obj* get_this();

			//ћетод get. »ћя (символ операции)
			wstring get_name();

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
			division(math_obj* _high, math_obj* _low);
			~division();
			
			//flags get_class_type();

			//ћетод get. ЁЋ≈ћ≈Ќ“
			virtual math_obj* get_this();

			//ћетод get. »ћя (символ операции)
			wstring get_name();

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
			power(math_obj* _high, math_obj* _low);
			~power();
			
			//flags get_class_type();

			//ћетод get. ЁЋ≈ћ≈Ќ“
			virtual math_obj* get_this();

			//ћетод get. »ћя (символ операции)
			wstring get_name();

			math_obj* get_result();
		};
	}
}

#endif // !OPERATIONS_H




