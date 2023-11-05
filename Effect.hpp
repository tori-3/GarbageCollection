#pragma once
struct DamageEffect : IEffect
{
	Vec2 m_center;

	double m_scale;

	DamageEffect(const Vec2& center, double scale);
	bool update(double t) override;
};

struct ExplosionEffect : IEffect
{
	ParticleSystem2D particleSystem;
	ParticleSystem2DParameters parameters;

	//Vec2 位置　double 大きさ　ColorF 色
	explicit ExplosionEffect(const Vec2& pos, double size = 100, const ColorF& color = HSV{ 20,0.8,1 });

	bool update(double t) override;
};

struct RingWaveEffect : IEffect
{
	Vec2 m_pos;

	explicit RingWaveEffect(const Vec2& pos);

	bool update(double t) override;
};

