#pragma once
temp_size_of_vect = general_var_const->size();
temp = wcspbrk(pDest, L")+-*/^");
if (temp != NULL)
{
	p_var = (wchar_t*)malloc((temp - pDest + 1) * 2);
	wcsncpy(p_var, pDest, temp - pDest);
	p_var[temp - pDest] = 0;
	temp = NULL;
}
else
{
	p_var = (wchar_t*)malloc((endPtr - pDest + 1) * 2);
	wcscpy(p_var, pDest);
}
//�������� �� �������, ���� ����������/�����/����� � ����� ������
for (count = 0; count < temp_size_of_vect; count++)
{
	//��������� ���, ������ ������� - �� ������ ������ t=y+2, ��� ���� � ��� �������� ����� y(x)=5*x
	if ((p_var == general_var_const->at(count)->read(L"name")) || (p_var == general_var_const->at(count)->read(L"func")))
	{
		//���� ������� ���������
		if (general_var_const->at(count)->read(L"type") == L"const")
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
		else if (general_var_const->at(count)->read(L"type") == L"varbl")
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
		else if (general_var_const->at(count)->read(L"type") == L"funct")
		{

			break;//���� ������. � ������ ������ ��� �������� ����� ������� � ������, ���������� ��������� ������������ ���������� � �����
		}
	}
}
//���� � ������� ��� ������ � ����� ������, ������� ����� ����������
if (count == temp_size_of_vect)
{
	temp = (wchar_t*)malloc(6 * sizeof(wchar_t) + wcslen(p_var));
	wcscpy(temp, L"varbl@");
	wcscat(temp, p_var);
	if ((high_pointer == NULL) && (low_pointer == NULL))
	{
		general_var_const->push_back(new var_const(temp, 0, current_elment));
		//��� ��������� -> �� ����� ��� ����� �� �������, �������� ��� �������� ������� ��� ������ ��������
		high_pointer = general_var_const->at(temp_size_of_vect);
		low_pointer = general_var_const->at(temp_size_of_vect);
		//����� ����� ����������� ��������� -> ��������� ��������� � ������.
		current_elment->point_left = general_var_const->at(temp_size_of_vect);
	}
	else
	{
		general_var_const->push_back(new var_const(temp, 0, current_elment));
		//������ ��������� -> �� ����� ��� ����� �� �������
		low_pointer = general_var_const->at(temp_size_of_vect);
		//������ ����� �������������� �������� �� ����� ��� ����� �� �������
		high_pointer->point_right = low_pointer;
	}
	//free(temp);
	if (current_elment->read(L"type") != L"funct")
	{
		temp = (wchar_t*)malloc(7 * sizeof(wchar_t) + wcslen(p_var) + wcslen(current_elment->read(L"name").c_str()));
		wcscpy(temp, L"funct@");
		wcscat(temp, current_elment->read(L"name").c_str());
		wcscat(temp, L"(");
		wcscat(temp, p_var);
		wcscat(temp, L")");
		current_elment->var_id = temp;
		current_elment->point_right = low_pointer;
		//free(temp);
	}
	else if (current_elment->read(L"type") == L"equat")//���� ������� ��� ����� ��� ������� ���������� ���������
	{

	}
	else
	{
		//������, ����� �� ������ �� �������, ������� ��������� �� ������, ��� �������� ����������,
		//�-� ���� f(x)=t+1, ���� ������ ����� - �� �� ����� ������� ���������� ����������
	}

}
pDest += wcslen(p_var);
free(p_var);
free(temp);
				}
			}




			//��������� �� ������ ��������� ������ �� �������� � ������
			temp = wcspbrk(pDest, L"()+-*^/=");
			if (temp == NULL)  //� ������ ������ ������, ��� ��� ��� ������������� ������, �� ������ ����� � �������� ���� �� ������)
			{
				temp = (wchar_t*)malloc(6 * sizeof(wchar_t));
				wcscpy(temp, L"error@");
				high_pointer = new var_const(temp, 2);
				general_var_const->pop_back();
				return high_pointer;
			}
			else
			{
				//���������� ��������� �� ��������
				p_var = (wchar_t*)malloc((temp - pDest) * 2);
				wcsncpy(p_var, pDest, temp - pDest);
				p_var[temp - pDest] = 0;
			}
			//���� ������ ���� f(2) f(x) f(g(x)) � ��
			if (*temp == '(')
			{
				p_temp = temp + 1;
				count = 1;
				//��������� ������ � ������������ ����������
				while (count != 0)
				{
					temp++;
					//���� ������ � ����� ������� �� ����� ����������� ������
					if (temp == endPtr)
					{
						temp = NULL;
						temp = (wchar_t*)malloc(6 * sizeof(wchar_t));
						wcscpy(temp, L"error@");
						high_pointer = new var_const(temp, 2);
						general_var_const->pop_back();
						return high_pointer;
					}
					else if (*temp == '(')
					{
						count++;
					}
					else if (*temp == ')')
					{
						count--;
					}
					else if (*temp == '=') //�������� ������ � ������ �����
					{
						temp = (wchar_t*)malloc(6 * sizeof(wchar_t));
						wcscpy(temp, L"error@");
						high_pointer = new var_const(temp, 3);
						general_var_const->pop_back();
						return high_pointer;
					}
				}
				//�������� �������� ����� � �������

				num = wcstod(p_temp + 1, &p_temp);
				//� ������� ������ �����
				if (p_temp == temp)
				{
					high_pointer = run_vector(p_var);
					if (high_pointer == NULL)
					{
						temp = NULL;
						temp = (wchar_t*)malloc(6 * sizeof(wchar_t));
						wcscpy(temp, L"error@");
						high_pointer = new var_const(temp, 4);
						general_var_const->pop_back();
						return high_pointer;
					}
					else if (high_pointer->read(L"type") == L"varbl")
					{
						temp = NULL;
						temp = (wchar_t*)malloc(6 * sizeof(wchar_t));
						wcscpy(temp, L"error@");
						high_pointer = new var_const(temp, 5);
						general_var_const->pop_back();
						return high_pointer;
					}
					else if (high_pointer->read(L"type") == L"funct")
					{
						current_element->point_left = new var_const(high_pointer);
						current_element->point_left->point_right = new var_const(L"0", num);
						high_pointer = current_element->point_left;
						low_pointer = current_element->point_left;
					}
					else
					{
						current_element->point_left = new var_const(high_pointer);
						high_pointer = current_element->point_left;
						low_pointer = current_element->point_left;
					}
				}
				//����� ����� ������ ���� ������� 
				//���� � �������  ��� ������� ��������
				else if (wcspbrk(p_temp, L"+-*^/(=") > temp)
				{
					//����� ����� ������ ���� ������� 
					if (*(p_temp - 1) == '(')
					{
						//���� ������� � ������ ������
						high_pointer = run_vector(p_var);
						//���� �� ������� ������ � ����� ������ ��� ������� ����������/��������� - ���������������
						if ((high_pointer == NULL) || (high_pointer->read(L"type") == L"varbl") || (high_pointer->read(L"type") == L"const"))
						{
							//���� ����� �������� ������ ���� ���-�� ����� �����
							if (*(temp + 1) != '=')
							{
								//����������
								temp = NULL;
								temp = (wchar_t*)malloc(6 * sizeof(wchar_t));
								wcscpy(temp, L"error@");
								high_pointer = NULL;
								high_pointer = new var_const(temp, -2);
								general_var_const->pop_back();
								return high_pointer;
							}
							else //����� ��� �����. ���������� ������� � ������ p_var, ���� ���� �������/����� - ��������������
							{
								name = (wchar_t*)malloc(sizeof(wchar_t) * 7 + (temp - pDest) * 2);
								wcscpy(name, L"funct@");
								wcscat(name, p_var);
								p_var = NULL;
								p_var = (wchar_t*)malloc((temp - p_temp) * 2);
								wcsncpy(p_var, pDest, temp - p_temp);
								p_var[temp - p_temp] = 0;
								wcscat(name, L"(");
								wcscat(name, p_var);
								wcscat(name, L")");
								//��� �������� ������� ���� �����
								low_pointer = run_vector(p_var);
								if (low_pointer == NULL)
								{
									if (high_pointer == NULL)
									{
										current_element->var_id = name;
										name = NULL;
										name = (wchar_t*)malloc(sizeof(wchar_t) * 6 + (temp - p_temp) * 2);
										wcscpy(name, L"varbl@");
										wcscat(name, p_var);
										//��� �������� ������� ���� �����
										general_var_const->push_back(new var_const(name, 0));
										current_element->point_collar = new var_const(general_var_const->at(temp_size_of_vect));
									}
									else
									{
										high_pointer->var_id = name;
										name = NULL;
										name = (wchar_t*)malloc(sizeof(wchar_t) * 6 + (temp - p_temp) * 2);
										wcscpy(name, L"varbl@");
										wcscat(name, p_var);
										//��� �������� ������� ���� �����
										current_element->var_id = name;
										high_pointer->point_collar = new var_const(current_element);
										current_element = high_pointer;
									}
								}
								else if (low_pointer->read(L"type") == L"varbl")
								{
									if (high_pointer == NULL)
									{
										current_element->var_id = name;
									}
									else
									{
										high_pointer->var_id = name;
										current_element = high_pointer;
										general_var_const->pop_back();
									}
									current_element->point_collar = new var_const(low_pointer);
								}
								else if (low_pointer->read(L"type") == L"const")
								{
									if (high_pointer == NULL)
									{
										current_element->var_id = name;
									}
									else
									{
										high_pointer->var_id = name;
										current_element = high_pointer;
										general_var_const->pop_back();

									}
									name = NULL;
									name = (wchar_t*)malloc(sizeof(wchar_t) * 6 + (temp - p_temp) * 2);
									wcscpy(name, L"varbl@");
									wcscat(name, p_var);
									low_pointer->var_id = name;
									current_element->point_collar = new var_const(low_pointer);
								}
								else if (low_pointer->read(L"type") == L"funct")
								{
									//����������
									temp = NULL;
									temp = (wchar_t*)malloc(6 * sizeof(wchar_t));
									wcscpy(temp, L"error@");
									high_pointer = NULL;
									high_pointer = new var_const(temp, -1);
									general_var_const->pop_back();
									return high_pointer;
								}
							}
						}
						//������� ������� 
						else if (high_pointer->read(L"type") == L"funct")
						{
							if (*(temp + 1) != '=')
							{
								//����������
								temp = NULL;
								temp = (wchar_t*)malloc(6 * sizeof(wchar_t));
								wcscpy(temp, L"error@");
								high_pointer = NULL;
								high_pointer = new var_const(temp, -2);
								general_var_const->pop_back();
								return high_pointer;
							}
							else //����� ��� �����. 
							{
								name = (wchar_t*)malloc(sizeof(wchar_t) * 7 + (temp - pDest) * 2);
								wcscpy(name, L"funct@");
								wcscat(name, p_var);
								p_var = NULL;
								p_var = (wchar_t*)malloc((temp - p_temp) * 2);
								wcsncpy(p_var, pDest, temp - p_temp);
								p_var[temp - p_temp] = 0;
								wcscat(name, L"(");
								wcscat(name, p_var);
								wcscat(name, L")");
								//��� �������� ������� ���� �����
								low_pointer = run_vector(p_var);
								if (low_pointer == NULL)
								{
									high_pointer->var_id = name;
									name = NULL;
									name = (wchar_t*)malloc(sizeof(wchar_t) * 6 + (temp - p_temp) * 2);
									wcscpy(name, L"varbl@");
									wcscat(name, p_var);
									//��� �������� ������� ���� �����
									current_element->var_id = name;
									high_pointer->point_collar = new var_const(current_element);
									current_element = high_pointer;
								}
								else if (low_pointer->read(L"type") == L"varbl")
								{
									if (high_pointer->point_collar->read(L"name") == low_pointer->read(L"name"))
									{
										current_element = high_pointer;
										//��� ��� � ������� � ������ ���������� ������� ������� ��������� ������� ������� �� �������������
										general_var_const->pop_back();
									}
									else
									{
										high_pointer->var_id = name;
										current_element = high_pointer;
										current_element->point_collar = new var_const(low_pointer);
										general_var_const->pop_back();

									}
								}
								//������� ����������� ������� ������� � ��������� - ��������� � ��������� ������� �� �����
								else if (low_pointer->read(L"type") == L"const")
								{
									current_element->point_left = new var_const(high_pointer);
									current_element->point_left->point_right = new var_const(low_pointer);
									high_pointer = current_element->point_left;
									low_pointer = current_element->point_left;

								}
								else if (low_pointer->read(L"type") == L"funct")
								{
									//����������
									temp = NULL;
									temp = (wchar_t*)malloc(6 * sizeof(wchar_t));
									wcscpy(temp, L"error@");
									high_pointer = NULL;
									high_pointer = new var_const(temp, -1);
									general_var_const->pop_back();
									return high_pointer;
								}
							}

						}
					}
					//���� ������� �������� ����� � ����� ���� �����-�� ������� ��� ��������
					else
					{
						temp = NULL;
						temp = (wchar_t*)malloc(6 * sizeof(wchar_t));
						wcscpy(temp, L"error@");
						high_pointer = NULL;
						high_pointer = new var_const(temp, 6);
						general_var_const->pop_back();
						return high_pointer;
					}
				}
				//� ������� ���� �����-���� �������� - �������� ������� ��� ������������� ������������ ��������� ��� ���������
				else
				{
					//����������
					temp = NULL;
					temp = (wchar_t*)malloc(6 * sizeof(wchar_t));
					wcscpy(temp, L"error@");
					high_pointer = NULL;
					high_pointer = new var_const(temp, -3);
					general_var_const->pop_back();
					return high_pointer;
				}
				//���������� ��������� �� ��������� ������� ����� ����������� ������
				pDest = temp + 1;
			}
			else if (*temp == ')')
			{
				temp = NULL;
				temp = (wchar_t*)malloc(6 * sizeof(wchar_t));
				wcscpy(temp, L"error@");
				high_pointer = new var_const(temp, 7);
				general_var_const->pop_back();
				return high_pointer;
			}
			else if (*temp == '=')
			{
				name = (wchar_t*)malloc(sizeof(wchar_t) * 6 + (temp - pDest) * 2);
				wcscpy(name, L"const@");
				wcscat(name, p_var);
				current_element->var_id = name;
				pDest = temp;

			}
			else //if (*temp == '+-/^*)')
			{
				high_pointer = run_vector(p_var);
				if (high_pointer == NULL)
				{
					name = (wchar_t*)malloc(sizeof(wchar_t) * 6 + (temp - pDest) * 2);
					wcscpy(name, L"varbl@");
					wcscat(name, p_var);
					//�������� ���� ������� �����
					general_var_const->push_back(new var_const(name, 0));
					name = NULL;
					name = (wchar_t*)malloc(sizeof(wchar_t) * 6);
					//������� ������� ���������� ����������
					wcscpy(name, L"equat@");
					//�������� ���� ������� �����
					current_element->var_id = name;
					current_element->point_collar = new var_const(general_var_const->at(temp_size_of_vect));
					current_element->point_left = current_element->point_collar;
					high_pointer = current_element->point_left;
					low_pointer = current_element->point_left;
				}
				else if (high_pointer->read(L"type") == L"const")
				{
					current_element->point_left = new var_const(high_pointer);
				}
				else if (high_pointer->read(L"type") == L"varbl")
				{
					name = (wchar_t*)malloc(sizeof(wchar_t) * 6);
					//������� ������� ���������� ����������
					wcscpy(name, L"equat@");
					//�������� ���� ������� �����
					current_element->var_id = name;
					current_element->point_collar = new var_const(high_pointer);
					current_element->point_left = current_element->point_collar;
					high_pointer = current_element->point_left;
					low_pointer = current_element->point_left;
				}
				else if (high_pointer->read(L"type") == L"funct")
				{
					//����������
					temp = NULL;
					temp = (wchar_t*)malloc(6 * sizeof(wchar_t));
					wcscpy(temp, L"error@");
					high_pointer = NULL;
					high_pointer = new var_const(temp, -2);
					general_var_const->pop_back();
					return high_pointer;
				}
				pDest = temp;
			}
					}
					//������ ������ �� � ������ ������
					else
					{
						temp = wcspbrk(pDest, L"()+-*^/=");
						if (temp == NULL)
						{
							//���������� ��������� �� �������� ��  pdest �� ����� ������
							p_var = (wchar_t*)malloc((endPtr - pDest) * 2);
							wcsncpy(p_var, pDest, endPtr - pDest);
							p_var[endPtr - pDest] = 0;
						}
						else
						{
							//���������� ��������� �� ��������
							p_var = (wchar_t*)malloc((temp - pDest) * 2);
							wcsncpy(p_var, pDest, temp - pDest);
							p_var[temp - pDest] = 0;
						}