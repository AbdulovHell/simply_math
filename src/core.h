#include <iostream>
#include <string>
#include <vector>
#include "input.h"

namespace Project {
	namespace Core {

		using namespace std;

		class var_const {
		public:
			var_const()
			{
				var_id = "";
				var = 0;
				point_left = NULL;
				point_right = NULL;
			}

			var_const(string _name, double _num)
			{
				var_id = _name;
				var = _num;
				point_left = NULL;
				point_right = NULL;
			}

			var_const(string _name, double _num, var_const * _pl, var_const *pr) {
				var_id = _name;
				var = _num;
				point_left = _pl;
				point_right = pr;
			}

			void copy(var_const var1)
			{
				var_id = var1.var_id;
				var = var1.var;
				point_left = var1.point_left;
				point_right = var1.point_right;
			}

			double recursion(var_const *pointer)
			{
				char operation = pointer[0].var_id[0];
				switch (operation) {
				case '0':
					return pointer[0].var;
				case'1':
					return recursion(pointer[0].point_left) + recursion(pointer[0].point_right);
				case '2':
					return recursion(pointer[0].point_left) * recursion(pointer[0].point_right);
				case '3':
					return recursion(pointer[0].point_left) / recursion(pointer[0].point_right);
					/*case '4':
					return pow(recursion(pointer[0].point_left), recursion(pointer[0].point_right));
					*/
				}
			}

			void arithmetic()
			{
				var = recursion(point_left);
			}


			string var_id;
			double var;
			var_const *point_left;
			var_const *point_right;
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
			input_var_const->reserve(input_size * 2);

			int temp_size_of_vect;
			pDest = varnameDest + 1;
			var_const *k = &input_var_const->at(current_size_of_vect);	//������ ��� ���� 4ewnfjk
																		//current_size_of_vect ����� ��������� ����� ���� �����������))


