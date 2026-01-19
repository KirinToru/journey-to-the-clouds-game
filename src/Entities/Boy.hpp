#pragma once
#include "SFML/Graphics.hpp"

class Boy {
public:
	Boy();	//Constructor

	//Func that activates physics
	void update(float dt);

	//Func that is rendering player on the screen
	void render(sf::RenderWindow & window);

private:
	sf::RectangleShape shape;	//Player's shape

	//Physics variables
	sf::Vector2f velocity;	//Velocity vector
	bool isGrounded;	//Is player on the ground

	//Player movement parameters
	float moveSpeed;
	float gravity;
	float jumpStrength;
};