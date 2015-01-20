#pragma once

#include <functional>

#include <glm/glm.hpp>

#include "Vertex.h"
#include "SpriteBatch.h"
#include "GLTexture.h"


namespace GEngine {
	class Particle2D {
	public:
		glm::fvec2 position = glm::vec2(0.0f);
		glm::fvec2 velocity = glm::vec2(0.0f);
		GEngine::ColorRGBA8 color;
		float life = 0.0f;
		float width = 0.0f;
	};

	inline void defaultParticleUpdate(Particle2D& particle, float deltaTime) {
		particle.position += particle.velocity * deltaTime;
	}

	class ParticleBatch2D {
	public:
		ParticleBatch2D();
		~ParticleBatch2D();

		void init(int maxParticles, float decayRate, GLTexture texture, std::function<void(Particle2D&, float)> updateFunc = defaultParticleUpdate);

		void update(float deltaTime);

		void draw(SpriteBatch* spriteBatch);

		void addParticle(const glm::fvec2& position, const glm::fvec2& velocity, const GEngine::ColorRGBA8& color, float width);

	private:
		int findFreeParticle();

		std::function<void(Particle2D&, float)> m_updateFunc;
		float m_decayRate = 0.1f;
		Particle2D* m_particles = nullptr;
		int m_maxParticles = 0;
		int m_lastFreeParticle = 0;
		GLTexture m_texture;
	};
}
