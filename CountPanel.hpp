#pragma once
class CountPanel {
public:
	double currentValue = 0.0;

	double velocity = 0.0;

	Vec2 pos;

	String textureName;

	CountPanel(const Vec2& pos, int32 value, const String& textureName) :pos{ pos }, currentValue{ static_cast<double>(value) }, textureName{ textureName } {

	}

	void update(int32  targetValue) {
		currentValue = Math::SmoothDamp(currentValue, targetValue, velocity, 0.3);
	}

	void draw()const {

		const RoundRect rect{ Arg::center = pos, 230, 70, 6 };
		rect.draw(ColorF{ 0.2, 0.25, 0.3 }).drawFrame(0, 3, Palette::White);

		const int32 value = static_cast<int32>(Math::Round(currentValue));
		RoundRect{ Arg::center = pos - Vec2{80,0},60,60,5 }.draw(Palette::Skyblue);
		TextureAsset(textureName).resized(60).drawAt(pos - Vec2{ 80,0 });
		FontAsset(U"NormalFont")(U"×{}"_fmt(value)).draw(30,Arg::leftCenter = pos-Vec2{40,0});
	}

private:
};
