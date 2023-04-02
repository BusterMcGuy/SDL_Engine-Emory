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




namespace ye {

	struct Sprite
	{

		struct Vec2
		{
			float x;
			float y;

		};

	public:

		SDL_Texture* pTexture;
		SDL_Rect src;
		SDL_Rect dst;
		//int aniFrameCount = 1;
		//float aniCurrentFrame = 0;
		//double rotationDegrees = 0;
		//SDL_RendererFlip flipState = SDL_FLIP_NONE;
		//Vec2 position;

		Sprite()

		{
			pTexture = nullptr;
			src = { 0, 0, 0, 0 };
			dst = { 0, 0, 0 ,0 };
		}

		void draw(SDL_Renderer* renderer)
		{
			int result = SDL_RenderCopy(renderer, pTexture, &src, &dst);
			if (result != 0)
			{
				std::cout << "Render Failed!" << SDL_GetError() << std::endl;
			}
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

		//Sprite(SDL_Renderer* renderer, const char* imageFilePath, int frameWidth, int frameHeight, int numberOfFramesInSheet) : Sprite(renderer, imageFilePath)
		//{
		//	src.x = 0;
		//	src.y = 0;

		//	dst.x = 0;
		//	dst.y = 0;
		//	dst.h = src.w = frameWidth;
		//	dst.w = src.h = frameHeight;

		//	//aniFrameCount = numberOfFramesInSheet;
		//}


		/*void draw(SDL_Renderer* renderer)
		{
			dst.x = position.x;
			dst.y = position.y;
			src.x = (int)aniCurrentFrame * src.w;


			int result = SDL_RenderCopyEx(renderer, pTexture, &src, &dst, rotationDegrees, NULL, flipState);
			if (result != 0)
			{
				std::cout << "Render Failed!" << SDL_GetError() << std::endl;
			}
			else
			{
				std::cout << "Render Completed!" << std::endl;
			}
		}*/
		//void setAnimFrameDimensions(int frameWidth, int frameHeight)
		//{
		//	src.w = frameWidth;
		//	src.h = frameHeight;
		//
		//}
		//void nextFrame(float frames)
		//{
		//	//aniCurrentFrame++;
		//	aniCurrentFrame += frames;
		//
		//	if (aniCurrentFrame >= aniFrameCount)
		//	{
		//		aniCurrentFrame = 0;
		//	}
		//	src.x += src.w ;
		//}
		//void addFrameTime(float frames)
		//{
		//	aniCurrentFrame += frames;
		//
		//}
		//void setPosition(int x, int y)
		//{
		//	position.x = x;
		//	position.y = y;
		//
		//}
		//
		//void setSize(int w, int h)
		//{
		//	dst.w = w;
		//	dst.h = h;
		//}
		//
		//Vec2 getSize()
		//{
		//	Vec2 sizeXY{ dst.w, dst.h };
		//	return sizeXY;
		//}
	};

}

using namespace ye;

Sprite newSprite;



//sprite renderAni = sprite();

//struct background
//{
//
//	
//	public:
//
//	SDL_Texture* pTexture;
//	SDL_Rect src;
//	SDL_Rect dst;
//	
//
//
//	// We can also create Member Functions that we call on.
//
//	
//
//	background()
//	{
//		pTexture = nullptr;
//		src = { 0, 0, 0, 0 };
//		dst = { 0, 0, 0 ,0 };
//
//	}
//
//
//	//a non-default constructor
//	background(SDL_Renderer* renderer, const char* imageFilePath)
//	{
//
//		pTexture = IMG_LoadTexture(renderer, imageFilePath);
//
//		if (pTexture == NULL)
//		{
//			std::cout << "Image Load Failed! " << SDL_GetError() << std::endl;
//		}
//		else
//		{
//			std::cout << "Image Load Succesful! " << std::endl;
//		}
//
//		if (SDL_QueryTexture(pTexture, NULL, NULL, &src.w, &src.h) != 0)
//		{
//			std::cout << "Query Texture Failed! " << SDL_GetError() << std::endl;
//		}
//		src.x = 0;
//		src.y = 0;
//
//		dst.x = 0;
//		dst.y = 0;
//		dst.w = src.w;
//		dst.h = src.h;
//	}
//
//	
//
//	//void draw(SDL_Renderer* renderer)
//	//{
//	//	
//	//
//	//
//	//	int result = SDL_RenderCopy(renderer, pTexture, &src, &dst);
//	//	if (result != 0)
//	//	{
//	//		std::cout << "Render Failed!" << SDL_GetError() << std::endl;
//	//	}
//	//	else
//	//	{
//	//		std::cout << "Render Completed!" << std::endl;
//	//	}
//	//}
//
//};

//sprite backgroundImage = sprite(); // call the default constructor



//struct userPlayer 
//{
//public:
//
//SDL_Texture* usership;
//SDL_Rect src;
//SDL_Rect dst;
//
//
//// We can also create Member Functions that we call on.
//
//
//	userPlayer()
//	{
//		std::cout << "Defualt Constructor" << std::endl;
//		usership = nullptr;
//		src = { 0, 0, 0, 0 };
//		dst = { 0, 0, 0 ,0 };
//
//	}
//
//
//	//a non-default constructor
//	userPlayer(SDL_Renderer* renderer, const char* imageFilePath)
//	{
//		std::cout << "user player Constructor" << std::endl;
//		usership = IMG_LoadTexture(renderer, imageFilePath);
//
//		if (usership == NULL)
//		{
//			std::cout << "Image Load Failed! " << SDL_GetError() << std::endl;
//		}
//		else
//		{
//			std::cout << "Image Load Succesful! " << std::endl;
//		}
//
//		if ( SDL_QueryTexture(usership, NULL, NULL, &src.w, &src.h) != 0)
//		{
//			std::cout << "Query Texture Failed! " << SDL_GetError() << std::endl;
//		}
//		src.x = 0;
//		src.y = 0;
//
//		dst.x = 0;
//		dst.y = 0;
//		dst.w = src.w;
//		dst.h = src.h;
//	}
//
//
//void draw(SDL_Renderer* renderer)
//{
//	int result = SDL_RenderCopy(renderer, usership, &src, &dst);
//	if (result != 0)
//	{
//		std::cout << "Render Failed!" << SDL_GetError() << std::endl;
//	}
//}
//
//};

//renderUserPlayer = sprite(); // call the default constructor
//sprite enemy1;

//struct sprite
//{
//public:
//
//	SDL_Texture* object;
//	SDL_Rect src;
//	SDL_Rect dst;
//
//
//	// We can also create Member Functions that we call on.
//
//
//	sprite()
//	{
//		std::cout << "Defualt Constructor" << std::endl;
//		object = nullptr;
//		src = { 0, 0, 0, 0 };
//		dst = { 0, 0, 0 ,0 };
//
//	}
//
//
//	//a non-default constructor
//	sprite(SDL_Renderer* renderer, const char* imageFilePath)
//	{
//		std::cout << "user player Constructor" << std::endl;
//		object = IMG_LoadTexture(renderer, imageFilePath);
//
//		if (object == NULL)
//		{
//			std::cout << "Image Load Failed! " << SDL_GetError() << std::endl;
//		}
//		else
//		{
//			std::cout << "Image Load Succesful! " << std::endl;
//		}
//
//		if (SDL_QueryTexture(object, NULL, NULL, &src.w, &src.h) != 0)
//		{
//			std::cout << "Query Texture Failed! " << SDL_GetError() << std::endl;
//		}
//		src.x = 0;
//		src.y = 0;
//
//		dst.x = 0;
//		dst.y = 0;
//		dst.w = src.w;
//		dst.h = src.h;
//	}
//
//
//	void draw(SDL_Renderer* renderer)
//	{
//		int result = SDL_RenderCopy(renderer, object, &src, &dst);
//		if (result != 0)
//		{
//			std::cout << "Render Failed!" << SDL_GetError() << std::endl;
//		}
//	}
//
//};

//sprite renderObject = sprite();
//sprite rock1;
//sprite lighthouse;
//sprite kelp2;

//struct bullet
//{
//public:
//
//	SDL_Texture* ammo;
//	SDL_Rect src;
//	SDL_Rect dst;
//
//
//	// We can also create Member Functions that we call on.
//
//
//	bullet()
//	{
//		ammo = nullptr;
//		src = { 0, 0, 0, 0 };
//		dst = { 0, 0, 0 ,0 };
//
//	}
//
//
//	//a non-default constructor
//	bullet(SDL_Renderer* renderer, const char* imageFilePath)
//	{
//
//		ammo = IMG_LoadTexture(renderer, imageFilePath);
//
//		if (ammo == NULL)
//		{
//			std::cout << "Image Load Failed! " << SDL_GetError() << std::endl;
//		}
//		else
//		{
//			std::cout << "Image Load Succesful! " << std::endl;
//		}
//
//		if (SDL_QueryTexture(ammo, NULL, NULL, &src.w, &src.h) != 0)
//		{
//			std::cout << "Query Texture Failed! " << SDL_GetError() << std::endl;
//		}
//		src.x = 0;
//		src.y = 0;
//
//		dst.x = 0;
//		dst.y = 0;
//		dst.w = src.w;
//		dst.h = src.h;
//	}
//
//
//	void draw(SDL_Renderer* renderer)
//	{
//		int result = SDL_RenderCopy(renderer, ammo, &src, &dst);
//		if (result != 0)
//		{
//			std::cout << "Render Failed!" << SDL_GetError() << std::endl;
//		}
//	}
//
//};

//bullet renderAmmo = bullet(); // call the default constructor


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

Sprite backgroundImage = Sprite();
Sprite playerShip = Sprite();
Sprite enemyShip = Sprite();
Sprite kelp = Sprite();
Sprite rock1 = Sprite();
Sprite lighthouse1 = Sprite();
Sprite renderAmmo = Sprite();

void load()
{
	//loading all textures...
	//SDL_Texture * IMG_LoadTexture(SDL_Renderer *renderer, const char *file)
	backgroundImage = Sprite(pRenderer, "../Assets/textures/Still_water_image.png");


	int backgroundWidth = backgroundImage.src.w;
	int backgroundHeight = backgroundImage.src.h;

	backgroundImage.dst.w = backgroundWidth;
	backgroundImage.dst.h = backgroundHeight;

	/////////////////////////////////////////////////

	playerShip = Sprite(pRenderer, "../Assets/textures/usership.png");


	int userWidth = playerShip.src.w / 10;
	int userHeight = playerShip.src.h / 10;

	playerShip.dst.w = userWidth;
	playerShip.dst.h = userHeight;
	playerShip.dst.x = (WINDOW_WIDTH / 8); //start with left eighth
	playerShip.dst.y = (WINDOW_HEIGHT / 2) - userHeight / 2; // in middle

	enemyShip = Sprite(pRenderer, "../Assets/textures/enemy1.png");
	enemyShip.dst.x = 1200;
	enemyShip.dst.y = 320;

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

	int frameWidth = 480 / 8;
	int frameHeight = 74;
	int frameCount = 8;
	//renderAni = Sprite(pRenderer, "../Assets/sprites/moving_kelp2.png", frameWidth, frameHeight, frameCount);
	//renderAni.setPosition(600, 300);



	/// ////////////////////////////////////////////////

	renderAmmo = Sprite(pRenderer, "../Assets/textures/cannonball.png");


	int ammoWidth = renderAmmo.src.w / 2;
	int ammoHeight = renderAmmo.src.h / 2;

	renderAmmo.dst.w = ammoWidth;
	renderAmmo.dst.h = ammoHeight;
	renderAmmo.dst.x = (WINDOW_WIDTH / 4); //start with left eighth
	renderAmmo.dst.y = (WINDOW_HEIGHT / 2); // and up





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



void Update()
{

	if (isUpPressed)
	{
		playerShip.dst.y -= playerSpeedPx / FPS;
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
	if (isShootPressed)
	{
		//if player is off cooldown
		if (playerShootCoolDownTimer <= 0.0f)
		{
			std::cout << "Shoot!" << std::endl;

			renderAmmo.dst.x = playerShip.dst.x + playerShip.dst.h - renderAmmo.dst.w;
			renderAmmo.dst.y = playerShip.dst.y + playerShip.dst.w / 2;

			//reset cool down timer
			playerShootCoolDownTimer = playerShootCoolDownDuration;

		}


	}



	//tick down shoot cooldown
	playerShootCoolDownTimer -= deltaTime;

	//move bullet
	renderAmmo.dst.x += 5;

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
	// keeping enemy within the screen 

	//width for player
	if (enemyShip.dst.x >= WINDOW_WIDTH - enemyShip.dst.w)
	{
		enemyShip.dst.x = WINDOW_WIDTH - enemyShip.dst.w;
	}

	if (enemyShip.dst.x <= WINDOW_WIDTH * 0)
	{
		enemyShip.dst.x = (WINDOW_HEIGHT * 0);
	}

	if (enemyShip.dst.x <= 0 + 10)
	{
		enemyShip.dst.x = 10;
	}

	//height for player
	if (enemyShip.dst.y >= WINDOW_HEIGHT - enemyShip.dst.h)
	{
		enemyShip.dst.y = WINDOW_HEIGHT - enemyShip.dst.h;
	}

	if (enemyShip.dst.y <= WINDOW_HEIGHT * 0)
	{
		enemyShip.dst.y = (WINDOW_HEIGHT * 0);
	}

	if (enemyShip.dst.y <= 0 + 10)
	{
		enemyShip.dst.y = 10;
	}

	//making the enemy move 
	enemyShip.dst.x = enemyShip.dst.x - 1;

	//renderAni.nextFrame(0.1);

}

void Draw()
{
	SDL_SetRenderDrawColor(pRenderer, 0, 0, 20, 255);
	SDL_RenderClear(pRenderer);

	//int result = SDL_RenderCopy(pRenderer, pMySprite, &mySpriteSrc, &mySpriteDst);
	//if (result != 0)
	//{
	//	std::cout << "Render Failed!" << SDL_GetError() << std::endl;
	//}

	// calling on the draw function for background
	backgroundImage.draw(pRenderer);

	// calling on user ship
	playerShip.draw(pRenderer);

	//drawing enemy
	enemyShip.draw(pRenderer);

	//calling on first object
	kelp.draw(pRenderer);

	//calling on rock1
	rock1.draw(pRenderer);

	//calling on cannonball
	renderAmmo.draw(pRenderer);

	// calling on lighthouse
	lighthouse1.draw(pRenderer);

	//calling on kelp2
	//kelp2.draw(pRenderer);

	//animation
	//renderAni.draw(pRenderer);

	//show the back buffer
	SDL_RenderPresent(pRenderer);
}


/**
 * \brief Program Entry Point
 */
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
	SDL_RenderPresent(pRenderer);


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
	getchar();
	return 0;
}

