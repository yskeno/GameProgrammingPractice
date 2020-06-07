#include "AIState.h"
#include <SDL/SDL_log.h>
#include "AIComponent.h"

void AIPatrol::Update(float deltaTime) {
	// Do some other updating
	SDL_Log("Updating %s state", GetName());

	bool dead = true;
	if (dead)
		mOwner->ChangeState("Death");
}

void AIPatrol::OnEnter() {
	SDL_Log("Entering %s state", GetName());
}

void AIPatrol::OnExit() {
	SDL_Log("Exiting %s state", GetName());
}



void AIDeath::Update(float deltaTime) {
	SDL_Log("Updating %s state", GetName());
}

void AIDeath::OnEnter() {
	SDL_Log("Entering %s state", GetName());
}

void AIDeath::OnExit() {
	SDL_Log("Exiting %s state", GetName());
}



void AIAttack::Update(float deltaTime) {
	SDL_Log("Updating %s state", GetName());
}

void AIAttack::OnEnter() {
	SDL_Log("Entering %s state", GetName());
}

void AIAttack::OnExit() {
	SDL_Log("Exiting %s state", GetName());
}