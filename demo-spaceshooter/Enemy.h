#pragma once

#include "Entity.h"
#include "Projectile.h"

enum EnemyType {
	STILL,
	X_MOVING,
	JUMPING,
	REVERSEJUMPING,
	X_MOVINGJUMPING
};

class Enemy : public Entity {
public:
	Enemy();
	~Enemy();

	void init(int textureID, glm::fvec2 speed, glm::vec2 pos, EnemyType enemyType);
	void draw(GEngine::SpriteBatch& spriteBatch);
	void update(std::vector<Projectile*> projectiles, float deltaTime);
	void applyJump();
	void applyCollisions(glm::fvec2(speed), std::vector<Projectile*> projectiles);

	const float JUMP_SPEED = 24.0f;

	bool destroyed = false;
	bool inAir = true;			// Player falling
	bool jumped = false;		// Player has jumped
	bool normalGravity = true;	// Gravity direction
	float gravityAcceleration = 0.80f; // Gravity velocity

	EnemyType type;
};