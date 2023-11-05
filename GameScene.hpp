#pragma once
#include"Common.hpp"
#include"Effect.hpp"
#include"Tool.hpp"
#include"StoneButton.hpp"

enum class GarbageType {
	Garbage, Iron, Gold, Bomb,Bulky,Rainbow
};

class Garbage {
public:
	P2Body body;

	Garbage(const P2Body& body):body {body} {}

	virtual ~Garbage(){}

	virtual void draw()const = 0;

	virtual GarbageType getType()const = 0;

	virtual void catchItem(GameData& data)const = 0;

	Vec2 getPos()const {
		return body.getPos();
	}

	P2BodyID id()const {
		return body.id();
	}
};

class NormalGarbage :public Garbage {
public:
	NormalGarbage(P2World& world,const Vec2& pos) :Garbage{ world.createCircle(P2Dynamic, pos, 20) } {

	}

	void draw()const override {
		TextureAsset(U"garbage").resized(65).rotated(body.getAngle()).drawAt(body.getPos());
	}

	void catchItem(GameData& data)const override{
		data.garbage += 1;
	}

	GarbageType getType()const override {
		return GarbageType::Garbage;
	}	
};

class BigGarbage :public Garbage {
public:
	BigGarbage(P2World& world, const Vec2& pos) :Garbage{ world.createCircle(P2Dynamic, pos, 20*2) } {

	}

	void draw()const override {
		TextureAsset(U"garbage").resized(65*2).rotated(body.getAngle()).drawAt(body.getPos());
	}

	void catchItem(GameData& data)const override {
		data.garbage += 5;
	}

	GarbageType getType()const override {
		return GarbageType::Garbage;
	}
};

class IronGarbage :public Garbage {
public:
	IronGarbage(P2World& world, const Vec2& pos) :Garbage{ world.createCircle(P2Dynamic, pos, 20) } {

	}

	void draw()const override {
		TextureAsset(U"iron").resized(65).rotated(body.getAngle()).drawAt(body.getPos());
	}

	void catchItem(GameData& data)const override {
		data.iron += 1;
	}

	GarbageType getType()const override {
		return GarbageType::Iron;
	}
};

class GoldGarbage :public Garbage {
public:
	GoldGarbage(P2World& world, const Vec2& pos) :Garbage{ world.createCircle(P2Dynamic, pos, 20) } {

	}

	void draw()const override {
		TextureAsset(U"gold").resized(65).rotated(body.getAngle()).drawAt(body.getPos());
	}

	void catchItem(GameData& data)const override {
		data.gold += 1;
	}

	GarbageType getType()const override {
		return GarbageType::Gold;
	}
};

class BombGarbage :public Garbage {
public:
	BombGarbage(P2World& world, const Vec2& pos) :Garbage{ world.createCircle(P2Dynamic, pos, 20) } {

	}

	void draw()const override {
		TextureAsset(U"bomb").resized(50).rotated(body.getAngle()).drawAt(body.getPos());
	}

	void catchItem(GameData& data)const override {}

	GarbageType getType()const override {
		return GarbageType::Bomb;
	}
};

class BulkyGarbage :public Garbage {
public:
	int32 bulkyNum = RandomBulkyNum();

	BulkyGarbage(P2World& world, const Vec2& pos) :Garbage{ world.createCircle(P2Dynamic, pos, 20*2) } {

	}

	void draw()const override {
		switch (bulkyNum)
		{
		case 0:
			TextureAsset(U"microwave").resized(120).rotated(body.getAngle()).drawAt(body.getPos());
			break;
		case 1:
			TextureAsset(U"washing").resized(100).rotated(body.getAngle()).drawAt(body.getPos());
			break;
		case 2:
			TextureAsset(U"blackBag").resized(80).rotated(body.getAngle()).drawAt(body.getPos());
			break;
		case 3:
			TextureAsset(U"鉄球").resized(200).rotated(body.getAngle()).drawAt(body.getPos());
			break;
		case 4:
			Circle{ body.getPos(),40 }.draw();
			TextureAsset(U"siv3d").resized(70).rotated(body.getAngle()).drawAt(body.getPos());
			break;
		case 5:
			TextureAsset(U"サインポール").resized(100).rotated(body.getAngle()).drawAt(body.getPos());
			break;
		case 6:
			TextureAsset(U"bagClosure").resized(80).rotated(body.getAngle()).drawAt(body.getPos());
			break;
		case 7:
			Circle{ body.getPos(),40 }.draw();
			TextureAsset(U"セミコロン").resized(100).rotated(body.getAngle()).drawAt(body.getPos());
			break;
		case 8:
			TextureAsset(U"shouyu").resized(100).rotated(body.getAngle()).drawAt(body.getPos());
			break;
		case 9:
			TextureAsset(U"袖ビーム").resized(120).rotated(body.getAngle()).drawAt(body.getPos());
			break;
		default:
			break;
		}
	}

