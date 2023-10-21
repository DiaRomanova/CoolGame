#include <iostream>
#include <Graphics/Window.hpp>
#include <Graphics/Image.hpp>
#include <Graphics/Sprite.hpp>
#include <Graphics/SpriteAnim.hpp>
#include <Graphics/Timer.hpp>
#include <Graphics/Font.hpp>
#include <Graphics/ResourceManager.hpp>
#include <Graphics/Input.hpp>
#include <Math/Transform2D.hpp>
#include <fmt/core.h>
#include <glm/vec2.hpp>

#include <Graphics/Keyboard.hpp>
#include<Graphics/TileMap.hpp>


using namespace Graphics;

Window window;
Image image;
Sprite sprite;
SpriteAnim idleAnim;
TileMap grassTiles;


const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

Math::Transform2D Player_transform;

float Player_speed = 60.0f;

void InitGame()
{
	Player_transform.setPosition({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 });

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

	Player_transform.setAnchor({ 16,32 });

	auto idle_sprites = ResourceManager::loadSpriteSheet("assets/Kings and Pigs/Sprites/01-King Human/Idle (78x58).png", 78, 58);
	idleAnim = SpriteAnim{ idle_sprites, 12 };

	auto grass_sprites = ResourceManager::loadSpriteSheet("assets/Kings and Pigs/Sprites/14-TileSets/OB9S6D0.jpg",16 ,16 );
	grassTiles = TileMap{ grass_sprites,30,30 };

	for (int i = 0; i < 30; ++i)
	{
		for (int j = 0; j < 30; ++j)
		{
			grassTiles(i, j) = (i * grass_sprites->getNumColumns() + j) % grass_sprites->getNumSprites();
		}
	}

	
	
	//Load tilemap
	/*auto block_Tiles = ResourceManager::loadSpriteSheet("assets/Kings and Pigs/Sprites/14 - TileSets/Terrain(32x32).png", 32, 32);
	blockTiles = TileMap{ block_Tiles, 64,64 };

	for (int i = 0; i < 64; ++i)
	{
		for (int j = 0; j < 64; ++j)
		{
			blockTiles (i, j) = (i * block_Tiles->getNumColumns() + j) % block_Tiles->getNumSprites();
		}
	}*/

	Timer       timer;
	double      totalTime = 0.0;
	uint64_t    frameCount = 0ull;
	std::string fps = "FPS: 0";

	InitGame();

	while (window)
	{

		Input::update();

		auto pos = Player_transform.getPosition();
		pos.x += Input::getAxis("Horizontal") * Player_speed * timer.elapsedSeconds();
		pos.y -= Input::getAxis("Vertical") * Player_speed * timer.elapsedSeconds();
		Player_transform.setPosition(pos);

		if (Input::getButton("Reload"))
		{
			InitGame();
		}
		

		auto rot = Player_transform.getRotation();
		if (Input::getKey(KeyCode::Q))
		{
			rot += 10.0 * timer.elapsedSeconds();
		}
		if (Input::getKey(KeyCode::E))
		{
			rot -= 10.0 * timer.elapsedSeconds();
		}
		Player_transform.setRotation(rot);

		idleAnim.update(timer.elapsedSeconds());
		
		image.clear(Color::White);

		//blockTiles.draw(image);

		//image.drawSprite(idleAnim,Player_transform);

		
		grassTiles.draw(image, -pos.x, -pos.y);

		image.drawSprite(idleAnim, (SCREEN_WIDTH - 78)/2, (SCREEN_HEIGHT - 29)/2 );

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