#include "../Header_Files/Button.h"

sf::Color SELECTEDCOLOR = sf::Color(100, 100, 100);

// Button takes in {name, color, width, height, pointerToWindow, PositionX, PositionY,
Button::Button(sf::Color col, int w, int h, sf::RenderWindow* win, int px, int py, const int& p)
	: m_button_win(win), m_createIndicator(p)
{
	if (!m_font.loadFromFile("../../../../Fonts/OpenSans-Bold.ttf"))
	{
		try
		{
			throw 22;
		}
		catch (const int e)
		{
			std::cout << "Error Code [" << e << "] <-- Couldnt Find/Load in Font File" << std::endl;
		}
	}
	m_button.setFillColor(col);
	m_button.setSize(sf::Vector2f(w, h));
	m_button.setPosition(px, py);

	m_indicator.setFillColor(sf::Color::Black);
	m_indicator.setSize(sf::Vector2f(10, 25));
	m_indicator.setPosition(m_button.getPosition().x + w - 15, m_button.getPosition().y + h / 2 - 12);

	m_bName.setFont(m_font);
	m_bName.setCharacterSize(40);
	m_bName.setPosition(px, py);
	m_bName.setColor(sf::Color::White);

}

void Button::setPos(int posx, int posy) {
	m_button.setPosition(posx, posy);
}

void Button::buttonDisplay() {
	m_button_win->draw(m_button);
	m_button_win->draw(m_bName);
	if (m_createIndicator) {
		m_button_win->draw(m_indicator);
	}
}

void Button::buttonDisplay(sf::RenderWindow& window) {
	window.draw(m_button);
	window.draw(m_bName);
	if (m_createIndicator) {
		window.draw(m_indicator);
	}
}

void Button::buttonSetup(std::string text, std::string id, int indicator) {
	m_bName.setString(text);
	m_id = id;
	m_createIndicator = indicator;
}

void Button::displayIndicator() {
	if (m_createIndicator) {
		m_button_win->draw(m_indicator);
	}
}

void Button::tweakTextPosition(int x, int y) {
	m_bName.setPosition(m_bName.getPosition().x + x, m_bName.getPosition().y + y);
}

void Button::tweakCharSize(int size) {
	m_bName.setCharacterSize(size);
}

void Button::tweakButtonPosition(sf::Vector2f pos) {
	m_button.setPosition(m_button.getPosition().x + pos.x, m_button.getPosition().y + pos.y);
}

void Button::ChangeButtonSize(sf::Vector2f size) {
	m_button.setSize(size);
}

void Button::setIndicatorColor(sf::Color color) {
	m_indicator.setFillColor(color);
}

bool Button::checkMouseClick(int x, int y) {
	return m_button.getGlobalBounds().contains(x, y);
}

void Button::setButtonSelected(bool sel) {
	if (sel) {
		m_button.setFillColor(SELECTEDCOLOR);
	}
	else {
		m_button.setFillColor(SELECTEDCOLOR);
	}
}
