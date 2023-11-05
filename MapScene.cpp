#include"MapScene.hpp"

MapScene::MapScene(const InitData& init)
	: MyScene{ init,Palette::Skyblue }
{
	getData().save();
}

void MapScene::update()
{
	AudioAsset(U"MapAudio").play(MixBus0);

	camera.update();

	if (homeButton.update()) {
		changeScene(U"Title",1s, CrossFade::Yes);
		AudioAsset(U"MapAudio").stop(1s);
	}

	edit ^= editButton.update();
	editButton.text = edit? U"編集終了":U"編集する";
	camera.setTargetCenter(Vec2{ 0, 185+ edit*50 });

	if (not explainFlg) {
		if (edit) {
			selecter.update();

			const auto tr = camera.createTransformer();

			// マウスカーソルがタイルメニュー上に無ければ
				// マウスカーソルがマップ上のどのタイルの上にあるかを取得する
			if (const auto index = view.ToIndex(Cursor::PosF()))
			{

				if (MouseL.pressed())
				{
					if (const auto num = selecter.use()) {
						int32 tmp = view.grid[*index];
						selecter.release(tmp);
						view.grid[*index] = *num;
					}
				}

				//0番の草を設置
				if (MouseR.pressed()) {
					selecter.nums[0] += 1;
					selecter.release(view.grid[*index]);
					view.grid[*index] = 0;
				}
			}

		}
		else {
			const auto tr = camera.createTransformer();

			// マウスカーソルがマップ上のどのタイルの上にあるかを取得する
			if (const auto index = view.ToIndex(Cursor::PosF()))
			{

				if (MouseL.pressed())
				{
					if (view.grid[*index] == 4) {
						changeScene(U"Game", 1s, CrossFade::Yes);
						AudioAsset(U"MapAudio").stop(1s);

					}
					else if (view.grid[*index] == 5) {
						changeScene(U"Shop", 1s, CrossFade::Yes);
					}
					else if (view.grid[*index] == 6) {
						changeScene(U"Museum", 1s, CrossFade::Yes);
						AudioAsset(U"MapAudio").stop(1s);
					}
					else if (view.grid[*index] == 10) {
						changeScene(U"Research", 1s, CrossFade::Yes);
					}
				}
			}
		}
	}
	cloud.update();

	explainFlg ^= explainButton.update();
	explainButton.text = explainFlg ? U"説明とじる" : U"説明";

	if (explainFlg) {
		editButton.active = false;
		edit = false;
	}
	else {
		editButton.active = true;
	}

}

void MapScene::draw()const
{
	Scene::SetBackground(Palette::Skyblue);


	{
		const auto tr = camera.createTransformer();
		view.draw();
		if (const auto index = view.ToIndex(Cursor::PosF()))
		{
			// マウスカーソルがあるタイルを強調表示する
			view.ToTile(*index).draw(ColorF{ 1.0, 0.2 });
		}
	}


	if (edit) {
		selecter.draw();
	}
	else {
		cloud.draw();
	}

	homeButton.draw();

	editButton.draw();

	explainButton.draw();

	if (explainFlg) {
		TextureAsset(U"説明2").resized(Scene::Width()).draw();
	}
}
