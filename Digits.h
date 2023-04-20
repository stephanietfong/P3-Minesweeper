#pragma once
#include <SFML/Graphics.hpp>

class Digit {
	sf::Sprite tilesprite;
	sf::IntRect spriterect;
	float positionx;
	float positiony;
public:
	Digit(sf::Texture& texture, float positionx, float positiony) {
		this->positionx = positionx;
		this->positiony = positiony;
		tilesprite.setTexture(texture);
		tilesprite.setPosition(positionx, positiony);		
	}

	void change_display() {
	}
};