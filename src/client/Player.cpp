#include "Player.h"
#include <iostream>

// Default constructor, just ensures that pointers are nulled
Player::Player()
{
	inputManager = NULL;
	playerID = 0;
}

Player::Player(InputManager* input)
{
	inputManager = input;
	playerID = 0;

	// Initilising our sprite
	sprite.setSize(sf::Vector2f(32.0f, 32.0f));

	switch (playerID)
	{
		// TODO: Place each player in different positions initially
		// TODO: Give each player different colors initially
	default:
		playerData.position = sf::Vector2f(32.0f, 32.0f);
		sprite.setPosition(playerData.position);
		sprite.setFillColor(sf::Color::Red);
	}
}

Player::~Player()
{
}

void Player::update(Tile** tiles)
{
	// Don't update if pointers are null
	if (inputManager == NULL)
		return;

	// Grabbing our position on the grid for this frame
	int gridX = (playerData.position.x + 16) / Tile::getSize();
	int gridY = (playerData.position.y + 16) / Tile::getSize();


	// Updating the velocity based on input, if the player isn't attacking
	if(playerData.state != (sf::Int32)playerData.SWING)
	{
		if (inputManager->keyHeld("left"))
		{
			playerData.velocity.x = -speed;
			playerData.state = (sf::Int32)playerData.LEFT;
		}
		else if (inputManager->keyHeld("right"))
		{
			playerData.velocity.x = speed;
			playerData.state = (sf::Int32)playerData.RIGHT;
		}
		else
		{
			playerData.velocity.x = 0.0f;
			playerData.state = (sf::Int32)playerData.IDLE;
		}

		if(inputManager->pressedOnce("up"))
		{
			// Preventing infinite jumps
			if(playerData.velocity.y == 0.0f)
			{
				playerData.velocity.y = -10.0f;
			}
		}
	}

	// Making the player fall if they're in the air
	// Outside the switch as the player could be moving in a direction + falling

	// If the tile under us is air
	if(tiles[gridX][gridY + 1].getID() == 0)
		playerData.velocity.y += speed / 10;	// Fall
	else
	{
		// Check if we're intersecting
		sf::FloatRect tileBounds = tiles[gridX][gridY + 1].getGlobalBounds();
		if(sprite.getGlobalBounds().intersects(tileBounds))
		{
			// Stop the player's fall
			playerData.velocity.y = 0.0f;

			// Moving the player out of the tile
			sprite.move(0, -(sprite.getGlobalBounds().top + Tile::getSize()- tileBounds.top));
		}
	}

	// If we're jumping and the tile above us is not air
	if(playerData.velocity.y < 0 && tiles[gridX][gridY - 1].getID() == 1)
	{
		sf::FloatRect tileBounds = tiles[gridX][gridY - 1].getGlobalBounds();

		// Check if we're colliding
		if(sprite.getGlobalBounds().intersects(tileBounds))
		{
			// Stop the player's ascent
			playerData.velocity.y = 0.0f;

			// Moving the player out of the tile
			sprite.move(0, tileBounds.top + tileBounds.height - sprite.getGlobalBounds().top);
		}
	}

	switch(playerData.state)
	{
		// Checking for collisions with the left side if we're moving left
		case ((sf::Int32)playerData.LEFT):
			if(tiles[gridX - 1][gridY].getID() == 1)
			{
				sf::FloatRect tileBounds = tiles[gridX - 1][gridY].getGlobalBounds();

				if(sprite.getGlobalBounds().intersects(tileBounds))
				{
					playerData.velocity.x = 0.0f;

					sprite.move(tileBounds.left + tileBounds.width - sprite.getGlobalBounds().left, 0);
				}
			}
			break;

		// Checking for collisions with the right side if we're moving right
		case ((sf::Int32)playerData.RIGHT):
			if(tiles[gridX + 1][gridY].getID() == 1)
			{
				sf::FloatRect tileBounds = tiles[gridX + 1][gridY].getGlobalBounds();

				if(sprite.getGlobalBounds().intersects(tileBounds))
				{
					playerData.velocity.x = 0.0f;

					sprite.move(-(sprite.getGlobalBounds().left + sprite.getGlobalBounds().width - tileBounds.left), 0);
				}
			}
			break;
	}

	// Moving based on velocity
	sprite.move(playerData.velocity);
	playerData.position = sprite.getPosition();
}

void Player::render(sf::RenderWindow& window)
{
	window.draw(sprite);
}