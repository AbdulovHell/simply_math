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
				if (pc[0].get_priority() > current_priority)
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
				char operation = pointer[0].var_id[0];
				switch (operation) {				
				case'+':
					return processing(pointer[0].point_left) + processing(pointer[0].point_right);
				case '*':
					return processing(pointer[0].point_left) * processing(pointer[0].point_right);
				case '/':
					return processing(pointer[0].point_left) / processing(pointer[0].point_right);
					/*case '4':
					return pow(recursion(pointer[0].point_left), recursion(pointer[0].point_right));
					*/
				default:       //��� ��������, ����� � ����������
					return pointer[0].var;
				}
			}

			string expression_processing(var_const *pointer)
			{
				if ((pointer[0].var_id == "+") || (pointer[0].var_id == "*") || (pointer[0].var_id == "/"))
				{
					return "(" + expression_processing(pointer[0].point_left) + " " + pointer[0].var_id + " " + expression_processing(pointer[0].point_right) + ")";
				}				
				else if (pointer[0].var_id == "0")
				{
					return to_string(pointer[0].var);
				}
				else
				{
					return pointer[0].var_id;
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
			

			string expresion()
			{
				return var_id + " = " + expression_processing(point_left);
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

		var_const pi = var_const("pi", 3.1415926535897932384626433832);
		var_const e = var_const("e", 2.71828182846);
		
		char* input_to_analize(char* input)
		{
			char* error_str = Project::Input::VerifyInput(input);
			if (error_str != NULL)
				return error_str;

			int input_size = strlen(input);

			char* pDest = input;	//start pointer
			char* endPtr = input + strlen(input) - 1;	//end pointer
			char *temp = NULL;

			vector<var_const>* input_var_const = new vector<var_const>;
			input_var_const->push_back(pi);
			input_var_const->push_back(e);

			var_const *high_pointer = NULL;
			var_const *low_pointer = NULL;

			char* varnameDest = strstr(input, "=");

			char *varname = (char*)malloc(varnameDest - input + 1);
			memcpy(varname, input, varnameDest - input);
			varname[varnameDest - input] = 0;

			int current_size_of_vect = input_var_const->size(); //����� 2, ������ �������� �������� �������� ��������
			input_var_const->push_back(var_const(varname, 0));
			input_var_const->reserve(input_size * 2 + current_size_of_vect);

			int temp_size_of_vect;
			pDest = varnameDest + 1;
			var_const *k = &input_var_const->at(current_size_of_vect);	

			int brakets_counter = 0;
			while (pDest <= endPtr) {

				if (*pDest == '+')
				{
					//�������� �������� ����� id "1"	
					temp_size_of_vect = input_var_const->size();
					//���� ��� ������ �������� � ���������
					if (high_pointer == low_pointer)
					{
						//���������� ��������, ����� ����� -> �� ���������� �����, �������� - �� ������� ���������
						input_var_const->push_back(var_const("+", brakets_counter, low_pointer, NULL, &input_var_const->at(current_size_of_vect))); 
						 //����� ����� ������� ����������� ��������� ��������� �� ��������� ��������
						input_var_const->at(current_size_of_vect).point_left = &input_var_const->at(temp_size_of_vect);
						high_pointer = &input_var_const->at(temp_size_of_vect);// ������� ��������� -- �� ��������� ��������
					}
					//���� ���� �����-���� �������� �� �����
					else
					{
						//���� ��������� ���������� ������������ �������� !������! ��� �����  ���������� �������
						if (high_pointer[0].get_priority() <= (brakets_counter + 1))
						{
							//���������� ��������, ����� ����� -> �� ���������� �����, �������� �� ���������� ��������
							input_var_const->push_back(var_const("+", brakets_counter, low_pointer, NULL, high_pointer));  
							high_pointer[0].point_right = &input_var_const->at(temp_size_of_vect);  //������ ����� ���������� -> �� ���������
							high_pointer = &input_var_const->at(temp_size_of_vect);                 //������� ��������� -> �� ��������� ��������
						}
						//���� ��������� ���������� ������������ �������� !������! ��� ��������� �������
						else
						{							
							//���� ��������� �������� � ���������� ����������� (�� ������� ��������� ����� ����� ���������)  !������! ��� ����� ���������� ������� (�������� �������� � ���������� �����������)
							if (input_var_const->at(current_size_of_vect).point_left[0].get_priority() >= (brakets_counter + 1))
							{
								//���������� �������� ��� ����� �����, ����� ����� -> �� ���������� ������������ ��������, �������� - �� ������� ���������
								input_var_const->push_back(var_const("+", brakets_counter, input_var_const->at(current_size_of_vect).point_left, NULL, &input_var_const->at(current_size_of_vect)));
								//�������� ���������� ��������� �������� -> �� ����� ��������
								input_var_const->at(current_size_of_vect).point_left[0].point_collar = &input_var_const->at(temp_size_of_vect);
								//��������� ����� ������� ��������� �� ��������� ��������
								input_var_const->at(current_size_of_vect).point_left = &input_var_const->at(temp_size_of_vect);								
							}

							//���� ��������� �������� � ���������� ����������� (�� ������� ��������� ����� ����� ���������)  !������! ���������� �������, 
							//�.�. �������� ���������� ���-�� ����� ��������� � ������������
							
							else
							{
								//�������� ��������� �� ������ �������� � ������� �����, ������� ����� ��������� ������ ��� ������ ���������� ������� 
								high_pointer = high_pointer[0].prioritize(brakets_counter + 1);
								//���������� ��������, ����� ����� -> ������ ����� ��������� ��������. �������� -> �� ���� ��������� ��������
								input_var_const->push_back(var_const("+", brakets_counter,high_pointer[0].point_right,NULL, high_pointer));
								//�������� ������� ������ (!) ��������� �������� -> ��������� ��������
								high_pointer[0].point_right[0].point_collar = &input_var_const->at(temp_size_of_vect);
								//������ ����� ��������� �������� -> ��������� ��������
								high_pointer[0].point_right = &input_var_const->at(temp_size_of_vect);
							}
							high_pointer = &input_var_const->at(temp_size_of_vect);  //������� ��������� �� ��������� ��������

						}
					}
					pDest++;
				}
				else if (*pDest == '*')
				{
					//�������� �������� ����� id "1"	
					temp_size_of_vect = input_var_const->size();
					//���� ��� ������ �������� � ���������
					if (high_pointer == low_pointer)
					{
						//���������� ��������, ����� ����� -> �� ���������� �����, �������� - �� ������� ���������
						input_var_const->push_back(var_const("*", brakets_counter, low_pointer, NULL, &input_var_const->at(current_size_of_vect)));
						//����� ����� ������� ����������� ��������� ��������� �� ��������� ��������
						input_var_const->at(current_size_of_vect).point_left = &input_var_const->at(temp_size_of_vect);
						high_pointer = &input_var_const->at(temp_size_of_vect);// ������� ��������� -- �� ��������� ��������
					}
					//���� ���� �����-���� �������� �� �����
					else
					{
						//���� ��������� ���������� ������������ �������� !������! ��� �����  ���������� �������
						if (high_pointer[0].get_priority() <= (brakets_counter + 2))
						{
							//���������� ��������, ����� ����� -> �� ���������� �����, �������� �� ���������� ��������
							input_var_const->push_back(var_const("*", brakets_counter, low_pointer, NULL, high_pointer));
							high_pointer[0].point_right = &input_var_const->at(temp_size_of_vect);  //������ ����� ���������� -> �� ���������
							high_pointer = &input_var_const->at(temp_size_of_vect);                 //������� ��������� -> �� ��������� ��������
						}
						//���� ��������� ���������� ������������ �������� !������! ��� ��������� �������
						else
						{
							//���� ��������� �������� � ���������� ����������� (�� ������� ��������� ����� ����� ���������)  !������!  ��� ����� ���������� ������� (�������� �������� � ���������� �����������)
							if (input_var_const->at(current_size_of_vect).point_left[0].get_priority() >= (brakets_counter + 2))
							{
								//���������� �������� ��� ����� �����, ����� ����� -> �� ���������� ������������ ��������, �������� - �� ������� ���������
								input_var_const->push_back(var_const("*", brakets_counter, input_var_const->at(current_size_of_vect).point_left, NULL, &input_var_const->at(current_size_of_vect)));
								//�������� ���������� ��������� �������� -> �� ����� ��������
								input_var_const->at(current_size_of_vect).point_left[0].point_collar = &input_var_const->at(temp_size_of_vect);
								//��������� ����� ������� ��������� �� ��������� ��������
								input_var_const->at(current_size_of_vect).point_left = &input_var_const->at(temp_size_of_vect);


							}

							//���� ��������� �������� � ���������� ����������� (�� ������� ��������� ����� ����� ���������)  !������! ���������� �������, 
							//�.�. �������� ���������� ���-�� ����� ��������� � ������������
							else
							{
								//�������� ��������� �� ������ �������� � ������� �����, ������� ����� ��������� ������ ��� ������ ���������� ������� 
								high_pointer = high_pointer[0].prioritize(brakets_counter + 2);
								//���������� ��������, ����� ����� -> ������ ����� ��������� ��������. �������� -> �� ���� ��������� ��������
								input_var_const->push_back(var_const("*", brakets_counter, high_pointer[0].point_right, NULL, high_pointer));
								//�������� ������� ������ (!) ��������� �������� -> ��������� ��������
								high_pointer[0].point_right[0].point_collar = &input_var_const->at(temp_size_of_vect);
								//������ ����� ��������� �������� -> ��������� ��������
								high_pointer[0].point_right = &input_var_const->at(temp_size_of_vect);
							}
							high_pointer = &input_var_const->at(temp_size_of_vect);  //������� ��������� �� ��������� ��������

						}
					}					
					pDest++;
				}
				else if (*pDest == '/')
				{
					//�������� �������� ����� id "1"	
					temp_size_of_vect = input_var_const->size();
					//���� ��� ������ �������� � ���������
					if (high_pointer == low_pointer)
					{
						//���������� ��������, ����� ����� -> �� ���������� �����, �������� - �� ������� ���������
						input_var_const->push_back(var_const("/", brakets_counter, low_pointer, NULL, &input_var_const->at(current_size_of_vect)));
						//����� ����� ������� ����������� ��������� ��������� �� ��������� ��������
						input_var_const->at(current_size_of_vect).point_left = &input_var_const->at(temp_size_of_vect);
						high_pointer = &input_var_const->at(temp_size_of_vect);// ������� ��������� -- �� ��������� ��������
					}
					//���� ���� �����-���� �������� �� �����
					else
					{
						//���� ��������� ���������� ������������ �������� !������! ��� �����  ���������� �������
						if (high_pointer[0].get_priority() <= (brakets_counter + 3))
						{
							//���������� ��������, ����� ����� -> �� ���������� �����, �������� �� ���������� ��������
							input_var_const->push_back(var_const("/", brakets_counter, low_pointer, NULL, high_pointer));
							high_pointer[0].point_right = &input_var_const->at(temp_size_of_vect);  //������ ����� ���������� -> �� ���������
							high_pointer = &input_var_const->at(temp_size_of_vect);                 //������� ��������� -> �� ��������� ��������
						}
						//���� ��������� ���������� ������������ �������� !������! ��� ��������� �������
						else
						{
							//���� ��������� �������� � ���������� ����������� (�� ������� ��������� ����� ����� ���������)  !������!  ��� ����� ���������� ������� (�������� �������� � ���������� �����������)
							if (input_var_const->at(current_size_of_vect).point_left[0].get_priority() >= (brakets_counter + 3))
							{
								//���������� �������� ��� ����� �����, ����� ����� -> �� ���������� ������������ ��������, �������� - �� ������� ���������
								input_var_const->push_back(var_const("/", brakets_counter, input_var_const->at(current_size_of_vect).point_left, NULL, &input_var_const->at(current_size_of_vect)));
								//�������� ���������� ��������� �������� -> �� ����� ��������
								input_var_const->at(current_size_of_vect).point_left[0].point_collar = &input_var_const->at(temp_size_of_vect);
								//��������� ����� ������� ��������� �� ��������� ��������
								input_var_const->at(current_size_of_vect).point_left = &input_var_const->at(temp_size_of_vect);


							}

							//���� ��������� �������� � ���������� ����������� (�� ������� ��������� ����� ����� ���������)  !������! ���������� �������, 
							//�.�. �������� ���������� ���-�� ����� ��������� � ������������
							else
							{
								//�������� ��������� �� ������ �������� � ������� �����, ������� ����� ��������� ������ ��� ������ ���������� ������� 
								high_pointer = high_pointer[0].prioritize(brakets_counter + 3);
								//���������� ��������, ����� ����� -> ������ ����� ��������� ��������. �������� -> �� ���� ��������� ��������
								input_var_const->push_back(var_const("/", brakets_counter, high_pointer[0].point_right, NULL, high_pointer));
								//�������� ������� ������ (!) ��������� �������� -> ��������� ��������
								high_pointer[0].point_right[0].point_collar = &input_var_const->at(temp_size_of_vect);
								//������ ����� ��������� �������� -> ��������� ��������
								high_pointer[0].point_right = &input_var_const->at(temp_size_of_vect);
							}
							high_pointer = &input_var_const->at(temp_size_of_vect);  //������� ��������� �� ��������� ��������

						}
					}
					pDest++;
				}
				else if (*pDest == '-')
				{
					//�������� ����� ���������� ��� �������� � ������������� ������, id "1"
					temp_size_of_vect = input_var_const->size();
					//���� ��������� ���������� � ������ (������ ����� ����� ������ ��� �����, ����� ����� ������ �� ����������)
					if ((high_pointer == NULL) && (low_pointer == NULL))
					{
						//�������� �������� ������ � ������ �����, �������� -> �� �����
						input_var_const->push_back(var_const("0", strtod(pDest, &pDest), &input_var_const->at(current_size_of_vect))); 
						 //��� ��������� -> �� �����, �������� ��� �������� ������� ��� ������ ��������
						high_pointer = &input_var_const->at(temp_size_of_vect);
						low_pointer = &input_var_const->at(temp_size_of_vect);
						//����� ����� ����������� ��������� -> ��������� ��������� � ������.
						input_var_const->at(current_size_of_vect).point_left = &input_var_const->at(temp_size_of_vect);
					}
					//���� ���� ������ �����/�������� ����� �������
					else
					{
						//���� ��� ������ �������� � ��������� (��������)
						if (high_pointer == low_pointer)
						{
							//������� ���������� ��������, ����� ����� -> �� ���������� �����, �������� �� �����
							input_var_const->push_back(var_const("+", brakets_counter, low_pointer, NULL, &input_var_const->at(current_size_of_vect)));
							input_var_const->at(current_size_of_vect).point_left = &input_var_const->at(temp_size_of_vect);//����� ����� ������� ����������� ��������� ��������� �� ��������� ��������
							high_pointer = &input_var_const->at(temp_size_of_vect);// ������� ��������� -- �� ��������� ��������
							temp_size_of_vect = input_var_const->size(); //������� ������� �� ������� ������������� ��� ��� ��� ������ ������ �����
							 //�������� �������� ������ � ������ �����, �������� -> �� ���� ��������
							input_var_const->push_back(var_const("0", strtod(pDest, &pDest), high_pointer));
							 //������ ��������� -> �� ��������� �����
							low_pointer = &input_var_const->at(temp_size_of_vect);
							//������ ����� �������������� �������� �� ��������� �����
							high_pointer[0].point_right = low_pointer;
						}
						//���� ����� ���� ������ ��������
						else
						{
							//���� �������� ��������� ����� � ������ ����� ��������, ������� ����� ������� (�.�. ��������� ����� � �������)
							if (brakets_counter > high_pointer[0].var)
								//���� ���������� �������� ��������� �� ����� (������ ����� �� ���-�� ���������)
								if (high_pointer[0].point_right != NULL)
								{
									//������� ���������� ��������, ����� ����� -> �� ���������� �����, �������� �� ���������� ��������
									input_var_const->push_back(var_const("+", brakets_counter, low_pointer, NULL, high_pointer));
									//��������� ���������� �������� �� �����, ������ ����� �� ����� �������� ��������
									high_pointer[0].point_right = &input_var_const->at(temp_size_of_vect);
									high_pointer = &input_var_const->at(temp_size_of_vect);// ������� ��������� -- �� ��������� ��������
									temp_size_of_vect = input_var_const->size(); //������� ������� �� ������� ������������� ��� ��� ��� ������ ������ �����
									 //�������� �������� ������ � ������ �����, �������� �� ��������� ��������
									input_var_const->push_back(var_const("0", strtod(pDest, &pDest), high_pointer));
									//������ ��������� -> �� ��������� �����
									low_pointer = &input_var_const->at(temp_size_of_vect);
									//������ ����� �������������� �������� �� ��������� �����
									high_pointer[0].point_right = &input_var_const->at(temp_size_of_vect);
								}
							//���� ���������� �������� �� ��������� �� ����� (������ ����� �� �� ��� �� ���������). 
							//����������� �������� �-�: 7*(-2+5), �.�. ���� ����� ����� ����� ����� ������
								else
								{
									//�������� �������� ������ � ������ �����, �������� �� ���� ��������
									input_var_const->push_back(var_const("0", strtod(pDest, &pDest), high_pointer));
									//������ ��������� -> �� ��������� �����
									low_pointer = &input_var_const->at(temp_size_of_vect);
									//������ ����� �������������� �������� �� ��������� �����. 
									high_pointer[0].point_right = low_pointer;
								}
							//���� �������� ��������� ����� � ������ ����� ��������, ������� ������ ����� � ��� �� ������ (��� ������ ��� ���)
							else
							{
								//���� ��������� ���������� ������������ �������� !������! ��� �����  ���������� �������
								if (high_pointer[0].get_priority() <= (brakets_counter + 1))
								{
									//���������� ��������, ����� ����� -> �� ���������� �����, �������� �� ���� ��������
									input_var_const->push_back(var_const("+", brakets_counter, low_pointer, NULL, high_pointer));  
									high_pointer[0].point_right = &input_var_const->at(temp_size_of_vect);  //������ ����� ���������� -> �� ���������
									high_pointer = &input_var_const->at(temp_size_of_vect);                 //������� ��������� -> �� ��������� ��������
								}
								//���� ��������� ���������� ������������ �������� !������! ��� ��������� �������
								else
								{
									//���� ��������� �������� � ���������� ����������� (�� ������� ��������� ����� ����� ���������)  !������!  ��� ����� ���������� �������
									if (input_var_const->at(current_size_of_vect).point_left[0].get_priority() >= (brakets_counter + 1))
									{
										//���������� �������� ��� ����� �����, ����� ����� -> �� ���������� ������������ ��������, �������� - �� ������� ���������
										input_var_const->push_back(var_const("+", brakets_counter, input_var_const->at(current_size_of_vect).point_left, NULL, &input_var_const->at(current_size_of_vect)));
										//�������� ���������� ��������� �������� -> �� ����� ��������
										input_var_const->at(current_size_of_vect).point_left[0].point_collar = &input_var_const->at(temp_size_of_vect);
										//��������� ����� ������� ��������� �� ��������� ��������
										input_var_const->at(current_size_of_vect).point_left = &input_var_const->at(temp_size_of_vect);


									}
									
									//���� ��������� �������� � ���������� ����������� (�� ������� ��������� ����� ����� ���������)  !������! ���������� �������, 
									//�.�. �������� ���������� ���-�� ����� ��������� � ������������
									
									else
									{
										//�������� ��������� �� ������ �������� � ������� �����, ������� ����� ��������� ������ ��� ������ ���������� ������� 
										high_pointer = high_pointer[0].prioritize(brakets_counter + 1);
										//���������� ��������, ����� ����� -> ������ ����� ��������� ��������. �������� -> �� ���� ��������� ��������
										input_var_const->push_back(var_const("+", brakets_counter, high_pointer[0].point_right, NULL, high_pointer));
										//�������� ������� ������ (!) ��������� �������� -> ��������� ��������
										high_pointer[0].point_right[0].point_collar = &input_var_const->at(temp_size_of_vect);
										//������ ����� ��������� �������� -> ��������� ��������
										high_pointer[0].point_right = &input_var_const->at(temp_size_of_vect);
									}
									high_pointer = &input_var_const->at(temp_size_of_vect);  //������� ��������� �� ��������� ��������			
								}
								temp_size_of_vect = input_var_const->size(); //������� ������� �� ������� ������������� ��� ��� ��� ������ ������ �����
								//�������� �������� ������ � ������ �����, �������� -> ���� ��������
								input_var_const->push_back(var_const("0", strtod(pDest, &pDest), high_pointer));
								//������ ��������� -> �� ��������� �����
								low_pointer = &input_var_const->at(temp_size_of_vect);
								//������ ����� �������������� �������� �� ��������� �����
								high_pointer[0].point_right = &input_var_const->at(temp_size_of_vect);
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
					temp_size_of_vect = input_var_const->size();	
					
					 // ���� ������ ����� ������ (� �������� ������������) � ������ ���������
					if ((high_pointer == NULL) && (low_pointer == NULL))
					{
						//�������� �������� ������ � ������ �����, �������� -> ���������
						input_var_const->push_back(var_const("0", strtod(pDest, &pDest), &input_var_const->at(current_size_of_vect)));
						//��� ��������� -> �� �����, �������� ��� �������� ������� ��� ������ ��������
						high_pointer = &input_var_const->at(temp_size_of_vect);
						low_pointer = &input_var_const->at(temp_size_of_vect);
						//����� ����� ����������� ��������� -> ��������� ��������� � ������.
						input_var_const->at(current_size_of_vect).point_left = &input_var_const->at(temp_size_of_vect);
					}
					// ���� ������ ����� �� ������ � ������ (���� �����-���� ��������)
					else
					{
						//�������� �������� ������ � ������ �����, �������� -> ���� �������� 
						input_var_const->push_back(var_const("0", strtod(pDest, &pDest),high_pointer));
						//������ ��������� -> �� ��������� �����
						low_pointer = &input_var_const->at(temp_size_of_vect);
						//������ ����� �������������� �������� �� ��������� �����
						high_pointer[0].point_right = low_pointer;
					}
				}
			}

			input_var_const->at(current_size_of_vect).arithmetic();
			//cout << input_var_const->at(current_size_of_vect).var_id << " = " << input_var_const->at(current_size_of_vect).var << endl;
			//��� ��������� ����������� ���������
			cout << input_var_const->at(current_size_of_vect).expresion()<<endl;
			int output_size = input_var_const->at(current_size_of_vect).var_id.size() + std::to_string(input_var_const->at(current_size_of_vect).var).size() + 50;
			char* output = (char*)malloc(output_size);
			for (int i = 0;i < output_size;i++)
				output[i] = 0;

			strcpy(output, input_var_const->at(current_size_of_vect).var_id.c_str());
			strcat(output, " = ");
			strcat(output, std::to_string(input_var_const->at(current_size_of_vect).var).c_str());

			return output;	
		}

	}
}