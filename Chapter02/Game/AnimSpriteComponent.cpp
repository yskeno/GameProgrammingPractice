#include "AnimSpriteComponent.h"

AnimSpriteComponent::AnimSpriteComponent(Actor* owner, int drawOrder)
	:SpriteComponent(owner, drawOrder)
	, mCurrFrames(0.0f)
	, mAnimFPS(24.0f)
{}

void AnimSpriteComponent::Update(float deltaTime) {
	SpriteComponent::Update(deltaTime);

	if (mAnimTextures.size() > 0) {
		// Update the current frame based on frame rate
		// and delta rime
		mCurrFrames += mAnimFPS * deltaTime;

		// Wrap current frame if needed
		while (mCurrFrames >= mAnimTextures.size()) {
			mCurrFrames -= mAnimTextures.size();
		}

		// Set the current texture
		SetTexture(mAnimTextures[static_cast<int>(mCurrFrames)]);
	}
}