	void catchItem(GameData& data)const override {
		data.iron += 3;
		data.gold += 1;
		data.collection[bulkyNum] = true;
	}

	GarbageType getType()const override {
		return GarbageType::Bulky;
	}

private:
	static int32 RandomBulkyNum() {

		DiscreteDistribution distribution{ {
			45,45,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25
		} };

		return DiscreteSample({0,1,2,3,4,5,6,7,8,9}, distribution);
	}

};

class RainbowGarbage :public Garbage {
public:

	RainbowGarbage(P2World& world, const Vec2& pos) :Garbage{ world.createCircle(P2Dynamic, pos, 20) } {

	}

	void draw()const override {
		TextureAsset(U"rainbow").resized(65).rotated(body.getAngle()).drawAt(body.getPos());
	}

	void catchItem(GameData& data)const override {}

	GarbageType getType()const override {
		return GarbageType::Rainbow;
	}
};

inline	Garbage* GenerateRandomGarbage(P2World& world,const Vec2& pos,const GameData& data) {

	static Array<std::function<Garbage* (P2World&,const Vec2&)>> options =
	{
		[](P2World& world,const Vec2& pos) {return new NormalGarbage{world,pos}; },
		[](P2World& world,const Vec2& pos) {return new IronGarbage{world,pos}; },
		[](P2World& world,const Vec2& pos) {return new GoldGarbage{world,pos}; },
		[](P2World& world,const Vec2& pos) {return new BombGarbage{world,pos}; },
		[](P2World& world,const Vec2& pos) {return new BulkyGarbage{world,pos}; },
		[](P2World& world,const Vec2& pos) {return new RainbowGarbage{world,pos}; },
		[](P2World& world,const Vec2& pos) {return new BigGarbage{world,pos}; }
	};

	DiscreteDistribution distribution{ {
			data.garbageRate(),
			data.ironRate(),
			data.goldRate(),
			data.bombRate(),
			data.bulkyRate(),
			data.rainbowRate(),
			data.bigGarbageRate()
		} };

	return DiscreteSample(options, distribution)(world,pos);
}

class MultipleTimes {
public:
	double accumlater = 0;
	int32 n = 0;
	double spawn = 0;

	MultipleTimes(double spawn) :spawn{spawn} {

	}

	void start(int32 _n) {
		n = _n;
		accumlater = 0;
	}

	bool update() {
		if (0<n&&interval(accumlater, spawn)) {
			--n;
			return true;
		}
		else {
			return false;
		}
	}
};

class Excavator {
public:

	P2Body hand;
	Vec2 pos;
	double angle = -30_deg;
	double excavatorAngle = 0;
	int32 mode = 0;
	double time = 0;

	MultipleTimes multi{0.1};

	Array<std::unique_ptr<Garbage>>holds;

	bool upgrade = false;

	Excavator(const Vec2& pos, P2World& world,bool upgrade) :pos{ pos }
		, hand{ world.createRect(P2Kinematic, Vec2{ 0,0 }, RectF{ Arg::center(0,0), 10,10 }) }
		,upgrade{upgrade}
	{
		double handLen = 120;
		hand.addRect(RectF{ -5,0,10,handLen });
		if (upgrade) {
			hand.addRect(RectF{ -handLen + 5-30 ,handLen ,handLen+30 ,10 });
		}
		else {
			hand.addRect(RectF{ -handLen + 5 ,handLen ,handLen ,10 });
		}
		hand.setAngle(angle);
		hand.setPos(pos);
	}

