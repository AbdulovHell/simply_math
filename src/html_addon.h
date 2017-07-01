#pragma once
#ifndef HTML_ADDON_H
#define HTML_ADDON_H

#include <iostream>

namespace Project {
	namespace HTML {
		using namespace std;

		enum class Actions : int {
			//жирный
			BOLD = 1,
			//курсив
			ITALIC = 2,
			//подчеркнутый
			UNDERLINE = 3,
			//верхний индекс
			SUPER = 4,
			//нижний индекс
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
		//добавление главного заголовка HTML, с указанием размера шрифта
		wstring AddHeader(wstring str, int GlobalFontSize);
		//вызывать дл€ каждой новой строки
		wstring NewString(wstring str);
		//изменение размера шрифта дл€ отдельного куска текста
		wstring ChangeFontSize(wstring str, int FontSize);
		//изменение цвета куска текста, использу€ числовой код цвета
		wstring ChangeTextColor(wstring str, uint32_t Color);
		//изменение цвета куска текста, использу€ цвет из набора готовых
		wstring ChangeTextColor(wstring str, HTMLColors Color);
		//применение эффектов к тексту, полный список в HTML::Actions
		wstring ApplyEffects(wstring str, ...);
		//удаление последнего примененного эффекта, тоесть удаление последнего добавленного тега, будь то размер шрифта, цвет или курсив
		wstring RemoveLastEffect(wstring str);
	}
}

#endif //HTML_ADDON_H