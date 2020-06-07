#pragma once
#include <vector>
#include "Math.h"

class Actor
{
public:
	// Used to track state of actor
	enum State {
		EActive,
		EPaused,
		EDead
	};

	// Constructor/destructor
	Actor(class Game* game);
	virtual ~Actor();

	// Called every frame to update the Actor
	// Update function called from Game (not overridable)
	void Update(float deltaTime);
	// Updates all the components attached to the actor (not overridable)
	void UpdateComponents(float deltaTime);
	// Any actor-specific update code (overridable)
	virtual void UpdateActor(float deltaTime);

	// ProcessInput() function called from Game (not overridable)
	void ProcessInput(const uint8_t* keyState);
	// Any actor-specific input code (overridable)
	// Actor subclasses that want custom input can override this.
	virtual void ActorInput(const uint8_t* keyState);

	// Getters/setters
	const Vector2& GetPosition() const { return mPosition; }
	void SetPosition(const Vector2& pos) { mPosition = pos; }
	float GetScale() const { return mScale; }
	void SetScale(float scale) { mScale = scale; }
	float GetRotation() const { return mRotation; }
	void SetRotation(float rotation) { mRotation = rotation; }

	Vector2 GetForward() const { return Vector2(Math::Cos(mRotation), -Math::Sin(mRotation)); }

	State GetState() const { return mState; }
	void SetState(State state) { mState = state; }

	class Game* GetGame() { return mGame; }

	// Add/remove components
	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);

private:
	// Actor's state
	State mState;

	// Transform
	Vector2 mPosition;	// Center position of actor
	float mScale;		// Uniform scale of actor (1.0f for 100%)
	float mRotation;	// Rotation angle (in radians)

	// Components held by this actor
	std::vector<class Component*> mComponents;
	class Game* mGame;
};

