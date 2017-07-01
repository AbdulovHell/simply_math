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
			LIME = 0x00ff00,
			BLUE = 0x0000ff,

			AQUA = 0x00ffff,
			PINK = 0xff00ff,
			YELLOW = 0xffff00,

			BLACK = 0x000000,
			WHITE = 0xffffff,

			LIGHTBLUE = 0x00aaff,
			MEDIUMBLUE = 0x0055ff,

			ORANGE = 0xffaa00,

			GREEN = 0x008000,
			SILVER = 0xC0C0C0,
			PURPLE = 0x800080,
			MAROON = 0x800000,
			OLIVE = 0x808000,
			TEAL = 0x008080,
			NAVY = 0x000080,

			GAINSBORO = 0xDCDCDC,
			GRAY = 0x808080,
			LIGHTGRAY = 0xD3D3D3,
			DARKGRAY = 0xA9A9A9,
			LIGHTSLATEGRAY = 0x778899,
			STALEGRAY = 0x708090
		};
		//���������� �������� ��������� HTML, � ��������� ������� ������ � ����� ����
		wstring AddHeader(wstring str, int GlobalFontSize, uint32_t Color);
		//���������� �������� ��������� HTML, � ��������� ������� ������ � ����� ����
		wstring AddHeader(wstring str, int GlobalFontSize, HTMLColors Color);
		//�������� ��� ������ ����� ������
		wstring NewString(wstring str);
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

		wstring Uint32ToWString(uint32_t Color);
	}
}

#endif //HTML_ADDON_H