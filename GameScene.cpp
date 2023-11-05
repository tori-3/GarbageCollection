#include"GameScene.hpp"

void ConveyorBelt(const Vec2& pos) {
	const double size = Scene::Size().x;
	constexpr int32 N = 10;
	RectF{ pos,size,20 }.draw(Palette::Black);
	for (auto i : step(N)) {
		const double d = Periodic::Sawtooth0_1(12.8s, Scene::Time() + i * 12.8 / N);
		Line{ pos + Vec2{d * size,0},pos + Vec2{d * size,20} }.draw(2);
	}

	double h = 20;
	int32 n = 3;
	for (auto i : step(n)) {
		double y = Scene::Height() - h / double(n) * i;
		Line{ Vec2{0,y},Vec2{Scene::Width(),y}}.draw();
	}
}

void GlassReflection(const Vec2& pos, int32 size) {

	Triangle(pos.x, pos.y, pos.x + size * 0.85, pos.y, pos.x, pos.y + size * 0.85).draw(ColorF(1, 0), ColorF(1, 0.2), ColorF(1, 0.2));
	Quad{ pos + Vec2{ size * 0.9, 0 }, pos + Vec2{ size, 0 }, pos + Vec2{ 0,size  }, pos + Vec2{ 0, size * 0.9 } }.draw(ColorF(1, 0.2));

}

void GlassWindow(const RectF& rect) {
	rect.draw(Palette::White);
	const RectF windowRect = rect.stretched(-5);
	windowRect.draw(Palette::Skyblue);
	GlassReflection(windowRect.pos, 50);

}

GameScene::GameScene(const InitData& init)
	: MyScene{ init,Palette::Gray }
{
}

void GameScene::update()
{
	Cursor::RequestStyle(U"MyCursor");

	AudioAsset(U"GameAudio").play(MixBus0);

	constexpr double StepTime = (1.0 / 200.0);
	for (auto i : step(interval(accumulatedTime, StepTime)))
	{
		world.update(StepTime);

		for (auto&& [pair, collision] : world.getCollisions())
		{
			if (pair.a == ground.id()) {
				bodies[pair.b]->body.setVelocity(Vec2{ 20000, 0 } *StepTime);
			}
			else if (pair.b == ground.id()) {
				bodies[pair.a]->body.setVelocity(Vec2{ 20000, 0 } *StepTime);
			}
		}
	}

	for (auto it = bodies.begin(); it != bodies.end();)
	{
		if (it->second->body.getPos().y > Scene::Height()) {
			it = bodies.erase(it);
		}
		else {
			++it;
		}
	}

	if (excavator.update(bodies, effect, getData())) {
		rainbowTimer.restart();
	}

	for (auto i : step(interval(accum, 1/getData().generateSpeed()))) {

		count++;

		auto body1 = rainbowTimer.isRunning()?new GoldGarbage{world,{-90, Scene::Height() - 100 - 100} } : GenerateRandomGarbage(world, {-90, Scene::Height() - 100 - 100},getData());
		bodies[body1->id()] = std::unique_ptr<Garbage>(body1);

		if (bodies[body1->id()]->getType() == GarbageType::Gold && RandomBool(1/3.0)) {
			int32 id = Crow::generateId();
			crowTable[id] = Crow{ body1->id() };
			if (RandomBool(getData().missileHitRate())) {
				missiles << Missile{ id };
			}
		}
	}

	for (auto& missile : missiles) {
		missile.update(crowTable, effect);
	}

	missiles.remove_if([](const Missile& missile) {return missile.isDie(); });

	for (auto it = crowTable.begin(); it != crowTable.end();)
	{
		it->second.update(bodies, effect);

		if (it->second.isDie()) {
			it = crowTable.erase(it);
		}
		else {
			++it;
		}
	}


	if (mapButton.update()) {
		changeScene(U"Map", 1s, CrossFade::Yes);
		AudioAsset(U"GameAudio").stop(1s);
	}

	garbageDisplay.update(getData().garbage);
	ironDisplay.update(getData().iron);
	goldDisplay.update(getData().gold);

	explainFlg ^= explainButton.update();
	explainButton.text = explainFlg?U"説明とじる":U"説明";
}

void GameScene::draw()const
{
	Scene::SetBackground(ColorF{0.5});

	GlassWindow(RectF{  40,30,100 });
	GlassWindow(RectF{ 140,30,100 });

	GlassWindow(RectF{ 440,30,100 });
	GlassWindow(RectF{ 540,30,100 });

	GlassWindow(RectF{ 840,30,100 });
	GlassWindow(RectF{ 940,30,100 });


	RectF{ 270,100,70,40 }.draw(Arg::top=ColorF{1},Arg::bottom=ColorF{0.7});

	RectF{ 210 ,230,70,40 }.draw(Arg::top = ColorF{ 1 }, Arg::bottom = ColorF{ 0.7 });

	RectF{250,0,40,700}.draw(Arg::left=ColorF{0.7},Arg::right=ColorF{1});

	fan.draw(Vec2{ 150,250 });

	//ディスプレイ
	RectF{320,50,400,500}.draw(ColorF{0.8});
	garbageDisplay.draw();
	ironDisplay.draw();
	goldDisplay.draw();

	//コンベアの後ろ
	{
		constexpr int32 n = 400;
		Rect{ 0,n,Scene::Width(),Scene::Height() - n }.draw(ColorF{ 0.4 });
	}
	excavator.draw();

	// すべてのボディを描画
	for (auto& [key, body] : bodies)
	{
		body->draw();
	}


	ConveyorBelt(Vec2{ 0,Scene::Size().y - 20 });

	for (auto it = crowTable.begin(); it != crowTable.end(); ++it)
	{
		it->second.draw();
	}

	for (const auto& missile : missiles) {
		missile.draw(getData().missile);
	}

	effect.update();

	mapButton.draw();

	explainButton.draw();

	if (explainFlg) {
		TextureAsset(U"説明1").resized(Scene::Width()).draw();
	}
}
