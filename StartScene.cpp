#include "stdafx.h"
#include "StartScene.hpp"

StartScene::StartScene(const InitData& init) :IScene{init} {

}

void StartScene::update() {
	timer.start();

	if (not timer.isRunning()) {
		changeScene(U"Title", 1s);
	}
}

void StartScene::draw() const {

	Scene::SetBackground(Palette::Black);

	const size_t length = static_cast<size_t>((4-timer.sF()) / 0.05);

	const Vec2 pos=FontAsset(U"NormalFont")(text).regionAt(70,Scene::Center()).pos;

	FontAsset(U"NormalFont")(text.substr(0, length)).draw(TextStyle::Outline(0.3, Palette::White),70, pos, ColorF{ 1,0 });
	{
		ScopedLightBloom target{ light };
		FontAsset(U"NormalFont")(text.substr(0, length)).draw(70,pos, Palette::Orange);
	}
	light.draw();
}