	//虹があったかどうか
	bool update(HashTable<P2BodyID, std::unique_ptr<Garbage>>& bodies, Effect& effect,GameData& data) {

		bool rainbowFlg = false;

		const RectF hitBox = upgrade ?
			RectF{ 800 - 30,0,150 + 30,Scene::Height() - 400 } : RectF{ 800,0,150,Scene::Height() - 400 };

		switch (mode)
		{
		case 0://待機
			if (KeyDown.down()) {
				mode = 1;
				AudioAsset(U"Excavator").play(MixBus1);
			}
			break;
		case 1://下がる
			if (not KeyDown.pressed()) {
				mode = 2;
				AudioAsset(U"Excavator").stop(0.2s);
			}
			else {
				pos.y += Scene::DeltaTime() * 200;
				pos.y = Min(pos.y, 550.0);
			}
			break;
		case 2://回る
			if (10_deg <= angle) {
				mode = 3;
				AudioAsset(U"Excavator").play(MixBus1);
			}
			else {
				angle += Scene::DeltaTime() * 120_deg;
			}
			break;
		case 3://上がる
			//上がり切ったら
			if (pos.y <= -400 + 700-100) {
				AudioAsset(U"Excavator").stop(0.2s);

				mode = 4;
				time = 0;

				bool hasBomb = false;
				for (auto it = bodies.begin(); it != bodies.end(); ++it)
				{
					if (hitBox.intersects(it->second->body.getPos())) {

						if (it->second->getType() == GarbageType::Bomb) {
							hasBomb = true;
							effect.add<ExplosionEffect>(it->second->getPos());
						}
					}
				}
				if (hasBomb) {
					for (auto it = bodies.begin(); it != bodies.end();)
					{
						if (hitBox.intersects(it->second->body.getPos())) {
							it = bodies.erase(it);
						}
						else {
							++it;
						}
					}
				}

			}
			else {
				pos.y -= Scene::DeltaTime() * 200;
			}
			break;
		case 4://少し待つ
			if (1 <= time) {
				mode = 5;
				for (auto it = bodies.begin(); it != bodies.end();)
				{
					if (hitBox.intersects(it->second->body.getPos())) {
						holds << std::move(it->second);
						it = bodies.erase(it);
					}
					else {
						++it;
					}
				}
			}
			else {
				time += Scene::DeltaTime();
			}
			break;
		case 5://全体が回る
			if (180_deg <= excavatorAngle) {
				mode = 6;

				int32 n = 0;

				for (auto it = holds.begin(); it != holds.end();++it)
				{
					n++;
					(*it)->catchItem(data);
					if ((*it)->getType() == GarbageType::Rainbow) {
						rainbowFlg = true;
					}
				}
				multi.start(n);
				holds.clear();
			}
			else {
				excavatorAngle += Scene::DeltaTime() * 360_deg;
			}
			break;
		case 6://回る(元の位置へ)
			angle = -30_deg;
			mode = 7;
			break;
		case 7://全体が回る(元の位置へ)
			if (excavatorAngle <= 0_deg) {
				mode = 0;
			}
			else {
				excavatorAngle -= Scene::DeltaTime() * 360_deg;
			}
			break;
		default:
			break;
		}

		hand.setAngle(angle);
		hand.setPos(pos);

		if (multi.update()) {
			AudioAsset(U"GetEffect").playOneShot(MixBus1);
		}

		return rainbowFlg;
	}

	void draw()const {

		RectF{ 1140,385,100,15 }.draw(Palette::Black);

		Transformer2D target{ Mat3x2::Scale(Math::Cos(excavatorAngle),1,{550 + 640,0 + 700}) };

		hand.draw(Palette::Black);

		Vec2 pos2{};
		Vec2 pos3{ 550 + 640 + 50,300+20 };

		if (auto p=Circle{ pos,250 }.intersectsAt(Circle{ pos3,300 })) {
			pos2 = (*p)[0];
		}

		Line{ pos2,pos3 }.draw(LineStyle::RoundCap, 20, Palette::Orange);

		Line{ pos2,hand.getPos() }.draw(LineStyle::RoundCap, 20,Palette::Orange);

		Circle{ pos2,8 }.draw(Palette::Black);

		Circle{ pos3,40 }.draw(Palette::Orange);

		TextureAsset{ U"clane" }.resized(200).drawAt({ 550 + 640,300 });

		//掴んでいるゴミを描画
		for (const auto& body : holds)
		{
			body->draw();
		}
	}
};


