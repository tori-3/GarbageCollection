#pragma once
class ItemPanel {
public:

	String name, desc;
	String icon;
	Array<String>texture;
	Vec2 pos{};

	ItemPanel() = default;

	ItemPanel(const String& name, const String& desc, const String& icon, const Array<String>& texture, const Vec2& pos);

	void draw()const;

	RoundRect getRoundRect()const {
		return 	RoundRect{ Arg::center = pos, 360, 150, 6 };
	}

};
