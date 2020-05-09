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
	, mMoveSpeedFactor(1.0f)
	, mMoveSpeedFactorMax(1.3f)
	, mIsPlaying(true)
	, mIs2PlayersMode(false)
	, mPaddle2Dir(0)
	, mPaddle2Pos{ static_cast<float>(windowW - 10 - thickness), static_cast<float>(windowH) / 2.0f }
{
}

bool Game::Initialize()
{
	// Initialize SDL
	// SDL subsystems full listed: https://wiki.libsdl.org
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult != 0)
	{
		// output messages to the console in SDL
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	// Is 2 Players Mode?
	mIs2PlayersMode = ModeSelect();

	// Create an SDL Window
	mWindow = SDL_CreateWindow(
		"Game Programming in C++(Chapter1)", // Window title
		100,								 // Top left x-coordinate of window
		100,								 // Top left y-coordinate of window
		windowW,							 // Width of window
		windowH,							 // Height of window
		0									 // Window Creation Flags (0 for no flag set)
	);
	if (!mWindow)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	//// Create SDL renderer
	mRenderer = SDL_CreateRenderer(
		mWindow // Window to create renderer for
		,
		-1 // Usually -1(let SDL decide). Graphicks driver to use - if the game has multiple windows, this might be relevant.
		,
		SDL_RENDERER_ACCELERATED		// Accelerated renderer(it takes advantage of graphics hardware)
		| SDL_RENDERER_PRESENTVSYNC // Enable vertical synchronization
	);
	if (!mRenderer)
	{
		SDL_Log("Failed to create renderer: %s", SDL_GetError);
		return false;
	}

	// Initialize mBall
	mBalls = {/*Vec*/ {/*Struct*/ {/*Vector2*/ static_cast<float>(windowW) / 2.0f, static_cast<float>(windowH) / 2.0f}, {-200.0f, 235.0f}} };

	return true;
}

