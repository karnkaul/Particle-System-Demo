#pragma once
#include "Particle.h"
#include <SFML/Graphics.hpp>

class ParticleManager : public sf::Drawable
{
private:
	sf::Vector2i emitter;
	std::vector<Particle> particles;
	sf::Texture texture;
	sf::Color sprite_color;
	Particle particle_template;

	// For debugging
	static unsigned int counter;

	virtual void draw(sf::RenderTarget&, sf::RenderStates) const;

public:
	bool scaleParticle = false;
	sf::Time average_lifetime;
	sf::Vector2f gravity;
	sf::Vector2u window_center;

	float repeller_force;
	bool isRepellerOn = false;

	ParticleManager(unsigned int, const sf::Texture&, const sf::Time& average_lifetime, const sf::Vector2i& emitter, 
		sf::Vector2u window_center, const sf::Color& = sf::Color::White);
	void update(const sf::Time&, const sf::Vector2i&);
	void setEmitter(const sf::Vector2i&);
	void addRemoveParticles(int);
	void addForce(float force_magnitude);
	ParticleManager& setGravity(sf::Vector2f);
	ParticleManager& setWindowCenter(sf::Vector2u);
	friend float rand(float, float);
	friend void rand_init();
};
