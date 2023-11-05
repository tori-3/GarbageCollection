#pragma once
class StoneButton {
public:
	RectF rect;
	String text;
	bool active=true;
	StoneButton() = default;
	StoneButton(const RectF& rect, const String& text, bool active = true);
	bool update();
	void draw()const;
};
