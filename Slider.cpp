#include "stdafx.h"
#include "Slider.hpp"

void SoundIcon(double volume, const Circle& circle, const ColorF& color) {

	const double w = circle.r / 2;
	const Vec2 pos = circle.center;

	if (volume == 0)Shape2D::Cross(w * 0.7, w * 0.1, { pos.x + w * 1.4,pos.y }).draw(color);
	if (0 < volume)Circle{ pos,w }.drawArc(45_deg, 90_deg, w * 0.1, 0, color);
	if (0.3 <= volume)Circle{ pos,w * 1.5 }.drawArc(45_deg, 90_deg, w * 0.1, 0, color);
	if (0.7 <= volume)Circle{ pos,w * 2 }.drawArc(45_deg, 90_deg, w * 0.1, 0, color);
	Triangle{ pos, w * 2, -90_deg }.draw(color);
	RectF{ Arg::center(pos.x - w * 0.7,pos.y),w }.draw(color);
}

Slider::Slider(double value, const Rect& rect) :value{ value }, rect{ rect } {}

double Slider::update() {
	if (rect.stretched(15).leftClicked())activeFlg = true;

	if (activeFlg) {
		Cursor::RequestStyle(CursorStyle::Hand);
		value = (Clamp<double>(Cursor::PosF().x - rect.x, 0, rect.w)) / rect.w;
		if (MouseL.up())activeFlg = false;
	}
	return value;
}

void Slider::draw()const {
	rect.draw(Palette::Black);
	RectF{ rect.pos,value * rect.w,rect.h }.draw({ 0.3, 0.5, 1.0 });
	RoundRect(Arg::center(value * rect.w + rect.x, rect.center().y), 20, 50, 10).draw();
}
