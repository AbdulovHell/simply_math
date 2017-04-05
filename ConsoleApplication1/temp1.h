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
//проходим по вектору, ищем переменную/конст/функц с таким именем
for (count = 0; count < temp_size_of_vect; count++)
{
	//проверяем имя, второе условие - на случай записи t=y+2, при этом у уже определён ранее y(x)=5*x
	if ((p_var == general_var_const->at(count)->read(L"name")) || (p_var == general_var_const->at(count)->read(L"func")))
	{
		//если найдена константа
		if (general_var_const->at(count)->read(L"type") == L"const")
		{
			if ((high_pointer == NULL) && (low_pointer == NULL))
			{
				//оба указателя -> на конст или перем из массива, тебуется для проверки условия при записи операции
				high_pointer = general_var_const->at(count);
				low_pointer = general_var_const->at(count);
				//левый рукав вычисляемой константы -> созданную структуру с числом.
				current_elment->point_left = general_var_const->at(count);
			}
			else
			{
				//нижний указатель -> на конст или перем из массива
				low_pointer = general_var_const->at(count);
				//Правый рукав предшествующей операции на конст или перем из массива
				high_pointer->point_right = low_pointer;
			}
			break;  //не имеет смысла считать дальше
		}
		//если найдена переменная
		else if (general_var_const->at(count)->read(L"type") == L"varbl")
		{
			//и она соответствует переменной на которую указывает функция
			if (current_elment->point_right == general_var_const->at(count))
			{
				if ((high_pointer == NULL) && (low_pointer == NULL))
				{
					//оба указателя -> на конст или перем из массива, тебуется для проверки условия при записи операции
					high_pointer = general_var_const->at(count);
					low_pointer = general_var_const->at(count);
					//левый рукав вычисляемой константы -> созданную структуру с числом.
					current_elment->point_left = general_var_const->at(count);
				}
				else
				{
					//нижний указатель -> на конст или перем из массива
					low_pointer = general_var_const->at(count);
					//Правый рукав предшествующей операции на конст или перем из массива
					high_pointer->point_right = low_pointer;
				}
				break;  //не имеет смысла считать дальше
			}
			//и она не соответствует переменной на которую указывает функция
			else
			{
				//ошибка, мы ничего не знаем о функциях нескольких аргументов и тп
			}

		}
		//если найдена функция	
		else if (general_var_const->at(count)->read(L"type") == L"funct")
		{

			break;//пока ошибка. в данном случая это вложение одной функции в другую, необходимо проверять соответствие переменных в обеих
		}
	}
}
//если в массиве нет ничего с таким именем, найдена новая переменная
if (count == temp_size_of_vect)
{
	temp = (wchar_t*)malloc(6 * sizeof(wchar_t) + wcslen(p_var));
	wcscpy(temp, L"varbl@");
	wcscat(temp, p_var);
	if ((high_pointer == NULL) && (low_pointer == NULL))
	{
		general_var_const->push_back(new var_const(temp, 0, current_elment));
		//оба указателя -> на конст или перем из массива, тебуется для проверки условия при записи операции
		high_pointer = general_var_const->at(temp_size_of_vect);
		low_pointer = general_var_const->at(temp_size_of_vect);
		//левый рукав вычисляемой константы -> созданную структуру с числом.
		current_elment->point_left = general_var_const->at(temp_size_of_vect);
	}
	else
	{
		general_var_const->push_back(new var_const(temp, 0, current_elment));
		//нижний указатель -> на конст или перем из массива
		low_pointer = general_var_const->at(temp_size_of_vect);
		//Правый рукав предшествующей операции на конст или перем из массива
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
	else if (current_elment->read(L"type") == L"equat")//пока оставлю это здесь для будущей реализации уравнений
	{

	}
	else
	{
		//случай, когда мы попали на функцию, которая указывает на другую, уже заданную переменную,
		//н-р ввод f(x)=t+1, пока ошибка ввода - мы не умеем функции нескольких переменных
	}

}
pDest += wcslen(p_var);
free(p_var);
free(temp);
				}
			}




			//указатель на первое вхождение одного из символов в строку
			temp = wcspbrk(pDest, L"()+-*^/=");
			if (temp == NULL)  //в данном случае ошибка, так как это бессмысленная запись, но данной ветки в принципе быть не должно)
			{
				temp = (wchar_t*)malloc(6 * sizeof(wchar_t));
				wcscpy(temp, L"error@");
				high_pointer = new var_const(temp, 2);
				general_var_const->pop_back();
				return high_pointer;
			}
			else
			{
				//записываем подстроку из символов
				p_var = (wchar_t*)malloc((temp - pDest) * 2);
				wcsncpy(p_var, pDest, temp - pDest);
				p_var[temp - pDest] = 0;
			}
			//есди запись вида f(2) f(x) f(g(x)) и тд
			if (*temp == '(')
			{
				p_temp = temp + 1;
				count = 1;
				//проверяем скобки с внутреннимии вложениями
				while (count != 0)
				{
					temp++;
					//если попали в конец строчки не найдя закрывающих скобок
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
					else if (*temp == '=') //открытая скобка и дальше равно
					{
						temp = (wchar_t*)malloc(6 * sizeof(wchar_t));
						wcscpy(temp, L"error@");
						high_pointer = new var_const(temp, 3);
						general_var_const->pop_back();
						return high_pointer;
					}
				}
				//пытаемся получить число в скобках

				num = wcstod(p_temp + 1, &p_temp);
				//в скобках только число
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
				//сразу после скобки идут символы 
				//если в скобках  нет никаких операций
				else if (wcspbrk(p_temp, L"+-*^/(=") > temp)
				{
					//сразу после скобки идут символы 
					if (*(p_temp - 1) == '(')
					{
						//ищем функцию с данным именем
						high_pointer = run_vector(p_var);
						//если не найдено ничего с таким именем или найдена переменная/константа - переопределение
						if ((high_pointer == NULL) || (high_pointer->read(L"type") == L"varbl") || (high_pointer->read(L"type") == L"const"))
						{
							//если после закрытой скобки есть что-то кроме равно
							if (*(temp + 1) != '=')
							{
								//доработать
								temp = NULL;
								temp = (wchar_t*)malloc(6 * sizeof(wchar_t));
								wcscpy(temp, L"error@");
								high_pointer = NULL;
								high_pointer = new var_const(temp, -2);
								general_var_const->pop_back();
								return high_pointer;
							}
							else //сразу идёт равно. определяем функцию с именем p_var, если была перемен/конст - переопределяем
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
								//тут наверное удалять надо хвост
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
										//тут наверное удалять надо хвост
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
										//тут наверное удалять надо хвост
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
									//доработать
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
						//найдена функция 
						else if (high_pointer->read(L"type") == L"funct")
						{
							if (*(temp + 1) != '=')
							{
								//доработать
								temp = NULL;
								temp = (wchar_t*)malloc(6 * sizeof(wchar_t));
								wcscpy(temp, L"error@");
								high_pointer = NULL;
								high_pointer = new var_const(temp, -2);
								general_var_const->pop_back();
								return high_pointer;
							}
							else //сразу идёт равно. 
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
								//тут наверное удалять надо хвост
								low_pointer = run_vector(p_var);
								if (low_pointer == NULL)
								{
									high_pointer->var_id = name;
									name = NULL;
									name = (wchar_t*)malloc(sizeof(wchar_t) * 6 + (temp - p_temp) * 2);
									wcscpy(name, L"varbl@");
									wcscat(name, p_var);
									//тут наверное удалять надо хвост
									current_element->var_id = name;
									high_pointer->point_collar = new var_const(current_element);
									current_element = high_pointer;
								}
								else if (low_pointer->read(L"type") == L"varbl")
								{
									if (high_pointer->point_collar->read(L"name") == low_pointer->read(L"name"))
									{
										current_element = high_pointer;
										//так как и функция и нужная переменная найдены удаляем последний элемент вектора за ненадобностью
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
								//найдены определённые заранее функция и константа - добавляем в выражение функцию от конст
								else if (low_pointer->read(L"type") == L"const")
								{
									current_element->point_left = new var_const(high_pointer);
									current_element->point_left->point_right = new var_const(low_pointer);
									high_pointer = current_element->point_left;
									low_pointer = current_element->point_left;

								}
								else if (low_pointer->read(L"type") == L"funct")
								{
									//доработать
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
					//если удалось прочесть число и далее есть какие-то символы без операций
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
				//в скобках есть какие-либо операции - вложение функций или использование константного выражения как аргумента
				else
				{
					//доработать
					temp = NULL;
					temp = (wchar_t*)malloc(6 * sizeof(wchar_t));
					wcscpy(temp, L"error@");
					high_pointer = NULL;
					high_pointer = new var_const(temp, -3);
					general_var_const->pop_back();
					return high_pointer;
				}
				//установить указатель на следующий элемент после закрывающей скобки
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
					//возможно надо удалить хвост
					general_var_const->push_back(new var_const(name, 0));
					name = NULL;
					name = (wchar_t*)malloc(sizeof(wchar_t) * 6);
					//текущий элемент становится уравнением
					wcscpy(name, L"equat@");
					//возможно надо удалять хвост
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
					//текущий элемент становится уравнением
					wcscpy(name, L"equat@");
					//возможно надо удалять хвост
					current_element->var_id = name;
					current_element->point_collar = new var_const(high_pointer);
					current_element->point_left = current_element->point_collar;
					high_pointer = current_element->point_left;
					low_pointer = current_element->point_left;
				}
				else if (high_pointer->read(L"type") == L"funct")
				{
					//доработать
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
					//символ найден не в начале строки
					else
					{
						temp = wcspbrk(pDest, L"()+-*^/=");
						if (temp == NULL)
						{
							//записываем подстроку из символов от  pdest до конца строки
							p_var = (wchar_t*)malloc((endPtr - pDest) * 2);
							wcsncpy(p_var, pDest, endPtr - pDest);
							p_var[endPtr - pDest] = 0;
						}
						else
						{
							//записываем подстроку из символов
							p_var = (wchar_t*)malloc((temp - pDest) * 2);
							wcsncpy(p_var, pDest, temp - pDest);
							p_var[temp - pDest] = 0;
						}