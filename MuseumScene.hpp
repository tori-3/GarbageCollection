#pragma once
#include"Common.hpp"
#include"TalkWindow.hpp"
#include"StoneButton.hpp"

class Exhibits {
public:
	Vec2 pos{};
	String textureName;
	String text;
	bool active=false;
	bool discovered = false;

	Exhibits(const String& textureName, const String& text,bool discovered)
		:textureName{ textureName }, text{ text }, discovered{discovered} {

	}

	void setPos(const Vec2& _pos) {
		pos = _pos;
	}

	void update() {
		bool click = RectF{ Arg::center = pos,100 }.leftClicked();

		if (not active && click) {
			active = true;
		}
		else if(active&& MouseL.down()&&not click){
			active = false;
		}
	}

	void draw()const{
		const RectF rect{ Arg::center = pos,60 };

		if (active) {
			rect.stretched(10).drawShadow(Vec2{ 0,2 }, 8, 3);
		}

		rect.draw(Palette::White);
		if(discovered)TextureAsset(textureName).resized(60).drawAt(pos);
		rect.drawFrame(0, 10, Palette::Brown);
		RectF{ Arg::center = pos + Vec2{0,60},40,15 }.draw(Palette::Black);
	}

	String getText() {
		return discovered ? text : U"まだ発見されていません。";
	}

};

class SpotLight {
public:

	Vec2 pos{};
	Vec2 velocity{};

	SpotLight(const Vec2& pos) :pos{pos} {

	}

	void update(const Vec2& target) {
		pos = Math::SmoothDamp(pos, target, velocity, 0.1, 1000);
	}

	void draw()const {
		Circle{ pos,100 }.draw(ColorF{ 1,0.8 });
	}

};

class ExhibitPanel {
public:
	RectF rect;
	String text;
	NinePatch ninePatch{ TextureAsset(U"panel"), 30 };

	ExhibitPanel(const RectF& rect, const String& text={}) :rect{rect}, text{text} {}

	void draw()const {
		ninePatch.draw(rect, 1.5);
		FontAsset(U"NormalFont")(text).draw(30, rect.stretched(-10. - 10));
	}
};

class MuseumScene : public MyScene
{
public:

	ExhibitPanel panel{ RectF{0,450,Scene::Width(),Scene::Height()-450}};

	SpotLight light1{ Vec2{-100,-100} };
	SpotLight light2{ Vec2{Scene::Width()+100,-100}};

	Array<Exhibits>exhibits;

	StoneButton mapButton{ RectF{20,20,200,50},U"マップ" };

	const MSRenderTexture renderTexture{ Scene::Size(), Scene::GetBackground() };
	const MSRenderTexture spotlight{ Scene::Size(), ColorF{1, 0} };

	MuseumScene(const InitData& init);

	void update() override;

	void draw() const override;
private:
};
