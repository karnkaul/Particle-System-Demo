#pragma once
#include "ParticleManager.h"
#include <iostream>
#include <math.h>

// 0 = OFF, 1 = Print errors and single outputs, 2 = print everything (I/O intensive)
const unsigned int DEBUG = 1;

// This will be added to template particle's deviation from centre of screen
const sf::Vector2f init_u = sf::Vector2f(30, -50);

unsigned int ParticleManager::counter = 0;

ParticleManager::ParticleManager(unsigned int count, const sf::Texture& texture, const sf::Time& average_lifetime, 
	const sf::Vector2i& emitter, sf::Vector2u window_center, const sf::Color& color)
{
	std::cout << "Particle manager " << ++counter << " created." << std::endl;
	rand_init();
	this->texture = texture;
	this->average_lifetime = average_lifetime;
	this->emitter = emitter + static_cast<sf::Vector2i>(sf::Vector2f(rand(5), rand(5)));
	this->sprite_color = color;
	
	sf::Vector2u texsize = this->texture.getSize();

	// Set template
	sf::Vector2f o = static_cast<sf::Vector2f>(window_center) * 0.5f;
	if(DEBUG >= 1)
		std::cout << "Origin: " << o.x << ", " << o.y<< std::endl;
	sf::Vector2f oe = static_cast<sf::Vector2f>(emitter) - o;
	oe = oe / sqrt(oe.x * oe.x + oe.y * oe.y);
	sf::Vector2f deviation = 100.0f * oe;
	Particle p(init_u + deviation, average_lifetime, emitter);
	p.sprite.setOrigin(static_cast<sf::Vector2f>( texsize) / 2.0f);
	p.spin = 1;
	p.setSpriteParams(texture, color, sf::Vector2f(0.2f, 0.2f));
	particle_template = p;

	particles = std::vector<Particle>(count, p);
}

void ParticleManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (_vec_p i = 0; i < particles.size(); i++) {
		target.draw(particles[i], states);
	}
}

void ParticleManager::update(const sf::Time& elapsed, const sf::Vector2i& mouse_position)
{
	for (_vec_p i = 0; i < particles.size(); ++i)
	{
		// Apply constant gravity
		particles[i].applyForce(gravity * particles[i].mass);

		// Apply central deviation force
		if (isRepellerOn)
		{
			sf::Vector2f force = repeller_force * sf::Vector2f(-1, -1);
			particles[i].applyForce(force);
		}

		// Fade to fully transparent 
		float fader = (particles[i].time_left.asSeconds() / particles[i].lifetime.asSeconds()) * 255;

		// Set color
		sf::Color color = particles[i].sprite.getColor();
		color = sf::Color(color.r, color.g, color.b, static_cast<sf::Uint8>(fader));
		particles[i].sprite.setColor(color);
		
		// Update
		particles[i].update(elapsed);

		// Reset particle
		if (particles[i].time_left <= sf::Time::Zero) 
		{
			// Randomise colour
			particles[i].sprite.setColor(sf::Color(rand(sprite_color.r, color.r / 2), rand(sprite_color.g, color.g / 2), rand(sprite_color.b, color.b / 2), static_cast<sf::Uint8>(255)));
			particles[i].reset();
		}
	}

	isRepellerOn = false;
}

void ParticleManager::setEmitter(const sf::Vector2i& emitter)
{
	this->emitter = emitter;
}

void ParticleManager::addRemoveParticles(int count)
{
	// Add new particles
	if (count > 0) {
		for (_vec_p i = 0; i < count; ++i)
		{
			particle_template.setSpriteParams(texture, sprite_color, sf::Vector2f(0.2f, 0.2f));
			particles.push_back(particle_template);
		}
	}

	// Remove particles only if particles.size() > |count|
	if (particles.size() > static_cast<_vec_p>(-count)) {
		//particles.resize(particles.size() - static_cast<_vec_p>(-count)); // Throwing weird errors :/
		for (int i = 0; i > count; --i)
			particles.pop_back();
	}

	if (DEBUG)
		std::cout << "Particle count: " << particles.size() << std::endl;
}

void ParticleManager::addForce(float force_magnitude)
{
	repeller_force = force_magnitude;
	isRepellerOn = true;
}

ParticleManager & ParticleManager::setGravity(sf::Vector2f gravity)
{
	this->gravity = gravity;
	return (*this);
}

ParticleManager & ParticleManager::setWindowCenter(sf::Vector2u window_center)
{
	this->window_center = window_center;
	return (*this);
}
