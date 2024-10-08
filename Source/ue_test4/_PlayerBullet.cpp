#include "_Scene.h"

void PlayerBullet::Init(xx::Shared<Player> owner_, float radians_, float radius_, float distance_)
{
	owner = owner_;
	scene = owner_->scene;
	radians = radians_;
	auto cos = std::cos(radians_);
	auto sin = std::sin(radians_);
	radius = radius_;
	speed = cSpeed;
	pos = owner_->pos + XY{cos, sin} * distance_;
	moveInc = {cos * speed, sin * speed};

	lifeEndTime = scene->time + Scene::framePerSeconds * 3;
}

bool PlayerBullet::Update()
{
	if (lifeEndTime < scene->time) return true;

	// hit check
	auto m = scene->monsters.FindFirstCrossBy9(pos.x, pos.y, radius);
	if (m)
	{
		// todo: effect?
		auto v = scene->rnd.Next<double>() * std::pow(10., scene->rnd.Next<int>(2, 300 - 10));
		scene->effectNumbers.Emplace().Init(scene, m->pos.x, m->pos.y, 1, v, scene->rnd.Next<bool>());
		
		scene->monsters.Remove(*m);
		return true;
	}

	pos += moveInc;
	// todo: out of range check

	return false;
}

void PlayerBullet::Draw(FTransform& t)
{
	double x, y, rx, rz, s;
	x = (double)(pos.x - Scene::gridCenter.x);
	y = (double)(pos.y - Scene::gridCenter.y);

	if (!(y < scene->screenMinY || y > scene->screenMaxY) && scene->screenWidth - std::abs(x - scene->camX) > (y - scene->screenMinY) * scene->screenGradient)	{
		rz = radians * (180.f / xx::gPI);
		rx = 90;
		s = radius / cUnitRadius;

		t.SetLocation({x, y, 0});
		t.SetRotation(UE::Math::TQuat<double>::MakeFromEuler({rx, 0, rz}));
		t.SetScale3D({s, s, s});
		scene->rendererBullets->AddInstance(t, scene->sprites_bullets[1], false, FLinearColor::White);
	}
}
