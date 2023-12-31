#include <Player.hpp>

#include <Graphics/Window.hpp>
#include <Graphics/Image.hpp>
#include <Graphics/Sprite.hpp>
#include <Graphics/SpriteAnim.hpp>
#include <Graphics/Timer.hpp>
#include<Graphics/TileMap.hpp>
#include <Graphics/Font.hpp>
#include <Graphics/ResourceManager.hpp>
#include <Graphics/Input.hpp>
#include <Math/Transform2D.hpp>

#include <fmt/core.h>
#include <glm/vec2.hpp>

#include <iostream>

using namespace Graphics;

Window window;
Image image;
TileMap grassTiles;


const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

Player player;

void InitGame()
{
	player.setPosition({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 });

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
	player = Player({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 }, SpriteAnim{ idle_sprites, 12 });

	auto grass_sprites = ResourceManager::loadSpriteSheet("assets/Kings and Pigs/Sprites/14-TileSets/OB9S6D0.jpg",16 ,16 );
	grassTiles = TileMap{ grass_sprites, 30, 30 };

	for (int i = 0; i < 30; ++i)
	{
		for (int j = 0; j < 30; ++j)
		{
			grassTiles(i, j) = (i * grass_sprites->getNumColumns() + j) % grass_sprites->getNumSprites();
		}
	}

	Timer       timer;
	double      totalTime = 0.0;
	uint64_t    frameCount = 0ull;
	std::string fps = "FPS: 0";

	InitGame();

	while (window)
	{
		// Update the input state
		Input::update();

		player.update(timer.elapsedSeconds());
		// Check collisions with player.
		// Screen space collision
		{
			auto aabb = player.getAABB();
			glm::vec2 correction{ 0 }; 
			if (aabb.min.x < 0)
			{
				correction.x = -aabb.min.x;
			}
			if (aabb.min.y < 0)
			{
				correction.y = -aabb.min.y;
			}
			if (aabb.max.x >= image.getWidth())
			{
				correction.x = image.getWidth() - aabb.max.x;
			}
			if (aabb.max.y >= image.getHeight())
			{
				correction.y = image.getHeight() - aabb.max.y;
			}

			// Apply correction
			player.translate(correction);
		}

		if (Input::getButton("Reload"))
		{
			InitGame();
		}
	
		//Render loop
		image.clear(Color::White);
		
		grassTiles.draw(image);

		player.draw(image);

		image.drawText(Font::Default, fps, 10, 10, Color::Black);

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