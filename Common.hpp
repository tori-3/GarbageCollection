#pragma once
# include <Siv3D.hpp>

// 共有するデータ
struct GameData
{
	double bgmVolume = 1;
	double effectVolume = 1;

	int32 money = 0;
	int32 garbage = 0;
	int32 iron = 0;
	int32 gold = 0;

	//施設レベル
	int32 magnet = 0;
	int32 cat = 0;
	int32 missile = 0;
	int32 research = 0;

	int32 house = 0;

	int32 upgradeArm = false;

	Grid<int32>grid{ 10, 10 };

	Array<bool>collection=Array<bool>(10,false);

	GameData() {

		JSON json = JSON::Load(U"save/save.json");

		if (json) {
			bgmVolume = json[U"BGMVolume"].get<double>();
			effectVolume = json[U"EffectVolume"].get<double>();

			money = json[U"Money"].get<int32>();
			garbage = json[U"Garbage"].get<int32>();
			iron = json[U"Iron"].get<int32>();
			gold = json[U"Golds"].get<int32>();

			magnet = json[U"Magnet"].get<int32>();
			cat = json[U"Cat"].get<int32>();
			missile = json[U"Missile"].get<int32>();
			research = json[U"Research"].get<int32>();
			house = json[U"House"].get<int32>();
			upgradeArm = json[U"UpgradeArm"].get<int32>();

			collection.clear();
			for (const auto& elem : json[U"Collection"].arrayView())
			{
				collection << elem.get<bool>();
			}
		}

		CSV csv{ U"save/mapData.csv" };

		if (csv) {
			for (auto p : step({ 10,10 }))
			{
				grid[p] = Parse<int32>(csv[p.x][p.y]);
			}
		}

		setVolume();
	}

	void save() {
		JSON json;
		json[U"BGMVolume"] = bgmVolume;
		json[U"EffectVolume"] = effectVolume;

		json[U"Money"] = money;
		json[U"Garbage"] = garbage;
		json[U"Iron"] = iron;
		json[U"Golds"] = gold;

		json[U"Magnet"] = magnet;
		json[U"Cat"] = cat;
		json[U"Missile"] = missile;
		json[U"Research"] = research;
		json[U"House"] = house;
		json[U"UpgradeArm"] = upgradeArm;

		json[U"Collection"] = collection;

		json.save(U"save/save.json");

		CSV csv;
		for (auto i : step(10)) {
			csv.writeRow(U"0", U"0", U"0", U"0", U"0", U"0", U"0", U"0", U"0", U"0");
		}

		for (auto p : step({ 10,10 }))
		{
			csv[p.x][p.y] = Format(grid[p]);
		}
		csv.save(U"save/mapData.csv");
	}

	~GameData() {
		save();
	}

	void setVolume() {
		GlobalAudio::BusSetVolume(MixBus0, bgmVolume);
		GlobalAudio::BusSetVolume(MixBus1, effectVolume);
	}

	Array<int32>buildingNum() {

		return {999,999,999,999,//草　木　川　道
			1-(int32)grid.count(4),//拠点
			1 - (int32)grid.count(5),//ショップ
			1 - (int32)grid.count(6),//博物館
			(0<magnet) - (int32)grid.count(7)
			,(0 < cat) - (int32)grid.count(8)
			,(0 < missile) - (int32)grid.count(9)
			,research - (int32)grid.count(10)
			,Clamp(house,0,4) - (int32)grid.count(11)
			,Clamp(house-4,0,4) - (int32)grid.count(12)
			,Clamp(house-8,0,2) - (int32)grid.count(13)
		};
	}

	int32 houseLv()const {
		if (house < 4) {
			return 1;
		}
		else if(house<8){
			return 2;
		}
		else {
			return 3;
		}
	}

	double garbageRate()const {
		return 1 - ironRate() - goldRate() - bulkyRate()-bombRate()-rainbowRate()- bigGarbageRate();
	}

	double bigGarbageRate()const {
		return 0.01;
	}

	//Lv0:5% Lv1:10% Lv2:15% Lv3:20%
	double ironRate()const {
		return 0.05 * (1 + magnet);
	}

	//Lv0:1% Lv1:2% Lv2:3% Lv3:4%
	double goldRate()const {
		return 0.01 * (1 + cat);
	}

	//0.5%
	double bulkyRate()const {
		return 0.005;
	}

	//5%
	double bombRate()const {
		return 0.05;
	}

	//0.05%
	double rainbowRate()const {
		return 0.0005;
	}

	double missileHitRate()const {
		if (missile == 0) {
			return 0;
		}
		else {
			return 0.3 + missile * 0.2;
		}
	}

	double generateSpeed()const {
		return 7 + house;
	}
};

// 共有するデータの型を指定
using App = SceneManager<String, GameData>;

class MyScene : public App::Scene
{
public:

	ColorF fadeColor;

	MyScene(const InitData& init, const ColorF& color) :IScene{ init }, fadeColor{color} {}

	void drawFadeIn(double t) const override
	{
		const ScopedViewport2D viewport{ 0, (int32)((1 - t) * Scene::Size().y), Scene::Size() };
		draw();
	}

	void drawFadeOut(double t) const override
	{
		const ScopedViewport2D viewport{ 0, (int32)(-t * Scene::Size().y), Scene::Size() };
		Rect{ Scene::Size() }.draw(fadeColor);
		draw();
	}

};
