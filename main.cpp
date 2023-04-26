#include <iostream>
#include <istream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <unordered_map>
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

void update_leaderboard_file(string name, int time) {
	ifstream leaderboardFile("files/leaderboard.txt");
	string temp, time_min, time_sec, player_name;
	int ranking = 0;
	while (getline(leaderboardFile, temp)) {
		std::istringstream stream(temp);
		getline(stream, time_min, ':');
		getline(stream, time_sec, ',');
		getline(stream, player_name, ',');
		int totaltime = (stoi(time_min) * 60) + stoi(time_sec);
		if (totaltime > time) {
			break;
		}
		ranking++;
	}
	cout << ranking << endl;
	leaderboardFile.close();

	ifstream leaderboardFile2("files/leaderboard.txt");
	string newfile, newline, str_minutes, str_seconds, line;
	int minutes = time / 60;
	int seconds = time - (minutes * 60);
	if (minutes < 10) { str_minutes = "0" + to_string(minutes); }
	else { str_minutes = to_string(minutes); }
	if (seconds < 10) { str_seconds = "0" + to_string(seconds); }
	else { str_seconds = to_string(seconds); }
	newline = str_minutes + ":" + str_seconds + "," + name + "\n";

	for (int i = 0; i < 5; i++) {
		if (i == ranking) {
			newfile += newline;
		}
		getline(leaderboardFile2, line);
		newfile += line + "\n";
	}
	leaderboardFile2.close();
	ofstream write_leaderboard("files/leaderboard.txt");
	write_leaderboard << newfile;
}

vector<string> read_leaderboard_file(string winnername) {
	ifstream leaderboardFile("files/leaderboard.txt");
	string temp, time_min, time_sec, name;
	vector<string> info;
	while (getline(leaderboardFile, temp)) {
		std::istringstream stream(temp);
		getline(stream, time_min, ':');
		getline(stream, time_sec, ',');
		getline(stream, name, ',');
		if (name == winnername) {
			name += "*";
		}
		info.push_back(name);
		info.push_back(time_min);
		info.push_back(time_sec);
	}
	return info;
}

