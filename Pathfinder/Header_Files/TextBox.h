#ifndef TEXTBOX_H
#define TEXTBOX_H
#include <iostream>
#include <sstream>
#include <SFMl/Graphics.hpp>

#define DELETE_KEY 8
#define ENTER_KEY 13
#define ESCAPE_KEY 27
#define SPACEBAR_KEY 32

class TextBox {
public:
	TextBox();
	TextBox(sf::RenderWindow*, int, sf::Color, bool);
	TextBox(sf::RenderWindow*, int, std::string, sf::Color, bool);
	TextBox(sf::RenderWindow*, int, int, sf::Color, bool);
	void InputLogic(int);
	void DeleteLastChar();
	void SetFont(sf::Font&);
	void SetPosition(sf::Vector2f);
	void SetLimit(bool, int);
	void SetSelected(bool);
	std::string returnText() { return text.str(); }
	void DrawText();
	void TypedOn(sf::Event);
	void SetText(std::string);
	void DeleteAllChars();
private:
	sf::RenderWindow* m_win;
	sf::Font m_Font;
	sf::Text m_textBox;
	std::ostringstream text;
	bool m_isSelected = false;
	bool m_hasLimit = false;
	int m_limit;
};


#endif // !TEXTBOX_H
