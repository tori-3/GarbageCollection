#pragma once
#include"Common.hpp"
#include"StoneButton.hpp"

class QuarterView {
public:

	// タイルの一辺の長さ（ピクセル）
	static inline constexpr Vec2 TileOffset{ 50, 25 };

	// タイルの厚み（ピクセル）
	static inline constexpr int32 TileThickness = 15;

	// マップの一辺のタイル数
	static inline constexpr int32 N = 10;


	Vec2 ToTileBottomCenter(const Point& index)const
	{
		const int32 i = index.manhattanLength();
		const int32 xi = (i < (N - 1)) ? 0 : (i - (N - 1));
		const int32 yi = (i < (N - 1)) ? i : (N - 1);
		const int32 k = (index.manhattanDistanceFrom(Point{ xi, yi }) / 2);
		const double posX = ((i < (N - 1)) ? (i * -TileOffset.x) : ((i - 2 * N + 2) * TileOffset.x));
		const double posY = (i * TileOffset.y);
		return{ (posX + TileOffset.x * 2 * k), posY };
	}
	Quad ToTile(const Point& index)const
	{
		const Vec2 bottomCenter = ToTileBottomCenter(index);

		return Quad{
			bottomCenter.movedBy(0, -TileThickness).movedBy(0, -TileOffset.y * 2),
			bottomCenter.movedBy(0, -TileThickness).movedBy(TileOffset.x, -TileOffset.y),
			bottomCenter.movedBy(0, -TileThickness),
			bottomCenter.movedBy(0, -TileThickness).movedBy(-TileOffset.x, -TileOffset.y)
		};
	}
	Quad ToColumnQuad(const int32 x)const
	{
		return{
			ToTileBottomCenter(Point{ x, 0 }).movedBy(0, -TileThickness).movedBy(0, -TileOffset.y * 2),
			ToTileBottomCenter(Point{ x, 0 }).movedBy(0, -TileThickness).movedBy(TileOffset.x, -TileOffset.y),
			ToTileBottomCenter(Point{ x, (N - 1) }).movedBy(0, -TileThickness).movedBy(0, 0),
			ToTileBottomCenter(Point{ x, (N - 1) }).movedBy(0, -TileThickness).movedBy(-TileOffset.x, -TileOffset.y)
		};
	}
	Quad ToRowQuad(const int32 y)const
	{
		return{
			ToTileBottomCenter(Point{ 0, y }).movedBy(0, -TileThickness).movedBy(-TileOffset.x, -TileOffset.y),
			ToTileBottomCenter(Point{ 0, y }).movedBy(0, -TileThickness).movedBy(0, -TileOffset.y * 2),
			ToTileBottomCenter(Point{ (N - 1), y }).movedBy(0, -TileThickness).movedBy(TileOffset.x, -TileOffset.y),
			ToTileBottomCenter(Point{ (N - 1), y }).movedBy(0, -TileThickness).movedBy(0, 0)
		};
	}
	Array<Quad> MakeColumnQuads()const
	{
		Array<Quad> quads;

		for (int32 x = 0; x < N; ++x)
		{
			quads << ToColumnQuad(x);
		}

		return quads;
	}
	Array<Quad> MakeRowQuads()const
	{
		Array<Quad> quads;

		for (int32 y = 0; y < N; ++y)
		{
			quads << ToRowQuad(y);
		}

		return quads;
	}
	Optional<Point> ToIndex(const Vec2& pos)const
	{
		int32 x = -1, y = -1;

		// タイルの列インデックスを調べる
		for (int32 i = 0; i < columnQuads.size(); ++i)
		{
			if (columnQuads[i].intersects(pos))
			{
				x = i;
				break;
			}
		}

		// タイルの行インデックスを調べる
		for (int32 i = 0; i < rowQuads.size(); ++i)
		{
			if (rowQuads[i].intersects(pos))
			{
				y = i;
				break;
			}
		}

		// インデックスが -1 の場合、タイル上にはない
		if ((x == -1) || (y == -1))
		{
			return none;
		}

		return Point{ x, y };
	}

