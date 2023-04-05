// Core Libraries
#include <crtdbg.h>
#include <iostream>
#include <Windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <vector>

//Global variables are accessible from any context 
constexpr float FPS = 60.0f; // target frames per second
constexpr float DELAY_TIME = 1000.0f / FPS; // target time between frames in ms
const int WINDOW_WIDTH = 1200;
const int WINDOW_HEIGHT = 600;
float deltaTime = 1.0f / FPS;

bool isGameRunning = true;


// Anything with a star * to its right is a pointer type. SDL_Window* is a memory address of an SDL_Window
//Pointers can also be set to point to memory address 0, which means nothing. We call these NULL pointers.
//Computer memory is addressable by one number, like a homeaddress on one very , very long street.

SDL_Window* pWindow = nullptr; // assignin a pointer to mullptr means the address is 0
SDL_Renderer* pRenderer = nullptr; // NULL is the address 0. =NULL is essentiall the same as NULLptr


SDL_Texture* pMySprite;
SDL_Rect mySpriteSrc;
SDL_Rect mySpriteDst;
SDL_Rect src;
SDL_Rect dst;
//////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////Ye - Namespace////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace ye
{

	struct Vec2
	{
		float x;
		float y;

	};

	struct Sprite
	{

	private:

		SDL_Texture* pTexture;
		SDL_Rect src;
		SDL_Rect dst;

		float animationCurrentFrame = 0.0f;

	public:

		double rotationDegrees = 0;
		SDL_RendererFlip flipState = SDL_FLIP_NONE;
		Vec2 position;
		int animationFrameCount = 1;


		void setSize(Vec2 sizeWidthHeight)
		{
			dst.w = sizeWidthHeight.x;
			dst.h = sizeWidthHeight.y;
		}

		void setSize(int w, int h)
		{
			dst.w = w;
			dst.h = h;
		}

		Vec2 getSize()
		{
			Vec2 sizeXY{ dst.w, dst.h };
			return sizeXY;
		}

		void SetSpriteSheetFrameSize(int width, int height)
		{
			src.w = width;
			src.h = height;
		}

		Sprite()
		{
			std::cout << "Sprite Default Constructor" << std::endl;
			pTexture = nullptr;
			src = SDL_Rect{ 0, 0, 0, 0 };
			dst = SDL_Rect{ 0, 0, 0 ,0 };
		}

		//a non-default constructor
		Sprite(SDL_Renderer* renderer, const char* imageFilePath)
		{

			std::cout << "Sprite filepath Constructor" << std::endl;
			src = SDL_Rect{ 0, 0, 0, 0 };

			pTexture = IMG_LoadTexture(renderer, imageFilePath);

			if (pTexture == NULL)
			{
				std::cout << "Image Load Failed! " << SDL_GetError() << std::endl;
			}
			else
			{
				std::cout << "Image Load Succesful! " << std::endl;
			}

			SDL_QueryTexture(pTexture, NULL, NULL, &src.w, &src.h);

			dst = SDL_Rect{ 0, 0, src.w, src.h };

		}

		Sprite(SDL_Renderer* renderer, const char* imageFilePath, int frameSizeX, int frameSizeY, int frameCount) : Sprite(renderer, imageFilePath)
		{
			SetSpriteSheetFrameSize(frameSizeX, frameSizeY);
			setSize(frameSizeX, frameSizeY);
			animationFrameCount = frameCount;

			pTexture = IMG_LoadTexture(renderer, imageFilePath);

			if (pTexture == NULL)
			{
				std::cout << "Image Load Failed! " << SDL_GetError() << std::endl;
			}
			else
			{
				std::cout << "Image Load Succesful! " << std::endl;
			}
		}

		void draw(SDL_Renderer* renderer)
		{
			dst.x = position.x;
			dst.y = position.y;
			SDL_RenderCopyEx(renderer, pTexture, &src, &dst, rotationDegrees, NULL, flipState);

		}

		void NextFrame()
		{
			SetFrame(animationCurrentFrame + 1);
		}

		void SetFrame(int frame)
		{
			animationCurrentFrame = frame % animationFrameCount;
			src.x = src.w * animationCurrentFrame;
		}

		void AddFrameTime(float timeScale)
		{
			animationCurrentFrame += timeScale;
			if (animationCurrentFrame >= animationFrameCount)
			{
				animationCurrentFrame -= animationFrameCount;
			}
		}

		void setPosition(int x, int y)
		{
			position.x = x;
			position.y = y;
		}


		SDL_Rect GetRect() const
		{
			SDL_Rect returnValue = dst;
			returnValue.x = position.x;
			returnValue.y = position.y;
			return returnValue;

		}
	}; // struct sprite


	class Bullet
	{
	public:

		Sprite sprite;
		Vec2 velocity;

		//move bullet
		void Update()
		{
			sprite.position.x += velocity.x * deltaTime;
			sprite.position.y += velocity.y * deltaTime;
		}

	};

	class Ship
	{
	public:

		Sprite sprite;
		float movesSpeedPx = 100;
		float fireRepeatDelay = 0.5f;


	private:

		float fireRepeatTimer = 0.0f;

	public:

		void Move(Vec2 input)
		{
			sprite.position.x += input.x * ((movesSpeedPx * deltaTime) + 0.5);
			sprite.position.y += input.y * ((movesSpeedPx * deltaTime) + 0.5);
		};


		void Shoot(bool towardRight, std::vector<Bullet>& container, Vec2 velocity)
		{
			//create new bullet
			Sprite renderAmmo = Sprite(pRenderer, "../Assets/textures/cannonball.png");

			//start bullet at player sprite pos
			renderAmmo.position.x = sprite.position.x;
			if (towardRight)
			{
				renderAmmo.position.x += sprite.getSize().x;
			}

			renderAmmo.position.y = sprite.position.y + (sprite.getSize().y / 2) - (renderAmmo.getSize().y / 2);

			Bullet bullet;
			bullet.sprite = renderAmmo;
			bullet.velocity = velocity;

			// add bullet to container
			container.push_back(bullet);

			//reset cool down timer
			fireRepeatTimer = fireRepeatDelay;

		};

		void Update()
		{
			fireRepeatTimer -= deltaTime;
		};

		bool CanShoot()
		{
			return (fireRepeatTimer <= 0.0f);
		}

	};

	// Part of AABB collision detection. 
	// Returns true if the bounds defined by minA and maxA overlap with the bounds defined by minB and maxB
	bool AreBoundsOverlapping(int minA, int maxA, int minB, int maxB)
	{
		bool isOverlapping = false;
		if (maxA >= minB && maxA <= maxB) // check if max of A is contained inside B
			isOverlapping = true;
		if (minA <= maxB && maxA >= maxB)
			isOverlapping = true;
		return isOverlapping;
	}

	bool AreSpritesColliding(Sprite A, Sprite B)
	{
		SDL_Rect boundsA = A.GetRect();
		SDL_Rect boundsB = B.GetRect();

		SDL_bool isColliding = SDL_HasIntersection(&boundsA, &boundsB);

		return (bool)isColliding;
	}


}

