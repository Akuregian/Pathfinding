#include "../Header_Files/TextBox.h"

bool checkIfIntegerInput(int charTyped) {
	// numbers 0-9, is ASCII for 48-57 
	if (charTyped >= 48 && charTyped <= 57) {
		return true;
	}
	return false;
}

TextBox::TextBox(sf::RenderWindow* window, int size, sf::Color color, bool selected) 
	:	m_win(window) {
	m_textBox.setCharacterSize(size);
	m_textBox.setFillColor(color);
	m_isSelected = selected;


	if (!m_Font.loadFromFile("../../../../Fonts/OpenSans-Bold.ttf")) {
		try {
			throw 22;
		}
		catch (const int e) {
			std::cout << "Error Code [" << e << "] <-- Couldnt Find/Load in Font File" << std::endl;
		}
	}

	SetFont(m_Font);


	if (m_isSelected) {
		m_textBox.setString("_");
	}
	else {
		m_textBox.setString("");
	}
}

TextBox::TextBox(sf::RenderWindow* window, int size, std::string inputText, sf::Color color, bool selected) 
	:	m_win(window) {
	m_textBox.setCharacterSize(size);
	m_textBox.setFillColor(color);
	m_isSelected = selected;
	text << inputText;

	if (!m_Font.loadFromFile("../../../../Fonts/OpenSans-Bold.ttf")) {
		try {
			throw 22;
		}
		catch (const int e) {
			std::cout << "Error Code [" << e << "] <-- Couldnt Find/Load in Font File" << std::endl;
		}
	}

	SetFont(m_Font);

	std::string txt = text.str();
	std::string newTxt = "";
	for (int i = 0; i < txt.length(); i++) {
		newTxt += txt[i];
	}
	m_textBox.setString(newTxt);
}

TextBox::TextBox(sf::RenderWindow* window, int size, int inputText, sf::Color color, bool selected)
	: m_win(window) {
	m_textBox.setCharacterSize(size);
	m_textBox.setFillColor(color);
	m_isSelected = selected;
	text << inputText;

	if (!m_Font.loadFromFile("../../../../Fonts/OpenSans-Bold.ttf")) {
		try {
			throw 22;
		}
		catch (const int e) {
			std::cout << "Error Code [" << e << "] <-- Couldnt Find/Load in Font File" << std::endl;
		}
	}

	SetFont(m_Font);

	std::string txt = text.str();
	std::string newTxt = "";
	for (int i = 0; i < txt.length(); i++) {
		newTxt += txt[i];
	}
	m_textBox.setString(newTxt);
}

void TextBox::InputLogic(int charTyped) {
	if (charTyped != DELETE_KEY && charTyped != ENTER_KEY && charTyped != ESCAPE_KEY && checkIfIntegerInput(charTyped)) {
		text << static_cast<char>(charTyped);
	}
	else if (charTyped == DELETE_KEY) {
		if (text.str().length() > 0) {
			DeleteLastChar();
		}
	}
	m_textBox.setString(text.str());
}

void TextBox::DeleteLastChar() {

	std::string txt = text.str();
	std::string newTxt = "";
	for (int i = 0; i < txt.length() - 1; i++) {
		newTxt += txt[i];
	}
	text.str("");
	text << newTxt;

	m_textBox.setString(text.str() + "_");
}

void TextBox::SetFont(sf::Font& font) {
	m_textBox.setFont(font);
}

void TextBox::SetPosition(sf::Vector2f pos) {
	m_textBox.setPosition(pos);
}

void TextBox::SetLimit(bool ToF, int lim) {
	m_hasLimit = ToF;
	m_limit = lim;
}

void TextBox::SetSelected(bool sel) {
	m_isSelected = sel;
}

void TextBox::TypedOn(sf::Event input) {
	if (m_isSelected) {
		int charTyped = input.text.unicode;
		if (charTyped < 128) {
			if (m_hasLimit) {
				if (text.str().length() < m_limit) {
					InputLogic(charTyped);
				}
				else if (text.str().length() >= m_limit && charTyped == DELETE_KEY) {
					DeleteLastChar();
				}
			}
			else {
				InputLogic(charTyped);
			}
		}
	}
}

void TextBox::DrawText() {
	m_win->draw(m_textBox);
}

void TextBox::SetText(std::string txt) {
	text << txt;
	m_textBox.setString(text.str());
}

void TextBox::DeleteAllChars() {
	std::string currText = m_textBox.getString();
	if (currText.size() > 1) {
		for (int i = 0; i <= text.str().length(); i++) {
			DeleteLastChar();
		}
	}
}