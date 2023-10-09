#include <Graphics/Window.hpp>
#include <Graphics/Sprite.hpp>
#include <Graphics/SpriteAnim.hpp>
#include <Graphics/Image.hpp>
#include <iostream>
#include <fmt/core.h>
#include <Graphics/Timer.hpp>
#include <Graphics/Font.hpp>
#include <Graphics/ResourceManager.hpp>
#include <Graphics/Keyboard.hpp>
#include <Graphics/Input.hpp>
#include<Graphics/TileMap.hpp>

using namespace Graphics;

Window window;
Image image;
Sprite sprite;
SpriteAnim idleAnim;
TileMap blockTiles;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

float Player_x = SCREEN_WIDTH / 2;
float Player_y= SCREEN_HEIGHT / 2;
float Player_speed = 60.0f;

void InitGame()
{
	 Player_x = SCREEN_WIDTH / 2;
	 Player_y = SCREEN_HEIGHT / 2;

}


int main()
{
	Input::mapButton("Reaload", [](std::span< const GamePadStateTracker> gamePadStates, const KeyboardStateTracker& keyboardState, const MouseStateTracker& mouseState){
		bool b = false;

	    for (auto& gamePadState : gamePadStates)
	    {
		    b = b || gamePadState.b == ButtonState::Pressed;
	    }

	    const bool r = keyboardState.isKeyPressed(KeyCode::R);
	    const bool enter = keyboardState.isKeyPressed(KeyCode::Enter);

	    return b || enter || r;
        });


	image.resize(SCREEN_WIDTH, SCREEN_HEIGHT);

	window.create(L"CoolGame", SCREEN_WIDTH, SCREEN_HEIGHT);
	window.show();
	window.setFullscreen(true);

	auto idle_sprites = ResourceManager::loadSpriteSheet("assets/Kings and Pigs/Sprites/01-King Human/Idle (78x58).png", 78, 58);

	idleAnim = SpriteAnim{ idle_sprites, 12 };



	Timer       timer;
	double      totalTime = 0.0;
	uint64_t    frameCount = 0ull;
	std::string fps = "FPS: 0";



	while (window)
	{

		Input::update();

		Player_x += Input::getAxis("Horizontal") * Player_speed * timer.elapsedSeconds();
		Player_y -= Input::getAxis("Vertical") * Player_speed * timer.elapsedSeconds();

		if (Input::getButton("Reload"))
		{
			InitGame();
		}

		//*auto keyState = Keyboard::getState();

		//if (keyState.W)
		//{
		//	Player_y -= Player_speed * timer.elapsedSeconds();
		//}
		//if (keyState.S)
		//{
		//	Player_y += Player_speed * timer.elapsedSeconds();
		//}
		//if (keyState.A)
		//{
		//	Player_x -= Player_speed * timer.elapsedSeconds();
		//}
		//if (keyState.D)
		//{
		//	Player_x += Player_speed * timer.elapsedSeconds();
		//}
		

		idleAnim.update(timer.elapsedSeconds());
		
		image.clear(Color::Black);

		image.drawText(Font::Default, fps, 10, 10, Color::White);

		image.drawSprite(idleAnim, static_cast<int>(Player_x), static_cast<int>(Player_y));

		window.present(image);

		Event e;
		while (window.popEvent(e))
		{
			switch (e.type)
			{
			case Event::Close:
				window.destroy();
				break;
			case Event::KeyPressed:
			{
				switch (e.key.code)
				{
				case KeyCode::Escape:
					window.destroy();
					break;
				case KeyCode::V:
					window.toggleVSync();
					break;
				}
			}
			break;
			}
		}
		timer.tick();
		++frameCount;

		totalTime += timer.elapsedSeconds();
		if (totalTime > 1.0)
		{
			fps = fmt::format("FPS: {:.3f}", static_cast<double>(frameCount) / totalTime);

			std::cout << fps << std::endl;

			frameCount = 0;
			totalTime = 0.0;
		}
	}

	std::cout << "Thank's for playing!"<< std::endl;

	return 0;
}