class Crow {
public:

	Crow() = default;

	static int32 generateId() {
		static int32 idNum = 0;
		return idNum++;
	}

	Vec2 pos{ -300,-300 };

	P2BodyID target;

	Vec2 velocity{};

	bool hold = false;

	bool hitFlg = false;

	Crow(P2BodyID target) :target{ target } {
	}

	void update(HashTable<P2BodyID, std::unique_ptr<Garbage>>& bodies, Effect& effect) {

		if (not hitFlg && Circle{ pos,80 }.leftClicked()&&not hold) {
			hit(effect);
		}

		if (bodies.contains(target) && not hitFlg) {
			if ((pos - bodies[target]->getPos()).length() < 20) {
				hold = true;
				bodies.erase(target);
			}
			else {
				pos = Math::SmoothDamp(pos, bodies[target]->getPos(), velocity, 0.1, 200);
			}
		}
		else {
			pos = Math::SmoothDamp(pos, { Scene::Width() + 100,-100 }, velocity, 0.1, 200);
		}
	}

	void hit(Effect& effect) {
		hitFlg = true;
		effect.add<DamageEffect>(pos, 1.5);
		AudioAsset(U"CrowDamage").playOneShot(MixBus1);
	}

	void draw()const {
		TextureAsset(U"crowBody").mirrored().resized(80).drawAt(pos);
		{
			Transformer2D _{ Mat3x2::Rotate(30_deg - Periodic::Sine0_1(0.2s) * 60_deg,pos + Vec2{10,-10}) };
			TextureAsset(U"crowWing").mirrored().resized(80).drawAt(pos);
		}

		if (hold) {
			TextureAsset(U"gold").resized(65).drawAt(pos + Vec2{ 0,20 });
		}
	}

	bool isDie()const {
		return Scene::Width() + 50 < pos.x;
	}

};

class Missile {
public:
	Vec2 pos{Scene::Width()+100,Random(Scene::Height())};

	int32 target;

	Vec2 velocity{};

	bool alive = true;

	Trail trail{ 1.0 };

	Missile(int32 target) :target{target} {}

	void update(HashTable<int32,Crow>&crows,Effect& effect) {

		if (crows.contains(target)) {

			pos = Math::SmoothDamp(pos, crows[target].pos, velocity, 0.01, 600);

			if (pos.asCircle(30).intersects(crows[target].pos)) {
				crows[target].hit(effect);
				alive = false;
			}
		}
		else {
			alive = false;
		}

		trail.update();
		trail.add(pos, Palette::Orange, 20);

	}

	void draw(int32 level)const {
		trail.draw(TextureAsset(U"particle"));
		TextureAsset(U"missile{}"_fmt(level)).scaled(0.5).rotated(velocity.getAngle()+90_deg).drawAt(pos);
	}

	bool isDie()const {
		return not alive;
	}
};

class Display {
public:
	double currentValue = 0.0;

	double velocity = 0.0;

	Vec2 pos;

	String textureName;

	Display(const Vec2& pos, int32 value, const String& textureName) :pos{ pos }, currentValue{ static_cast<double>(value) }, textureName{textureName} {

	}

	void update(int32  targetValue) {
		currentValue = Math::SmoothDamp(currentValue, targetValue, velocity, 0.3);
	}

