#pragma once
#include <SFML/Graphics.hpp>
#include <map>
using namespace std;

struct Texture {
	sf::Texture hidden;
	sf::Texture happy;
	sf::Texture debug;
	sf::Texture pause;
	sf::Texture play;
	sf::Texture leaderboard;
	sf::Texture flag;
	sf::Texture revealed;
	sf::Texture mine;
	sf::Texture lose;
	sf::Texture win;
	sf::Texture one;
	sf::Texture two;
	sf::Texture three;
	sf::Texture four;
	sf::Texture five;
	sf::Texture six;
	sf::Texture seven;
	sf::Texture eight;
	sf::Texture digits;

	Texture() {
		hidden.loadFromFile("files/images/tile_hidden.png");
		happy.loadFromFile("files/images/face_happy.png");
		debug.loadFromFile("files/images/debug.png");
		pause.loadFromFile("files/images/pause.png");
		play.loadFromFile("files/images/play.png");
		leaderboard.loadFromFile("files/images/leaderboard.png");
		flag.loadFromFile("files/images/flag.png");
		revealed.loadFromFile("files/images/tile_revealed.png");
		mine.loadFromFile("files/images/mine.png");
		lose.loadFromFile("files/images/face_lose.png");
		win.loadFromFile("files/images/face_win.png");
		one.loadFromFile("files/images/number_1.png");
		two.loadFromFile("files/images/number_2.png");
		three.loadFromFile("files/images/number_3.png");
		four.loadFromFile("files/images/number_4.png");
		five.loadFromFile("files/images/number_5.png");
		six.loadFromFile("files/images/number_6.png");
		seven.loadFromFile("files/images/number_7.png");
		eight.loadFromFile("files/images/number_8.png");
		digits.loadFromFile("files/images/digits.png");
	}
};
