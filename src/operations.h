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
			leaf_ptr point_left;	//левый операнд math_obj*
			leaf_ptr point_right;	//правый операнд
			leaf_ptr point_collar; //указатель на предыдущую операцию	

			leaf_ptr prioritize_rec(int* _p);



		public:		
			
			operations();	
			operations(leaf_ptr _pl);
			operations(leaf_ptr _pl, leaf_ptr _pr);
			virtual ~operations();
			
			//ћетод get. “»ѕ
			virtual flags get_class_type();

			//ћетод get. –ј«ћ≈–
			virtual size_t get_sizeof();			
			
			//ћетод get. ” ј«ј“≈Ћ№ "левый рукав"
			virtual leaf_ptr get_pl();
			//ћетод assing. ” ј«ј“≈Ћ№ "левый рукав"
			virtual void assing_pl(leaf_ptr& _pointer);


			//ћетод get. ” ј«ј“≈Ћ№ "правый рукав"
			virtual leaf_ptr get_pr();
			//ћетод assing. ” ј«ј“≈Ћ№ "правый рукав"
			virtual void assing_pr(leaf_ptr _pointer);


			//ћетод get. ” ј«ј“≈Ћ№ "воротник"
			virtual leaf_ptr get_pc();
			//ћетод assing. ” ј«ј“≈Ћ№ "воротник"
			virtual void assing_pc(leaf_ptr _pointer);
			
			//¬озвращает результат выполнени€ операции в виде мат. объекта. 
			virtual math_obj* get_result()=0;
			
			bool define_operation(leaf_ptr _high, leaf_ptr _low, leaf_ptr _obj, leaf_ptr _this);

			leaf_ptr prioritize(int _priority);
			
			
		};

		class addition :
			public operations {
		protected:
			//uint32_t point_left;		//наследуетс€ (operations), левый операнд
			//uint32_t point_right;	//наследуетс€ (operations), правый операнд
			//uint32_t point_collar;	//наследуетс€ (operations), указатель на предыдущую операцию
		public:
			addition();
			addition(leaf_ptr _pl);
			addition(leaf_ptr _pl, leaf_ptr _pr);
			addition(leaf_ptr _high, leaf_ptr _low, leaf_ptr _obj, leaf_ptr _this) throw (...);

			~addition();
			
			flags get_class_type(); 

			//ћетод get. ЁЋ≈ћ≈Ќ“
			//math_obj* get_this();

			//ћетод get. »ћя (символ операции)
			wstring get_name();

			math_obj* get_result();			

			int get_priority();

			virtual void copy_to(void * _ptr);

			virtual math_obj* copy(math_obj* _original);

		};

		class subtraction :
			public operations {
		protected:
			//uint32_t point_left;		//наследуетс€ (operations), левый операнд
			//uint32_t point_right;	//наследуетс€ (operations), правый операнд
			//uint32_t point_collar;	//наследуетс€ (operations), указатель на предыдущую операцию

		public:
			subtraction();
			subtraction(leaf_ptr _pl);
			subtraction(leaf_ptr _pl, leaf_ptr _pr);
			subtraction(leaf_ptr _pl, leaf_ptr _pr, leaf_ptr _pc);
			subtraction(leaf_ptr _high, leaf_ptr _low, leaf_ptr _obj, leaf_ptr _this)throw (...);
			~subtraction();

			flags get_class_type();

			//ћетод get. ЁЋ≈ћ≈Ќ“
			//virtual math_obj* get_this();

			//ћетод get. »ћя (символ операции)
			wstring get_name();

			math_obj* get_result();			

			int get_priority();

			virtual void copy_to(void * _ptr);

			virtual math_obj* copy(math_obj* _original);
		};

		class multiplication :
			public operations {
		protected:
			//uint32_t point_left;		//наследуетс€ (operations), левый операнд
			//uint32_t point_right;	//наследуетс€ (operations), правый операнд
			//uint32_t point_collar;	//наследуетс€ (operations), указатель на предыдущую операцию
		public:
			multiplication();
			multiplication(leaf_ptr _pl);
			multiplication(leaf_ptr _pl, leaf_ptr _pr);
			multiplication(leaf_ptr _high, leaf_ptr _low, leaf_ptr _obj, leaf_ptr _this)throw (...);
			~multiplication();
						
			flags get_class_type();

			//ћетод get. ЁЋ≈ћ≈Ќ“
			//virtual math_obj* get_this();

			//ћетод get. »ћя (символ операции)
			wstring get_name();

			math_obj* get_result();

			int get_priority();

			virtual void copy_to(void * _ptr);

			virtual math_obj* copy(math_obj* _original);
		};		

		class division :
			public operations {
		protected:
			//uint32_t point_left;		//наследуетс€ (operations), левый операнд
			//uint32_t point_right;	//наследуетс€ (operations), правый операнд
			//uint32_t point_collar;	//наследуетс€ (operations), указатель на предыдущую операцию
		public:
			division();
			division(leaf_ptr _pl);
			division(leaf_ptr _pl, leaf_ptr _pr);
			division(leaf_ptr _high, leaf_ptr _low, leaf_ptr _obj, leaf_ptr _this)throw (...);
			~division();
			
			flags get_class_type();

			//ћетод get. ЁЋ≈ћ≈Ќ“
			//virtual math_obj* get_this();

			//ћетод get. »ћя (символ операции)
			wstring get_name();

			math_obj* get_result();

			int get_priority();

			virtual void copy_to(void * _ptr);

			virtual math_obj* copy(math_obj* _original);
		};	

		class power :
			public operations {
		protected:
			//uint32_t point_left;		//наследуетс€ (operations), левый операнд
			//uint32_t point_right;	//наследуетс€ (operations), правый операнд
			//uint32_t point_collar;	//наследуетс€ (operations), указатель на предыдущую операцию
		public:
			power();
			power(leaf_ptr _pl);
			power(leaf_ptr _pl, leaf_ptr _pr);
			power(leaf_ptr _high, leaf_ptr _low, leaf_ptr _obj, leaf_ptr _this) throw (...);
			~power();
			
			flags get_class_type();

			//ћетод get. ЁЋ≈ћ≈Ќ“
			//virtual math_obj* get_this();

			//ћетод get. »ћя (символ операции)
			wstring get_name();

			math_obj* get_result();

			int get_priority();

			virtual void copy_to(void * _ptr);

			virtual math_obj* copy(math_obj* _original);
		};
	}
}

#endif // !OPERATIONS_H




