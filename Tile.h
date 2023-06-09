#pragma once
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Texture.h"

using namespace std;

class Tile {
public:
	sf::Sprite tilesprite;
	float positionx;
	float positiony;

	Tile() {
		this->positionx = 0;
		this->positiony = 0;
	}

	Tile(sf::Texture& texture, float positionx, float positiony) {
		this->positionx = positionx;
		this->positiony = positiony;
		tilesprite.setTexture(texture);
		tilesprite.setPosition(positionx, positiony);
	}
};

class Digit : public Tile {
public:
	Digit(sf::Texture& texture, float positionx, float positiony) : Tile(texture, positionx, positiony) {
		tilesprite.setTextureRect(sf::IntRect(0 * 21, 0, 21, 32));
	}

	void change_display(int num) {
		tilesprite.setTextureRect(sf::IntRect(num * 21, 0, 21, 32));
	}
};

class Button : public Tile {
public:
	Button(sf::Texture& texture, float positionx, float positiony) : Tile(texture, positionx, positiony) {}
};

class Hidden : public Tile {
public:
	bool hasbeenrevealed = false;
	Hidden(sf::Texture& texture, float positionx, float positiony) : Tile(texture, positionx, positiony) {}
};

class Flag : public Tile {
public:
	bool flagplaced = true;
	Flag(sf::Texture& texture, float positionx, float positiony) : Tile(texture, positionx, positiony) {}
};

class Mine : public Tile {
public:
	bool mineclicked = false;
	Mine(sf::Texture& texture, float positionx, float positiony) : Tile(texture, positionx, positiony) {}

};

class Number : public Tile {
public: 
	Number** neighbors;
	bool isrevealed = false;
	bool ismine = false;

	Number(sf::Texture& texture, float positionx, float positiony) : Tile(texture, positionx, positiony) {
		neighbors = new Number* [8] {nullptr};
	}
	
