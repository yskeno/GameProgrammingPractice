#include "Game.h"

int main(int argc, char** argv) {
	// constructs an instance of Game class.
	Game game;
	bool success = game.Initialize();
	if (success)
		game.RunLoop();
	game.Shutdown();
	return 0;
}