	// 各列の四角形
	const Array<Quad> columnQuads = MakeColumnQuads();

	// 各行の四角形
	const Array<Quad> rowQuads = MakeRowQuads();

	// 各タイルのテクスチャ
	Array<String> textures;
	// タイルの種類
	Grid<int32>& grid;
	//{
	//	{1,1,1,1,1,1,1,1,4,3},
	//	{1,1,1,1,1,1,1,1,4,3},
	//	{1,1,1,1,1,1,1,1,4,3},
	//	{1,1,1,1,1,1,1,1,4,3},
	//	{1,1,1,1,1,1,1,1,4,3},
	//	{2,2,2,2,2,2,2,2,5,2},
	//	{0,0,0,0,0,0,0,7,4,3},
	//	{0,0,0,0,0,0,0,6,4,3},
	//	{0,0,0,0,0,0,0,9,4,3},
	//	{0,0,0,0,0,0,0,10,4,3}
	//};

	QuarterView(GameData& data) :grid{data.grid} {
		textures <<  U"grass" ;//0
		textures <<  U"tree" ;//1
		textures <<  U"" ;//2
		textures << U"" ;//3
		textures << U"offices" ;//4
		textures << U"shop" ;//5
		textures <<  U"museum" ;//6
		textures <<   U"magnet{}"_fmt(data.magnet) ;//7
		textures <<  U"manekineko{}"_fmt(data.cat);//8
		textures << U"rocketLv{}"_fmt(data.missile);//9
		textures << U"research";//10
		textures << U"house1";//11
		textures << U"house2";//12
		textures << U"house3";//13

	}

	void buyingLand() {
		//土地が解放されていたらアスファルトにする。
		for (auto y : step(5)) {
			for (auto x : step(8)) {
				grid[y][x] = 0;
			}
		}
	}

	int32 gridAt(const Point& index)const {
		if (InRange(index.x,0, grid.size().x-1)  && InRange(index.y,0,grid.size().y-1)) {
			return grid[index];
		}
		else {
			return 0;
		}
	}


	void draw()const {
		// 上から順にタイルを描く
		for (int32 i = 0; i < (N * 2 - 1); ++i)
		{
			// x の開始インデックス
			const int32 xi = (i < (N - 1)) ? 0 : (i - (N - 1));

			// y の開始インデックス
			const int32 yi = (i < (N - 1)) ? i : (N - 1);

			// 左から順にタイルを描く
			for (int32 k = 0; k < (N - Abs(N - i - 1)); ++k)
			{
				// タイルのインデックス
				const Point index{ (xi + k), (yi - k) };

				// そのタイルの底辺中央の座標
				const Vec2 pos = ToTileBottomCenter(index);

				if (grid[index] == 2) {
					bool N = gridAt(index + Point{ -1,0 }) == 2;
					bool E = gridAt(index + Point{ 0,-1 }) == 2;
					bool S = gridAt(index + Point{ 1,0 }) == 2;
					bool W = gridAt(index + Point{ 0,1 }) == 2;
					int32 sum = N + E + S + W;
					String name;
					if (sum < 2) {
						if (E || W) {
							name = U"riverEW";
						}
						else {
							name = U"riverNS";
						}
					}
					else if(sum==2){
						name = U"river";
						if (N)name += U"N";
						if (E)name += U"E";
						if (S)name += U"S";
						if (W)name += U"W";
					}
					else {
						name = U"riverEW";
					}
					Size size = TextureAsset(name).size();
					// 底辺中央を基準にタイルを描く
					TextureAsset(name).scaled(100 / double(size.x)).draw(Arg::bottomCenter = pos);
				}
				else if (grid[index] == 3) {
					bool N = gridAt(index + Point{ -1,0 })==3;
					bool E = gridAt(index + Point{ 0,-1 })==3;
					bool S = gridAt(index + Point{ 1,0 })==3;
					bool W = gridAt(index + Point{ 0,1 })==3;
					int32 sum = N + E + S + W;
					String name;
					if (sum < 2) {
						if (E || W) {
							name = U"roadEW";
						}
						else {
							name = U"roadNS";
						}
					}
					else {
						name = U"road";
						if (N)name += U"N";
						if (E)name += U"E";
						if (S)name += U"S";
						if (W)name += U"W";
					}
					Size size = TextureAsset(name).size();
					// 底辺中央を基準にタイルを描く
					TextureAsset(name).scaled(100 / double(size.x)).draw(Arg::bottomCenter = pos);

				}
				else {
					Size size = TextureAsset(textures[grid[index]]).size();
					// 底辺中央を基準にタイルを描く
					TextureAsset(textures[grid[index]]).scaled(100 / double(size.x)).draw(Arg::bottomCenter = pos);
				}
			}
		}
	}

};