using namespace ye;

// enemy ships / shooting
Ship enemyShip;
std::vector<Ship> enemyContainer;
std::vector<Bullet> enemyBulletContainer;
float enemySpawnDelay = 2.0f;
float enemySpawnTimer = 0.0f;

//character bullets
std::vector<Bullet> bulletContainer;

///////////

Sprite backgroundImage;
Sprite backgroundImage2;

Ship playerShip;

Sprite kelp;
Sprite rock1;
Sprite lighthouse;

Sprite newSprite;


//////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////Init////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
// this is the exact same as joss' now
bool Init()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cout << "Sdl Init Failed" << SDL_GetError() << std::endl;
		return false;
	}

	std::cout << "Sdl Init Success" << std::endl;


	// Display Main SDL Window
	pWindow = SDL_CreateWindow("Thornewell_101466157", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0
		// (borderless) SDL_WINDOW_MAXIMIZED | SDL_WINDOW_BORDERLESS 
	);


	if (pWindow == NULL) //if the window creations failed...
	{
		std::cout << "Window Creation Failed! " << SDL_GetError() << std::endl;
		return false;
	}
	else
	{
		std::cout << "Window Creation Succesful! " << std::endl;
	}

	//get pointer to SDL_Renderer object for use of drawing sprites
	pRenderer = SDL_CreateRenderer(pWindow, -1, 0);

	if (pRenderer == NULL)
	{
		std::cout << "Renderer Creation Failed! " << SDL_GetError() << std::endl;
		return false;
	}
	else
	{
		std::cout << "Renderer Creation Succesful! " << std::endl;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////Load////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

void load()
{
	//loading all textures...

	backgroundImage = Sprite(pRenderer, "../Assets/textures/Still_water_image2.png");
	backgroundImage.position = { 0,0 };
	backgroundImage.setSize(1200, 600);

	backgroundImage2 = Sprite(pRenderer, "../Assets/textures/Still_water_image3.png");
	backgroundImage2.position = { 1200,0 };
	backgroundImage2.setSize(1200, 600);


	/////////////////////////////////////////////////

	char* imageToLoad = "../Assets/textures/usership.png";
	playerShip.sprite = Sprite(pRenderer, imageToLoad);

	Vec2 playerSize = playerShip.sprite.getSize();
	int playerWidth = playerSize.x / 10;
	int playerHeight = playerSize.y / 10;

	playerShip.sprite.setSize(playerWidth, playerHeight);

	playerShip.sprite.position.x = (WINDOW_WIDTH / 8); //start with left eighth
	playerShip.sprite.position.y = (WINDOW_HEIGHT / 2) - playerHeight / 2; // in middle


	/////////////////////////////////////////////////

	kelp = Sprite(pRenderer, "../Assets/textures/kelp.png");
	kelp.position = { 500, 300 };


	rock1 = Sprite(pRenderer, "../Assets/textures/rock1.png");
	rock1.position = { 700, 200 };

	/////////////////////////////////////////////////

	char* imageToLoad2 = "../Assets/textures/lighthouse.png";
	lighthouse = Sprite(pRenderer, imageToLoad2);

	Vec2 lighthouseSize = lighthouse.getSize();
	int lighthouseWidth = lighthouseSize.x / 4;
	int lighthouseHeight = lighthouseSize.y / 4;

	lighthouse.setSize(lighthouseWidth, lighthouseHeight);
	lighthouse.position = { 1000, 450 };

	///////////////////////////////////////////////////

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////Input////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

// player input variables
bool isUpPressed = false;
bool isDownPressed = false;
bool isShootPressed = false;
bool isForwardPressed = false;
bool isBackPressed = false;

//const float playerSpeedPx = 600.0f; //pixels per second
//const float playerShootCoolDownDuration = 0.1f; // time between shots
//float playerShootCoolDownTimer = 0.0f; //determines when we can shoot again
int bulletSpeed = 600.0f;

void Input()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case (SDL_KEYDOWN):

			switch (event.key.keysym.scancode)
			{
			case (SDL_SCANCODE_W):
				isUpPressed = true;

				break;

			case(SDL_SCANCODE_S):
				isDownPressed = true;

				break;

			case(SDL_SCANCODE_A):
				isBackPressed = true;

				break;

			case (SDL_SCANCODE_D):
				isForwardPressed = true;

				break;

			case(SDL_SCANCODE_SPACE):
				isShootPressed = true;


				break;
			}
			break;
		}

		switch (event.type)
		{
		case(SDL_KEYUP):
			switch (event.key.keysym.scancode)
			{
			case(SDL_SCANCODE_W):
				isUpPressed = false;

				break;

			case (SDL_SCANCODE_S):
				isDownPressed = false;

				break;

			case (SDL_SCANCODE_A):
				isBackPressed = false;

				break;

			case(SDL_SCANCODE_D):
				isForwardPressed = false;

				break;

			case (SDL_SCANCODE_SPACE):
				isShootPressed = false;

				break;
			}
		}
	}

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////Update////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