	void determine_neighbors(vector<vector<int>>& configuration, vector<vector<Number>>& numberedtiles, int tilerow, int tilecol) {
		int numBombs = 0;
		if (tilerow == 0) {
			neighbors[6] = &numberedtiles.at(1).at(tilecol);

			if (configuration.at(1).at(tilecol) == 9) { 
				numberedtiles.at(1).at(tilecol).ismine = true;
				numBombs += 1;}

			if (tilecol != configuration.at(0).size() - 1) {
				neighbors[4] = &numberedtiles.at(0).at(tilecol + 1);
				neighbors[7] = &numberedtiles.at(1).at(tilecol + 1);

				if (configuration.at(0).at(tilecol + 1) == 9) {
					numberedtiles.at(0).at(tilecol + 1).ismine = true;
					numBombs += 1;}
				if (configuration.at(1).at(tilecol + 1) == 9) {
					numberedtiles.at(1).at(tilecol + 1).ismine = true;
					numBombs += 1;}
			}
			if (tilecol != 0) {
				neighbors[3] = &numberedtiles.at(0).at(tilecol - 1);
				neighbors[5] = &numberedtiles.at(1).at(tilecol - 1);

				if (configuration.at(0).at(tilecol - 1) == 9) {
					numberedtiles.at(0).at(tilecol - 1).ismine = true;
					numBombs += 1;}
				if (configuration.at(1).at(tilecol - 1) == 9) {
					numberedtiles.at(1).at(tilecol - 1).ismine = true;
					numBombs += 1;}
			}
		}

		else if (tilerow == configuration.size() - 1) {
			neighbors[1] = &numberedtiles.at(tilerow - 1).at(tilecol);

			if (configuration.at(tilerow - 1).at(tilecol) == 9) { 
				numberedtiles.at(tilerow - 1).at(tilecol).ismine = true;
				numBombs += 1; }

			if (tilecol != configuration.at(tilerow).size() - 1) {
				neighbors[4] = &numberedtiles.at(tilerow).at(tilecol + 1);
				neighbors[2] = &numberedtiles.at(tilerow - 1).at(tilecol + 1);

				if (configuration.at(tilerow).at(tilecol + 1) == 9) { 
					numberedtiles.at(tilerow).at(tilecol + 1).ismine = true;
					numBombs += 1; }
				if (configuration.at(tilerow - 1).at(tilecol + 1) == 9) { 
					numberedtiles.at(tilerow - 1).at(tilecol + 1).ismine = true;
					numBombs += 1; }
			}
			if (tilecol != 0) {
				neighbors[3] = &numberedtiles.at(tilerow).at(tilecol - 1);
				neighbors[0] = &numberedtiles.at(tilerow - 1).at(tilecol - 1);

				if (configuration.at(tilerow).at(tilecol - 1) == 9) {
					numberedtiles.at(tilerow).at(tilecol - 1).ismine = true;
					numBombs += 1;}
				if (configuration.at(tilerow - 1).at(tilecol - 1) == 9) {
					numberedtiles.at(tilerow -1).at(tilecol - 1).ismine = true;
					numBombs += 1;}
			}
		}

		else if (tilecol == 0) {
			neighbors[4] = &numberedtiles.at(tilerow).at(1);

			if (configuration.at(tilerow).at(1) == 9) { 
				numberedtiles.at(tilerow).at(1).ismine = true;
				numBombs += 1; }
			if (tilerow != 0) {
				neighbors[1] = &numberedtiles.at(tilerow - 1).at(0);
				neighbors[2] = &numberedtiles.at(tilerow - 1).at(1);

				if (configuration.at(tilerow - 1).at(0) == 9) {
					numberedtiles.at(tilerow - 1).at(0).ismine = true;
					numBombs += 1; }
				if (configuration.at(tilerow - 1).at(1) == 9) { 
					numberedtiles.at(tilerow - 1).at(1).ismine = true; 
					numBombs += 1; }
			}
			if (tilerow != configuration.size() - 1) {
				neighbors[6] = &numberedtiles.at(tilerow + 1).at(0);
				neighbors[7] = &numberedtiles.at(tilerow + 1).at(1);

				if (configuration.at(tilerow + 1).at(0) == 9) {
					numberedtiles.at(tilerow + 1).at(0).ismine = true; 
					numBombs += 1;}
				if (configuration.at(tilerow + 1).at(1) == 9) {
					numberedtiles.at(tilerow + 1).at(1).ismine = true; 
					numBombs += 1;}
			}
		}

		else if (tilecol == configuration.at(tilerow).size() - 1) {
			neighbors[3] = &numberedtiles.at(tilerow).at(tilecol - 1);
			if (configuration.at(tilerow).at(tilecol - 1) == 9) {
				numberedtiles.at(tilerow).at(tilecol - 1).ismine = true;
				numBombs += 1;}
			if (tilerow != 0) {
				neighbors[1] = &numberedtiles.at(tilerow - 1).at(tilecol);
				neighbors[0] = &numberedtiles.at(tilerow - 1).at(tilecol);


				if (configuration.at(tilerow - 1).at(tilecol) == 9) { 
					numberedtiles.at(tilerow - 1).at(tilecol).ismine = true;
					numBombs += 1; }
				if (configuration.at(tilerow - 1).at(tilecol - 1) == 9) { 
					numberedtiles.at(tilerow - 1).at(tilecol - 1).ismine = true;
					numBombs += 1; }
			}
			if (tilerow != configuration.size() - 1) {
				neighbors[5] = &numberedtiles.at(tilerow + 1).at(tilecol - 1);
				neighbors[6] = &numberedtiles.at(tilerow + 1).at(tilecol);

				if (configuration.at(tilerow + 1).at(tilecol - 1) == 9) {
					numberedtiles.at(tilerow + 1).at(tilecol - 1).ismine = true;
					numBombs += 1; }
				if (configuration.at(tilerow + 1).at(tilecol) == 9) { 
					numberedtiles.at(tilerow + 1).at(tilecol).ismine = true;
					numBombs += 1; }
			}
		}

		else {
			neighbors[0] = &numberedtiles.at(tilerow - 1).at(tilecol - 1);
			neighbors[1] = &numberedtiles.at(tilerow - 1).at(tilecol);
			neighbors[2] = &numberedtiles.at(tilerow - 1).at(tilecol + 1);
			neighbors[3] = &numberedtiles.at(tilerow).at(tilecol-1);
			neighbors[4] = &numberedtiles.at(tilerow).at(tilecol + 1);
			neighbors[5] = &numberedtiles.at(tilerow + 1).at(tilecol - 1);
			neighbors[6] = &numberedtiles.at(tilerow + 1).at(tilecol);
			neighbors[7] = &numberedtiles.at(tilerow + 1).at(tilecol + 1);

			if (configuration.at(tilerow - 1).at(tilecol - 1) == 9) {
				numberedtiles.at(tilerow - 1).at(tilecol - 1).ismine = true;
				numBombs += 1;}
			if (configuration.at(tilerow - 1).at(tilecol) == 9) {
				numberedtiles.at(tilerow - 1).at(tilecol).ismine = true;
				numBombs += 1;}
			if (configuration.at(tilerow - 1).at(tilecol + 1) == 9) {
				numberedtiles.at(tilerow - 1).at(tilecol + 1).ismine = true;
				numBombs += 1;}
			if (configuration.at(tilerow).at(tilecol - 1) == 9) {
				numberedtiles.at(tilerow).at(tilecol - 1).ismine = true;
				numBombs += 1;}
			if (configuration.at(tilerow).at(tilecol + 1) == 9) {
				numberedtiles.at(tilerow).at(tilecol + 1).ismine = true;
				numBombs += 1;}
			if (configuration.at(tilerow + 1).at(tilecol - 1) == 9) {
				numberedtiles.at(tilerow + 1).at(tilecol - 1).ismine = true;
				numBombs += 1;}
			if (configuration.at(tilerow + 1).at(tilecol) == 9) { 
				numberedtiles.at(tilerow + 1).at(tilecol).ismine = true;
				numBombs += 1;}
			if (configuration.at(tilerow + 1).at(tilecol + 1) == 9) {
				numberedtiles.at(tilerow + 1).at(tilecol + 1).ismine = true;
				numBombs += 1;}
		}
		if (configuration.at(tilerow).at(tilecol) != 9) {
			configuration.at(tilerow).at(tilecol) = numBombs;
		}
	}
};