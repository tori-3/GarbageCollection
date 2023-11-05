#include"Common.hpp"
#include"TitleScene.hpp"
#include"GameScene.hpp"
#include"MapScene.hpp"
#include"ShopScene.hpp"
#include"MuseumScene.hpp"
#include"ResearchScene.hpp"
#include"StartScene.hpp"

std::unique_ptr<AudioAssetData> MakeAudioAssetData()
{
	auto tad = std::make_unique<AudioAssetData>();
	tad->onLoad = [](AudioAssetData& data, const String&)
	{
		data.audio = Audio{ U"audio/gamejam_game.wav", Arg::loopBegin = 16s };
		return true;
	};
	return tad;
}

Image CreateCursorImage()
{
	Image image{ 32, 32 };
	constexpr int32 L1 = 30;
	constexpr int32 L2 = 20;
	Vec2 pos0{};
	Vec2 pos1{ 0,L1 };
	Vec2 pos2 = (*Circle{ pos0,L1 }.intersectsAt(Circle{ pos1,L2 }))[0];
	Triangle{ pos0,pos1,pos2 }.overwrite(image, Palette::Red);
	return image;
}

void Main()
{
	Cursor::RegisterCustomCursorStyle(U"MyCursor", CreateCursorImage(), Point{ 0, 0 });

	{
		TextReader reader{ Resource(U"data.txt") };

		// オープンに失敗
		if (not reader)
		{
			// 例外を投げて終了
			throw Error{ U"Failed to open `data.txt`" };
		}
		String path;

		while (reader.readLine(path))
		{
			TextureAsset::Register(FileSystem::BaseName(path), Resource(path), TextureDesc::Mipped);
			TextureAsset::LoadAsync(FileSystem::BaseName(path));
		}
	}

	FontAsset::Register(U"NormalFont", FontMethod::MSDF, 50);
	AudioAsset::Register(U"GameAudio", MakeAudioAssetData());
	AudioAsset::Register(U"MapAudio", U"audio/gamejam_map.wav");
	AudioAsset::Register(U"Excavator", GMInstrument::SynCalliope, PianoKey::C3, 1.5s);
	AudioAsset::Register(U"GetEffect", GMInstrument::Marimba, PianoKey::C5, 0.5s, 0.5s, 0.5);
	AudioAsset::Register(U"CrowDamage", GMInstrument::Gunshot, PianoKey::C6, 0.5s);
	AudioAsset::Register(U"StoneButtonAudio", GMInstrument::Woodblock, PianoKey::C4, 0.5s);

	FontAsset::LoadAsync(U"NormalFont");
	AudioAsset::LoadAsync(U"GameAudio");
	AudioAsset::LoadAsync(U"MapAudio");
	AudioAsset::LoadAsync(U"Excavator");
	AudioAsset::LoadAsync(U"GetEffect");
	AudioAsset::LoadAsync(U"CrowDamage");
	AudioAsset::LoadAsync(U"StoneButtonAudio");

	Window::Resize(1280, 720);
	Window::SetTitle(U"Garbage Collection");

	// シーンマネージャーを作成
	// ここで GameData が初期化される
	App manager;
	manager.add<StartScene>(U"Start");
	manager.add<TitleScene>(U"Title");
	manager.add<GameScene>(U"Game");
	manager.add<MapScene>(U"Map");
	manager.add<ShopScene>(U"Shop");
	manager.add<MuseumScene>(U"Museum");
	manager.add<ResearchScene>(U"Research");

	//museum
	manager.init(U"Start");
	manager.changeScene(U"Start", 0s);

	Effect effect;	

	while (System::Update())
	{
		if (not manager.update())
		{
			break;
		}
		if (MouseL.down()) {
			effect.add<RingWaveEffect>(Cursor::Pos());
		}
		effect.update();
	}
}
