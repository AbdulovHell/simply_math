IO.h

  namespace core
  
    void reduce(int* u, int* d);  //сокращение дроби, рекурсия
      u - числитель, d - знаменатель
    
    bool isInteger(double var); //проверка на целое
      возвращает true, если число является целым
    
    int isntPeriodical(double num); //проверка дроби на переодичность  
      возвращает количество знаков после запятой, -1 если дробь условно бесконечно переодическая

  namespace IO
  
    wchar_t* err_str(int pos); //составляет строку с указателем до неизвестного символа
      pos - позиция неизвестного символа в исходной строке
    
    wchar_t* wXchar (wchar_t* str);  //удаление пробелов
      возвращает указатель на принимаемую строку
    
    wchar_t* VerifyInput(wchar_t* input);	//возвращает строку, описывающую ошибку в строке input, иначе NULL.
    
    wstring to_string(double var, var_type type, int decimals); //представление double в виде wстроки с выбором параметров
      var - переменная, type - как представить, decimals - требуемое количество знаков после запятой(пока от 0 до 9)
    
    enum class var_type : char {
            FRACTIONAL=1,	//дробное
            INTEGER_ROUND=2,	//целое с округлением
            INTEGER=3,	//целая часть
            TOWER=4 //неправильная дробь
		};
    
    wchar_t* to_char_string(double var, var_type type, int decimals); //аналогично для wchar
    
core.h