void UpdatePlayer()
{
	ye::Vec2 inputVector{};

	if (isUpPressed)
	{
		inputVector.y = -1;

		if (playerShip.sprite.position.y < 0)
			playerShip.sprite.position.y = 0;
	}
	if (isDownPressed)
	{

		inputVector.y = 1;
		const int lowestPointOnScreen = WINDOW_HEIGHT - playerShip.sprite.getSize().y;
		if (playerShip.sprite.position.y > lowestPointOnScreen)
		{
			playerShip.sprite.position.y = lowestPointOnScreen;
		}
	}
	if (isForwardPressed)
	{
		inputVector.x = 1;

		if (playerShip.sprite.position.x >= WINDOW_WIDTH - playerShip.sprite.getSize().x)
		{
			playerShip.sprite.position.x = WINDOW_WIDTH - playerShip.sprite.getSize().x;
		}
	}
	if (isBackPressed)
	{
		inputVector.x = -1;
		const int lowestSideOnScreen = 0;
		if (playerShip.sprite.position.x <= lowestSideOnScreen)
		{
			playerShip.sprite.position.x = lowestSideOnScreen;
		}
	}

	//if player is off cooldown
	if (isShootPressed && playerShip.CanShoot())
	{
		bool toRight = true;
		Vec2 velocity = { 500, 0 };
		playerShip.Shoot(toRight, bulletContainer, velocity);

	}

	playerShip.Move(inputVector);
	playerShip.Update();
}

