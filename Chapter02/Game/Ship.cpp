#include "Ship.h"
#include "AnimSpriteComponent.h"
#include "Game.h"

const float HorizontalSpeed = 250.0f;
const float VerticalSpeed = 300.0f;

Ship::Ship(Game* game)
	: Actor(game)
	, mRightSpeed(0.0f)
	, mDownSpeed(0.0f) {
	// Create an animated sprite component
	// with the associated textures
	AnimSpriteComponent* asc = new AnimSpriteComponent(this);
	std::vector<SDL_Texture*> anims = {
		game->GetTexture("Assets/Ship01.png"),
		game->GetTexture("Assets/Ship02.png"),
		game->GetTexture("Assets/Ship03.png"),
		game->GetTexture("Assets/Ship04.png"),
	};
	asc->SetAnimTextures(anims);
}

void Ship::UpdateActor(float deltaTime) {
	Actor::UpdateActor(deltaTime);
	// Update position based on speeds and delta time
	Vector2 pos = GetPosition();
	pos.x += mRightSpeed * deltaTime;
	pos.y += mDownSpeed * deltaTime;

	// Restrict position to left half of screen
	if (pos.x < 25.0f)
		pos.x = 25.0f;
	else if (pos.x > mWindowW / 2)	// ***yskeno*** Default = 500.0f
		pos.x = mWindowW / 2;
	if (pos.y < 25.0f)
		pos.y = 25.0f;
	else if (pos.y > mWindowH - 25)	// ***yskeno*** Default = 734.0f
		pos.y = mWindowH - 25;
	SetPosition(pos);
}

void Ship::ProcessKeyboard(const uint8_t* state) {
	mRightSpeed = 0.0f;
	mDownSpeed = 0.0f;
	// right/left
	if (state[SDL_SCANCODE_D])
		mRightSpeed += HorizontalSpeed;
	if (state[SDL_SCANCODE_A])
		mRightSpeed -= HorizontalSpeed;
	// up/down
	if (state[SDL_SCANCODE_S])
		mDownSpeed += VerticalSpeed;
	if (state[SDL_SCANCODE_W])
		mDownSpeed -= VerticalSpeed;
}