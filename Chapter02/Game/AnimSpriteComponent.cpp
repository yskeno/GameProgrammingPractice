#include "AnimSpriteComponent.h"

AnimSpriteComponent::AnimSpriteComponent(Actor* owner, int drawOrder)
	:SpriteComponent(owner, drawOrder)
	, mCurrFrames(0.0f)
	, mAnimFPS(24.0f)
	, mRendererFlip(SDL_FLIP_NONE)
	, mAnimStartIndex(0)
	, mAnimEndIndex(0)
	, mIsLooping(true)
	, mIsPlaying(false)
	, mToRight(true)
{}

void AnimSpriteComponent::Update(float deltaTime) {
	SpriteComponent::Update(deltaTime);

	if (mAnimTextures.size() > 0) {
		if (mIsLooping) {
			// Update the current frame based on frame rate
			// and delta rime
			mCurrFrames += mAnimFPS * deltaTime;

			// Wrap current frame if needed
			// while (mCurrFrame >= mAnimTextures.size()) {
			//	mCurrFrame -= mAnimTextures.size();}
			while (mCurrFrames > mAnimEndIndex) {
				mCurrFrames -= mAnimEndIndex;
			}
		}
		else {
			if (mIsPlaying) {
				if (mCurrFrames < mAnimStartIndex)
					mCurrFrames = mAnimStartIndex;
				if (mCurrFrames != mAnimEndIndex)
					mCurrFrames += mAnimFPS * deltaTime;
				// Check end of animations and return to startIndex.
				if (mCurrFrames > mAnimEndIndex)
					mCurrFrames = mAnimEndIndex;
				else if (mCurrFrames == mAnimEndIndex) {
					mCurrFrames = 0;
					//mCurrFrames = static_cast<float>(mAnimStartIndex);
					mIsPlaying = false;
				}
			}
		}
	}
	// Set the current texture
	SetTexture(mAnimTextures[static_cast<int>(mCurrFrames)]);
}


void AnimSpriteComponent::SetAnimTextures(const std::vector<SDL_Texture*>& textures, bool isLooping) {
	mIsLooping = isLooping;
	mAnimTextures = textures;
	if (mAnimTextures.size() > 0) {
		// Set the active texture to first frame
		mCurrFrames = 0.0f;
		mAnimStartIndex = 0;
		mAnimEndIndex = static_cast<int>(mAnimTextures.size()) - 1;
		SetTexture(mAnimTextures[static_cast<int>(mAnimStartIndex)]);
	}
}

void AnimSpriteComponent::PlayAnim(int startIndex, int endIndex) {
	if (mAnimStartIndex > mAnimEndIndex || mAnimStartIndex > (mAnimTextures.size() - 1) || mAnimEndIndex > (mAnimTextures.size() - 1)) {
		SDL_Log("Invalid Index: %i is out of range in mAnimTextures", mAnimStartIndex);
		return;
	}
	mIsPlaying = true;
	if (mToRight)
		mRendererFlip = SDL_FLIP_NONE;
	else
		mRendererFlip = SDL_FLIP_HORIZONTAL;
	mAnimStartIndex = startIndex;
	mAnimEndIndex = endIndex;
}

void AnimSpriteComponent::Draw(SDL_Renderer* renderer) {
	SpriteComponent::Draw(renderer, mRendererFlip);
}