void EnemySpawner()
{
	enemyShip.sprite = Sprite(pRenderer, "../Assets/textures/enemy1.png");
	enemyShip.sprite.position.x = 1200 + 54; //spawns them outside of the screen on the right hand side
	enemyShip.sprite.position.y = rand() % 720 - 61; //randomized pos

	Ship enemy1;
	enemy1.sprite = enemyShip.sprite; //sprites stay the same
	enemy1.fireRepeatDelay = 100.0f; //setting fire speed
	enemy1.movesSpeedPx = 100; // making sure it moves at the same time

	enemyContainer.push_back(enemy1);

	Bullet enemyBullet1;
	enemyBulletContainer.push_back(enemyBullet1);

	enemySpawnTimer = enemySpawnDelay;
}

void detectCollisions()
{

	Sprite& playerSprite = playerShip.sprite;

	//std::vector<Bullet>::iterator
	for (auto it = enemyBulletContainer.begin(); it != enemyBulletContainer.end();)
	{
		Sprite& bulletSprite = it->sprite;

		if (AreSpritesColliding(playerSprite, bulletSprite))
		{
			playerShip.sprite.rotationDegrees += 10;

			//destroy the bullet
			it = enemyBulletContainer.erase(it);

		}
		else
		{
			it++;
		}
	}

	//player bullets vs enemy ship

	for (auto itBullet = bulletContainer.begin(); itBullet != bulletContainer.end();)
	{

		for (auto itEnemy = enemyContainer.begin(); itEnemy != enemyContainer.end();)
		{
			Sprite& bullet = itBullet->sprite;
			Sprite& enemy = itEnemy->sprite;

			if (AreSpritesColliding(itBullet->sprite, itEnemy->sprite))
			{
				//destroy both
				itBullet = bulletContainer.erase(itBullet);
				itEnemy = enemyContainer.erase(itEnemy);

				if (itBullet == bulletContainer.end())
				{
					break;
				}
			}
			else
			{
				itEnemy++;
			}

		}
		if (itBullet != bulletContainer.end())
		{
			itBullet++;
		}
	}
}



