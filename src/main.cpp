// Core Libraries
#include <crtdbg.h>
#include <iostream>
#include <Windows.h>
#include <Windows.h>
#include <SDL.h>
#include <SDL_image.h>
// This is global space - All these are global variables
constexpr float FPS = 60.0f; // Target FPS
constexpr float DELAY_TIME = 1000.0f / FPS; // Target time between frames in miliseconds
bool isGameRunning = true;

SDL_Window* pWindow = nullptr; // Assigning a pointer to nullptr means the address is 0
SDL_Renderer* pRenderer = nullptr; // NULL is the address 0. so it is baiscally the same as nullptr
SDL_Texture* pMysprite = NULL;
								   // Set up the game window, start SDL features, etc...
bool initialize()
{

	SDL_Init(SDL_INIT_EVERYTHING);
	pWindow = SDL_CreateWindow("Emorys Lab Demo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1200, 600, 0);
	if (pWindow == NULL)
	{
		std::cout << "Oof, your window has failed. Here's why - " << SDL_GetError() << std::endl;

		

	}
	else
	{
		std::cout << "Window has opened, succes.\n";

	}

	
	pRenderer = SDL_CreateRenderer(pWindow, -1, 0); //What is happening
	if(pRenderer == NULL)
	{
		std::cout << "Renderer has failed, L" << SDL_GetError << std::endl;

		

	}
	else
	{
		std::cout << "Renderer Success!" << std::endl;

	}

	return true;


	






	
	


}

//Anything with a star to the right is a pointer - SDL_Window* is a memory adress of an SDL_Window
// Pointers can also be set to point to memory adress 0, meaining "no address". We call these NULL pointers
//  Computer memory is addressible by one numbere, like a home address on one very very long street. No house is on that street or at that address though. 


//Loading Textures
//SDL_Texture * IMG_LoadTexture(SDL_RENDERER *renderer, const char *file);

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


	isGameRunning = initialize();




	SDL_Texture* pMySprite = IMG_LoadTexture(pRenderer, "../Assets/textures/RealExplodingCat.jpg");

	if (pMySprite == NULL)
	{
		std::cout << "Image load failed" << SDL_GetError << std::endl;



	}
	else
	{
		std::cout << "Image load Success! W" << std::endl;

	}

	void Draw();
	{
		/*SDL_Rect mySpriteSrc;
		mySpriteSrc.x = 0;
		mySpriteSrc.y = 0;
		mySpriteSrc.w = 643;
		mySpriteSrc.h = 296;

		int shipWidth = mySpriteSrc.w / 4;
		int shipHeight = mySpriteSrc.h / 4;

		SDL_Rect mySpriteDst;
		mySpriteDst.x (WINDOW_WIDTH) */



		int result = SDL_RenderCopy(pRenderer, pMySprite, NULL, NULL);
		if (result != 0)
		{
			std::cout << "render Failed" << SDL_GetError << std::endl;

		}
	}
	SDL_RenderPresent(pRenderer);

	//Display main SDL Window
	// Ger pointed to SDL_Window Object
	

	
	// Display Main SDL Window
	//Game::Instance().Init("SDL_Engine v0.29", 100, 100, 800, 600, false);
	
	// Main Game Loop
	while(isGameRunning = true)
	{
		
		
		
		
		
		pRenderer = SDL_CreateRenderer(pWindow, -1, 0); //What is happening
		
		
		
		const auto frame_start = static_cast<float>(SDL_GetTicks());
		int Frames = 0;
		

		if (const float frame_time = static_cast<float>(SDL_GetTicks()) - frame_start;
			frame_time < DELAY_TIME)
		{
			SDL_Delay(static_cast<int>(DELAY_TIME - frame_time));
		}
		





		// delta time
		const auto delta_time = (static_cast<float>(SDL_GetTicks()) - frame_start) / 1000.0f;
	

		//frames++;
		
	}

	
	return 0;
}

