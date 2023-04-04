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

namespace ye
{

	struct Vec2
	{
		float x;
		float y;

	};

	struct Sprite
	{

		void setPosition(int x, int y)
		{
			position.x = x;
			position.y = y;

		}
		Vec2 getSize()
		{
			Vec2 sizeXY{ dst.w, dst.h };
			return sizeXY;
		}
		void setSize(int w, int h)
		{
			dst.w = w;
			dst.h = h;
		}

	public:

		Vec2 position;
		SDL_Texture* pTexture;
		SDL_Rect src;
		SDL_Rect dst;
		double rotationDegrees = 0;
		SDL_RendererFlip flipState = SDL_FLIP_NONE;



		Sprite()
		{
			pTexture = nullptr;
			src = { 0, 0, 0, 0 };
			dst = { 0, 0, 0 ,0 };
		}

		void draw(SDL_Renderer* renderer)
		{

			int result = SDL_RenderCopy(renderer, pTexture, &src, &dst);
			/*if (result != 0)
			{
				std::cout << "Render Failed!" << SDL_GetError() << std::endl;
			}*/
		}

		//a non-default constructor
		Sprite(SDL_Renderer* renderer, const char* imageFilePath)
		{

			pTexture = IMG_LoadTexture(renderer, imageFilePath);

			if (pTexture == NULL)
			{
				std::cout << "Image Load Failed! " << SDL_GetError() << std::endl;
			}
			else
			{
				std::cout << "Image Load Succesful! " << std::endl;
			}

			if (SDL_QueryTexture(pTexture, NULL, NULL, &src.w, &src.h) != 0)
			{
				std::cout << "Query Texture Failed! " << SDL_GetError() << std::endl;
			}
			src.x = 0;
			src.y = 0;

			dst.x = 0;
			dst.y = 0;
			dst.w = src.w;
			dst.h = src.h;
		}

		SDL_Rect GetRect() const
		{
			SDL_Rect returnValue = dst;
			returnValue.x = position.x;
			returnValue.y = position.y;
			return returnValue;

		}
	};
	class Bullet
	{
	public:

		Sprite sprite;
		Vec2 velocity;
		void Update()
		{
			sprite.dst.x += velocity.x * deltaTime;

			sprite.dst.y += velocity.y * deltaTime;
		}

	};

	class Enemy
	{
	public:

		Sprite sprite;
		float fireRepeatDelay = 100.0f;
		float movesSpeedPx = 100;


	private:

		float fireRepeatTimer = 0.0f;

	public:

		void Move(Vec2 input)
		{

			sprite.dst.x += input.x * (movesSpeedPx * deltaTime);
			sprite.dst.y += input.y * (movesSpeedPx * deltaTime);
		};