	void draw()const {
		const int32 value = static_cast<int32>(Math::Round(currentValue));
		RectF{ Arg::center=pos,320,60 }.draw(Palette::Black);
		Digital_Display(value, 9, pos, 20, Palette::Red);

		RectF{ Arg::center=pos - Vec2{190,0},60 }.draw(ColorF{0.2});
		TextureAsset(textureName).resized(80).drawAt(pos - Vec2{ 190,0 });
	}

private:
	void Digital_Display(int32 number, int32 digits, const  Vec2& pos, double size, const ColorF& color, bool hide = true)const
	{

		const double d = size / 20*2, w = size / 10*2;
		const Vec2 x{ size / 2 + d,0 }, y{ 0,size / 2 + d };

		//背景の色
		constexpr ColorF back_color = ColorF{ 0.3 };

		//0-9 数字を表示
		//10 何も表示しない
		//11 マイナスを表示
		constexpr bool table[7][12] = {
			{1,0,1,1,0,1,1,1,1,1,0,0},//上
			{1,0,0,0,1,1,1,0,1,1,0,0},//左上
			{1,1,1,1,1,0,0,1,1,1,0,0},//右上
			{0,0,1,1,1,1,1,0,1,1,0,1},//中心
			{1,0,1,0,0,0,1,0,1,0,0,0},//左下
			{1,1,0,1,1,1,1,1,1,1,0,0},//右下
			{1,0,1,1,0,1,1,0,1,1,0,0},//下
		};

		const bool minus = number < 0;
		number = Abs(number);
		const int32 number_digits = (int32)log10(number);

		for (auto i : step(digits)) {

			const double gap = size + size / 2*1.5;
			const Vec2 vec{ pos.x + (digits - 1) * gap / 2 - i * gap,pos.y };//位置の計算
			int32 num = (number / (int32)pow(10, i)) % 10;//i+1桁目の値を取得
			if (hide && number_digits < i && i != 0)num = 10;//hideがtrueの時はいらない0は表示しない
			if (minus && i == number_digits + 1)num = 11;//値がマイナスの時は左に"-"をつける。

			RoundRect{ Arg::center(vec - 2 * y), size, w, d }.draw(table[0][num] ? color : back_color);//上
			RoundRect{ Arg::center(vec - x - y), w, size, d }.draw(table[1][num] ? color : back_color);//左上
			RoundRect{ Arg::center(vec + x - y), w, size, d }.draw(table[2][num] ? color : back_color);//右上
			RoundRect{ Arg::center(vec), size, w, d }.draw(table[3][num] ? color : back_color);//中心
			RoundRect{ Arg::center(vec - x + y), w, size, d }.draw(table[4][num] ? color : back_color);//左下
			RoundRect{ Arg::center(vec + x + y), w, size, d }.draw(table[5][num] ? color : back_color);//右下
			RoundRect{ Arg::center(vec + 2 * y), size, w, d }.draw(table[6][num] ? color : back_color);//下
		}
	}
};

class VentilationFan {
public:

	const Texture fan;

	const double size;

	VentilationFan(double size_ = 100) :size{ size_ }, fan{ 0xf863_icon, static_cast<int32>(size_ * 0.8) } {}

	void draw(const Vec2& pos = { 0,0 })const {

		RectF rect{ Arg::center = pos,size,size };
		rect.rounded(size * 0.05).draw(Palette::White);
		pos.asCircle(size * 0.4).draw(Palette::Black);
		fan.rotated(Scene::Time() * 180_deg).drawAt(pos);
		pos.asCircle(size * 0.1).draw(Palette::Gray);

		for (const auto i : step(1, 9)) {
			rect.top().movedBy(0, size * 0.1 * i).draw(size * 0.03, Palette::Gray);
		}
	}
};

// ゲームシーン
class GameScene : public MyScene
{
public:

	int32 count = 0;


	double accumulatedTime = 0.0;
	P2World world{ 980 };

	const P2Body ground = world.createLine(P2Static, Vec2{ 0, 0 }, Line{ -100, Scene::Height() - 20, Scene::Width(), Scene::Height() - 20 });
	const P2Body wall = world.createLine(P2Static, Vec2{ 0,0 }, Line{ -100,0,-100,Scene::Width() });

	HashTable<P2BodyID,std::unique_ptr<Garbage>>bodies;

	//Array<Crow>crows;

	HashTable<int32, Crow>crowTable;

	Array<Missile>missiles;

	double accum = 0;

	Excavator excavator{ {940, 200},world,getData().upgradeArm};

	Effect effect;

	StoneButton mapButton{ RectF{20,20,200,50},U"マップ" };
	StoneButton explainButton{ RectF{20,90,200,50},U"説明" };

	bool explainFlg=false;

	Display garbageDisplay{ Vec2{550,100},getData().garbage,U"garbage"};
	Display ironDisplay{ Vec2{550,180},getData().iron,U"iron" };
	Display goldDisplay{ Vec2{550,260},getData().gold,U"gold" };

	VentilationFan fan{ 150 };

	Timer rainbowTimer{ 10s };

	GameScene(const InitData& init);

	void update() override;

	void draw() const override;
private:
};