bool Game::ModeSelect() {
	const SDL_MessageBoxButtonData modeButtons[] = {
		// {.flags, .buttonid, .text}
		{0, 1, "2 Players"},
		{SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 0, "1 Player"},
	};
	const SDL_MessageBoxColorScheme msgBoxColor{ {
		{255, 0, 0},   // [SDL_MESSAGEBOX_COLOR_BACKGROUND]
		{0, 255, 0},   // [SDL_MESSAGEBOX_COLOR_TEXT]
		{255, 255, 0}, // [SDL_MESSAGEBOX_COLOR_BUTTON_BORDER]
		{0, 0, 255},   // [SDL_MESSAGEBOX_COLOR_BUTTON_BACKGROUND]
		{255, 0, 255}  // [SDL_MESSAGEBOX_COLOR_BUTTON_SELECTED]
	} };
	const SDL_MessageBoxData modeMsgBoxData = {
		0,							// .flags
		NULL,						// .window
		"Play mode select",			// .title
		"",							// .message
		SDL_arraysize(modeButtons), // .numbuttons
		modeButtons,				// .buttons
		&msgBoxColor				// .colorScheme
	};
	int buttonId;
	if (SDL_ShowMessageBox(&modeMsgBoxData, &buttonId) < 0)
	{
		SDL_Log("error displaying message box");
		return false;
	}
	if (buttonId == 1)
	{
		SDL_Log("PlayMode: 2 Players");
		return true;
	}
	else
	{
		SDL_Log("PlayMode: 1 Players");
		return false;
	}
}
void Game::Shutdown()
{
	// the opposite of Initialize.
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

void Game::RunLoop()
{
	// keeps running iterations of the game loop until mIsRunning becomes false.
	while (mIsRunning)
	{
		// 3 helper functions for each phase of the game loop
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput()
{
	SDL_Event event;
	// While there are still events in the queue
	while (SDL_PollEvent(&event))
	{
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

	// Update paddle2 direction based on I/K keys
	if (mIs2PlayersMode)
	{
		mPaddle2Dir = 0;
		if (state[SDL_SCANCODE_I])
			mPaddle2Dir -= 1;
		if (state[SDL_SCANCODE_K])
			mPaddle2Dir += 1;
	}
}

void Game::UpdateGame()
{
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
	UpdatePaddlePos(mPaddleDir, mPaddlePos, deltaTime);

	// Update paddle2 position based on direction(I/K keys input)
	if (mIs2PlayersMode)
		UpdatePaddlePos(mPaddle2Dir, mPaddle2Pos, deltaTime);

	float diff;
	// Update ball position based on ball
	for (auto& ball : mBalls)
	{
		ball.mBallPos.x += ball.mBallVel.x * deltaTime;
		ball.mBallPos.y += ball.mBallVel.y * deltaTime;

		// Bounce if needed
		// ***for x-coordinate***
		// Did we intersect with the paddle?
		// Which side(upper/lower) the ball is on.
		// + value: ball is upper from paddle
		// - value: ball is lower from paddle
		diff = mPaddlePos.y - ball.mBallPos.y;
		// Take absolute value of difference
		// Get distance between paddlePos and ballPos.
		diff = (diff > 0.0f) ? diff : -diff;

		// Our y-difference is small enough
		// the ball exists within the paddle height positions.
		if (diff <= paddleH / 2.0f
			// We are in the correct x-position
			// the ball x-position is not same as the paddle x-position
			&& ball.mBallPos.x <= 25.0f && ball.mBallPos.x >= 20.0f
			// the ball is moving to the left
			&& ball.mBallVel.x < 0.0f)
			BounceOffPaddle(ball.mBallVel);

		// Did the ball go off the screen?(if so, end game)
		else if (GoOffScreen(ball.mBallPos, ball.mBallVel))
			return;

		// Did the ball collide with the right wall?(when single player mode)
		else if (!mIs2PlayersMode && ball.mBallPos.x >= (windowW - thickness) && ball.mBallVel.x > 0.0f)
			ball.mBallVel.x *= -1.0f;

		// for player 2 paddle
		if (mIs2PlayersMode)
		{
			float diff2 = mPaddle2Pos.y - ball.mBallPos.y;
			diff2 = (diff2 > 0.0f) ? diff2 : -diff2;
			if (diff2 <= paddleH / 2.0f && ball.mBallPos.x >= (windowW - 25.0f) && ball.mBallPos.x <= (windowW - 20.0f) && ball.mBallVel.x > 0.0f)
				BounceOffPaddle(ball.mBallVel);
		}

		// ***for y-coordinate***
		// Did the ball collide with the top wall?
		// Did the ball collide with the bottom wall?
		// if ( (ball is into top wall && ball move upward) || (for the bottom) )
		if ((ball.mBallPos.y <= thickness && ball.mBallVel.y < 0.0f) || (ball.mBallPos.y >= (windowH - thickness) && ball.mBallVel.y > 0.0f))
			ball.mBallVel.y *= -1.0f;
	}
}

void Game::GenerateOutput()
{
	// 1.Clear the back buffer to a color(the game's current buffer).
	// 2.Draw the entire game scene.
	// 3.Swap the front buffer and back buffer.

	// Set draw color to blue
	if (mIsPlaying)
		SDL_SetRenderDrawColor(
			mRenderer,
			0,	 // R
			0,	 // G
			255, // B
			255	 // A
		);
	else
		SDL_SetRenderDrawColor(
			mRenderer,
			255, // R
			0,	 // G
			0,	 // B
			255	 // A
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
		0,		  // Top left x
		0,		  // Top left y
		windowW,  // Width (hard-coded)
		thickness // Height
	};
	// draw the rectangle
	SDL_RenderFillRect(mRenderer, &wall);

	// Draw bottom wall
	// same rectangle as the top except that the top-left y coordinate
	wall.y = windowH - thickness;
	SDL_RenderFillRect(mRenderer, &wall);

	// Draw paddle
	// convert(static_cast) from floats into integer(which SDL_Rect uses).
	SDL_Rect paddle{
		static_cast<int>(mPaddlePos.x),
		static_cast<int>(mPaddlePos.y - paddleH / 2),
		thickness,
		static_cast<int>(paddleH) };
	SDL_RenderFillRect(mRenderer, &paddle);

	if (mIs2PlayersMode)
	{
		// Drae paddle for player 2
		paddle.x = static_cast<int>(mPaddle2Pos.x);
		paddle.y = static_cast<int>(mPaddle2Pos.y - paddleH / 2);
		paddle.w = thickness;
		paddle.h = static_cast<int>(paddleH);
		SDL_RenderFillRect(mRenderer, &paddle);
	}
	else
	{
		// Draw right wall
		wall.x = windowW - thickness;
		wall.y = 0;
		wall.w = thickness;
		wall.h = windowH;
		SDL_RenderFillRect(mRenderer, &wall);
	}

	// Draw ball
	for (auto& ballclass : mBalls)
	{
		SDL_Rect ball{
			static_cast<int>(ballclass.mBallPos.x - thickness / 2),
			static_cast<int>(ballclass.mBallPos.y - thickness / 2),
			thickness,
			thickness };
		SDL_RenderFillRect(mRenderer, &ball);
	}

	// Swap front buffer and back buffer
	SDL_RenderPresent(mRenderer);
}

void Game::UpdatePaddlePos(const int& paddleDir, Vector2& paddlePos, const float& deltaTime)
{
	// Update paddle position based on direction(W/S keys input)
	if (paddleDir != 0)
	{
		// paddle move speed is 300.0f pixels/seconds
		paddlePos.y += paddleDir * 300.0f * mMoveSpeedFactor * deltaTime;
		// Make sure paddle doesn't move off screen!
		// for boundary on top of screen
		if (paddlePos.y < (paddleH / 2.0f + thickness))
			paddlePos.y = paddleH / 2.0f + thickness;
		// for boundary on bottom of screen
		else if (paddlePos.y > (windowH - paddleH / 2.0f - thickness))
			paddlePos.y = windowH - paddleH / 2.0f - thickness;
	}
}

void Game::BounceOffPaddle(Vector2& mBallVel)
{
	mBallVel.x *= -1.0f;

	// Paddle & Ball move speed faster
	mMoveSpeedFactor = (mMoveSpeedFactorMax > mMoveSpeedFactor * 1.05f) ? mMoveSpeedFactor * 1.05f : mMoveSpeedFactorMax;
	if (mMoveSpeedFactorMax != mMoveSpeedFactor)
		mBallVel.x *= mMoveSpeedFactor;
	SDL_Log("mBallVel.x: %f mBallVel.y: %f mPaddle mMoveSpeedFactor: %f / %f", mBallVel.x, mBallVel.y, mMoveSpeedFactor, mMoveSpeedFactorMax);
}

bool Game::GoOffScreen(Vector2& mBallPos, Vector2& mBallVel)
{
	// Did the ball go off the screen?(if so, end game)
	if (mBallPos.x <= 0.0f || (mIs2PlayersMode && mBallPos.x >= windowW))
	{
		// Set BG color red.
		mIsPlaying = false;
		GenerateOutput();
		// Continue dialog and reset "only" ball position so the ball moves differently from last game.
		if (mBallPos.x <= 0.0f)
			if (!mIs2PlayersMode)
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, "Game Over!", "See you next time.", mWindow);
			else
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, "Game Set!", "Player 2 win!", mWindow);
		else
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, "Game Set!", "Player 1 win!", mWindow);

		// Reset only the ball position.
		mBallPos.x = static_cast<float>(windowW) / 2.0f;
		mBallPos.y = static_cast<float>(windowH) / 2.0f;
		mBallVel = { -200.0f, 235.0f };
		mMoveSpeedFactor = 1.0f;
		mIsPlaying = true;
		return true;

		//mIsRunning = false;
	}
	return false;
}