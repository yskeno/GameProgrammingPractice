#include "Laser.h"
#include "Game.h"
#include "CircleComponent.h"
#include "Asteroid.h"

Laser::Laser(Game* game) {

}

void Laser::UpdateActor(float deltaTime) {
	// Do you intersect with an asteroid?
	for (auto ast : GetGame()->GetAsteroids()) {
		if (Intersect(*mCircle, *(ast->GetCircle()))) {
			// If this laser intersects with an asteroid,
			// set ourselves and the asteroid to dead
			SetState(EDead);
			ast->SetState(EDead);
			break;
		}
	}
}