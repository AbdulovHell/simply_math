#include <iostream>
#include <string>
#include <vector>
#include "input.h"

namespace Project {
	namespace Core {

		using namespace std;

		class var_const {
		private:
			var_const *prioritize_processing(var_const *pc, int current_priority)
			{
				//���� ��������� ����������� �������� !������! ������� ��������
				if (pc->get_priority() > current_priority)
				{
					//�������� ����� ��� ��� ��� ��������� ��������
					return prioritize_processing(pc, current_priority);
				}
				//���� ��������� ����������� �������� !������! ��� ����� ���������� ������� ��������
				else
				{
					//������� ��������� �� ����������� ��������
					return pc;
				}
			}

			double processing(var_const *pointer)
			{
				const char* operation = pointer->var_id.c_str();
				switch (*operation) {
				case'+':
					return processing(pointer->point_left) + processing(pointer->point_right);
				case '*':
					return processing(pointer->point_left) * processing(pointer->point_right);
				case '/':
					return processing(pointer->point_left) / processing(pointer->point_right);
					/*case '4':
					return pow(recursion(pointer[0].point_left), recursion(pointer[0].point_right));
					*/
				default:       //��� ��������, ����� � ����������
					return pointer->var;
				}
			}

			string expression_processing(var_const *pointer)
			{
				if ((pointer->var_id == "*") || (pointer->var_id == "/"))
				{
					return expression_processing(pointer->point_left) + " " + pointer->var_id + " " + expression_processing(pointer->point_right);
				}
				else if (pointer->var_id == "+")
				{
					if ((pointer->point_collar->var_id == "*") || (pointer->point_collar->var_id == "/"))
						return "(" + expression_processing(pointer->point_left) + " " + pointer->var_id + " " + expression_processing(pointer->point_right) + ")";
					else
						return expression_processing(pointer->point_left) + " " + pointer->var_id + " " + expression_processing(pointer->point_right);
				}
				else if (pointer->var_id == "0")
				{
					return to_string(pointer->var);
				}
				else
				{
					return pointer->read("name");
				}
			}

		public:
			var_const()
			{
				var_id = "";
				var = 0;
				point_left = NULL;
				point_right = NULL;
				point_collar = NULL;
			}

			var_const(string _name, double _num)
			{
				var_id = _name;
				var = _num;
				point_left = NULL;
				point_right = NULL;
				point_collar = NULL;
			}

			var_const(string _name, double _num, var_const *_pc)
			{
				var_id = _name;
				var = _num;
				point_left = NULL;
				point_right = NULL;
				point_collar = _pc;
			}

			var_const(string _name, double _num, var_const * _pl, var_const *_pr) {
				var_id = _name;
				var = _num;
				point_left = _pl;
				point_right = _pr;
				point_collar = NULL;
			}

			var_const(string _name, double _num, var_const * _pl, var_const *_pr, var_const *_pc) {
				var_id = _name;
				var = _num;
				point_left = _pl;
				point_right = _pr;
				point_collar = _pc;
			}

			void copy(var_const var1)
			{
				var_id = var1.var_id;
				var = var1.var;
				point_left = var1.point_left;
				point_right = var1.point_right;
			}

			int get_priority()
			{
				char operation = var_id[0];
				switch (operation)
				{
				case '+':
					return 1 + (int)var;
				case '*':
					return 2 + (int)var;
				case '/':
					return 3 + (int)var;
				default:
					return 0;
				}
			}
			
			string read(string arg)
			{
				char* id = strstr(&var_id[0], "@");
				char* out = (char*)malloc(5*sizeof(char));
				char* temp = NULL;
				if (id != NULL)
				{
					if (arg == "type")
					{
						strncpy(out, var_id.c_str(), 5);
						out[5] = 0;
						return out;
					}
					else if (arg == "name")
					{
						return id + 1;
					}
					else if (arg == "func")
					{
						strncpy(out, var_id.c_str(), 5);
						out[5] = 0;						
						if (out == "funct")
						{
							free(out);
							temp = strstr(&var_id[0], "(");
							out = (char*)malloc(temp - (id + 1));
							strncpy(out, id + 1, temp - (id + 1));
							return out;
						}
						else //if ((out == "const")||(out == "varbl"))
						{
							return id + 1;
						}
					}
					else
					{
						return "fuck up";
					}
				}
				else
				{
					return var_id;
				}				
			}