void display_leadershipWindow(sf::RenderWindow& window, int width, int height, Board& board, sf::Font& font, string name = "") {
	auto start_pause = chrono::high_resolution_clock::now();
	window.create(sf::VideoMode(width, height), "Minesweeper!", sf::Style::Close);

	vector<string> players = read_leaderboard_file(name);

	for (int i = 0; i < players.size(); i++) {
		cout << players.at(i) << endl;
	}

	sf::Text leaderboardText = setText(font, sf::String("LEADERBOARD"), 20, sf::Color::White, sf::Vector2f(width / 2.0f, height / 2.0f - 120), true, true);
	sf::String players_message = "";
	int counter = 1;
	for (int i = 0; i < players.size() - 2; i += 3) {
		players_message += (to_string(counter) + "." + "\t" + players.at(i + 1) + ":" + players.at(i + 2) + "\t" + players.at(i) + "\n\n");
		counter++;
		if (counter == 6) {
			break;
		}
	}
	sf::Text playersText = setText(font, players_message, 18, sf::Color::White, sf::Vector2f(width / 2.0f, height / 2.0f + 20), true, false);

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				board.leaderboardopen = false;
				auto stop_pause = chrono::high_resolution_clock::now();
				auto passtime = chrono::duration_cast<chrono::seconds>(stop_pause - start_pause).count();
				board.addpausetime(passtime);
				window.close();
				return;
			}
		}
		window.clear(sf::Color::Blue);
		window.draw(leaderboardText);
		window.draw(playersText);
		window.display();

	}
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

	vector<Digit> digits;
	Digit counter_negative(texture.digits, 12, 32 * (rows + 0.5f) + 16);
	Digit counter_digit1(texture.digits, 33, 32 * ( rows + 0.5f) + 16);
	Digit counter_digit2(texture.digits, 54, 32 * (rows + 0.5f) + 16);
	Digit first_minute(texture.digits, columns * 32 - 97, 32 * (rows + 0.5f) + 16);
	Digit sec_minute(texture.digits, columns * 32 - 76, 32 * (rows + 0.5f) + 16);
	Digit first_second(texture.digits, columns * 32 - 54, 32 * (rows + 0.5f) + 16);
	Digit sec_second(texture.digits, columns * 32 - 33, 32 * (rows + 0.5f) + 16);
	digits.push_back(counter_digit1);
	digits.push_back(counter_digit2);
	digits.push_back(first_minute);
	digits.push_back(sec_minute);
	digits.push_back(first_second);
	digits.push_back(sec_second);
	digits.push_back(counter_negative);

	vector<Button> buttons;
	Button happyface(texture.happy, (columns / 2.0) * 32 - 32, 32 * (rows + 0.5f));
	Button debug(texture.debug, (columns * 32) - 304, 32 * (rows + 0.5f));
	Button pause(texture.pause, (columns * 32) - 240, 32 * (rows + 0.5f));
	Button play(texture.play, (columns * 32) - 240, 32 * (rows + 0.5f));
	Button leaderboard(texture.leaderboard, (columns * 32) - 176, 32 * (rows + 0.5f));
	Button lose(texture.lose, (columns / 2.0) * 32 - 32, 32 * (rows + 0.5f));
	Button win(texture.win, (columns / 2.0) * 32 - 32, 32 * (rows + 0.5f));
	buttons.push_back(happyface);
	buttons.push_back(debug);
	buttons.push_back(pause);
	buttons.push_back(play);
	buttons.push_back(leaderboard);
	buttons.push_back(lose);
	buttons.push_back(win);

	Board board(columns, rows, numMines, buttons, digits);
	board.create_board(texture, numMines);

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
					gameWindow.create(sf::VideoMode(width, height), "Game Time!", sf::Style::Close);
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

	chrono::high_resolution_clock::time_point starttime = chrono::high_resolution_clock::now();
	chrono::high_resolution_clock::time_point currenttime;
	int time;

	chrono::high_resolution_clock::time_point start_pausetime;
	chrono::high_resolution_clock::time_point stop_pausetime;
	int passtime;

	while (gameWindow.isOpen()) {
		sf::Event event;
		currenttime = chrono::high_resolution_clock::now();
		time = chrono::duration_cast<chrono::seconds>(currenttime - starttime).count();
		board.settime(time);

		while (gameWindow.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				gameWindow.close();
			}

			if (event.type == sf::Event::MouseButtonPressed) {
				sf::Mouse mouse;
				auto coordinate = mouse.getPosition(gameWindow);

				if (!board.gamelost && !board.gamewon && !board.leaderboardopen) {
					if (event.mouseButton.button == sf::Mouse::Right) {
						board.placeremove_flag(coordinate.x, coordinate.y, texture);
					}
					if (event.mouseButton.button == sf::Mouse::Left) {
						auto debugcoordinates = debug.tilesprite.getGlobalBounds();
						auto restartcoordinates = happyface.tilesprite.getGlobalBounds();
						auto pausecoordinates = pause.tilesprite.getGlobalBounds();
						auto leaderboardcoordinates = leaderboard.tilesprite.getGlobalBounds();
						if (restartcoordinates.contains(coordinate.x, coordinate.y)) {
							board.reset();
							board.create_board(texture, numMines);
						}
						if (pausecoordinates.contains(coordinate.x, coordinate.y)) {
							board.ispause = !board.ispause;
							if (board.ispause) {
								start_pausetime = chrono::high_resolution_clock::now();
							}
							else {
								stop_pausetime = chrono::high_resolution_clock::now();
								passtime = chrono::duration_cast<chrono::seconds>(stop_pausetime - start_pausetime).count();
								board.addpausetime(passtime);
							}
						}
						if (leaderboardcoordinates.contains(coordinate.x, coordinate.y)) {
							board.leaderboardopen = true;
							gameWindow.clear(sf::Color::White);
							board.draw_board(gameWindow, texture);
							gameWindow.display();
							display_leadershipWindow(leadershipWindow, width / 2, height / 2, board, font);;
						}
						if (debugcoordinates.contains(coordinate.x, coordinate.y) && !board.ispause && !board.leaderboardopen) {
							board.debugmode = !board.debugmode;
						}
						if (!board.ispause) {
							board.check(coordinate.x, coordinate.y, texture);
							board.gamewon = board.check_if_won();

						}
					}
				}
				if (board.gamewon && !board.gameoverscreenshown) {
					gameWindow.clear(sf::Color::White);
					board.draw_board(gameWindow, texture);
					gameWindow.display();
					int finaltime = board.get_time();
					string name_without_line = static_cast<string>(name);
					name_without_line.erase(name.getSize() - 1, 1);
					name = name_without_line;
					update_leaderboard_file(name, finaltime);
					display_leadershipWindow(leadershipWindow, width / 2, height / 2, board, font, name);
					board.gameoverscreenshown = true;
				}
			}
		}
		gameWindow.clear(sf::Color::White);
		board.draw_board(gameWindow, texture);
		gameWindow.display();
	}

	return 0;
}