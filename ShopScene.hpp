#pragma once
#include"Common.hpp"
#include"StoneButton.hpp"
#include"ItemPanel.hpp"
#include"CountPanel.hpp"

class SellButton {
public:

	ItemPanel panel;
	StoneButton button1;
	StoneButton button10;
	StoneButton button100;

	SellButton(const String& name, const String& desc, const String& icon, const Array<String>& texture, const Vec2& pos)
		:panel{ name , desc , icon , texture ,pos },
		button1{ RectF{Arg::center(pos.x - 90,pos.y + 50),80,40},U"1",false },
		button10{ RectF{Arg::center(pos.x,pos.y + 50),80,40},U"10",false },
		button100{ RectF{Arg::center(pos.x + 90,pos.y + 50),80,40},U"100" ,false }
	{
	}

	int32 update(int32& num) {

		button1.active = 1 <= num;
		button10.active = 10 <= num;
		button100.active = 100 <= num;

		if (button1.update()) {
			num -= 1;
			return 1;
		}
		if (button10.update()) {
			num -= 10;
			return 10;
		}
		if (button100.update()) {
			num -= 100;
			return 100;
		}
		return 0;
	}

	void draw()const
	{
		panel.draw();
		button1.draw();
		button10.draw();
		button100.draw();
	}
};

class BuyButton {
public:

	ItemPanel panel;

	bool complete = false;

	StoneButton button;

	int32 iron;
	int32 gold;
	int32 money;

	BuyButton(const String& name, const String& desc, const String& icon, const Array<String>& texture, const Vec2& pos,int32 iron,int32 gold,int32 money)
		:panel{ name , desc, icon, texture ,pos },
		button{ RectF{Arg::center(pos.x,pos.y + 50),80,40},U"買う",false },
		iron{iron},gold{gold},money{money}
	{
	}

	bool update(GameData& data,int32& item,int32 max=1) {

		if (max<=item) {
			complete = true;
			return false;
		}

		if(iron<=data.iron&&gold<=data.gold&&money<=data.money){
			button.active = (item<max);
		}
		else {
			button.active = false;
		}
		if (button.update()) {
			data.iron -= iron;
			data.gold -= gold;
			data.money -= money;
			item += 1;
			return true;
		}
		else {
			return false;
		}
	}

	void draw()const
	{
		panel.draw();

		if (not complete) {
			button.draw();
		}
		else {
			FontAsset(U"ItemButton1")(U"完了✓").drawAt(button.rect.center());
		}
	}
};


// ゲームシーン
class ShopScene : public MyScene
{
public:
	StoneButton mapButton{ RectF{20,20,200,50},U"マップ" };

	CountPanel garbagePanel{ Vec2{350,60},getData().garbage,U"garbage" };
	CountPanel ironPanel{ Vec2{600,60},getData().iron,U"iron" };
	CountPanel goldPanel{ Vec2{850,60},getData().gold,U"gold" };
	CountPanel moneyPanel{ Vec2{1100,60},getData().money,U"coin" };


	SellButton garbageCell{ U"ゴミを売る",U"ゴミ$0→$1×1",U"coin",{U"garbage",U"coin"},{240,200} };
	SellButton ironCell{ U"鉄を売る",U"鉄$0→$1×5",U"coin",{U"iron",U"coin"},{240,400} };
	SellButton goldCell{ U"金を売る",U"金$0→$1×20",U"coin",{U"gold",U"coin"},{240,600} };

	BuyButton buyMagnet{ U"磁石を建てる",U"鉄が増加します。\n$0×50,$1×15,$2×300",U"magnet1",{U"iron",U"gold",U"coin"},{640,200},50,15,300};
	BuyButton buyCat{ U"招き猫を建てる",U"金が増加します。\n$0×80,$1×30,$2×400",U"manekineko1",{U"iron",U"gold",U"coin"},{640,400},80,30,400};
	BuyButton buyMissile{ U"発射台を建てる",U"カラスの撃退を手伝います。\n$0×100,$1×25,$2×500",U"rocketLv1",{U"iron",U"gold",U"coin"},{640,600},100,25,500};
	BuyButton buyResearch{ U"研究所を建てる",U"施設をLvUPできます。\n$0×200,$1×50,$2×800",U"research",{U"iron",U"gold",U"coin"},{1040,200},200,50,800 };
	BuyButton buyHouse{ U"家を建てる",U"ゴミの量が増えます。\n$0×100,$1×20,$2×300",U"house1",{U"iron",U"gold",U"coin"},{1040,400},100,20,300 };
	BuyButton buyArm{ U"アーム拡大",U"アームが大きくなります。\n$0×300,$1×10,$2×500",U"garbage",{U"iron",U"gold",U"coin"},{1040,600},300,10,500 };

	ShopScene(const InitData& init);

	void update() override;

	void draw() const override;
private:
};
