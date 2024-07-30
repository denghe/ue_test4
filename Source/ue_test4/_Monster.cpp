#include "_Scene.h"

void Monster::Init(Scene* scene_, XY pos_, float radius_, float speed_)
{
	scene = scene_;
	frameIndexMax = scene->sprites_monster03.Num();
	pos = pos_;
	radius = radius_;
	speed = speed_;
	originalPos = pos;
}

bool Monster::Update()
{
	// step animation
	frameIndex += 1.f / 5;
	if (frameIndex >= frameIndexMax)
	{
		frameIndex -= frameIndexMax;
	}

	// check player distance, avoidance
	if (auto& player = scene->player)
	{
		auto d = pos - player->pos;
		auto dxx = d.x * d.x;
		auto dyy = d.y * d.y;
		auto dd = dxx + dyy;
		auto r2 = radius * 3 + player->radius;
		if (dd < r2 * r2)
		{
			if (dd < std::numeric_limits<float>::epsilon()) // avoid SQRT == 0
			{
				auto radians = scene->rnd.Next<float>() * M_PI * 2;
				pos.x += std::cos(radians) * speed;
				pos.y += std::sin(radians) * speed;
			}
			else if (dxx < std::numeric_limits<float>::epsilon()) // avoid same row
			{
				if (scene->rnd.Next<bool>())
				{
					pos.x += speed;
				}
				else
				{
					pos.x -= speed;
				}
			}
			else if (dyy < std::numeric_limits<float>::epsilon()) // avoid same column
			{
				if (scene->rnd.Next<bool>())
				{
					pos.y += speed;
				}
				else
				{
					pos.y -= speed;
				}
			}
			else
			{
				pos += d / std::sqrt(dd) * speed;
			}
			lastPlayerPos = player->pos;
		}
		else if (lastPlayerPos != player->pos)
		{
			if (pos != originalPos)
			{
				d = originalPos - pos;
				dd = d.Mag2();
				if (dd <= speed * speed)
				{
					pos = originalPos;
				}
				else
				{
					pos += d / std::sqrt(dd) * speed;
				}
			}
		}
	}

	return false;
}

void Monster::Draw(FTransform& t)
{
	double x, y, rx, rz, s;
	x = (double)(pos.x - Scene::gridCenter.x);
	y = (double)(pos.y - Scene::gridCenter.y);

	if (!(x < scene->screenMinX || x > scene->screenMaxX || y < scene->screenMinY || y > scene->screenMaxY))
	{
		// auto secs = xx::NowEpochSeconds();
		// scene->Log( xx::ToString( xx::NowEpochSeconds(secs) ));

		rz = 0;
		rx = 0;
		s = radius / unitRadius;

		t.SetLocation({x, y, 0});
		t.SetRotation(UE::Math::TQuat<double>::MakeFromEuler({rx, 0, rz}));
		t.SetScale3D({s, s, s});
		scene->rendererChars->AddInstance(t, scene->sprites_monster03[(int)frameIndex], false, FLinearColor::White);
	}
}