			string expresion()
			{
				return read("name") + " = " + expression_processing(point_left);
			}

			var_const *prioritize(int current_priority)
			{
				return prioritize_processing(point_collar, current_priority);
			}

			void arithmetic()
			{
				var = processing(point_left);
			}


			string var_id;
			double var;
			var_const *point_left;		//����� �����
			var_const *point_right;		//������ �����
			var_const *point_collar;	//��������
		};

		var_const pi = var_const("const@pi", 3.1415926535897932384626433832);
		var_const e = var_const("const@e", 2.7182818284590452353602874713527);

		vector<var_const*>* general_var_const;

		void Init() {
			general_var_const = new vector<var_const*>;
			general_var_const->push_back(&pi);
			general_var_const->push_back(&e);
		}

		
		var_const* filling_vector(char* pDest, char* endPtr, var_const* current_elment)
		{			
			int temp_size_of_vect;
			int count;
			var_const *high_pointer = NULL;
			var_const *low_pointer = NULL;
			char* p_var;
			char* temp;
			int brakets_counter = 0;
			while (pDest <= endPtr) {

				if (*pDest == '+')
				{									
					//���� ��� ������ �������� � ���������
					if (high_pointer == low_pointer)
					{
						//���������� ��������, ����� ����� -> �� ���������� �����, �������� - �� ������� ���������
						current_elment->point_left = &var_const("+", brakets_counter, low_pointer, NULL, current_elment);
						current_elment->point_left->var_id = "+";
						//����� ����� ������� ����������� ��������� ��������� �� ��������� ��������
						high_pointer = current_elment->point_left;
					}
					//���� ���� �����-���� �������� �� �����
					else
					{
						//���� ��������� ���������� ������������ �������� !������! ��� �����  ���������� �������
						if (high_pointer->get_priority() <= (brakets_counter + 1))
						{
							//���������� ��������, ����� ����� -> �� ���������� �����, �������� �� ���������� ��������
							high_pointer->point_right = &var_const("+", brakets_counter, low_pointer, NULL, high_pointer);	
							high_pointer->point_right->var_id = "+";
							high_pointer = high_pointer->point_right;                 //������� ��������� -> �� ��������� ��������
						}
						//���� ��������� ���������� ������������ �������� !������! ��� ��������� �������
						else
						{
							//���� ��������� �������� � ���������� ����������� (�� ������� ��������� ����� ����� ���������)  !������! ��� ����� ���������� ������� (�������� �������� � ���������� �����������)
							if (current_elment->point_left->get_priority() >= (brakets_counter + 1))
							{
								//���������� �������� ��� ����� �����, ����� ����� -> �� ���������� ������������ ��������, �������� - �� ������� ���������
								high_pointer = &var_const("+", brakets_counter, current_elment->point_left, NULL, current_elment);
								high_pointer->var_id = "+";
								//�������� ���������� ��������� �������� -> �� ����� ��������
								current_elment->point_left->point_collar = high_pointer;
								//��������� ����� ������� ��������� �� ��������� ��������
								current_elment->point_left = high_pointer;
							}

							//���� ��������� �������� � ���������� ����������� (�� ������� ��������� ����� ����� ���������)  !������! ���������� �������, 
							//�.�. �������� ���������� ���-�� ����� ��������� � ������������

							else
							{
								//�������� ��������� �� ������ �������� � ������� �����, ������� ����� ��������� ������ ��� ������ ���������� ������� 
								high_pointer = high_pointer->prioritize(brakets_counter + 1);
								//���������� ��������, ����� ����� -> ������ ����� ��������� ��������. �������� -> �� ���� ��������� ��������
								//�������� ������� ������ (!) ��������� �������� -> ��������� ��������
								high_pointer->point_right->point_collar = &var_const("+", brakets_counter, high_pointer->point_right, NULL, high_pointer);
								high_pointer->point_right->point_collar->var_id = "+";
								//������ ����� ��������� �������� -> ��������� ��������
								high_pointer->point_right = high_pointer->point_right->point_collar;
								high_pointer = high_pointer->point_right;
							}
						}
					}
					pDest++;
				}
				else if (*pDest == '*')
				{										
					//���� ��� ������ �������� � ���������
					if (high_pointer == low_pointer)
					{
						//���������� ��������, ����� ����� -> �� ���������� �����, �������� - �� ������� ���������
						current_elment->point_left = &var_const("*", brakets_counter, low_pointer, NULL, current_elment);
						current_elment->point_left->var_id = "*";
						//����� ����� ������� ����������� ��������� ��������� �� ��������� ��������
						high_pointer = current_elment->point_left;
					}
					//���� ���� �����-���� �������� �� �����
					else
					{
						//���� ��������� ���������� ������������ �������� !������! ��� �����  ���������� �������
						if (high_pointer->get_priority() <= (brakets_counter + 2))
						{
							//���������� ��������, ����� ����� -> �� ���������� �����, �������� �� ���������� ��������
							high_pointer->point_right = &var_const("*", brakets_counter, low_pointer, NULL, high_pointer);
							high_pointer->point_right->var_id = "*";
							high_pointer = high_pointer->point_right;                 //������� ��������� -> �� ��������� ��������
						}
						//���� ��������� ���������� ������������ �������� !������! ��� ��������� �������
						else
						{
							//���� ��������� �������� � ���������� ����������� (�� ������� ��������� ����� ����� ���������)  !������! ��� ����� ���������� ������� (�������� �������� � ���������� �����������)
							if (current_elment->point_left->get_priority() >= (brakets_counter + 2))
							{
								//���������� �������� ��� ����� �����, ����� ����� -> �� ���������� ������������ ��������, �������� - �� ������� ���������
								high_pointer = &var_const("*", brakets_counter, current_elment->point_left, NULL, current_elment);
								high_pointer->var_id = "*";
								//�������� ���������� ��������� �������� -> �� ����� ��������
								current_elment->point_left->point_collar = high_pointer;
								//��������� ����� ������� ��������� �� ��������� ��������
								current_elment->point_left = high_pointer;
							}

							//���� ��������� �������� � ���������� ����������� (�� ������� ��������� ����� ����� ���������)  !������! ���������� �������, 
							//�.�. �������� ���������� ���-�� ����� ��������� � ������������

							else
							{
								//�������� ��������� �� ������ �������� � ������� �����, ������� ����� ��������� ������ ��� ������ ���������� ������� 
								high_pointer = high_pointer->prioritize(brakets_counter + 2);
								//���������� ��������, ����� ����� -> ������ ����� ��������� ��������. �������� -> �� ���� ��������� ��������
								//�������� ������� ������ (!) ��������� �������� -> ��������� ��������
								high_pointer->point_right->point_collar = &var_const("*", brakets_counter, high_pointer->point_right, NULL, high_pointer);
								high_pointer->point_right->point_collar->var_id = "*";
								//������ ����� ��������� �������� -> ��������� ��������
								high_pointer->point_right = high_pointer->point_right->point_collar;
								high_pointer = high_pointer->point_right;
							}
						}
					}					
					pDest++;
				}
				else if (*pDest == '/')
				{					
					//���� ��� ������ �������� � ���������
					if (high_pointer == low_pointer)
					{
						//���������� ��������, ����� ����� -> �� ���������� �����, �������� - �� ������� ���������
						current_elment->point_left = &var_const("/", brakets_counter, low_pointer, NULL, current_elment);
						current_elment->point_left->var_id = "/";
						//����� ����� ������� ����������� ��������� ��������� �� ��������� ��������
						high_pointer = current_elment->point_left;
					}
					//���� ���� �����-���� �������� �� �����
					else
					{
						//���� ��������� ���������� ������������ �������� !������! ��� �����  ���������� �������
						if (high_pointer->get_priority() <= (brakets_counter + 3))
						{
							//���������� ��������, ����� ����� -> �� ���������� �����, �������� �� ���������� ��������
							high_pointer->point_right = &var_const("/", brakets_counter, low_pointer, NULL, high_pointer);
							high_pointer->point_right->var_id = "/";
							high_pointer = high_pointer->point_right;                 //������� ��������� -> �� ��������� ��������
						}
						//���� ��������� ���������� ������������ �������� !������! ��� ��������� �������
						else
						{
							//���� ��������� �������� � ���������� ����������� (�� ������� ��������� ����� ����� ���������)  !������! ��� ����� ���������� ������� (�������� �������� � ���������� �����������)
							if (current_elment->point_left->get_priority() >= (brakets_counter + 3))
							{
								//���������� �������� ��� ����� �����, ����� ����� -> �� ���������� ������������ ��������, �������� - �� ������� ���������
								high_pointer = &var_const("/", brakets_counter, current_elment->point_left, NULL, current_elment);
								high_pointer->var_id = "/";
								//�������� ���������� ��������� �������� -> �� ����� ��������
								current_elment->point_left->point_collar = high_pointer;
								//��������� ����� ������� ��������� �� ��������� ��������
								current_elment->point_left = high_pointer;
							}

							//���� ��������� �������� � ���������� ����������� (�� ������� ��������� ����� ����� ���������)  !������! ���������� �������, 
							//�.�. �������� ���������� ���-�� ����� ��������� � ������������

							else
							{
								//�������� ��������� �� ������ �������� � ������� �����, ������� ����� ��������� ������ ��� ������ ���������� ������� 
								high_pointer = high_pointer->prioritize(brakets_counter + 3);
								//���������� ��������, ����� ����� -> ������ ����� ��������� ��������. �������� -> �� ���� ��������� ��������
								//�������� ������� ������ (!) ��������� �������� -> ��������� ��������
								high_pointer->point_right->point_collar = &var_const("/", brakets_counter, high_pointer->point_right, NULL, high_pointer);
								high_pointer->point_right->point_collar->var_id = "/";
								//������ ����� ��������� �������� -> ��������� ��������
								high_pointer->point_right = high_pointer->point_right->point_collar;
								high_pointer = high_pointer->point_right;
							}
						}
					}
					pDest++;
				}
				else if (*pDest == '-')
				{
					//�������� ����� ���������� ��� �������� � ������������� ������					
					//���� ��������� ���������� � ������ (������ ����� ����� ������ ��� �����, ����� ����� ������� �� ����������)
					if ((high_pointer == NULL) && (low_pointer == NULL))
					{
						//�������� �������� ������ � ������ �����, �������� -> ���������
						current_elment->point_left = &var_const("0", strtod(pDest, &pDest), current_elment);
						current_elment->point_left->var_id = "0";
						//��� ��������� -> �� �����, �������� ��� �������� ������� ��� ������ ��������
						low_pointer = current_elment->point_left;
						high_pointer = low_pointer;
					}
					//���� ���� ������ �����/�������� ����� �������
					else
					{
						//���� ��� ������ �������� � ��������� (��������)
						if (high_pointer == low_pointer)
						{
							//������� ���������� ��������, ����� ����� -> �� ���������� �����, �������� �� �����
							current_elment->point_left = &var_const("+", brakets_counter, low_pointer, NULL, current_elment);
							current_elment->point_left->var_id = "+";
							high_pointer = current_elment->point_left;							
							high_pointer->point_right = &var_const("0", strtod(pDest, &pDest), high_pointer);
							high_pointer->point_right->var_id = "0";
							low_pointer = high_pointer->point_right;
						}
						//���� ����� ���� ������ ��������
						else
						{
							//���� �������� ��������� ����� � ������ ����� ��������, ������� ����� ������� (�.�. ��������� ����� � �������)
							if (brakets_counter > high_pointer->var)
							{
								//���� ���������� �������� ��������� �� ����� (������ ����� �� ���-�� ���������) �-� 7*(5-2)
								if (high_pointer->point_right != NULL)
								{
									//������� ���������� ��������, ����� ����� -> �� ���������� �����, �������� �� ���������� ��������
									high_pointer->point_right = &var_const("+", brakets_counter, low_pointer, NULL, high_pointer);
									high_pointer->point_right->var_id = "+";
									high_pointer = high_pointer->point_right;
									
									 //�������� �������� ������ � ������ �����, �������� �� ��������� ��������
									 //������ ����� �������������� �������� �� ��������� �����
									high_pointer->point_right = &var_const("0", strtod(pDest, &pDest), high_pointer);
									high_pointer->point_right->var_id = "0";
									//������ ��������� -> �� ��������� �����
									low_pointer = high_pointer->point_right;	
								}
							//���� ���������� �������� �� ��������� �� ����� (������ ����� �� �� ��� �� ���������). 
							//����������� �������� �-�: 7*(-2+5), �.�. ���� ����� ����� ����� ����� ������
								else
								{
									//�������� �������� ������ � ������ �����, �������� �� ���� ��������
									//������ ����� �������������� �������� �� ��������� �����. 
									high_pointer->point_right = &var_const("0", strtod(pDest, &pDest), high_pointer);
									high_pointer->point_right->var_id = "0";
									//������ ��������� -> �� ��������� �����
									low_pointer = high_pointer->point_right;	
								}
							}
							//���� �������� ��������� ����� � ������ ����� ��������, ������� ������ ����� � ��� �� ������ (��� ������ ��� ���)
							else
							{
								//���� ��������� ���������� ������������ �������� !������! ��� �����  ���������� �������
								if (high_pointer->get_priority() <= (brakets_counter + 1))
								{
									//���������� ��������, ����� ����� -> �� ���������� �����, �������� �� ���������� ��������
									high_pointer->point_right = &var_const("+", brakets_counter, low_pointer, NULL, high_pointer);
									high_pointer->point_right->var_id = "+";
									high_pointer = high_pointer->point_right;                 //������� ��������� -> �� ��������� ��������
								}
								//���� ��������� ���������� ������������ �������� !������! ��� ��������� �������
								else
								{
									//���� ��������� �������� � ���������� ����������� (�� ������� ��������� ����� ����� ���������)  !������! ��� ����� ���������� ������� (�������� �������� � ���������� �����������)
									if (current_elment->point_left->get_priority() >= (brakets_counter + 1))
									{
										//���������� �������� ��� ����� �����, ����� ����� -> �� ���������� ������������ ��������, �������� - �� ������� ���������
										high_pointer = &var_const("+", brakets_counter, current_elment->point_left, NULL, current_elment);
										high_pointer->var_id = "+";
										//�������� ���������� ��������� �������� -> �� ����� ��������
										current_elment->point_left->point_collar = high_pointer;
										//��������� ����� ������� ��������� �� ��������� ��������
										current_elment->point_left = high_pointer;
									}

									//���� ��������� �������� � ���������� ����������� (�� ������� ��������� ����� ����� ���������)  !������! ���������� �������, 
									//�.�. �������� ���������� ���-�� ����� ��������� � ������������

									else
									{
										//�������� ��������� �� ������ �������� � ������� �����, ������� ����� ��������� ������ ��� ������ ���������� ������� 
										high_pointer = high_pointer->prioritize(brakets_counter + 1);
										//���������� ��������, ����� ����� -> ������ ����� ��������� ��������. �������� -> �� ���� ��������� ��������
										//�������� ������� ������ (!) ��������� �������� -> ��������� ��������
										high_pointer->point_right->point_collar = &var_const("+", brakets_counter, high_pointer->point_right, NULL, high_pointer);
										high_pointer->point_right->point_collar->var_id = "+";
										//������ ����� ��������� �������� -> ��������� ��������
										high_pointer->point_right = high_pointer->point_right->point_collar;
										high_pointer = high_pointer->point_right;
									}
								}
								
								 //�������� �������� ������ � ������ �����, �������� -> ���� ��������
								 //������ ����� �������������� �������� �� ��������� �����
								high_pointer->point_right = &var_const("0", strtod(pDest, &pDest), high_pointer);
								high_pointer->point_right->var_id = "0";
								//������ ��������� -> �� ��������� �����
								low_pointer = high_pointer->point_right;
							}
						}
					}
				}
				else if (*pDest == '(')
				{                  //����� ������ ����������� ������� ������ �� �����, ������ ������ �������� id �������
					brakets_counter += 3;
					pDest++;
				}
				else if (*pDest == ')')
				{				//������ ������ �������� ������� ������ �� �����, ������ ������ �������� id �������
					brakets_counter -= 3;
					pDest++;
				}
				else if ((*pDest == '1') || (*pDest == '2') || (*pDest == '3') || (*pDest == '4') || (*pDest == '5') || (*pDest == '6') || (*pDest == '7') || (*pDest == '8') || (*pDest == '9') || (*pDest == '0'))
				{                   //����� ����� ����� id "0"					
					// ���� ������ ����� ������ (� �������� ������������) � ������ ���������
					if ((high_pointer == NULL) && (low_pointer == NULL))
					{
						//�������� �������� ������ � ������ �����, �������� -> ���������
						current_elment->point_left = &var_const("0", strtod(pDest, &pDest), current_elment);
						current_elment->point_left->var_id = "0";
						//��� ��������� -> �� �����, �������� ��� �������� ������� ��� ������ ��������
						low_pointer = current_elment->point_left;
						high_pointer = low_pointer;							
					}
					// ���� ������ ����� �� ������ � ������ (���� �����-���� ��������)
					else
					{
						//�������� �������� ������ � ������ �����, �������� -> ���� �������� 
						high_pointer->point_right = &var_const("0", strtod(pDest, &pDest), high_pointer);
						high_pointer->point_right->var_id = "0";
						low_pointer = high_pointer->point_right ;
					}
				}
				else
				{
					temp_size_of_vect = general_var_const->size();
					temp = strpbrk(pDest, ")+-*/^");
					if (temp != NULL)
					{
						p_var = (char*)malloc(temp - pDest + 1);
						strncpy(p_var, pDest, temp - pDest);
						p_var[temp - pDest] = 0;
						temp = NULL;
					}
					else
					{
						p_var = (char*)malloc(endPtr - pDest + 1);
						strcpy(p_var, pDest);						
					}
					//�������� �� �������, ���� ����������/�����/����� � ����� ������
					for (count = 0; count < temp_size_of_vect; count++)
					{
						//��������� ���, ������ ������� - �� ������ ������ t=y+2, ��� ���� � ��� �������� ����� y(x)=5*x
						if ((p_var == general_var_const->at(count)->read("name")) || (p_var == general_var_const->at(count)->read("func")))
						{
							//���� ������� ���������
							if (general_var_const->at(count)->read("type") == "const")
							{
								if ((high_pointer == NULL) && (low_pointer == NULL))
								{
									//��� ��������� -> �� ����� ��� ����� �� �������, �������� ��� �������� ������� ��� ������ ��������
									high_pointer = general_var_const->at(count);
									low_pointer = general_var_const->at(count);
									//����� ����� ����������� ��������� -> ��������� ��������� � ������.
									current_elment->point_left = general_var_const->at(count);
								}
								else
								{
									//������ ��������� -> �� ����� ��� ����� �� �������
									low_pointer = general_var_const->at(count);
									//������ ����� �������������� �������� �� ����� ��� ����� �� �������
									high_pointer->point_right = low_pointer;
								}
								break;  //�� ����� ������ ������� ������
							}
							//���� ������� ����������
							else if (general_var_const->at(count)->read("type") == "varbl")
							{
								//� ��� ������������� ���������� �� ������� ��������� �������
								if (current_elment->point_right == general_var_const->at(count))
								{
									if ((high_pointer == NULL) && (low_pointer == NULL))
									{
										//��� ��������� -> �� ����� ��� ����� �� �������, �������� ��� �������� ������� ��� ������ ��������
										high_pointer = general_var_const->at(count);
										low_pointer = general_var_const->at(count);
										//����� ����� ����������� ��������� -> ��������� ��������� � ������.
										current_elment->point_left = general_var_const->at(count);
									}
									else
									{
										//������ ��������� -> �� ����� ��� ����� �� �������
										low_pointer = general_var_const->at(count);
										//������ ����� �������������� �������� �� ����� ��� ����� �� �������
										high_pointer->point_right = low_pointer;
									}
									break;  //�� ����� ������ ������� ������
								}
								//� ��� �� ������������� ���������� �� ������� ��������� �������
								else
								{
									//������, �� ������ �� ����� � �������� ���������� ���������� � ��
								}

							}
							//���� ������� �������	
							else if (general_var_const->at(count)->read("type") == "funct")
							{
								
								break;//���� ������. � ������ ������ ��� �������� ����� ������� � ������, ���������� ��������� ������������ ���������� � �����
							}
						}
					}
					//���� � ������� ��� ������ � ����� ������, ������� ����� ����������
					if (count == temp_size_of_vect)
					{
						temp = (char*)malloc(6 * sizeof(char) + strlen(p_var));
						strcpy(temp, "varbl@");
						strcat(temp, p_var);
						if ((high_pointer == NULL) && (low_pointer == NULL))
						{
							general_var_const->push_back(&var_const(temp, 0, current_elment));
							general_var_const->at(temp_size_of_vect)->var_id = temp;
							//��� ��������� -> �� ����� ��� ����� �� �������, �������� ��� �������� ������� ��� ������ ��������
							high_pointer = general_var_const->at(temp_size_of_vect);
							low_pointer = general_var_const->at(temp_size_of_vect);
							//����� ����� ����������� ��������� -> ��������� ��������� � ������.
							current_elment->point_left = general_var_const->at(temp_size_of_vect);
						}
						else
						{
							general_var_const->push_back(&var_const(temp, 0, current_elment));
							general_var_const->at(temp_size_of_vect)->var_id = temp;
							//������ ��������� -> �� ����� ��� ����� �� �������
							low_pointer = general_var_const->at(temp_size_of_vect);
							//������ ����� �������������� �������� �� ����� ��� ����� �� �������
							high_pointer->point_right = low_pointer;
						}
						//free(temp);
						if (current_elment->read("type") == "const")
						{
							temp = (char*)malloc(7 * sizeof(char) + strlen(p_var) + strlen(current_elment->read("name").c_str()));
							strcpy(temp, "funct@");
							strcat(temp, current_elment->read("name").c_str());
							strcat(temp, "(");
							strcat(temp, p_var);
							strcat(temp, ")");
							current_elment->var_id = temp;
							current_elment->point_right = low_pointer;
							//free(temp);
						}
						else if(current_elment->read("type") == "equat")//���� ������� ��� ����� ��� ������� ���������� ���������
						{
							
						}
						else
						{
							//������, ����� �� ������ �� �������, ������� ��������� �� ������, ��� �������� ����������,
							//�-� ���� f(x)=t+1, ���� ������ ����� - �� �� ����� ������� ���������� ����������
						}

					}
					pDest+=strlen(p_var);
					free(p_var);
					free(temp);
				}
			}
			return current_elment;
		}

