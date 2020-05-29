#include "Ship.h"
#include "SpriteComponent.h"
#include "InputComponent.h"
#include "Game.h"
#include "Laser.h"
#include "CircleComponent.h"
#include "Asteroid.h"

Ship::Ship(Game* game)
	:Actor(game)
	, mLaserCooldown(0.0f)
	, mCircle(nullptr)
	, mInvincibleTime(1.0f)
{
	// Create a sprite component
	SpriteComponent* sc = new SpriteComponent(this, 150);
	sc->SetTexture(game->GetTexture("Assets/Ship.png"));

	// Create an input component and set keys/speed
	InputComponent* ic = new InputComponent(this);
	ic->SetForwardKey(SDL_SCANCODE_W);
	ic->SetBackKey(SDL_SCANCODE_S);
	ic->SetClockwiseKey(SDL_SCANCODE_A);
	ic->SetCounterClockwiseKey(SDL_SCANCODE_D);
	ic->SetMaxForwardSpeed(300.0f);
	ic->SetMaxAngularSpeed(Math::TwoPi);

	mCircle = new CircleComponent(this);
	mCircle->SetRadius(40.0f);
}

void Ship::UpdateActor(float deltaTime) {
	mLaserCooldown -= deltaTime;

	// ***yskeno*** Exercise3.2
	if (mInvincibleTime > 0.0f) {
		mInvincibleTime -= deltaTime;

		if (GetScale() == 1.0f)
			SetScale(0.0f);
		else
			SetScale(1.0f);

		if (mInvincibleTime < 0.0f) {
			mInvincibleTime = 0.0f;
			SetScale(1.0f);
		}
	}
	else {
		for (auto ast : GetGame()->GetAsteroids()) {
			if (Intersect(*mCircle, *(ast->GetCircle()))) {
				SetState(EDead);
				mInvincibleTime = 1.0f;
				break;
			}
		}
	}
}

void Ship::ActorInput(const uint8_t* keyState) {
	if (keyState[SDL_SCANCODE_SPACE] && mLaserCooldown <= 0.0f) {
		// Create alaser and set its position/rotation to mine
		Laser* laser = new Laser(GetGame());
		laser->SetPosition(GetPosition());
		laser->SetRotation(GetRotation());

		// Reset laser cooldown (half second)
		mLaserCooldown = 0.5f;
	}
}