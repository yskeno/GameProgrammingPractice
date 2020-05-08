#pragma once
#include "SDL/SDL.h"

// Vector2 struct just stores x/y coordinates(for now)
// for ball and paddle, because these doesn't work with hard coding x/y.
struct Vector2
{
	float x;
	float y;
};

class Game
{
public:
	Game();
	// Initialize the game
	bool Initialize();
	// Runs the game loop until the game is over
	void RunLoop();
	// Shutdown the game
	void Shutdown();
private:
	// Helper functions for the game loop
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	// Window created by SDL
	SDL_Window* mWindow;
	// Renderer for 2D drawing
	SDL_Renderer* mRenderer;
	// Number of ticks since start of game(msec)
	Uint32 mTicksCount;
	// Game should continue to run
	bool mIsRunning;

	// Pong specific
	// Position of paddle
	Vector2 mPaddlePos;
	// Direction of paddle
	int mPaddleDir;
	// Position of ball
	Vector2 mBallPos;
	// Velocity of ball
	Vector2 mBallVel;

	// Addition
	bool mIsPlaying;
	float mPaddleSpeedFactor;
	float mPaddleMaxSpeedFactor;
};

