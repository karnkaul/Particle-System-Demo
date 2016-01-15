#include <iostream>
//#include "ParticleSystem.h"
#include "Particle.h"
#include "ParticleManager.h"
#include "SFML/Audio.hpp"

// Window params
const short int AA_LEVEL = 8;
const bool VSYNC = true;
const float VOLUME = 80;

// Particle system params
const unsigned int N = 1000, FSVM = 2;
const sf::Time TTL = sf::seconds(5);
const sf::Vector2f GRAVITY(0, 40);

// Global variables
bool isRepellerOn = false;
sf::Texture texture1, texture2, texture3;
std::vector<ParticleManager> pms;
sf::Clock world;
sf::Vector2u window_size;

// Declarations
void exit(sf::RenderWindow&);
void modifyPms(sf::Event&);
void addRemoveParticles(int count);

int main()
{
	sf::ContextSettings _settings;
	_settings.antialiasingLevel = AA_LEVEL;

	sf::RenderWindow window(sf::VideoMode::getFullscreenModes()[FSVM], "Particles", sf::Style::Default, _settings);
	//sf::RenderWindow window(sf::VideoMode::getFullscreenModes()[0], "Particles", sf::Style::Fullscreen, _settings);
	window.setVerticalSyncEnabled(VSYNC);

	sf::Music music;
	music.openFromFile("../Assets/Music.ogg");
	music.setLoop(true);
	music.setVolume(VOLUME);
	music.play();

	sf::Clock clock;
	texture1.loadFromFile("../Assets/Texture.png");
	texture2.loadFromFile("../Assets/Texture2.png");
	texture3.loadFromFile("../Assets/Texture3.png");
	
	window_size = sf::Vector2u(window.getSize());
	ParticleManager pm(500, texture1, TTL, sf::Vector2i(400, 400), window_size);
	pms.push_back(pm.setGravity(GRAVITY).setWindowCenter(sf::Vector2u(window.getSize())));

	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event)) {
			switch (event.type)
			{
			case sf::Event::Closed:
				exit(window);
				break;

			case sf::Event::KeyPressed:
				switch (event.key.code) {
				case sf::Keyboard::Up:
					addRemoveParticles(100);
					break;
				case sf::Keyboard::Down:
					addRemoveParticles(-100);
					break;
				case sf::Keyboard::Space:
					isRepellerOn = !isRepellerOn;
				}

			case sf::Event::MouseButtonPressed:
				modifyPms(event);
				break;

			default:
				break;
			}
		}

		sf::Time elapsed = clock.restart();
		sf::Vector2i mouse = sf::Mouse::getPosition();

		window.clear(sf::Color::Black);

		for (std::vector<ParticleManager>::size_type i = 0; i < pms.size(); ++i)
		{
			if (!pms.empty()) {
				if (isRepellerOn)
					pms[i].addForce(100);
				pms[i].update(elapsed, mouse);
				window.draw(pms[i]);
			}
		}
		window.display();
	}

	return 0;
}

void addRemoveParticles(int count)
{
	for (std::vector<ParticleManager>::iterator pm = pms.begin(); pm != pms.end(); pm++)
	{
		(*pm).addRemoveParticles(count);
	}
}

void modifyPms(sf::Event& event)
{
	if (event.mouseButton.button == sf::Mouse::Button::Left)
	{
		std::cout << "Request for new particle manager at: (" << event.mouseButton.x << ", " << event.mouseButton.y << ")" << std::endl;

		// Cycle textures
		sf::Texture* temp;
		sf::Time newTTL = TTL + sf::seconds(rand(10) - 5);
		switch (static_cast<int>(pms.size()) % 3)
		{
		case 0:
			temp = &texture1;
			break;
		case 1:
			temp = &texture2;
			break;
		default:
			temp = &texture3;
			break;
		}

		ParticleManager pm(500, *temp, newTTL, sf::Vector2i(event.mouseButton.x, event.mouseButton.y), window_size);
		pms.push_back(pm.setGravity(GRAVITY));
	}
	else if (event.mouseButton.button == sf::Mouse::Button::Right)
	{
		std::cout << "Request to delete particle manager " << pms.size() << std::endl;
		if(!pms.empty())
			pms.pop_back();
	}
}

void exit(sf::RenderWindow& window)
{
	// do other stuff
	window.close();
}
