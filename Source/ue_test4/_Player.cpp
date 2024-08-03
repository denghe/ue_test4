#include "_Scene.h"

void Player::Init(Scene* scene_, XY pos_, float radius_, float speed_)
{
	scene = scene_;
	frameIndexMax = scene->sprites_player.Num();
	pos = pos_;
	radius = radius_;
	speed = speed_;
}

bool Player::Update()
{
	if (scene->playerMoving)
	{
		// move control
		auto& mv = scene->playerMoveValue;
		pos += mv * speed;

		// flip check
		if (mv.x != 0)
		{
			flipX = mv.x < 0;
		}

		// step animation
		frameIndex += 1.f / 5;
		if (frameIndex >= frameIndexMax)
		{
			frameIndex -= frameIndexMax;
		}

		// todo: limit move range
	}

	auto bak = radians;
	auto d = scene->mouseGridPos - pos;
	radians = xx::RotateControl::LerpAngleByFixed(std::atan2(d.y, d.x), radians, frameMaxChangeRadian);
	
	// small shoot
	constexpr float bulletSpeed = 16;
	if (scene->mouseBtn1Pressed)
	{
		scene->playerBullets.Emplace().Init(xx::SharedFromThis(this), radians, 14, bulletSpeed, bulletSpeed);
	}

	// big shoot
	if (scene->mouseBtn2Pressed)
	{
		auto rd = bak - radians;
		if (rd >= xx::gPI)
		{
			rd -= xx::g2PI;
		}
		else if (rd < xx::gNPI)
		{
			rd += xx::g2PI;
		}
		constexpr int shootCount = 100;		// shoot count
		auto radiansStep = rd / shootCount;	// linear angle
		for (int i = 0; i < shootCount; ++i)
		{
			scene->playerBullets.Emplace().Init(xx::SharedFromThis(this), radians + radiansStep * i, 14
			                                    , bulletSpeed, bulletSpeed / shootCount * i);
		}
	}

	// alive
	return false;
}

void Player::Draw(FTransform& t)
{
	double x, y, rx, rz, s;
	x = (double)(pos.x - Scene::gridCenter.x);
	y = (double)(pos.y - Scene::gridCenter.y);
	if (flipX) rz = 180;
	else rz = 0;
	rx = 0;
	s = radius / unitRadius;

	// camera follow player
	scene->camX = x;
	scene->camY = y;

	t.SetLocation({x, y, 0});
	t.SetRotation(UE::Math::TQuat<double>::MakeFromEuler({rx, 0, rz}));
	t.SetScale3D({s, s, s});
	scene->rendererChars->AddInstance(t, scene->sprites_player[(int)frameIndex], false, FLinearColor::White);
}
