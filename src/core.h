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
				if ((pointer[0].var_id == "*") || (pointer[0].var_id == "/"))
				{
					return expression_processing(pointer[0].point_left) + " " + pointer[0].var_id + " " + expression_processing(pointer[0].point_right);
				}
				else if (pointer[0].var_id == "+")
				{
					if ((pointer[0].point_collar[0].var_id == "*") || (pointer[0].point_collar[0].var_id == "/"))
						return "(" + expression_processing(pointer[0].point_left) + " " + pointer[0].var_id + " " + expression_processing(pointer[0].point_right) + ")";
					else
						return expression_processing(pointer[0].point_left) + " " + pointer[0].var_id + " " + expression_processing(pointer[0].point_right);
				}
				else if (pointer[0].var_id == "0")
				{
					return to_string(pointer[0].var);
				}
				else
				{
					return pointer[0].read("name");
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
		var_const e = var_const("const@e", 2.71828182846);

		vector<var_const>* general_var_const;

		void Init() {
			general_var_const = new vector<var_const>;
			general_var_const->push_back(pi);
			general_var_const->push_back(e);
		}


		var_const* filling_vector(char* pDest, char* endPtr, vector<var_const> *input_var_const,int current_size_of_vect)
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
								input_var_const->push_back(var_const("+", brakets_counter, high_pointer[0].point_right, NULL, high_pointer));
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
							{
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
						input_var_const->push_back(var_const("0", strtod(pDest, &pDest), high_pointer));
						//������ ��������� -> �� ��������� �����
						low_pointer = &input_var_const->at(temp_size_of_vect);
						//������ ����� �������������� �������� �� ��������� �����
						high_pointer[0].point_right = low_pointer;
					}
				}
				else
				{
					temp_size_of_vect = input_var_const->size();
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
							if (input_var_const->at(count).read("type") == "const")
							{
								//���� ������� ���������
								if (p_var == input_var_const->at(count).read("name"))
								{
									if ((high_pointer == NULL) && (low_pointer == NULL))
									{
										//��� ��������� -> �� ����� ��� ����� �� �������, �������� ��� �������� ������� ��� ������ ��������
										high_pointer = &input_var_const->at(count);
										low_pointer = &input_var_const->at(count);
										//����� ����� ����������� ��������� -> ��������� ��������� � ������.
										input_var_const->at(current_size_of_vect).point_left = &input_var_const->at(count);
									}
									else
									{
										//������ ��������� -> �� ����� ��� ����� �� �������
										low_pointer = &input_var_const->at(count);
										//������ ����� �������������� �������� �� ����� ��� ����� �� �������
										high_pointer[0].point_right = low_pointer;
									}
									break;  //�� ����� ������ ������� ������
								}
							}
							//���� ������� ����������
							else if (input_var_const->at(count).read("type") == "varbl")
							{												
								if (p_var == input_var_const->at(count).read("name"))
									//� ��� ������������� ���������� �� ������� ��������� �������
									if (input_var_const->at(current_size_of_vect).point_right == &input_var_const->at(count))
									{
										if ((high_pointer == NULL) && (low_pointer == NULL))
										{
											//��� ��������� -> �� ����� ��� ����� �� �������, �������� ��� �������� ������� ��� ������ ��������
											high_pointer = &input_var_const->at(count);
											low_pointer = &input_var_const->at(count);
											//����� ����� ����������� ��������� -> ��������� ��������� � ������.
											input_var_const->at(current_size_of_vect).point_left = &input_var_const->at(count);
										}
										else
										{
											//������ ��������� -> �� ����� ��� ����� �� �������
											low_pointer = &input_var_const->at(count);
											//������ ����� �������������� �������� �� ����� ��� ����� �� �������
											high_pointer[0].point_right = low_pointer;
										}
										break;  //�� ����� ������ ������� ������
									}
									//� ��� �� ������������� ���������� �� ������� ��������� �������
									else
									{
										//������, �� ������ �� ����� � �������� ���������� ���������� � ��
									}
								
							}
							else if (input_var_const->at(count).read("type") == "funct")
							{
								//���� ������� �������
								if (p_var == input_var_const->at(count).read("name"))
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
							input_var_const->push_back(var_const(temp, 0, &input_var_const->at(current_size_of_vect)));
							//��� ��������� -> �� ����� ��� ����� �� �������, �������� ��� �������� ������� ��� ������ ��������
							high_pointer = &input_var_const->at(temp_size_of_vect);
							low_pointer = &input_var_const->at(temp_size_of_vect);
							//����� ����� ����������� ��������� -> ��������� ��������� � ������.
							input_var_const->at(current_size_of_vect).point_left = &input_var_const->at(count);
						}
						else
						{
							input_var_const->push_back(var_const(temp, 0, &input_var_const->at(current_size_of_vect)));
							//������ ��������� -> �� ����� ��� ����� �� �������
							low_pointer = &input_var_const->at(temp_size_of_vect);
							//������ ����� �������������� �������� �� ����� ��� ����� �� �������
							high_pointer[0].point_right = low_pointer;
						}
						//free(temp);
						if (input_var_const->at(current_size_of_vect).read("type") == "const")
						{
							temp = (char*)malloc(7 * sizeof(char) + strlen(p_var) + strlen(input_var_const->at(current_size_of_vect).read("name").c_str()));
							strcpy(temp, "funct@");
							strcat(temp, &input_var_const->at(current_size_of_vect).read("name")[0]);
							strcat(temp, "(");
							strcat(temp, p_var);
							strcat(temp, ")");
							input_var_const->at(current_size_of_vect).var_id = temp;
							input_var_const->at(current_size_of_vect).point_right = &input_var_const->at(temp_size_of_vect);
							//free(temp);
						}
						else if(input_var_const->at(current_size_of_vect).read("type") == "equat")//���� ������� ��� ����� ��� ������� ���������� ���������
						{
							
						}
						else
						{
							//������, ����� �� ������ �� �������, ������� ��������� �� ������, ��� �������� ����������,
							//�-� ���� f(x)=t+1, ���� ������ ����� - �� �� ����� ������� ���������� ����������
						}

					}
					pDest+=strlen(p_var);
					//free(p_var);
					//free(temp);
				}
			}
			return &input_var_const->at(current_size_of_vect);
		}

		char* analized_output(char* _pDest, char* _endPtr, vector<var_const> *_input_var_const, int _current_size_of_vect)
		{
			var_const* current_type = filling_vector(_pDest, _endPtr, _input_var_const, _current_size_of_vect);
			string expr;
			int output_size;
			if (current_type->read("type") == "const")
			{
				current_type->arithmetic();
				expr = current_type->expresion();
				output_size = strlen(&current_type->var_id[0]) + strlen(&to_string(current_type->var)[0]) + 10 + strlen(expr.c_str());
				char* output = (char*)malloc(output_size);
				for (int i = 0; i < output_size; i++)
					output[i] = 0;
				strcpy(output, expr.c_str());
				strcat(output,"\n");
				strcat(output, current_type->read("name").c_str());
				strcat(output, " = ");
				strcat(output, std::to_string(current_type->var).c_str());
				return output;
			}	
			else if (current_type->read("type") == "funct")
			{				
				expr = current_type->expresion();
				output_size = strlen(&current_type->var_id[0]) + strlen(&to_string(current_type->var)[0]) + 10 + strlen(expr.c_str());
				char* output = (char*)malloc(output_size);
				for (int i = 0; i < output_size; i++)
					output[i] = 0;
				strcpy(output, expr.c_str());
				strcat(output, "\n");
				//strcat(output, current_type->read("name").c_str());
				//strcat(output, " = ");
				//strcat(output, std::to_string(current_type->var).c_str());
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
						general_var_const->push_back(var_const(temp, 0));
						free(temp);
						//���������� �������, ��������� ������ �������  �� ��������� ����������						
						size_of_vect = general_var_const->size();
						temp = (char*)malloc(equal_right - input + 7);
						strcpy(temp, "funct@");
						strcat(temp, equal_left);
						general_var_const->push_back(var_const(temp, 0));
						general_var_const->reserve(input_size * 2 + size_of_vect);
						free(temp);
						general_var_const->at(size_of_vect).point_right = &general_var_const->at(size_of_vect - 1);
						general_var_const->at(size_of_vect - 1).point_collar = &general_var_const->at(size_of_vect);
						point_start = equal_right + 1;

						return analized_output(point_start, point_end, general_var_const, size_of_vect);

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
					if (equal_left == general_var_const->at(count).read("func"))
					{
						if (general_var_const->at(count).read("type") == "const")
						{
							point_start = equal_right + 1;
							return analized_output(point_start, point_end, general_var_const, count);
						}
						else if (general_var_const->at(count).read("type") == "varbl")
						{
							/*temp = (char*)malloc(equal_right - input + 7);
							strcpy(temp, "const@");
							strcat(temp, equal_left);
							general_var_const->at(count).var_id = temp;*/
							point_start = equal_right + 1;
							//free(temp);
							return analized_output(point_start, point_end, general_var_const, count);
						}
						else if (general_var_const->at(count).read("type") == "funct")
						{
							point_start = equal_right + 1;
							return analized_output(point_start, point_end, general_var_const, count);
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
					general_var_const->push_back(var_const(temp, 0));
					general_var_const->reserve(input_size * 2 + size_of_vect);
					free(temp);
					point_start = equal_right + 1;
					return analized_output(point_start, point_end, general_var_const, size_of_vect);
				}				
			}
		}
	}
}