#include <Graphics/Window.hpp>
#include<Graphics/Sprite.hpp>
#include<Graphics/SpriteAnim.hpp>
#include<Graphics/Image.hpp>
#include <iostream>
#include<fmt/core.h>
#include<Graphics/Timer.hpp>
#include<Graphics/Font.hpp>
#include<Graphics/ResourceManager.hpp>

using namespace Graphics;

Window window;
Image image;
Sprite sprite;
SpriteAnim idleAnim;


const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

int main()
{
	image.resize(SCREEN_WIDTH, SCREEN_HEIGHT);

	window.create(L"CoolGame", SCREEN_WIDTH, SCREEN_HEIGHT);
	window.show();

	auto idle_sprites = ResourceManager::loadSpriteSheet("assets/Kings and Pigs/Sprites/01-King Human/Idle (78x58).png", 78, 58);

	idleAnim = SpriteAnim{ idle_sprites, 12 };



	Timer       timer;
	double      totalTime = 0.0;
	uint64_t    frameCount = 0ull;
	std::string fps = "FPS: 0";



	while (window)
	{
		idleAnim.update(timer.elapsedSeconds());
		
		image.clear(Color::Black);

		image.drawText(Font::Default, fps, 10, 10, Color::White);

		image.drawSprite(idleAnim, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

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