#include "stdafx.h"
#include "ResearchScene.hpp"
ResearchScene::ResearchScene(const InitData& init)
	: MyScene{ init,ColorF{ Palette::Blue }*0.3 }
{
	


}

void ResearchScene::update()
{
	garbagePanel.update(getData().garbage);
	ironPanel.update(getData().iron);
	goldPanel.update(getData().gold);
	moneyPanel.update(getData().money);

	if (mapButton.update()) {
		changeScene(U"Map", 1s, CrossFade::Yes);
	}

	if (magnetButtonLv1.update(getData().magnet)) {
		changeScene(U"Shop", 1s, CrossFade::Yes);
	}

	if (catButtonLv1.update(getData().cat)) {
		changeScene(U"Shop", 1s, CrossFade::Yes);
	}

	if (missileButtonLv1.update(getData().missile)) {
		changeScene(U"Shop", 1s, CrossFade::Yes);
	}

	if (magnetButtonLv2.update(getData().magnet,500<=getData().money)) {
		getData().magnet += 1;
		getData().money -= 500;
	}
	if (catButtonLv2.update(getData().cat,500 <= getData().money)) {
		getData().cat += 1;
		getData().money -= 500;
	}
	if (missileButtonLv2.update(getData().missile, 500 <= getData().money)) {
		getData().missile += 1;
		getData().money -= 500;
	}

	if (magnetButtonLv3.update(getData().magnet, 800 <= getData().money)) {
		getData().magnet += 1;
		getData().money -= 800;
	}

	if (catButtonLv3.update(getData().cat, 800 <= getData().money)) {
		getData().cat += 1;
		getData().money -= 800;
	}

	if (missileButtonLv3.update(getData().missile, 800 <= getData().money)) {
		getData().missile += 1;
		getData().money -= 800;
	}

}

void ResearchScene::draw()const
{
	Scene::SetBackground(ColorF{ Palette::Blue }*0.3);

	magnetButtonLv1.draw();
	magnetButtonLv2.draw();
	magnetButtonLv3.draw();
	catButtonLv1.draw();
	catButtonLv2.draw();
	catButtonLv3.draw();
	missileButtonLv1.draw();
	missileButtonLv2.draw();
	missileButtonLv3.draw();

	garbagePanel.draw();
	ironPanel.draw();
	goldPanel.draw();
	moneyPanel.draw();

	mapButton.draw();
}
