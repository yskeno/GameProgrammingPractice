#include "SpriteComponent.h"
#include "Actor.h"
#include "Game.h"

SpriteComponent::SpriteComponent(Actor* owner, int drawOrder)
	: Component(owner)
	, mTexture(nullptr)
	, mDrawOrder(drawOrder)
	, mTexHeight(0)
	, mTexWidth(0)
{
	mOwner->GetGame()->AddSprite(this);
}

SpriteComponent::~SpriteComponent() {
	mOwner->GetGame()->RemoveSprite(this);
}
void SpriteComponent::Draw(SDL_Renderer* renderer) {
	if (mTexture) {
		// Owner's position variable specifies the center position of the actor.
		// So, compute the coordinates for the top-left corner.
		SDL_Rect r;
		// Scale the width/height by owner's scale
		r.w = static_cast<int>(mTexWidth * mOwner->GetScale());
		r.h = static_cast<int>(mTexHeight * mOwner->GetScale());
		// Center the rectangle around the position of owner
		r.x = static_cast<int>(mOwner->GetPosition().x - r.w / 2);
		r.y = static_cast<int>(mOwner->GetPosition().y - r.h / 2);

		// Draw (have to convert angle from radians to degrees, and clockwise to counter)
		// Renders a texture to rhe rendering target
		// Returns 0 on success, negative value on failure
		/*SDL_RenderCopyEx(
			SDL_Renderer* renderer,		// Render target to draw to
			SDL_Texture* texture,		// Texture to draw
			const SDL_Rect* srcrect,	// Part of texture to draw (null if whole)
			const SDL_Rect* dstrect,	// Rectangle to draw on the target
			double angle,				// Rotation angle (in degree, clockwise)
			const SDL_Point* center,	// Point to rotate about (nullptr for center)
			SDL_RenderFlip flip			// How to flip texture (usually SDL_FLIP_NONE)
		);*/
		SDL_RenderCopyEx(renderer,
			mTexture,
			nullptr,
			&r,
			// Actor uses an angle in radians so convert to degree.
			// In a SDL, positive angle is clockwise.
			// But this is the opposite of the unit circle so negate the angle.
			-Math::ToDegrees(mOwner->GetRotation()),
			nullptr,
			SDL_FLIP_NONE);
	}
}

void SpriteComponent::SetTexture(SDL_Texture* texture) {
	mTexture = texture;
	// Get width/height of texture
	SDL_QueryTexture(texture, nullptr, nullptr, &mTexWidth, &mTexHeight);
}