#include "stdafx.h"
#include "MuseumScene.hpp"

BlendState MakeBlendState()
{
	BlendState blendState = BlendState::Default2D;
	blendState.srcAlpha = Blend::SrcAlpha;
	blendState.dstAlpha = Blend::DestAlpha;
	blendState.opAlpha = BlendOp::Add;
	return blendState;
}

MuseumScene::MuseumScene(const InitData& init)
	: MyScene{ init,Palette::Black }
{
	exhibits << Exhibits{ U"microwave",U"<電子レンジ>\n捨てられた電子レンジです。",getData().collection[0]};
	exhibits << Exhibits{ U"washing",U"<洗濯機>\n捨てられた洗濯機です。",getData().collection[1] };
	exhibits << Exhibits{ U"blackBag",U"<怪しい袋>\n異臭がする。",getData().collection[2] };
	exhibits << Exhibits{ U"鉄球",U"<1000℃に熱した鉄球>\nみんな大好き。",getData().collection[3] };
	exhibits << Exhibits{ U"siv3d",U"<Siv3D>\nみんなもゲームを作ろう！！",getData().collection[4] };
	exhibits << Exhibits{ U"サインポール",U"<床屋のくるくる回っているあれ>\nよくわからないけど楽しい。\n世界中で使われているらしい。",getData().collection[5] };
	exhibits << Exhibits{ U"bagClosure",U"<食パンを止めるあれ>\nあれです！",getData().collection[6] };
	exhibits << Exhibits{ U"セミコロン",U"<セミコロン>\nどこかのソースコードから落っこちたらしい。。。",getData().collection[7] };
	exhibits << Exhibits{ U"shouyu",U"<お弁当に入ってる魚の醤油入れ>\nなんかいいよね！",getData().collection[8] };
	exhibits << Exhibits{ U"袖ビーム",U"<ガードレールの端の部分>\n袖ビーーーーーーーーーーム！！！",getData().collection[9] };

	//・怪しい袋
	//・1000度に熱した鉄球
	//・Siv3Dのマーク
	//・床屋のくるくる回っているやつ
	//・食パンを止めるあれ
	//・セミコロン；
	//・お弁当に入ってる魚の醤油入れ
	//・ガードレールの端の部分

	for (auto i : step(10)) {
		exhibits[i].setPos({ (i % 5 + 1) * Scene::Width() / 6,150 + (i / 5) * 200 });
	}
}

void MuseumScene::update()
{
	bool activeFlg = false;

	for (auto i : step(10)) {
		exhibits[i].update();
		if (exhibits[i].active) {
			activeFlg = true;
			light1.update(exhibits[i].pos+Vec2{10,0});
			light2.update(exhibits[i].pos + Vec2{ -10,0 });
			panel.text=exhibits[i].getText();
		}
	}
	if (not activeFlg) {
		light1.update(Vec2{ -100,-100 });
		light2.update(Vec2{ Scene::Width() + 100,-100 });
		panel.text = U"説明を見たいゴミを選択してください。";
	}

	if (mapButton.update()) {
		changeScene(U"Map", 1s, CrossFade::Yes);
	}
}

void MuseumScene::draw()const
{
	Scene::SetBackground(Palette::White);

	{
		const ScopedRenderTarget2D renderTarget{ renderTexture.clear(Scene::GetBackground()) };

		//ここに描画
		for (auto i : step(10)) {
			exhibits[i].draw();
		}

	}
	Graphics2D::Flush();
	renderTexture.resolve();

	{
		const ScopedRenderTarget2D target{ spotlight.clear(ColorF{1,0}) };
		{
			const ScopedRenderStates2D blend{ MakeBlendState() };

			//for (auto i : step(10)) {
			//	exhibits[i].drawLight();
			//}

			light1.draw();
			light2.draw();
		}
		//ライトを描画していないところは透明になる
		renderTexture.draw();
	}

	Graphics2D::Flush();
	spotlight.resolve();

	//描画
	renderTexture.draw(ColorF{ 0.3 });//暗くする
	spotlight.draw();

	panel.draw();
	mapButton.draw();
}
