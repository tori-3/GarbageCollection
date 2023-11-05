#pragma once
#include"Common.hpp"
#include"StoneButton.hpp"
#include"Slider.hpp"



// ゲームシーン
class TitleScene : public MyScene
{
public:

	static bool first;

	StoneButton startButton{ RectF{Arg::center(1100,440),300,80},U"はじめる"};
	StoneButton endButton{ RectF{Arg::center(1100,540),300,80},U"おわる" };
	StoneButton licenceButton{ RectF{Arg::center(1100,640),300,80},U"ライセンス" };
	Slider effectVolumeSlider{ getData().effectVolume,Rect{Arg::center(220,50),200,20} };
	Slider bgmVolumeSlider{ getData().bgmVolume,Rect{Arg::center(220,150),200,20} };

	TitleScene(const InitData& init);

	void update() override;

	void draw() const override;

	void drawFadeIn(double t)const override;
private:
};
