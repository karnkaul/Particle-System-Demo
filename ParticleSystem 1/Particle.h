#pragma once
#include <SFML/Graphics.hpp>
#include "Utility.h"

const float  PI = 3.142;

class Particle : public sf::Drawable, public sf::Transformable
{
public:
	float mass;
	short int spin = 0;
	sf::Vector2f velocity, init_velocity;
	sf::Vector2f acceleration;
	sf::Time lifetime, time_left;
	sf::Sprite sprite;

	Particle(sf::Vector2f init_velocity=sf::Vector2f(50, -50), sf::Time lifetime=sf::seconds(5.0f), 
		sf::Vector2i emitter = sf::Vector2i(0, 0), float mass = 1);

	// Particle Manager is responsible for calling reset(), update(), and applyForce()
	void reset();
	void reset(const sf::Vector2i&); 		// Reset to non-default position
	void update(const sf::Time&);
	void applyForce(const sf::Vector2f&);

	friend float rand(float, float);		// Random range function

	// Convenience function
	void setSpriteParams(const sf::Texture&, const sf::Color& color = sf::Color::White, 
		const sf::Vector2f& scale = sf::Vector2f(1, 1));

private:
	sf::Vector2i reset_pos;
	virtual void draw(sf::RenderTarget&, sf::RenderStates) const;
};
typedef std::vector<Particle>::size_type _vec_p;
