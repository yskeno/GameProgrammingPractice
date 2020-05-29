#pragma once
#include "Actor.h"

class Ship :
	public Actor
{
public:
	Ship(class Game* game);

	void UpdateActor(float deltaTime) override;
	void ActorInput(const uint8_t* keyState) override;

	void ResetPosition();

private:
	class CircleComponent* mCircle;
	// the amount of time until the player can fire again.
	float mLaserCooldown;

	Vector2 mStartPosition;
	float mInvincibleTime;
};