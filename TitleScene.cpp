#include"TitleScene.hpp"

bool TitleScene::first = true;

TitleScene::TitleScene(const InitData& init)
	: MyScene{ init,Palette::Black }
{
	getData().save();
}

void TitleScene::update()
{
	first = false;

	AudioAsset(U"GameAudio").play(MixBus0);

	if (startButton.update()) {
		changeScene(U"Map", 1s,CrossFade::Yes);
		AudioAsset(U"GameAudio").stop(1s);
	}

	if (endButton.update()) {
		System::Exit();
	}
	
	if (licenceButton.update()) {
		LicenseManager::ShowInBrowser();
	}

	getData().bgmVolume= bgmVolumeSlider.update();

	bool tmpActive = effectVolumeSlider.activeFlg;
	getData().effectVolume = effectVolumeSlider.update();
	getData().setVolume();

	if (tmpActive&& not effectVolumeSlider.activeFlg) {
		AudioAsset(U"GetEffect").playOneShot(MixBus1);
	}

}

void TitleScene::draw()const
{
	Scene::SetBackground(ColorF{0.5});

	TextureAsset(U"title").resized(Scene::Width()).draw();

	FontAsset(U"NormalFont")(U"効果音").drawAt(30, Vec2{ 60,50 });
	effectVolumeSlider.draw();
	SoundIcon(getData().effectVolume, Circle{ 370,50,40 });

	FontAsset(U"NormalFont")(U"BGM").drawAt(30, Vec2{60,150});
	bgmVolumeSlider.draw();
	SoundIcon(getData().bgmVolume, Circle{ 370,150,40 });

	startButton.draw();
	endButton.draw();
	licenceButton.draw();
}

void TitleScene::drawFadeIn(double t)const {

	if (first) {
		App::Scene::drawFadeIn(t);
	}
	else {
		MyScene::drawFadeIn(t);
	}
}
