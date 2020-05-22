#include "Character.h"
#include "AnimSpriteComponent.h"
#include "Game.h"

const float HorizontalSpeed = 250.0f;
const float VerticalSpeed = 300.0f;

Character::Character(Game* game)
	: Actor(game)
	, mRightSpeed(0.0f) {
	mAsc = new AnimSpriteComponent(this);
	std::vector<SDL_Texture*> anims = {
		// Walking
		game->GetTexture("Assets/Character01.png"),
		game->GetTexture("Assets/Character02.png"),
		game->GetTexture("Assets/Character03.png"),
		game->GetTexture("Assets/Character04.png"),
		game->GetTexture("Assets/Character05.png"),
		game->GetTexture("Assets/Character06.png"),
		// Jumping
		game->GetTexture("Assets/Character07.png"),
		game->GetTexture("Assets/Character08.png"),
		game->GetTexture("Assets/Character09.png"),
		game->GetTexture("Assets/Character10.png"),
		game->GetTexture("Assets/Character11.png"),
		game->GetTexture("Assets/Character12.png"),
		game->GetTexture("Assets/Character13.png"),
		game->GetTexture("Assets/Character14.png"),
		game->GetTexture("Assets/Character15.png"),
		// Punch
		game->GetTexture("Assets/Character16.png"),
		game->GetTexture("Assets/Character17.png"),
		game->GetTexture("Assets/Character18.png"),
	};
	mAsc->SetAnimTextures(anims, false);
}

void Character::UpdateActor(float deltaTime) {
	Actor::UpdateActor(deltaTime);
	// Update position based on speeds and delta time
	Vector2 pos = GetPosition();
	pos.x += mRightSpeed * deltaTime;


	// Restrict position to left half of screen
	if (pos.x < 25.0f) {
		pos.x = 25.0f;
		//mAsc->StopAnim();
	}
	else if (pos.x >= mWindowW - 25.0f) {
		pos.x = mWindowW - 25.0f;
		//mAsc->StopAnim();
	}
	SetPosition(pos);
}

void Character::ProcessKeyboard(const uint8_t* state) {
	mRightSpeed = 0.0f;
	// right/left
	if (state[SDL_SCANCODE_RIGHT]) {
		mRightSpeed += HorizontalSpeed;
		mAsc->SetDirection(true);
		mAsc->PlayAnim(0, 5);
	}
	if (state[SDL_SCANCODE_LEFT]) {
		mRightSpeed -= HorizontalSpeed;
		mAsc->SetDirection(false);
		mAsc->PlayAnim(0, 5);
	}
	// jumping
	if (state[SDL_SCANCODE_UP])
		mAsc->PlayAnim(6, 14);
	// punch
	if (state[SDL_SCANCODE_DOWN])
		mAsc->PlayAnim(15, 17);
}