		char* analized_output(char* _pDest, char* _endPtr, var_const* _current_elment)
		{
			_current_elment = filling_vector(_pDest, _endPtr, _current_elment);
			string expr;
			int output_size;
			if (_current_elment->read("type") == "const")
			{
				_current_elment->arithmetic();
				expr = _current_elment->expresion();
				output_size = strlen(_current_elment->var_id.c_str()) + strlen(to_string(_current_elment->var).c_str()) + 10 + strlen(expr.c_str());
				char* output = (char*)malloc(output_size);
				for (int i = 0; i < output_size; i++)
					output[i] = 0;
				strcpy(output, expr.c_str());
				strcat(output,"\n");
				strcat(output, _current_elment->read("name").c_str());
				strcat(output, " = ");
				strcat(output, std::to_string(_current_elment->var).c_str());
				return output;
				
			}	
			else if (_current_elment->read("type") == "funct")
			{				
				expr = _current_elment->expresion();
				output_size = strlen(_current_elment->var_id.c_str()) + strlen(to_string(_current_elment->var).c_str()) + 10 + strlen(expr.c_str());
				char* output = (char*)malloc(output_size);
				for (int i = 0; i < output_size; i++)
					output[i] = 0;
				strcpy(output, expr.c_str());
				strcat(output, "\n");
				//strcat(output, _current_elment->read("name").c_str());
				//strcat(output, " = ");
				//strcat(output, std::to_string(_current_elment->var).c_str());
				return output;
			}
			
			
			//������ ��� ������ � ���� ������ ��� ������ �����
			//cout << input_var_const->at(current_size_of_vect).expresion() << endl;
			
		}

