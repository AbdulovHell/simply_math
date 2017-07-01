#include "html_addon.h"
#include <stdarg.h>
#include <iostream>
#include <stdio.h>

using namespace std;

wstring Project::HTML::AddHeader(wstring str, int GlobalFontSize, uint32_t Color)
{
	wstring sta;
	wstring end;
	wstring mid;

	wchar_t* buf = new wchar_t[4];
	swprintf(buf, 4, L"%d", GlobalFontSize);
	wstring FontSizeStr = buf;
	delete[] buf;
	//...<body bgcolor=#FFFFFF style=...
	sta = L"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\
<html><head><meta name = \"qrichtext\" content = \"1\" / ><style type = \"text/css\">\
p, li{ white - space: pre - wrap; }\
</style></head><body ";
	sta += L"bgcolor=#";
	sta += Uint32ToWString(Color);
	sta += L" style = \" font-family:'Verdana'; font-size:";
	sta += FontSizeStr;
	sta += L"pt; font-weight:400; font-style:normal;\">";
	end = L"</body></html>";
	mid = str;

	str = sta;
	str += mid;
	str += end;
	
	return str;
}

wstring Project::HTML::AddHeader(wstring str, int GlobalFontSize, HTMLColors Color)
{
	wstring sta;
	wstring end;
	wstring mid;

	wchar_t* buf = new wchar_t[4];
	swprintf(buf, 4, L"%d", GlobalFontSize);
	wstring FontSizeStr = buf;
	delete[] buf;
	//...<body bgcolor=#FFFFFF style=...
	sta = L"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\
<html><head><meta name = \"qrichtext\" content = \"1\" / ><style type = \"text/css\">\
p, li{ white - space: pre - wrap; }\
</style></head><body ";
	sta += L"bgcolor=#";
	sta += Uint32ToWString((uint32_t)Color);
	sta += L" style = \" font-family:'Verdana'; font-size:";
	sta += FontSizeStr;
	sta += L"pt; font-weight:400; font-style:normal;\">";
	end = L"</body></html>";
	mid = str;

	str = sta;
	str += mid;
	str += end;

	return str;
}

wstring Project::HTML::NewString(wstring str)
{
	wstring sta;
	wstring end;
	wstring mid;

	sta = L"<p style = \" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">";
	end = L"</p>";
	mid = str;

	str = sta;
	str += mid;
	str += end;

	return str;
}

wstring Project::HTML::ChangeFontSize(wstring str, int FontSize)
{
	wstring sta;
	wstring end;
	wstring mid;

	wchar_t* buf = new wchar_t[4];
	swprintf(buf, 4, L"%d", FontSize);
	wstring FontSizeStr = buf;
	delete[] buf;

	sta = L"<span style = \"font-size:";
	sta += FontSizeStr;
	sta += L"pt;\">";

	end = L"</span>";
	mid = str;

	str = sta;
	str += mid;
	str += end;

	return str;
}

wstring Project::HTML::ChangeTextColor(wstring str, uint32_t Color)
{
	wstring sta;
	wstring end;
	wstring mid;

	sta = L"<span style = \"color:#";
	sta += Uint32ToWString(Color);
	sta += L";\">";

	end = L"</span>";
	mid = str;

	str = sta;
	str += mid;
	str += end;

	return str;
}

wstring Project::HTML::ChangeTextColor(wstring str, HTMLColors Color)
{
	wstring sta;
	wstring end;
	wstring mid;

	sta = L"<span style = \"color:#";
	sta += Uint32ToWString((uint32_t)Color);
	sta += L";\">";

	end = L"</span>";
	mid = str;

	str = sta;
	str += mid;
	str += end;

	return str;
}

wstring Project::HTML::ApplyEffects(wstring str, ...)
{
	va_list ap;
	int num;
	wstring sta;
	wstring end;
	wstring mid;
	bool inf = true;

	bool needBold = false;
	bool needItalic = false;
	bool needUnderline = false;
	bool needSuper = false;
	bool needSub = false;

	va_start(ap, str);

	do {
		num = va_arg(ap, int);
		if (num == 0x03426DA8) break;	//магия!
		switch (num) {
		case (int)Project::HTML::Actions::BOLD:
			needBold = true;
			break;
		case (int)Project::HTML::Actions::ITALIC:
			needItalic = true;
			break;
		case (int)Project::HTML::Actions::UNDERLINE:
			needUnderline = true;
			break;
		case (int)Project::HTML::Actions::SUPER:
			needSuper = true;
			needSub = false;
			break;
		case (int)Project::HTML::Actions::SUB:
			needSuper = false;
			needSub = true;
			break;
		default:	//если магия не сработает...
			inf = false;
			break;
		}
	} while (inf);

	if (!(needBold || needItalic || needSub || needSuper || needUnderline)) return str;

	sta = L"<span style = \"";

	if (needBold) sta += L"font-weight:600; ";
	if (needItalic) sta += L"font-style:italic; ";
	if (needUnderline) sta += L"text-decoration:underline; ";
	if (needSub) sta += L"vertical-align:sub; ";
	if (needSuper) sta += L"vertical-align:super; ";

	sta += L"\">";

	end = L"</span>";
	mid = str;

	str = sta;
	str += mid;
	str += end;

	return str;
}

wstring Project::HTML::RemoveLastEffect(wstring str)
{
	size_t len = str.size()+1;
	wchar_t* tempStr = new wchar_t[len];
	wcscpy(tempStr,str.c_str());

	wchar_t *start;
	
    for (size_t i = 0; i < len; i++) {
		if (tempStr[i] == '>') {
			start = &tempStr[i] + 1;
			break;
		}
	}
	for (int i = len - 1; i >= 0; i--) {
		if (tempStr[i] == '<') {
			tempStr[i]=0;
			break;
		}
	}

	str = start;

	delete[] tempStr;
	return str;
}

wstring Project::HTML::Uint32ToWString(uint32_t Color)
{
	wchar_t* buf = new wchar_t[9];
	swprintf(buf, 9, L"%6x", Color);
	for (int i = 0; i < 9; i++)
		if (buf[i] == ' ')
			buf[i] = '0';
	wstring ColorStr = buf;
	delete[] buf;
	return ColorStr;
}
