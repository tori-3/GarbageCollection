#pragma once
#include"Common.hpp"
#include"StoneButton.hpp"
#include"ItemPanel.hpp"
#include"CountPanel.hpp"

class UpgradeButton {
public:

	ItemPanel panel;

	bool complete = false;

	StoneButton button;

	bool enabled = true;

	int32 level = 0;
	int32 dataLevel = -1;

	UpgradeButton(const String& name, const String& desc, const String& icon, const Array<String>& texture, const Vec2& pos,int32 level)
		:panel{ name , desc, icon, texture ,pos },level{level},
		button{ RectF{Arg::center(pos.x,pos.y + 50),150,40},level==1?U"ショップに行く":U"アップグレード",false}
	{
	}

	bool update(int32 _level,bool active=true) {
		dataLevel = _level;
		button.active = ((dataLevel+1) == level)&&active;
		return button.update();
	}

	void draw()const
	{
		if (level<3) {
			if (dataLevel < level) {
				RectF{ Arg::center = panel.pos + Vec2{0,100},50,50 }.draw(Arg::left=ColorF{0.3},Arg::right=ColorF{0.7});
			}
			else {
				RectF{ Arg::center = panel.pos + Vec2{0,100},50,50 }.draw(Arg::left = ColorF{ Palette::Greenyellow }, Arg::right = ColorF{ 1 });
			}
		}

		panel.draw();
		if ((dataLevel + 1) == level) {
			panel.getRoundRect().drawFrame(0, 5, Palette::Greenyellow);
		}

		if (dataLevel<level) {
			button.draw();
		}
		else {
			FontAsset(U"ItemButton1")(U"完了✓").drawAt(button.rect.center());
		}

		if (dataLevel+1<level) {
			panel.getRoundRect().draw(ColorF{ 0,0.7 });
		}
	}
};

// ゲームシーン
class ResearchScene : public MyScene
{
public:

	CountPanel garbagePanel{ Vec2{350,60},getData().garbage,U"garbage" };
	CountPanel ironPanel{ Vec2{600,60},getData().iron,U"iron" };
	CountPanel goldPanel{ Vec2{850,60},getData().gold,U"gold" };
	CountPanel moneyPanel{ Vec2{1100,60},getData().money,U"coin" };

	StoneButton mapButton{ RectF{20,20,200,50},U"マップ" };

	UpgradeButton magnetButtonLv1{ U"磁石Lv1",U"鉄が増加します。",U"magnet1",{},{240,200},1 };
	UpgradeButton magnetButtonLv2{ U"磁石Lv2",U"鉄がより増加します。\n$0×500",U"magnet2",{U"coin"},{240,400},2 };
	UpgradeButton magnetButtonLv3{ U"磁石Lv3",U"鉄がより増加します。\n$0×800",U"magnet3",{U"coin"},{240,600},3 };
	UpgradeButton catButtonLv1{ U"招き猫Lv1",U"金が増加します。",U"manekineko1",{},{640,200},1};
	UpgradeButton catButtonLv2{ U"招き猫Lv2",U"金がより増加します。\n$0×500",U"manekineko2",{U"coin"},{640,400},2};
	UpgradeButton catButtonLv3{ U"招き猫Lv3",U"金がより増加します。\n$0×800",U"manekineko3",{U"coin"},{640,600},3 };
	UpgradeButton missileButtonLv1{ U"発射台Lv1",U"カラスの撃退を手伝います。",U"rocketLv1",{},{1040,200},1 };
	UpgradeButton missileButtonLv2{ U"発射台Lv2",U"カラスの撃退率が上がります。\n$0×500",U"rocketLv2",{U"coin"},{1040,400},2 };
	UpgradeButton missileButtonLv3{ U"発射台Lv3",U"カラスの撃退率が上がります。\n$0×800",U"rocketLv3",{U"coin"},{1040,600},3 };

	ResearchScene(const InitData& init);

	void update() override;

	void draw() const override;
private:
};
