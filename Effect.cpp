#include "stdafx.h"
#include "Effect.hpp"

DamageEffect::DamageEffect(const Vec2& center, double scale)
	: m_center{ center }
	, m_scale{ scale } {
	TextureAsset::Register(U"DamageEffect", U"💥"_emoji);
}

bool DamageEffect::update(double t)
{
	const double scale = (m_scale * (t - 0.5));
	TextureAsset(U"DamageEffect").scaled(scale).drawAt(m_center);
	return (t < 0.5);
}

//Vec2 位置　double 大きさ　ColorF 色
ExplosionEffect::ExplosionEffect(const Vec2& pos, double size, const ColorF& color) {
	AudioAsset::Register(U"Explosion",GMInstrument::Gunshot, PianoKey::C2, 0.5s);

	CircleEmitter2D emitter;
	emitter.r = size * 0.35;
	parameters.startSize = size;
	parameters.startSpeed = size;
	parameters.startColor = color;
	particleSystem.setEmitter(emitter);
	particleSystem.setTexture(TextureAsset(U"particle"));
	particleSystem.setParameters(parameters);
	particleSystem.setPosition(pos);
	particleSystem.prewarm();
	AudioAsset(U"Explosion").playOneShot(MixBus1);
}

bool ExplosionEffect::update(double t)
{
	parameters.rate = t < 0.2 ? 100 : 0;
	particleSystem.setParameters(parameters);
	particleSystem.update();
	particleSystem.draw();
	return (t < 1.0);
}


RingWaveEffect::RingWaveEffect(const Vec2& pos)
	: m_pos{ pos } {}

bool RingWaveEffect::update(double t)
{
	// イージング
	const double e1 = EaseOutExpo(Min(t / 2, 1.0));

	Circle{ m_pos, (e1 * 50) }.drawFrame((10.0 * (1.0 - e1)), ColorF{ 1,0.7 });

	if (0.2 < t) {
		const double e2 = EaseOutExpo((t - 0.2) / 2);
		Circle{ m_pos, (e2 * 50) }.drawFrame((10.0 * (1.0 - e2)), ColorF{ 1,0.7 });
	}

	return (t < 1.0);
}
