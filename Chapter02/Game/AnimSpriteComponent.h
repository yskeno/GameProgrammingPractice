// technique like flipbook animation: a seried of static 2D images
// a vector of the different images corresponding to each frame

#pragma once
#include "SpriteComponent.h"
#include <vector>
class AnimSpriteComponent :
	public SpriteComponent
{
public:
	AnimSpriteComponent(class Actor* owner, int drawOrder = 100);
	// Update animation every frame (overriden from component)
	void Update(float deltaTime) override;

	void Draw(SDL_Renderer* renderer) override;

	// Set the textures used for animation
	// simply sets the mAnimTextures to provided vector
	// and resets mCurrFrame to zero.
	// also calls the SetTexture() and passes in the first frame of the animation.
	// ***yskeno*** Exercise 2.2 - add bool isLooping parameter.
	void SetAnimTextures(const std::vector<SDL_Texture*>& textures, bool isLooping = true);

	// Set/get the animation FPS
	float GetAnimFPS() const { return mAnimFPS; }
	void SetAnimFPS(float fps) { mAnimFPS = fps; }

	// ***yskeno*** Exercise 2.2
	void PlayAnim(int startIndex, int endIndex);
	void StopAnim() { mIsPlaying = false; };
	void SetDirection(bool toTheRight) { mToRight = toTheRight; }

private:
	// All textures in the animation
	std::vector<SDL_Texture*> mAnimTextures;

	// Current frame displayed
	float mCurrFrames;
	// Animation frame rate
	// allows different animated sprites to run at different frame rates.
	// also allows the animation to dynamically speed up or slow down.
	float mAnimFPS;

	// ***yskeno*** Exercise 2.2
	bool mIsLooping;
	bool mIsPlaying;
	int mAnimStartIndex;
	int mAnimEndIndex;
	bool mToRight;

	SDL_RendererFlip mRendererFlip;
};