		char* input_to_analize(char* input)
		{
			char* error_str = Project::Input::VerifyInput(input);
			if (error_str != NULL)
				return error_str;

			int input_size = strlen(input);
			int size_of_vect;
			int count;
			int  brackets_left = 0;
			int brackets_right = 0;
			char* point_start = input;	//start pointer
			char* point_end = input + strlen(input) - 1;	//end pointer			
			
			char* equal_right = strstr(input, "="); // ����� � ������ �� �����
			char* temp;
			char *equal_left = (char*)malloc(equal_right - input + 1); //����� �� �����
			memcpy(equal_left, input, equal_right - input);
			equal_left[equal_right - input] = 0;
			vector<var_const*>* k = general_var_const;
			//���� ������ ����� ����� ������ ���
			if (equal_right == point_end)
			{

			}
			//���� ������ ���-�� ����
			//���� ����� ���� ��������
			else if (strpbrk(equal_left, "+*/^") != NULL)
			{
			
			}
			//���� ����� ���� �����
			else if (strstr(equal_left, "-") != NULL)
			{

			}
			//��� ��������������
			else if (strstr(equal_left, "(") != NULL)
			{
				if (strstr(equal_left, ")") == NULL)
				{
					                                 //error
				}
				else 
				{
					temp = equal_left;
					while (temp <= equal_right)
					{
						if (strstr(temp, "(") != NULL)
						{
							brackets_left++;
							temp = strstr(temp, "(") + 1;
						}
					}
					temp = equal_left;
					while(temp <= equal_right)
					{
						if (strstr(temp, ")") != NULL)
						{
							brackets_right++;
							temp = strstr(temp, ")") + 1;
						}							
					}
					if ((brackets_left > 1)||(brackets_right > 1)||(brackets_right != brackets_left))
					{
						//error
					}
					//����� �� ����� ��� ��������, �� ���� ���� ������ � ���� ����� ������ � ��� ����
					else if (strpbrk(equal_left, "0123456789") == NULL)
					{
						//���������� ���������� - ��, ��� ����� � �������
						//���������� ����� �������� �������, ����������� ���������� �� ��� ������ ���������� � �������
						//���� ���������� - ������ �� ���������� - ������ ������� ����� ������� �� ��
						size_of_vect = general_var_const->size();
						temp = (char*)malloc(strstr(temp, ")") - strstr(temp, "(") + 7);
						strcpy(temp, "varbl@");
						strcat(temp, strstr(equal_left, "(") + 1);
						temp[strstr(temp, ")") - temp] = 0;
						general_var_const->push_back(&var_const(temp, 0));
						general_var_const->at(general_var_const->size() - 1)->var_id = temp;
						free(temp);
						//���������� �������, ��������� ������ �������  �� ��������� ����������						
						size_of_vect = general_var_const->size();
						temp = (char*)malloc(equal_right - input + 7);
						strcpy(temp, "funct@");
						strcat(temp, equal_left);
						general_var_const->push_back(&var_const(temp, 0));
						general_var_const->at(general_var_const->size() - 1)->var_id = temp;
						//general_var_const->reserve(input_size * 2 + size_of_vect);
						free(temp);
						general_var_const->at(size_of_vect)->point_right = general_var_const->at(size_of_vect - 1);
						general_var_const->at(size_of_vect - 1)->point_collar = general_var_const->at(size_of_vect);
						point_start = equal_right + 1;

						return analized_output(point_start, point_end, general_var_const->at(size_of_vect));

					}
					//����� �� ����� ��� ��������, �� ���� ���� ������ � ���� ����� ������ � �����
					else
					{
						//��� ���� ����������� � ������������������� �������
						//��� ����� ��� ���������� �������� ������� � ����� (������ ���� f(3.5)), ����� ������� f ��� ����������
						//�� ���� ����� ��� ��������� ������ ����� ����� ����� ������ ���, �.�. ������ ������� � �������� ���������
						//�� ��� ����� ���� ����������, �������� f(3.5) = t (������� ����� ��������� ����� ���������)
					}
				}
			}
			else if (strpbrk(equal_left, "0123456789") != NULL)
			{

			}
			//����� �� ����� ����� ������ ����� 
			else
			{				
				size_of_vect = general_var_const->size();
				for (count = 0; count < size_of_vect; count++)
				{
					if (equal_left == general_var_const->at(count)->read("func"))
					{
						if (general_var_const->at(count)->read("type") == "const")
						{
							point_start = equal_right + 1;
							return analized_output(point_start, point_end, general_var_const->at(count));
						}
						else if (general_var_const->at(count)->read("type") == "varbl")
						{
							temp = (char*)malloc(equal_right - input + 7);
							strcpy(temp, "const@");
							strcat(temp, equal_left);
							general_var_const->at(count)->var_id = temp;
							point_start = equal_right + 1;
							free(temp);
							return analized_output(point_start, point_end, general_var_const->at(count));
						}
						else if (general_var_const->at(count)->read("type") == "funct")
						{
							point_start = equal_right + 1;
							return analized_output(point_start, point_end, general_var_const->at(count));
						}
						else
						{
							return "fuck up";//��� ����� ��� ��� ��� ����� ����
						}
					}
				}
				if (count == size_of_vect)
				{
					temp = (char*)malloc(equal_right - input + 7);
					strcpy(temp, "const@");
					strcat(temp, equal_left);					
					general_var_const->push_back(&var_const(temp, 0));
					general_var_const->at(general_var_const->size() - 1)->var_id = temp;
					//general_var_const->reserve(input_size * 2 + size_of_vect);
					free(temp);
					point_start = equal_right + 1;
					return analized_output(point_start, point_end, general_var_const->at(size_of_vect));
				}				
			}
		}
	}
}