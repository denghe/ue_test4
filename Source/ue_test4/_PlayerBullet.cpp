#include "_Scene.h"

void PlayerBullet::Init(xx::Shared<Player> owner_, float radians_, float radius_, float speed_)
{
	owner = owner_;
	scene = owner_->scene;
	radians = radians_;
	auto cos = std::cos(radians_);
	auto sin = std::sin(radians_);
	radius = radius_;
	speed = speed_;
	//auto r2 = owner_->radius;// + radius_;
	pos = owner_->pos; // + XY{cos * r2, sin * r2};
	moveInc = {cos * speed_, sin * speed_};

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

	if (!(x < scene->screenMinX || x > scene->screenMaxX || y < scene->screenMinY || y > scene->screenMaxY))
	{
		rz = radians * (180.f / xx::gPI);
		rx = 90;
		s = radius / unitRadius;

		t.SetLocation({x, y, 0});
		t.SetRotation(UE::Math::TQuat<double>::MakeFromEuler({rx, 0, rz}));
		t.SetScale3D({s, s, s});
		scene->rendererBullets->AddInstance(t, scene->sprites_bullets[1], false, FLinearColor::White);
	}
}
