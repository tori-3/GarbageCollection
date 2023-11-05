#pragma once
void SoundIcon(double volume, const Circle& circle, const ColorF& color = Palette::White);

class Slider {
public:

	double value = 1.0;

	Rect rect;

	bool activeFlg = false;

	Slider(double value, const Rect& rect);

	double update();

	void draw()const;

};
