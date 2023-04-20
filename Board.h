#pragma once
#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "Tile.h"
#include "Button.h"
#include "Texture.h"

mt19937 random_mt;
using namespace std;

struct Board {
	int columns;
	int rows;
	int numMines;
	bool ispause = false;
	bool debugmode = false;
	bool gameover = false;

	vector<vector<int>> configuration;
	vector<vector<Tile>> hiddentiles;
	vector<vector<Number>> numberedtiles;

	vector<Tile> flags;
	vector<Mine> mines;
	vector<Button> buttons;

	Board(int columns, int rows, int mines, vector<Button> buttons) {
		this->columns = columns;
		this->rows = rows;
		this->numMines = mines;
		this->buttons = buttons;

		vector<int> tempvector;
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < columns; j++) {
				tempvector.push_back(0);
			}
			configuration.push_back(tempvector);
			tempvector.clear();
		}

		//source for random number generator-https://stackoverflow.com/questions/7560114/random-number-c-in-some-range
		random_device rd;
		mt19937 gen(rd());
		uniform_int_distribution<int> randrow(0, rows - 1);
		uniform_int_distribution<int> randcol(0, columns - 1);
		int total = 0;
		int rowtemp = 0;
		int coltemp = 0;
		while (total < mines + 1) {
			rowtemp = randrow(gen);
			coltemp = randcol(gen);
			if (configuration.at(rowtemp).at(coltemp) == 0) {
				configuration.at(randrow(gen)).at(randcol(gen)) = 9;
				total += 1;
			}
		}

		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < columns; j++) {
				cout << configuration.at(i).at(j) << " ";
			}
			cout << endl;
		}
		cout << endl;
	}

	void create_board(Texture& texture) {
		for (int i = 0; i < rows; i++) {
			vector<Tile> temp_hiddens;
			vector<Number> temp_nums;
			for (int j = 0; j < columns; j++) {
				Tile tile_hidden(texture.hidden, j * 32, i * 32);
				Number tile_num(texture.revealed, j * 32, i * 32);
				temp_hiddens.push_back(tile_hidden);
				temp_nums.push_back(tile_num);
			}
			hiddentiles.push_back(temp_hiddens);
			numberedtiles.push_back(temp_nums);
			temp_hiddens.clear();
			temp_nums.clear();

		}

		for (int i = 0; i < configuration.size(); i++) {
			for (int j = 0; j < configuration[i].size(); j++) {
				if (configuration.at(i).at(j) == 9) {
					Mine mine(texture.mine, j * 32, i * 32);
					mines.push_back(mine);
				}
			}
		}

		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < columns; j++) {
				numberedtiles.at(i).at(j).determine_neighbors(configuration, numberedtiles, i, j);
			}
		}

		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < columns; j++) {
				if (configuration.at(i).at(j) == 1) {
					numberedtiles.at(i).at(j).tilesprite.setTexture(texture.one);
				}
				else if (configuration.at(i).at(j) == 2) {
					numberedtiles.at(i).at(j).tilesprite.setTexture(texture.two);
				}
				else if (configuration.at(i).at(j) == 3) {
					numberedtiles.at(i).at(j).tilesprite.setTexture(texture.three);
				}
				else if (configuration.at(i).at(j) == 4) {
					numberedtiles.at(i).at(j).tilesprite.setTexture(texture.four);
				}
				else if (configuration.at(i).at(j) == 5) {
					numberedtiles.at(i).at(j).tilesprite.setTexture(texture.five);
				}
				else if (configuration.at(i).at(j) == 6) {
					numberedtiles.at(i).at(j).tilesprite.setTexture(texture.six);
				}
				else if (configuration.at(i).at(j) == 7) {
					numberedtiles.at(i).at(j).tilesprite.setTexture(texture.seven);
				}
				else if (configuration.at(i).at(j) == 8) {
					numberedtiles.at(i).at(j).tilesprite.setTexture(texture.eight);
				}
			}
		}

		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < columns; j++) {
				cout << configuration.at(i).at(j) << " ";
			}
			cout << endl;
		}
	}

	void draw_board(sf::RenderWindow& window, Texture& texture) {
		for (int i = 0; i < hiddentiles.size(); i++) {
			for (int j = 0; j < hiddentiles[i].size(); j++) {
				window.draw(hiddentiles.at(i).at(j).tilesprite);
			}
		}

		for (int i = 0; i < flags.size(); i++) {
			window.draw(flags.at(i).tilesprite);
		}

		for (int i = 0; i < buttons.size() - 1; i++) {
			if (ispause && i == 2) { continue; }
			window.draw(buttons.at(i).buttonsprite);
			if (!ispause && i == 2) { i++; }
		}

		for (int i = 0; i < numberedtiles.size(); i++) {
			for (int j = 0; j < numberedtiles[i].size(); j++) {
				if (numberedtiles.at(i).at(j).isrevealed) {
					hiddentiles.at(i).at(j).tilesprite.setTexture(texture.revealed);
					window.draw(hiddentiles.at(i).at(j).tilesprite);
					window.draw(numberedtiles.at(i).at(j).tilesprite);
				}
			}
		}

		for (int i = 0; i < mines.size(); i++) {
			if (mines.at(i).mineclicked) {
				hiddentiles.at(mines.at(i).positiony / 32).at(mines.at(i).positionx / 32).tilesprite.setTexture(texture.revealed);
				window.draw(hiddentiles.at(mines.at(i).positiony / 32).at(mines.at(i).positionx / 32).tilesprite);
				window.draw(mines.at(i).tilesprite);
			}
		}

		if (debugmode) {
			for (int i = 0; i < mines.size(); i++) {
				window.draw(mines.at(i).tilesprite);
			}
		}
		
		if (gameover) {
			window.draw(buttons.at(5).buttonsprite);
		}
	}

	void placeremove_flag(float coordinatex, float coordinatey, Texture& texture) {
		for (int i = 0; i < flags.size(); i++) {
			auto flagcoordinates = flags.at(i).tilesprite.getGlobalBounds();
			if (flagcoordinates.contains(coordinatex, coordinatey)) {
				flags.erase(flags.begin() + i);
				return;
			}
		}
		for (int i = 0; i < configuration.size(); i++) {
			for (int j = 0; j < configuration[i].size(); j++) {
				auto tilecoordinates = hiddentiles.at(i).at(j).tilesprite.getGlobalBounds();
				if (tilecoordinates.contains(coordinatex, coordinatey)) {
					cout << numberedtiles.at(i).at(j).getTexture() << endl;
					Tile flag_tile(texture.flag, hiddentiles.at(i).at(j).positionx, hiddentiles.at(i).at(j).positiony);
					flags.push_back(flag_tile);
					return;
				}
			}
		}
	}

	void reveal_neighbors(Number& tile, Texture& texture) {
		if (tile.isrevealed || tile.ismine) {
			return;
		}
		tile.isrevealed = true;
		if (tile.neighbors[1] == nullptr || tile.neighbors[3] == nullptr || tile.neighbors[4] == nullptr || tile.neighbors[6] == nullptr) {
			return;
		}
		if (tile.tilesprite.getTexture() == &(texture.revealed)) {
			reveal_neighbors(*tile.neighbors[1], texture);
			reveal_neighbors(*(tile.neighbors[3]), texture);
			reveal_neighbors(*(tile.neighbors[4]), texture);
			reveal_neighbors(*(tile.neighbors[6]), texture);
		}
	}

	void check(float xcoordinate, float ycoordinate, Texture& texture) {
		for (int i = 0; i < mines.size(); i++) {
			auto minecoordinates = mines.at(i).tilesprite.getGlobalBounds();
			if (minecoordinates.contains(xcoordinate, ycoordinate)) {
				gameover = true;
				mines.at(i).mineclicked = true;
			}
		}

		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < columns; j++) {
				auto tilecoordinates = numberedtiles.at(i).at(j).tilesprite.getGlobalBounds();
				if (tilecoordinates.contains(xcoordinate, ycoordinate)) {
					reveal_neighbors(numberedtiles.at(i).at(j), texture);

				}
			}
		}
	
	}


	/*void debug_mode(sf::RenderWindow& window) {
		for (int i = 0; i < mines.size(); i++) {
			window.draw(mines.at(i).tilesprite);
		}
	}*/
};