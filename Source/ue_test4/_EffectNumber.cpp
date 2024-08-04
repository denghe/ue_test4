#include "_Scene.h"

void EffectNumber::Init(Scene* scene_, float x_, float y_, float scale_, double v_, bool criticalHit_)
{
	scene = scene_;
	pos.x = x_;
	pos.y = y_;
	scale = scale_;
	criticalHit = criticalHit_;
	endLifeTime = scene->time + cLife;

	// fill sprites by v_
	char buf[12];
	spritesLen = xx::ToStringEN(v_, buf);
	for (int i = 0; i < spritesLen; ++i)
	{
		sprites[i] = scene->sprites_font[buf[i] - 32];
	}
}

bool EffectNumber::Update()
{
	z += cIncZ;
	return endLifeTime < scene->time;;
}

void EffectNumber::Draw(FTransform& t)
{
	double x, y, rx, rz, s;
	x = (double)(pos.x - Cfg::gridCenter.x);
	y = (double)(pos.y - Cfg::gridCenter.y);
	rz = 0;
	rx = -15;
	s = this->scale;
	FLinearColor c;
	if (criticalHit)
	{
		c = FLinearColor::White;
	}
	else
	{
		c = FLinearColor::Red;
	}

	for (int i = 0; i < spritesLen; ++i)
	{
		t.SetLocation({x, y, z});
		t.SetRotation(UE::Math::TQuat<double>::MakeFromEuler({rx, 0, rz}));
		t.SetScale3D({s, s, s});
		scene->rendererEffects->AddInstance(t, sprites[i], false, c);
		x += 8 * s;
	}
}
