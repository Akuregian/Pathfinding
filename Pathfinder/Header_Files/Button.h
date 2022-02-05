#ifndef BUTTON_H
#define BUTTON_H
#include <iostream>
#include<SFML/Graphics.hpp>

class Button
{
public:
	Button(sf::Color, int, int, sf::RenderWindow*, int, int, const int&);
	void setPos(int, int);
	void buttonDisplay();
	void buttonDisplay(sf::RenderWindow&);
	void displayIndicator();
	void buttonSetup(std::string, std::string, int);
	void tweakTextPosition(int, int);
	void tweakCharSize(int);
	void tweakButtonPosition(sf::Vector2f);
	void ChangeButtonSize(sf::Vector2f);
	void setIndicatorColor(sf::Color);
	bool checkMouseClick(int, int);
	void setButtonSelected(bool selected);
	std::string returnId() { return m_id; }

private:
	int m_createIndicator = 0;
	std::string m_id;
	sf::RenderWindow* m_button_win = nullptr;
	sf::Text m_bName;
	sf::Font m_font;
	sf::RectangleShape m_button;
	sf::RectangleShape m_indicator;
};

#endif