		void Shoot(bool towardRight, std::vector<Bullet>& container, Vec2 velocity)
		{

			//create new bullet
			Sprite renderAmmo = Sprite(pRenderer, "../Assets/textures/bullet.png");

			//start bullet at player sprite pos
			renderAmmo.dst.x = sprite.dst.x;
			if (towardRight)
			{
				renderAmmo.dst.x += sprite.getSize().x;
			}

			renderAmmo.dst.y = sprite.dst.y + (sprite.getSize().y / 2) - (sprite.getSize().x / 2);


			Bullet bullets;
			bullets.sprite = renderAmmo;
			bullets.velocity = velocity;
			// add bullet to a container
			container.push_back(bullets);

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

	bool AreSpritesColliding(const Sprite& A, const Sprite& B)
	{
		// get the bounds of each sprite on x and y
		int minAx, maxAx, minBx, maxBx;
		int minAy, maxAy, minBy, maxBy;

		SDL_Rect boundsA = A.GetRect();
		SDL_Rect boundsB = B.GetRect();

		// Sprite A
		minAx = boundsA.x;
		minAy = boundsA.y;
		maxAx = boundsA.x + boundsA.w;
		maxAy = boundsA.y + boundsA.h;

		// Sprite B
		minBx = boundsB.x;
		minBy = boundsB.y;
		maxBx = boundsB.x + boundsB.w;
		maxBy = boundsB.y + boundsB.h;

		bool overlapOnX = AreBoundsOverlapping(minAx, maxAx, minBx, maxBx);
		bool overlapOnY = AreBoundsOverlapping(minAx, maxAx, minBx, maxBx);

		SDL_bool isColliding = SDL_HasIntersection(&boundsA, &boundsB);
		return isColliding;
	}

}

using namespace ye;

// enemy ships / shooting
Enemy enemyShip;
std::vector<Enemy> enemyContainer;
std::vector<Bullet> enemyBulletContainer;
float enemySpawnDelay = 2.0f;
float enemySpawnTimer = 0.0f;


Sprite backgroundImage = Sprite();
Sprite backgroundImage2 = Sprite();
Sprite playerShip = Sprite();

Sprite kelp = Sprite();
Sprite rock1 = Sprite();
Sprite lighthouse1 = Sprite();

std::vector<Sprite> bulletContainer;
Sprite newSprite;



//init is short for initialize, we are setting p the game window, start SDL feature, ect
bool Init()
{

	SDL_Init(SDL_INIT_EVERYTHING);

	// Display Main SDL Window
	// get pointer to SDL_WINDOW object

	pWindow = SDL_CreateWindow("Thornewell_101466157", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0
		// (borderless) SDL_WINDOW_MAXIMIZED | SDL_WINDOW_BORDERLESS 
	);


	if (pWindow == NULL) //if the window creations failed...
	{
		std::cout << "Window Creation Failed! " << SDL_GetError() << std::endl;
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
	}
	else
	{
		std::cout << "Renderer Creation Succesful! " << std::endl;
	}
	return true;
}



void load()
{
	//loading all textures...
	//SDL_Texture * IMG_LoadTexture(SDL_Renderer *renderer, const char *file)
	backgroundImage = Sprite(pRenderer, "../Assets/textures/Still_water_image2.png");

	backgroundImage2 = Sprite(pRenderer, "../Assets/textures/Still_water_image3.png");

	backgroundImage2.dst.w = 1200;
	backgroundImage2.dst.h = 600;
	backgroundImage2.dst.x = 1200;

	backgroundImage.dst.w = 1200;
	backgroundImage.dst.h = 600;
	backgroundImage.dst.x = 0;

	/////////////////////////////////////////////////

	playerShip = Sprite(pRenderer, "../Assets/textures/realexplodingcat1.png");


	int userWidth = playerShip.src.w / 10;
	int userHeight = playerShip.src.h / 10;

	playerShip.dst.w = userWidth;
	playerShip.dst.h = userHeight;
	playerShip.dst.x = (WINDOW_WIDTH / 8); //start with left eighth
	playerShip.dst.y = (WINDOW_HEIGHT / 2) - userHeight / 2; // in middle


	/////////////////////////////////////////////////

	kelp = Sprite(pRenderer, "../Assets/textures/kelp.png");


	int objectWidth = kelp.src.w / 2;
	int objectHeight = kelp.src.h / 2;

	kelp.dst.w = objectWidth;
	kelp.dst.h = objectHeight;
	kelp.dst.x = (WINDOW_WIDTH / 2); //start with left eighth
	kelp.dst.y = (WINDOW_HEIGHT / 7); // and up


	rock1 = Sprite(pRenderer, "../Assets/textures/rock1.png");
	rock1.dst.x = 700;
	rock1.dst.y = 200;

	lighthouse1 = Sprite(pRenderer, "../Assets/textures/lighthouse.png");

	int lighthouseWidth = lighthouse1.src.w / 4;
	int lighthouseHeight = lighthouse1.src.h / 4;

	lighthouse1.dst.w = lighthouseWidth;
	lighthouse1.dst.h = lighthouseHeight;
	lighthouse1.dst.x = 1000;
	lighthouse1.dst.y = 450;

	///////////////////////////////////////////////////

}
// player input variables
bool isUpPressed = false;
bool isDownPressed = false;
bool isShootPressed = false;
bool isForwardPressed = false;
bool isBackPressed = false;
const float playerSpeedPx = 600.0f; //pixels per second
const float playerShootCoolDownDuration = 0.5f; // time between shots
float playerShootCoolDownTimer = 0.0f; //determines when we can shoot again
int bulletSpeed = 350.0f;

//////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////Input////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
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

void UpdatePlayer()
{
	ye::Vec2 inputVector;

	if (isUpPressed)
	{
		playerShip.dst.y -= playerSpeedPx * deltaTime;

		if (playerShip.dst.y < 0)
			playerShip.dst.y = 0;
	}
	if (isDownPressed)
	{

		playerShip.dst.y += playerSpeedPx * deltaTime;

	}
	if (isForwardPressed)
	{
		playerShip.dst.x += playerSpeedPx / FPS;

	}
	if (isBackPressed)
	{
		playerShip.dst.x -= playerSpeedPx * deltaTime;

	}

	void detectCollisions();
	{

		Sprite& playerSprite = playerShip;

		//std::vector<Bullet>::iterator
		for (auto it = enemyBulletContainer.begin(); it != enemyBulletContainer.end();)
		{
			Sprite& bulletSprite = it->sprite;
			if (AreSpritesColliding(playerSprite, bulletSprite))
			{
				//std::cout << "Player Hit";
				playerShip.rotationDegrees += 10;
				it = enemyBulletContainer.erase(it);

			}
			if (it != enemyBulletContainer.end())
			{
				it++;
			}
		}

		//else
	//{
	//	it++;
	//}
		//destroy bullet

// //player bullets vs enemy ship
//for (auto itBullet = bulletContainer.begin(); itBullet != bulletContainer.end();)
//{
//Sprite& bullet = itBullet->sprite;
//	for (auto itEnemy = enemyContainer.begin(); itEnemy != enemyContainer.end();)
//	{
//		if (AreSpritesColliding(itBullet-> sprite, itEnemy-> sprite))
//
//	}
//}
	}
}

void EnemySpawner()
{
	enemyShip.sprite = Sprite(pRenderer, "../Assets/textures/enemy1.png");
	enemyShip.sprite.dst.x = 1200 + 54; //spawns them outside of the screen on the right hand side
	enemyShip.sprite.dst.y = rand() % 720 - 61; //randomized pos



	Enemy enemy1;
	enemy1.sprite = enemyShip.sprite; //sprites stay the same
	enemy1.fireRepeatDelay = 100.0f; //setting fire speed
	enemy1.movesSpeedPx = 100; // making sure it moves at the same time

	enemyContainer.push_back(enemy1);

	Bullet enemyBullet1;
	enemyBulletContainer.push_back(enemyBullet1);

	enemySpawnTimer = enemySpawnDelay;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////Update////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void Update()
{
	UpdatePlayer();

	backgroundImage.dst.x -= 2;

	if (backgroundImage.dst.x <= -1200)
	{
		backgroundImage.dst.x = 0;
	}

	backgroundImage2.dst.x -= 2;

	if (backgroundImage2.dst.x <= 0)
	{
		backgroundImage2.dst.x = 1200;
	}


	if (isShootPressed)
	{
		//if player is off cooldown
		if (playerShootCoolDownTimer <= 0.0f)
		{

			std::cout << "Shoot!" << std::endl;

			Sprite renderAmmo = Sprite(pRenderer, "../Assets/textures/cannonball.png");

			renderAmmo.dst.x = playerShip.dst.x + playerShip.dst.h - renderAmmo.dst.w;
			renderAmmo.dst.y = playerShip.dst.y + playerShip.dst.w / 2;

			bulletContainer.push_back(renderAmmo);

			//reset cool down timer
			playerShootCoolDownTimer = playerShootCoolDownDuration;

		}


	}

	//tick down shoot cooldown
	playerShootCoolDownTimer -= deltaTime;

	//move bullet
	for (int i = 0; i < bulletContainer.size(); i++)
	{
		bulletContainer[i].dst.x += bulletSpeed * deltaTime;
	}

	//keeping player within the screen

	//width for player
	if (playerShip.dst.x >= WINDOW_WIDTH - playerShip.dst.w)
	{
		playerShip.dst.x = WINDOW_WIDTH - playerShip.dst.w;
	}

	if (playerShip.dst.x <= WINDOW_WIDTH * 0)
	{
		playerShip.dst.x = (WINDOW_HEIGHT * 0);
	}

	if (playerShip.dst.x <= 0 + 10)
	{
		playerShip.dst.x = 10;
	}

	//height for player
	if (playerShip.dst.y >= WINDOW_HEIGHT - playerShip.dst.h)
	{
		playerShip.dst.y = WINDOW_HEIGHT - playerShip.dst.h;
	}

	if (playerShip.dst.y <= WINDOW_HEIGHT * 0)
	{
		playerShip.dst.y = (WINDOW_HEIGHT * 0);
	}

	if (playerShip.dst.y <= 0 + 10)
	{
		playerShip.dst.y = 10;
	}
	////////////////////////////////////

	for (int i = 0; i < enemyBulletContainer.size(); i++)
	{
		enemyBulletContainer[i].Update();
	}

	for (int i = 0; i < enemyContainer.size(); i++)
	{
		Enemy& enemyBois = enemyContainer[i];
		enemyBois.Move({ -1, 0 });
		enemyBois.Update();
		if (enemyBois.CanShoot())
		{
			bool towardsRight = false;
			Vec2 velocity = { -400, 0 };
			enemyBois.Shoot(towardsRight, enemyBulletContainer, velocity);

		}

	}

	enemySpawnTimer -= deltaTime;

	if (enemySpawnTimer <= 0)
	{
		EnemySpawner();
	}

	//detectCollisions();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////Draw////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void Draw()
{
	SDL_SetRenderDrawColor(pRenderer, 0, 0, 20, 255);
	SDL_RenderClear(pRenderer);

	// calling on the draw function for background
	backgroundImage.draw(pRenderer);
	backgroundImage2.draw(pRenderer);

	// calling on user ship
	playerShip.draw(pRenderer);

	//calling on first object
	kelp.draw(pRenderer);

	//calling on rock1
	rock1.draw(pRenderer);

	//player bullets
	for (int i = 0; i < bulletContainer.size(); i++)
	{
		bulletContainer[i].draw(pRenderer);
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
	lighthouse1.draw(pRenderer);

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

