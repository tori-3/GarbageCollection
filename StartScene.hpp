#pragma once
#include"Common.hpp"
#include"Tool.hpp"

class StartScene : public App::Scene
{
public:

	const String text{ U"Made by\nとりさん　やっさん　maru\n     \nMade with\nSiv3D" };

	LightBloom light{ Scene::Size() };

	Timer timer{4s};

	StartScene(const InitData& init);

	void update() override;

	void draw() const override;

private:

	static void Brick(const Rect& rect, int32 n, const ColorF& color = Color(162, 72, 43), const ColorF& back_color = Palette::White) {

		const ScopedViewport2D viewport{ rect.draw(back_color) };
		const double hight = rect.h / (double)n;
		const double width = hight * 2;
		const double d = hight * 0.1;

		for (auto x : step((int32)(rect.w / hight) + 2))
		{
			for (auto y : step(n))
			{
				if (IsEven(x + y))
				{
					RectF{ (x * hight + d / 2 - hight), (y * hight + d / 2), width - d,hight - d }.draw(color);
				}
			}
		}
	}
};
