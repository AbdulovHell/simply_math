#pragma once
#ifndef HTML_ADDON_H
#define HTML_ADDON_H

#include <iostream>

namespace Project {
	namespace HTML {
		using namespace std;

		enum class Actions : int {
			//������
			BOLD = 1,
			//������
			ITALIC = 2,
			//������������
			UNDERLINE = 3,
			//������� ������
			SUPER = 4,
			//������ ������
			SUB = 5
		};

		enum class HTMLColors : uint32_t {
			RED = 0xff0000,
			GREEN = 0x00ff00,
			BLUE = 0x0000ff,

			TEAL = 0x00ffff,
			PINK = 0xff00ff,
			YELLOW = 0xffff00,

			BLACK = 0x000000,
			WHITE = 0xffffff,

			LIGHTBLUE = 0x00aaff,
			MEDIUMBLUE = 0x0055ff,

			ORANGE = 0xffaa00
		};
		//���������� �������� ��������� HTML, � ��������� ������� ������
		wstring AddHeader(wstring str, int GlobalFontSize);
		//��������� ������� ������ ��� ���������� ����� ������
		wstring ChangeFontSize(wstring str, int FontSize);
		//��������� ����� ����� ������, ��������� �������� ��� �����
		wstring ChangeTextColor(wstring str, uint32_t Color);
		//��������� ����� ����� ������, ��������� ���� �� ������ �������
		wstring ChangeTextColor(wstring str, HTMLColors Color);
		//���������� �������� � ������, ������ ������ � HTML::Actions
		wstring ApplyEffects(wstring str, ...);
		//�������� ���������� ������������ �������, ������ �������� ���������� ������������ ����, ���� �� ������ ������, ���� ��� ������
		wstring RemoveLastEffect(wstring str);
	}
}

#endif //HTML_ADDON_H