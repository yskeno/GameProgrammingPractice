#include "Game.h"

const int thickness = 15;
const float paddleH = 100.0f;

// yskeno: modify Window size.
// default is 1080*768.
const int windowW = 640;
const int windowH = 480;

// Constructor: Member Initializer List
// Initialize member variables by declaration order.
Game::Game()
	: mWindow(nullptr)
	, mRenderer(nullptr)
	, mTicksCount(0)
	, mIsRunning(true)
	, mPaddleDir(0)
	, mPaddlePos{ 10.0f, static_cast<float>(windowH) / 2.0f }
	, mBallPos{ static_cast<float>(windowW) / 2.0f, static_cast<float>(windowH) / 2.0f }
	, mBallVel{ -200.0f, 235.0f }
{
}

bool Game::Initialize() {
	// Initialize SDL
	// SDL subsystems full listed: https://wiki.libsdl.org
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult != 0) {
		// output messages to the console in SDL
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	// Create an SDL Window
	mWindow = SDL_CreateWindow(
		"Game Programming in C++(Chapter1)",	// Window title
		100,		// Top left x-coordinate of window
		100,		// Top left y-coordinate of window
		windowW,	// Width of window
		windowH,	// Height of window
		0			// Window Creation Flags (0 for no flag set)
	);
	if (!mWindow) {
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	//// Create SDL renderer
	mRenderer = SDL_CreateRenderer(
		mWindow						// Window to create renderer for
		, -1						// Usually -1(let SDL decide). Graphicks driver to use - if the game has multiple windows, this might be relevant.
		, SDL_RENDERER_ACCELERATED	// Accelerated renderer(it takes advantage of graphics hardware)
		| SDL_RENDERER_PRESENTVSYNC	// Enable vertical synchronization
	);
	if (!mRenderer) {
		SDL_Log("Failed to create renderer: %s", SDL_GetError);
		return false;
	}

	// Initialize mBall & mPaddlePos
	/* Initialize in constructor initialization list
	mPaddlePos.x = 10.0f; mPaddlePos.y = static_cast<float>(windowH) / 2.0f; mBallPos.x = static_cast<float>(windowW) / 2.0f;
	mBallPos.y = static_cast<float>(windowH) / 2.0f; mBallVel.x = -200.0f; mBallVel.y = 235.0f;
	*/
	return true;
}

void Game::Shutdown() {
	// the opposite of Initialize.
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();

}

void Game::RunLoop() {
	// keeps running iterations of the game loop until mIsRunning becomes false.
	while (mIsRunning) {
		// 3 helper functions for each phase of the game loop
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput() {
	SDL_Event event;
	// While there are still events in the queue
	while (SDL_PollEvent(&event)) {
		switch (event.type)
		{
			// Handle different event types here
			// If we get an SDL_QUIT event, end game loop
		case SDL_QUIT:
			mIsRunning = false;
			break;
		}
	}

	// Get state of keyboard
	// SDL_GetKeyboardState: returns a pointer to an array that contains the current state of the keyboard
	const Uint8* state = SDL_GetKeyboardState(NULL);
	// if escape is pressed, also end loop
	if (state[SDL_SCANCODE_ESCAPE])
		mIsRunning = false;

	// Update paddle direction based on W/S keys
	mPaddleDir = 0;
	if (state[SDL_SCANCODE_W])
		mPaddleDir -= 1;
	if (state[SDL_SCANCODE_S])
		mPaddleDir += 1;
	// when both W/S key are pressed, mPaddleDir is zero because add and subtract from mPaddleDir(0).
}

void Game::UpdateGame() {
	// frame limiting
	// Wait until 16ms has elapsed since last frame
	// because physics will have difference in behavior on the frame rate.
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;

	// Delta time is the difference in ticks from last frame
	// (converted to seconds)
	// SDL_GetTicks() returns the msec since initialization
	// elapsed sec = (current msec from init - last frame msec from init)/1000.0f
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

	// Clamp maximum delta time value
	// To prevent huge delta time while stop on a breakpoint in the debugger.
	// 0.05sec is the frame length of 20FPS.
	if (deltaTime > 0.05f)
		deltaTime = 0.05f;

	// Update tick counts (for next frame)
	mTicksCount = SDL_GetTicks();

	// Update paddle position based on direction(W/S keys input)
	if (mPaddleDir != 0) {
		// paddle move speed is 300.0f pixels/seconds
		mPaddlePos.y += mPaddleDir * 300.0f * deltaTime;
		// Make sure paddle doesn't move off screen!
		// for boundary on top of screen
		if (mPaddlePos.y < (paddleH / 2.0f + thickness))
			mPaddlePos.y = paddleH / 2.0f + thickness;
		// for boundary on bottom of screen
		else if (mPaddlePos.y > (windowH - paddleH / 2.0f - thickness))
			mPaddlePos.y = windowH - paddleH / 2.0f - thickness;
	}

	// Update ball position based on ball 
	mBallPos.x += mBallVel.x * deltaTime;
	mBallPos.y += mBallVel.y * deltaTime;

	// Bounce if needed
	// ***for x-coordinate***
	// Did we intersect with the paddle?
	// Which side(upper/lower) the ball is on.
	float diff = mPaddlePos.y - mBallPos.y;
	// Take absolute value of difference
	// diff > 0.0f is "the ball is on lower side".
	diff = (diff > 0.0f) ? diff : -diff;
	// Our y-difference is small enough
	// the ball exists within the paddle height positions.
	if (diff <= paddleH / 2.0f
		// We are in the correct x-position
		// the ball x-position is not same as the paddle x-position
		&& mBallPos.x <= 25.0f && mBallPos.x >= 20.0f
		// the ball is moving to the left
		&& mBallVel.x < 0.0f)
		mBallVel.x *= -1.0f;
	// Did the ball go off the screen?(if so, end game)
	else if (mBallPos.x <= 0.0f) {
		// Continue dialog and reset "only" ball position so the ball moves differently from last game.
		if (0 == SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, "Game Over!", "See you next time.", mWindow)) {
			// Reset only the ball position.
			mBallPos.x = static_cast<float>(windowW) / 2.0f;
			mBallPos.y = static_cast<float>(windowH) / 2.0f;
			mBallVel = { -200.0f, 235.0f };
			return;
		}
		mIsRunning = false;
	}
	// Did the ball collide with the right wall?
	else if (mBallPos.x >= (windowW - thickness) && mBallVel.x > 0.0f)
		mBallVel.x *= -1.0f;

	// ***for y-coordinate***
	// Did the ball collide with the top wall?
	// Did the ball collide with the bottom wall?
	// if (ball is into top wall && ball move upward) || <for the bottom>
	if ((mBallPos.y <= thickness && mBallVel.y < 0.0f)
		|| (mBallPos.y >= (windowH - thickness) && mBallVel.y > 0.0f))
		mBallVel.y *= -1.0f;
}

void Game::GenerateOutput() {
	// 1.Clear the back buffer to a color(the game's current buffer).
	// 2.Draw the entire game scene.
	// 3.Swap the front buffer and back buffer.

	// Set draw color to blue
	SDL_SetRenderDrawColor(
		mRenderer,
		0,		// R
		0,		// G
		255,	// B
		255		// A
	);

	// Clear back buffer
	SDL_RenderClear(mRenderer);

	// Draw walls
	// Change the draw color to white
	SDL_SetRenderDrawColor(mRenderer, 200, 200, 200, 200);

	// Draw top walls
	// top-left corner of screen is (0,0).
	// positive x is to the right, and positive y is down.
	SDL_Rect wall{
		0,			// Top left x
		0,			// Top left y
		windowW,	// Width (hard-coded)
		thickness	// Height
	};
	// draw the rectangle
	SDL_RenderFillRect(mRenderer, &wall);

	// Draw bottom wall
	// same rectangle as the top except that the top-left y coordinate 
	wall.y = windowH - thickness;
	SDL_RenderFillRect(mRenderer, &wall);

	// Draw right wall
	wall.x = windowW - thickness;
	wall.y = 0;
	wall.w = thickness;
	wall.h = windowH;
	SDL_RenderFillRect(mRenderer, &wall);


	// Draw paddle
	// convert(static_cast) from floats into integer(which SDL_Rect uses).
	SDL_Rect paddle{
		static_cast<int>(mPaddlePos.x),
		static_cast<int>(mPaddlePos.y - paddleH / 2),
		thickness,
		static_cast<int>(paddleH)
	};
	SDL_RenderFillRect(mRenderer, &paddle);

	// Draw ball
	SDL_Rect ball{
		static_cast<int>(mBallPos.x - thickness / 2),
		static_cast<int>(mBallPos.y - thickness / 2),
		thickness,
		thickness
	};
	SDL_RenderFillRect(mRenderer, &ball);

	// Swap front buffer and back buffer
	SDL_RenderPresent(mRenderer);
}