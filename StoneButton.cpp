#include "stdafx.h"
#include "StoneButton.hpp"

StoneButton::StoneButton(const RectF &rect, const String &text, bool active) :rect{ rect }, text{ text }, active{ active }
{
	//フォントを登録
	FontAsset::Register(U"StoneButtonFont", FontMethod::MSDF, 50);
}

bool StoneButton::update() {

	//マウスの形を変える
	if (rect.mouseOver() && active)Cursor::RequestStyle(CursorStyle::Hand);

	if (rect.leftClicked() && active) {
		AudioAsset(U"StoneButtonAudio").playOneShot(MixBus1);
	}
	//クリックしたらtrueを返す
	return rect.leftClicked() && active;
}

void StoneButton::draw()const {

	if (rect.mouseOver() && active)rect.drawFrame(10, Palette::Red);

	rect.draw(ColorF{ 0.5 }).drawFrame(15, 0, ColorF{ 0.5 }, rect.leftPressed() && active ? ColorF{ 1 } : ColorF{ 0.2 });

	//文字を描画
	FontAsset(U"StoneButtonFont")(text).drawAt((rect.h - 15) * 0.7, rect.center());

	if (not active)rect.draw(ColorF{ 0,0.5 });
}