			int brakets_counter = 0;
			while (pDest <= endPtr) {

				if (*pDest == '+')
				{
					//�������� �������� ����� id "1"	
					temp_size_of_vect = input_var_const->size();
					//���� ��� ������ �������� � ���������
					if (high_pointer == low_pointer)
					{
						input_var_const->push_back(var_const("1", brakets_counter, low_pointer, NULL));  //���������� ��������, ����� ����� -> �� ���������� �����
						input_var_const->at(current_size_of_vect).point_left = &input_var_const->at(temp_size_of_vect);//����� ����� ������� ����������� ��������� ��������� �� ��������� ��������
						high_pointer = &input_var_const->at(temp_size_of_vect);// ������� ��������� -- �� ��������� ��������
					}
					//���� ���� �����-���� �������� �� �����
					else
					{
						//���� ��������� ���������� ������������ �������� !������! ��� �����  ���������� �������
						if ((strtod(high_pointer[0].var_id.c_str(), &temp) + high_pointer[0].var) <= (brakets_counter + 1))
						{
							input_var_const->push_back(var_const("1", brakets_counter, low_pointer, NULL));  //���������� ��������, ����� ����� -> �� ���������� �����
							high_pointer[0].point_right = &input_var_const->at(temp_size_of_vect);  //������ ����� ���������� -> �� ���������
							high_pointer = &input_var_const->at(temp_size_of_vect);                 //������� ��������� -> �� ��������� ��������
						}
						//���� ��������� ���������� ������������ �������� !������! ��� ��������� �������
						else
						{
							//input_var_const->at(temp_size_of_vect) = var_const("1", brakets_counter, high_pointer, NULL);  //���������� ��������, ����� ����� -> �� ���������� ��������

							//���� ��������� �������� � ���������� ����������� (�� ������� ��������� ����� ����� ���������)  !������! ���������� �������
							if ((strtod(input_var_const->at(current_size_of_vect).point_left[0].var_id.c_str(), &temp) + input_var_const->at(current_size_of_vect).point_left[0].var) > (brakets_counter + 1))
							{
								//���������� �������� ��� ����� �����, ����� ����� -> �� ���������� ������������ ��������
								input_var_const->push_back(var_const("1", brakets_counter, input_var_const->at(current_size_of_vect).point_left, NULL));
								//��������� ����� ������� ���������� �� ��������� ��������
								input_var_const->at(current_size_of_vect).point_left = &input_var_const->at(temp_size_of_vect);


							}

							//���� ��������� �������� � ���������� ����������� (�� ������� ��������� ����� ����� ���������)  !������! ���������� �������, 
							//�.�. �������� ���������� ���-�� ����� ��������� � ������������
							else
							{
								input_var_const->at(current_size_of_vect).point_left[0].point_right = &input_var_const->at(temp_size_of_vect);//��� �� �����������
							}
							high_pointer = &input_var_const->at(temp_size_of_vect);  //������� ��������� �� ��������� ��������

						}
					}
					pDest++;
				}
				else if (*pDest == '*')
				{                     //�������� ��������� ����� id "2"		
					temp_size_of_vect = input_var_const->size();
					//���� ��� ������ �������� � ���������
					if (high_pointer == low_pointer)
					{
						input_var_const->push_back(var_const("2", brakets_counter, low_pointer, NULL));  //���������� ��������, ����� ����� -> �� ���������� �����
						input_var_const->at(current_size_of_vect).point_left = &input_var_const->at(temp_size_of_vect);//����� ����� ������� ����������� ��������� ��������� �� ��������� ��������
						high_pointer = &input_var_const->at(temp_size_of_vect);// ������� ��������� -- �� ��������� ��������
					}
					//���� ���� �����-���� �������� �� �����
					else
					{
						//���� ��������� ���������� ������������ �������� !������! ��� �����  ���������� �������
						if ((strtod(high_pointer[0].var_id.c_str(), &temp) + high_pointer[0].var) <= (brakets_counter + 1))
						{
							input_var_const->push_back(var_const("2", brakets_counter, low_pointer, NULL));  //���������� ��������, ����� ����� -> �� ���������� �����
							high_pointer[0].point_right = &input_var_const->at(temp_size_of_vect);  //������ ����� ���������� -> �� ���������
							high_pointer = &input_var_const->at(temp_size_of_vect);                 //������� ��������� -> �� ��������� ��������
						}
						//���� ��������� ���������� ������������ �������� !������! ��� ��������� �������
						else
						{
							//input_var_const->at(temp_size_of_vect) = var_const("1", brakets_counter, high_pointer, NULL);  //���������� ��������, ����� ����� -> �� ���������� ��������

							//���� ��������� �������� � ���������� ����������� (�� ������� ��������� ����� ����� ���������)  !������! ���������� �������
							if ((strtod(input_var_const->at(current_size_of_vect).point_left[0].var_id.c_str(), &temp) + input_var_const->at(current_size_of_vect).point_left[0].var) > (brakets_counter + 1))
							{
								//���������� �������� ��� ����� �����, ����� ����� -> �� ���������� ������������ ��������
								input_var_const->push_back(var_const("2", brakets_counter, input_var_const->at(current_size_of_vect).point_left, NULL));
								//��������� ����� ������� ���������� �� ��������� ��������
								input_var_const->at(current_size_of_vect).point_left = &input_var_const->at(temp_size_of_vect);


							}

							//���� ��������� �������� � ���������� ����������� (�� ������� ��������� ����� ����� ���������)  !������! ���������� �������, 
							//�.�. �������� ���������� ���-�� ����� ��������� � ������������
							else
							{
								input_var_const->at(current_size_of_vect).point_left[0].point_right = &input_var_const->at(temp_size_of_vect);//��� �� �����������
							}
							high_pointer = &input_var_const->at(temp_size_of_vect);  //������� ��������� �� ��������� ��������

						}
					}
					pDest++;
				}
				else if (*pDest == '/')
				{                     //�������� ������� ����� id "3"
					temp_size_of_vect = input_var_const->size();
					//���� ��� ������ �������� � ���������
					if (high_pointer == low_pointer)
					{
						input_var_const->push_back(var_const("3", brakets_counter, low_pointer, NULL));  //���������� ��������, ����� ����� -> �� ���������� �����
						input_var_const->at(current_size_of_vect).point_left = &input_var_const->at(temp_size_of_vect);//����� ����� ������� ����������� ��������� ��������� �� ��������� ��������
						high_pointer = &input_var_const->at(temp_size_of_vect);// ������� ��������� -- �� ��������� ��������
					}
					//���� ���� �����-���� �������� �� �����
					else
					{
						//���� ��������� ���������� ������������ �������� !������! ��� �����  ���������� �������
						if ((strtod(high_pointer[0].var_id.c_str(), &temp) + high_pointer[0].var) <= (brakets_counter + 1))
						{
							input_var_const->push_back(var_const("3", brakets_counter, low_pointer, NULL));  //���������� ��������, ����� ����� -> �� ���������� �����
							high_pointer[0].point_right = &input_var_const->at(temp_size_of_vect);  //������ ����� ���������� -> �� ���������
							high_pointer = &input_var_const->at(temp_size_of_vect);                 //������� ��������� -> �� ��������� ��������
						}
						//���� ��������� ���������� ������������ �������� !������! ��� ��������� �������
						else
						{
							//input_var_const->at(temp_size_of_vect) = var_const("1", brakets_counter, high_pointer, NULL);  //���������� ��������, ����� ����� -> �� ���������� ��������

							//���� ��������� �������� � ���������� ����������� (�� ������� ��������� ����� ����� ���������)  !������! ���������� �������
							if ((strtod(input_var_const->at(current_size_of_vect).point_left[0].var_id.c_str(), &temp) + input_var_const->at(current_size_of_vect).point_left[0].var) > (brakets_counter + 1))
							{
								//���������� �������� ��� ����� �����, ����� ����� -> �� ���������� ������������ ��������
								input_var_const->push_back(var_const("3", brakets_counter, input_var_const->at(current_size_of_vect).point_left, NULL));
								//��������� ����� ������� ���������� �� ��������� ��������
								input_var_const->at(current_size_of_vect).point_left = &input_var_const->at(temp_size_of_vect);


							}

							//���� ��������� �������� � ���������� ����������� (�� ������� ��������� ����� ����� ���������)  !������! ���������� �������, 
							//�.�. �������� ���������� ���-�� ����� ��������� � ������������
							else
							{
								input_var_const->at(current_size_of_vect).point_left[0].point_right = &input_var_const->at(temp_size_of_vect);//��� �� �����������
							}
							high_pointer = &input_var_const->at(temp_size_of_vect);  //������� ��������� -> �� ��������� ��������

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
						input_var_const->push_back(var_const("0", strtod(pDest, &pDest))); //�������� �������� ������ � ������ �����
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
							input_var_const->push_back(var_const("1", brakets_counter, low_pointer, NULL));  //������� ���������� ��������, ����� ����� -> �� ���������� �����
							input_var_const->at(current_size_of_vect).point_left = &input_var_const->at(temp_size_of_vect);//����� ����� ������� ����������� ��������� ��������� �� ��������� ��������
							high_pointer = &input_var_const->at(temp_size_of_vect);// ������� ��������� -- �� ��������� ��������
							temp_size_of_vect = input_var_const->size(); //������� ������� �� ������� ������������� ��� ��� ��� ������ ������ �����
							input_var_const->push_back(var_const("0", strtod(pDest, &pDest))); //�������� �������� ������ � ������ �����
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
									input_var_const->push_back(var_const("1", brakets_counter, low_pointer, NULL));//������� ���������� ��������, ����� ����� -> �� ���������� �����
									high_pointer[0].point_right = &input_var_const->at(temp_size_of_vect);//��������� ���������� �������� �� �����
									high_pointer = &input_var_const->at(temp_size_of_vect);// ������� ��������� -- �� ��������� ��������
									temp_size_of_vect = input_var_const->size(); //������� ������� �� ������� ������������� ��� ��� ��� ������ ������ �����
									input_var_const->push_back(var_const("0", strtod(pDest, &pDest))); //�������� �������� ������ � ������ �����
																									   //������ ��������� -> �� ��������� �����
									low_pointer = &input_var_const->at(temp_size_of_vect);
									//������ ����� �������������� �������� �� ��������� �����
									high_pointer[0].point_right = low_pointer;
								}
							//���� ���������� �������� �� ��������� �� ����� (������ ����� �� �� ��� �� ���������). 
							//����������� �������� �-�: 7*(-2+5), �.�. ���� ����� ����� ����� ����� ������
								else
								{
									input_var_const->push_back(var_const("0", strtod(pDest, &pDest))); //�������� �������� ������ � ������ �����
									low_pointer = &input_var_const->at(temp_size_of_vect);//������ ��������� -> �� ��������� �����
																						  //������ ����� �������������� �������� �� ��������� �����. � ������ ������ ����� ���� ������������� ��������
									high_pointer[0].point_right = low_pointer;
								}
							//���� �������� ��������� ����� � ������ ����� ��������, ������� ������ ����� � ��� �� ������ (��� ������ ��� ���)
							else
							{
								//���� ��������� ���������� ������������ �������� !������! ��� �����  ���������� �������
								if ((strtod(high_pointer[0].var_id.c_str(), &temp) + high_pointer[0].var) <= (brakets_counter + 1))
								{
									input_var_const->push_back(var_const("1", brakets_counter, low_pointer, NULL));  //���������� ��������, ����� ����� -> �� ���������� �����
									high_pointer[0].point_right = &input_var_const->at(temp_size_of_vect);  //������ ����� ���������� -> �� ���������
									high_pointer = &input_var_const->at(temp_size_of_vect);                 //������� ��������� -> �� ��������� ��������
								}
								//���� ��������� ���������� ������������ �������� !������! ��� ��������� �������
								else
								{
									//input_var_const->at(temp_size_of_vect) = var_const("1", brakets_counter, high_pointer, NULL);  //���������� ��������, ����� ����� -> �� ���������� ��������

									//���� ��������� �������� � ���������� ����������� (�� ������� ��������� ����� ����� ���������)  !������! ���������� �������
									if ((strtod(input_var_const->at(current_size_of_vect).point_left[0].var_id.c_str(), &temp) + input_var_const->at(current_size_of_vect).point_left[0].var) > (brakets_counter + 1))
									{
										//���������� �������� ��� ����� �����, ����� ����� -> �� ���������� ������������ ��������
										input_var_const->push_back(var_const("1", brakets_counter, input_var_const->at(current_size_of_vect).point_left, NULL));
										//��������� ����� ������� ���������� �� ��������� ��������
										input_var_const->at(current_size_of_vect).point_left = &input_var_const->at(temp_size_of_vect);


									}

									//���� ��������� �������� � ���������� ����������� (�� ������� ��������� ����� ����� ���������)  !������! ���������� �������, 
									//�.�. �������� ���������� ���-�� ����� ��������� � ������������
									else
									{
										input_var_const->at(current_size_of_vect).point_left[0].point_right = &input_var_const->at(temp_size_of_vect);//��� �� �����������
									}
									high_pointer = &input_var_const->at(temp_size_of_vect);  //������� ��������� �� ��������� ��������			
								}
								temp_size_of_vect = input_var_const->size(); //������� ������� �� ������� ������������� ��� ��� ��� ������ ������ �����
								input_var_const->push_back(var_const("0", strtod(pDest, &pDest))); //�������� �������� ������ � ������ �����
																								   //������ ��������� -> �� ��������� �����
								low_pointer = &input_var_const->at(temp_size_of_vect);
								//������ ����� �������������� �������� �� ��������� �����
								high_pointer[0].point_right = low_pointer;
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
					temp_size_of_vect = input_var_const->size();	//3? ����� ������ ��� 4
					input_var_const->push_back(var_const("0", strtod(pDest, &pDest))); //�������� �������� ������ � ������ �����
																					   // ���� ������ ����� ������ (� �������� ������������) � ������ ���������
					if ((high_pointer == NULL) && (low_pointer == NULL))
					{
						//��� ��������� -> �� �����, �������� ��� �������� ������� ��� ������ ��������
						high_pointer = &input_var_const->at(temp_size_of_vect);
						low_pointer = &input_var_const->at(temp_size_of_vect);
						//����� ����� ����������� ��������� -> ��������� ��������� � ������.
						input_var_const->at(current_size_of_vect).point_left = &input_var_const->at(temp_size_of_vect);
					}
					// ���� ������ ����� �� ������ � ������ (���� �����-���� ��������)
					else
					{
						//������ ��������� -> �� ��������� �����
						low_pointer = &input_var_const->at(temp_size_of_vect);
						//������ ����� �������������� �������� �� ��������� �����
						high_pointer[0].point_right = low_pointer;
					}
				}
			}

			input_var_const->at(current_size_of_vect).arithmetic();
			//cout << input_var_const->at(current_size_of_vect).var_id << " = " << input_var_const->at(current_size_of_vect).var << endl;

			int output_size = input_var_const->at(current_size_of_vect).var_id.size() + std::to_string(input_var_const->at(current_size_of_vect).var).size() + 50;
			char* output = (char*)malloc(output_size);
			for (int i = 0;i < output_size;i++)
				output[i] = 0;

			strcpy(output, input_var_const->at(current_size_of_vect).var_id.c_str());
			strcat(output, " = ");
			strcat(output, std::to_string(input_var_const->at(current_size_of_vect).var).c_str());

			return output;	//��� ����
		}

	}
}