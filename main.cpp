#include <iostream>
#include <istream>
#include <fstream>
#include <string>
#include <SFML/Graphics.hpp>
#include "Board.h"
#include "Tile.h"

using namespace std;

sf::Text setText(sf::Font& font, sf::String message, int size, sf::Color color, sf::Vector2f position, bool bold, bool underlined) {
	sf::Text text;
	text.setFont(font);
	text.setString(message);
	text.setCharacterSize(size);
	text.setFillColor(color);
	sf::FloatRect rect = text.getLocalBounds();
	text.setOrigin(rect.left + rect.width / 2.0f, rect.top + rect.height / 2.0f);
	text.setPosition(position);
	if (bold) {
		text.setStyle(sf::Text::Bold);
	}
	if (underlined) {
		text.setStyle(sf::Text::Underlined);
	}
	return text;
}

sf::Text updateText(sf::Text& text, sf::String message, sf::Vector2f position) {
	text.setString(message);
	sf::FloatRect rect = text.getLocalBounds();
	text.setOrigin(rect.left + rect.width / 2.0f, rect.top + rect.height / 2.0f);
	text.setPosition(position);
	return text;
}

int main() {
	ifstream configurationFile("files/board_config.cfg");
	string temp;
	int columns, rows, width, height, numMines;
	getline(configurationFile, temp);
	columns = stoi(temp); // 25
	width = columns * 32;
	getline(configurationFile, temp);
	rows = stoi(temp); // 16
	height = rows * 32 + 100;
	getline(configurationFile, temp);
	numMines = stoi(temp); // 50

	sf::RenderWindow welcomeWindow(sf::VideoMode(width, height), "Minesweeper!", sf::Style::Close);
	welcomeWindow.setVisible(true);
	sf::RenderWindow gameWindow;
	gameWindow.setVisible(false);
	sf::RenderWindow leadershipWindow;
	leadershipWindow.setVisible(false);

	sf::Font font;
	if (!font.loadFromFile("files/font.ttf")) {
		cout << "Error loading font!" << endl;
	}
	sf::String name = "|";
	sf::Text welcomeText = setText(font, sf::String("WELCOME TO MINESWEEPER!"), 24, sf::Color::White, sf::Vector2f(width / 2.0f, height / 2.0f - 150), true, true);
	sf::Text inputPromptText = setText(font, sf::String("Enter your name:"), 20, sf::Color::White, sf::Vector2f(width / 2.0f, height / 2.0f - 75), true, false);
	sf::Text inputText = setText(font, name, 18, sf::Color::Yellow, sf::Vector2f(width / 2.0f, height / 2.0f - 45), true, false);

	Texture texture;

	vector<Button> buttons;
	Button happyface(texture.happy, (columns / 2.0) * 32 - 32, (32 * rows) + 0.5f);
	Button debug(texture.debug, (columns * 32) - 304, (32 * rows) + 0.5f);
	Button pause(texture.pause, (columns * 32) - 240, (32 * rows) + 0.5f);
	Button play(texture.play, (columns * 32) - 240, (32 * rows) + 0.5f);
	Button leadership(texture.leaderboard, (columns * 32) - 176, (32 * rows) + 0.5f);
	Button lose(texture.lose, (columns / 2.0) * 32 - 32, (32 * rows) + 0.5f);
	buttons.push_back(happyface);
	buttons.push_back(debug);
	buttons.push_back(pause);
	buttons.push_back(play);
	buttons.push_back(leadership);
	buttons.push_back(lose);

	Board board(columns, rows, numMines, buttons);
	board.create_board(texture);


	while (welcomeWindow.isOpen()) {
		sf::Event event;
		while (welcomeWindow.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				welcomeWindow.close();
			}
			if (event.type == sf::Event::KeyPressed) {
				for (int i = sf::Keyboard::A; i <= sf::Keyboard::Z; i++) {
					if (sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(i))) {
						if (name == "|") {
							name.insert(name.getSize() - 1, char(i + 65));
						}
						else if (name.getSize() < 11) {
							name.insert(name.getSize() - 1, char(i + 97));
						}
						inputText = updateText(inputText, name, sf::Vector2f(width / 2.0f, height / 2.0f - 45));
					}
				}
				if (name != "|" && sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(sf::Keyboard::Backspace))) {
					string temp = static_cast<string>(name);
					temp.erase(name.getSize() - 2, 1);
					name = temp;
					inputText = updateText(inputText, name, sf::Vector2f(width / 2.0f, height / 2.0f - 45));
				}
				if (name != "|" && sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(sf::Keyboard::Enter))) {
					welcomeWindow.close();
					gameWindow.create(sf::VideoMode(width, height), "Minesweeper!", sf::Style::Close);
					gameWindow.setVisible(true);
				}
			}
		}
		welcomeWindow.clear(sf::Color::Blue);
		welcomeWindow.draw(welcomeText);
		welcomeWindow.draw(inputPromptText);
		welcomeWindow.draw(inputText);
		welcomeWindow.display();
	}

	while (gameWindow.isOpen()) {
		sf::Event event;
		while (gameWindow.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				gameWindow.close();
			}

			if (event.type == sf::Event::MouseButtonPressed) {
				sf::Mouse mouse;
				auto coordinate = mouse.getPosition(gameWindow);
				if (event.mouseButton.button == sf::Mouse::Right) {
					board.placeremove_flag(coordinate.x, coordinate.y, texture);
				}
				if (event.mouseButton.button == sf::Mouse::Left) {
					auto debugcoordinates = debug.buttonsprite.getGlobalBounds();
					if (debugcoordinates.contains(coordinate.x, coordinate.y)) {
						board.debugmode = !board.debugmode;
					}
					board.check(coordinate.x, coordinate.y, texture);
				}

			}

		}
		gameWindow.clear();
		board.draw_board(gameWindow, texture);
		gameWindow.display();
	}


	return 0;
}