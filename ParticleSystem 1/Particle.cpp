#pragma once
#include "Particle.h"
#include <iostream>

Particle::Particle(sf::Vector2f init_velocity, sf::Time lifetime, sf::Vector2i emitter, float mass)
{
	this->mass = mass;
	this->init_velocity = init_velocity;
	this->lifetime = sf::seconds(rand(lifetime.asSeconds()) + 2);
	reset_pos = emitter;
}

void Particle::draw(sf::RenderTarget& target, sf::RenderStates states) const 
{
	target.draw(sprite, states);
}

// Particle Manager is responsible for calling particle state functions.
void Particle::reset(const sf::Vector2i& emitter)
{
	reset_pos = emitter;
	reset();
}

// Particle Manager is responsible for calling particle state functions.
void Particle::reset()
{
	// Reset position
	sf::Vector2f fuzzed_reset_pos = static_cast<sf::Vector2f>(reset_pos) + sf::Vector2f(rand(20, 10), rand(20, 10));
	sprite.setPosition(fuzzed_reset_pos);

	// Reset alive for to at least one second
	time_left = sf::seconds(rand(lifetime.asSeconds(), 2));

	// Reset initial velocity
	sf::Vector2f fuzzed_init_velocity = sf::Vector2f(
		rand(init_velocity.x, 0.5f * init_velocity.x), 
		rand(init_velocity.y, 0.5f * init_velocity.y));
	velocity = fuzzed_init_velocity;

	// Alternate spin direction (if non-zero)
	spin = -spin;
}

void Particle::update(const sf::Time& elapsed)
{
	time_left -= elapsed;

	velocity += elapsed.asSeconds() * acceleration;
	acceleration = sf::Vector2f(0, 0);
	sprite.setPosition(sprite.getPosition() + elapsed.asSeconds() * velocity);
	sprite.rotate(spin * rand(10));
}

void Particle::applyForce(const sf::Vector2f& force)
{
	acceleration = force / mass;
}

void Particle::setSpriteParams(const sf::Texture& texture, const sf::Color& color, const sf::Vector2f& scale)
{
	sprite.setTexture(texture);
	sprite.setColor(color);
	sprite.setScale(scale);
}
