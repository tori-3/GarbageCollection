#include"ShopScene.hpp"

ShopScene::ShopScene(const InitData& init)
	: MyScene{ init,ColorF{ 0.3, 0.6, 0.4 } }
{

}

void ShopScene::update()
{
	buyHouse.panel.name = U"家を建てる {}/10"_fmt(getData().house);

	buyHouse.panel.icon = U"house{}"_fmt(getData().houseLv());

	garbagePanel.update(getData().garbage);
	ironPanel.update(getData().iron);
	goldPanel.update(getData().gold);
	moneyPanel.update(getData().money);

	if (mapButton.update()) {
		changeScene(U"Map", 1s, CrossFade::Yes);
		AudioAsset(U"gameAudio").stop(1s);
	}

	getData().money += garbageCell.update(getData().garbage)*1;
	getData().money += ironCell.update(getData().iron)*5;
	getData().money += goldCell.update(getData().gold)*20;

	buyMagnet.update(getData(), getData().magnet);
	buyCat.update(getData(), getData().cat);
	buyMissile.update(getData(), getData().missile);
	buyResearch.update(getData(), getData().research);
	buyHouse.update(getData(), getData().house,10);
	buyArm.update(getData(), getData().upgradeArm);

}

void ShopScene::draw()const
{
	Scene::SetBackground(ColorF{ 0.3, 0.6, 0.4 });

	garbageCell.draw();
	ironCell.draw();
	goldCell.draw();

	buyMagnet.draw();
	buyCat.draw();
	buyMissile.draw();
	buyResearch.draw();
	buyHouse.draw();
	buyArm.draw();

	garbagePanel.draw();
	ironPanel.draw();
	goldPanel.draw();
	moneyPanel.draw();


	mapButton.draw();
}
