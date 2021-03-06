#ifndef CLIENT_H
#define CLIENT_H
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/UdpSocket.hpp>
#include <vector>
#include "InputManager.h"
#include "ImageManager.h"
#include "Tile.h"
#include "Player.h"
#include "PlayerData.h"
#include "NetworkedPlayer.h"

class Client
{
	public:
		Client();
		~Client();
		int run();
	private:
		// Returns the position in the vector for the tile (x,y) onscreen
		int getVectorPos(int x, int y);
		void update();
		void simulatePlayers();
		void render();
		inline sf::Int32 serverTime() { return serverStartTime + clock.getElapsedTime().asMilliseconds(); + pingTime; }
	private:
		bool close = false;
		bool windowFocus = true;
		bool sendUpdate = false;
		const sf::Vector2i windowBounds;

		InputManager inputManager;
		ImageManager imageManager;
		sf::Clock updateClock;
		sf::Clock clock;
		sf::RenderWindow window;
		sf::Event event;

		sf::Int32 pingTime;
		sf::Int32 serverStartTime;

		sf::TcpSocket* tcpSocket;
		sf::UdpSocket* udpSocket;

		Player* player;	// The player that this client controls
		NetworkedPlayer* otherPlayers[3];

		Tile** tiles;
};
#endif//CLIENT_H
