#include "NavComponent.h"
#include "Tile.h"

NavComponent::NavComponent(class Actor* owner, int updateOrder)
	:MoveComponent(owner, updateOrder)
	, mNextNode(nullptr)
{
}

void NavComponent::Update(float deltaTime) {
	if (mNextNode){
		// If you've reached the next point, advance along path
		Vector2 diff = mOwner->GetPosition() - mNextNode->GetPosition();
		if (Math::NearZero(diff.Length(), 2.0f)) {
			mNextNode = mNextNode->GetParent();
			TurnTo(mNextNode->GetPosition());
		}
	}

	MoveComponent::Update(deltaTime);
}

// Rotates the actor to face a point
void NavComponent::TurnTo(const Vector2& pos) {
	// Vector from me to pos
	Vector2 dir = pos - mOwner->GetPosition();
	// New andle is just atan2 of this dir vector
	// (Negate y because +y is down on screen)
	float angle = Math::Atan2(-dir.y, dir.x);
	mOwner->SetRotation(angle);
}