void Update()
{

	UpdatePlayer();

	//this will be moving background... do not delete

	backgroundImage.position.x -= 2;

	if (backgroundImage.position.x <= -1200)
	{
		backgroundImage.position.x = 0;
	}

	backgroundImage2.position.x -= 2;

	if (backgroundImage2.position.x <= 0)
	{
		backgroundImage2.position.x = 1200;
	}

	//move all bullets across the screen
	for (int i = 0; i < bulletContainer.size(); i++)
	{
		bulletContainer[i].Update();
	}

	//check this later
	for (int i = 0; i < enemyBulletContainer.size(); i++)
	{
		enemyBulletContainer[i].Update();
	}

	for (int i = 0; i < enemyContainer.size(); i++)
	{
		Ship& enemyBois = enemyContainer[i];
		enemyBois.Move({ -1, 0 });
		enemyBois.Update();
		if (enemyBois.CanShoot())
		{
			bool towardsRight = false;
			Vec2 velocity = { -400, 0 };
			enemyBois.Shoot(towardsRight, enemyBulletContainer, velocity);
		}
	}

	//check these later
	enemySpawnTimer -= deltaTime;

	if (enemySpawnTimer <= 0)
	{
		EnemySpawner();
	}

	detectCollisions();

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////Draw////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//this should be cleaned up now
void Draw()
{
	SDL_SetRenderDrawColor(pRenderer, 0, 0, 20, 255);
	SDL_RenderClear(pRenderer);

	// calling on the draw function for background

	backgroundImage.draw(pRenderer);
	backgroundImage2.draw(pRenderer);

	// calling on user ship
	playerShip.sprite.draw(pRenderer);

	//calling on first object
	kelp.draw(pRenderer);

	//calling on rock1
	rock1.draw(pRenderer);

	//draw ALL bullets on screen
	for (int i = 0; i < bulletContainer.size(); i++)
	{
		bulletContainer[i].sprite.draw(pRenderer);
	}

	//enemy
	for (int i = 0; i < enemyContainer.size(); i++)
	{
		enemyContainer[i].sprite.draw(pRenderer);
	}
	//enemy bullets
	for (int i = 0; i < enemyBulletContainer.size(); i++)
	{
		enemyBulletContainer[i].sprite.draw(pRenderer);
	}
	// calling on lighthouse
	lighthouse.draw(pRenderer);


	//show the back buffer
	SDL_RenderPresent(pRenderer);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////Main////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* args[])
{

	// show and position the application console
	AllocConsole();
	auto console = freopen("CON", "w", stdout);
	const auto window_handle = GetConsoleWindow();
	MoveWindow(window_handle, 100, 700, 800, 200, TRUE);


	isGameRunning = Init();

	load();

	//show back buffer which we have been drawing to prior. this is part of a common rendering tech called double buffering.
	//SDL_RenderPresent(pRenderer);


	// Main Game Loop, each iteration of the loop is one frame of the game
	while (isGameRunning)
	{
		//get time in ms at the start of the time
		const auto frame_start = static_cast<float>(SDL_GetTicks());

		//CALLING Three functions in sequence...
		Input();    //input from player
		Update();   //update game state
		Draw();     //draw to screen

		if (const float frame_time = static_cast<float>(SDL_GetTicks()) - frame_start; //statement excecuted before if statement
			frame_time < DELAY_TIME) //if statement condition (if we completed our input-update-draw in less than target time
		{
			//wait for the difference, do nothing
			SDL_Delay(static_cast<int>(DELAY_TIME - frame_time));
		}

		// delta time
		const auto delta_time = (static_cast<float>(SDL_GetTicks()) - frame_start) / 1000.0f;


	}

	//getchar();
	return 0;
}