class Selecter {
public:

	static constexpr int32 rect_size = 80;

	static constexpr int32 rect_gap = 10;

	static constexpr int32 rect_num = 14;

	Vec2 pos;

	Array<String>texture;

	Array<int32>nums;

	int32 select = 0;

	Selecter(const Vec2& pos, const Array<String>& texture, const Array<int32>& nums)
		:pos{ pos }, texture{ texture }, nums{ nums } {}

	double rect_x(int32 i)const {
		double gap = rect_size + rect_gap;
		return  pos.x - (rect_num - 1) * gap / 2 + i * gap;
	}

	int32 update() {

		for (auto i : step(rect_num)) {
			if (getRect(i).leftClicked()) {
				select = i;
			}
		}

		return select;
	}

	Optional<int32> use() {
		if (nums[select] == 0)return none;
		else {
			nums[select] -= 1;
			return select;
		}
	}

	void release(int32 _select) {
		nums[_select] += 1;
	}

	void draw()const {

		for (auto i : step(rect_num)) {
			getRect(i).draw(Palette::White);
			TextureAsset(texture[i]).resized(rect_size).draw(Arg::bottomCenter=getRect(i).bottomCenter());
			if(nums[i]<100)FontAsset(U"NormalFont")(nums[i]).draw(20,Arg::bottomLeft=getRect(i).bottom().end, Palette::Darkred);
			if (nums[i] == 0)getRect(i).draw(ColorF{ 0,0.7 });
		}


		getRect(select).drawFrame(0, 5, Palette::Red);
	}

private:
	RectF getRect(int32 i)const {
		return RectF{ Arg::center(rect_x(i), pos.y), rect_size };
	}
};

class Cloud {
public:
	Array<Vec2>points;

	Cloud() :points{ PoissonDisk2D{ Scene::Size(), 500}.getPoints() } {
		TextureAsset::Register(U"Cloud", U"☁"_emoji);
	}

	void update() {
		for (auto& point : points)
		{
			point.x -= Scene::DeltaTime() * 30;
			if (point.x <= -100) {
				point.x += Scene::Width() + 200;
				point.y = Random(Scene::Height());
			}
		}
	}

	void draw()const {
		for (const auto& point : points)
		{
			TextureAsset(U"Cloud").drawAt(point, AlphaF(0.5));
		}
	}
};

// ゲームシーン
class MapScene : public MyScene
{
public:

	StoneButton homeButton{ RectF{20,20,200,50},U"ホーム" };

	StoneButton explainButton{ RectF{20,90,200,50},U"説明" };

	StoneButton editButton{ RectF{20,160,200,50},U"編集する" };

	bool explainFlg = false;

	bool edit = false;

	QuarterView view{getData()};

	Cloud cloud;

	Selecter selecter{Vec2{Scene::Center().x,Scene::Height()-50},
		{U"grass",U"tree",U"riverNS",U"roadNS",U"offices",U"shop",U"museum",U"magnet{}"_fmt(Max(1,getData().magnet)),U"manekineko{}"_fmt(Max(1,getData().cat)),U"rocketLv{}"_fmt(Max(1,getData().missile)),U"research",U"house1",U"house2",U"house3"},
		getData().buildingNum()
	};

	Camera2D camera{ Vec2{ 0, 185+50-50 },  Scene::Size().x / double(50 * 10 * 2),CameraControl::None_ };

	MapScene(const InitData& init);

	void update() override;

	void draw() const override;
private:
};
