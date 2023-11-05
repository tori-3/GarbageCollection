#include "stdafx.h"
#include "ItemPanel.hpp"

ItemPanel::ItemPanel(const String& name, const String& desc, const String& icon, const Array<String>& texture, const Vec2& pos)
	:name{ name }, desc{ desc }, icon{ icon }, texture{ texture }, pos{ pos }
{
	FontAsset::Register(U"ItemButton1", 30);
	FontAsset::Register(U"ItemButton2", 20);
}

void ItemPanel::draw()const
{
	const RoundRect r0{ Arg::center = pos, 360, 150, 6 };
	const RoundRect r1{ r0.rect.pos + Vec2{5, 5}, 90, 90, 5 };

	r0.draw(ColorF{ 0.2, 0.25, 0.3 }).drawFrame(0, 3, Palette::White);
	r1.draw(Palette::Skyblue);
	TextureAsset(icon).resized(80).drawAt(r1.center());
	FontAsset(U"ItemButton1")(name).draw(r1.rect.pos.movedBy(100, 0));

	const Vec2 penPos0 = r1.rect.pos.movedBy(100, 42);
	Vec2 penPos = penPos0;
	bool onTag = false;

	for (const auto& glyph : FontAsset(U"ItemButton2").getGlyphs(desc))
	{
		if (onTag)
		{
			TextureAsset(texture[(glyph.codePoint - U'0')]).resized(25).draw(penPos);
			penPos.x += 25;
			onTag = false;
			continue;
		}

		if (glyph.codePoint == U'$')
		{
			onTag = true;
			continue;
		}

		onTag = false;

		if (glyph.codePoint == U'\n')
		{
			penPos.x = penPos0.x;
			penPos.y += FontAsset(U"ItemButton2").height();
		}
		else
		{
			glyph.texture.draw(penPos + glyph.getOffset());
			penPos.x += glyph.xAdvance;
		}
	}
}
