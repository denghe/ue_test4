#include "ue_test4.h"
#include "Modules/ModuleManager.h"
#include "PaperGroupedSpriteComponent.h"

IMPLEMENT_PRIMARY_GAME_MODULE(FDefaultGameModuleImpl, ue_test4, "ue_test4");

void Scene::HandlePlayerInput()
{
	if (playerUsingKeyboard)
	{
		if (!playerKBMovingUp && !playerKBMovingDown && !playerKBMovingLeft && !playerKBMovingRight
			|| playerKBMovingUp && playerKBMovingDown && playerKBMovingLeft && playerKBMovingRight)
		{
			playerMoveValue.x = 0.f;
			playerMoveValue.y = 0.f;
			playerMoving = false;
		}
		else if (!playerKBMovingUp && playerKBMovingDown && playerKBMovingLeft && playerKBMovingRight)
		{
			playerMoveValue.x = 0.f;
			playerMoveValue.y = 1.f;
			playerMoving = true;
		}
		else if (playerKBMovingUp && !playerKBMovingDown && playerKBMovingLeft && playerKBMovingRight)
		{
			playerMoveValue.x = 0.f;
			playerMoveValue.y = -1.f;
			playerMoving = true;
		}
		else if (playerKBMovingUp && playerKBMovingDown && !playerKBMovingLeft && playerKBMovingRight)
		{
			playerMoveValue.x = 1.f;
			playerMoveValue.y = 0.f;
			playerMoving = true;
		}
		else if (playerKBMovingUp && playerKBMovingDown && playerKBMovingLeft && !playerKBMovingRight)
		{
			playerMoveValue.x = -1.f;
			playerMoveValue.y = 0.f;
			playerMoving = true;
		}
		else if (playerKBMovingUp && playerKBMovingDown
			|| playerKBMovingLeft && playerKBMovingRight)
		{
			playerMoveValue.x = 0.f;
			playerMoveValue.y = 0.f;
			playerMoving = false;
		}
		else if (playerKBMovingUp && playerKBMovingLeft)
		{
			playerMoveValue.x = -xx::gSQRT2_1;
			playerMoveValue.y = -xx::gSQRT2_1;
			playerMoving = true;
		}
		else if (playerKBMovingUp && playerKBMovingRight)
		{
			playerMoveValue.x = xx::gSQRT2_1;
			playerMoveValue.y = -xx::gSQRT2_1;
			playerMoving = true;
		}
		else if (playerKBMovingDown && playerKBMovingLeft)
		{
			playerMoveValue.x = -xx::gSQRT2_1;
			playerMoveValue.y = xx::gSQRT2_1;
			playerMoving = true;
		}
		else if (playerKBMovingDown && playerKBMovingRight)
		{
			playerMoveValue.x = xx::gSQRT2_1;
			playerMoveValue.y = xx::gSQRT2_1;
			playerMoving = true;
		}
		else if (playerKBMovingUp)
		{
			playerMoveValue.x = 0;
			playerMoveValue.y = -1;
			playerMoving = true;
		}
		else if (playerKBMovingDown)
		{
			playerMoveValue.x = 0;
			playerMoveValue.y = 1;
			playerMoving = true;
		}
		else if (playerKBMovingLeft)
		{
			playerMoveValue.x = -1;
			playerMoveValue.y = 0;
			playerMoving = true;
		}
		else if (playerKBMovingRight)
		{
			playerMoveValue.x = 1;
			playerMoveValue.y = 0;
			playerMoving = true;
		}
		// if (playerMoving)
		// {
		// 	auto s = xx::ToString(playerKBMovingUp, " ", playerKBMovingDown, " ", playerKBMovingLeft, " ",
		// 	                      playerKBMovingRight, " ", playerMoveValue);
		// 	Log(s);
		// }
	}
	if (playerMoving)
	{
		playerDirection = playerMoveValue;
	}
}


void Scene::Init()
{
	// create player & init
	player = xx::MakeShared<Player>();
	player->Init(this, gridCenter, 28, 3);

	// space index init
	monsters.Init(Scene::numRows, Scene::numCols, Scene::cellSize);

	// // create monsters
	// for (int i = 0; i < 100000; ++i)
	// {
	// 	monsters.EmplaceInit(this, XY{
	// 		     Scene::gridCenter.x + FMath::FRandRange(-2000., 2000.),
	// 		     Scene::gridCenter.y + FMath::FRandRange(-2000., 2000.)
	// 	}, 140);
	// }

	// space index search data init
	srdd.Init(numRows / 2, cellSize);

	// create monsters by search data
	for (auto& i : srdd.idxs)
	{
		monsters.EmplaceInit(this, XY{gridCenter.x + (float)i.x * 14, gridCenter.y + (float)i.y * 14}, 14, 2);
		//if (monsters.Count() >= 1000) break;	// limit for test
	}
}

void Scene::Update(float delta)
{
	HandlePlayerInput();

	timePool += delta;
	while (timePool >= frameDelaySeconds)
	{
		timePool -= frameDelaySeconds;
		++time;

		if (player)
		{
			if (player->Update())
			{
				player.Reset();
			}
		}

		monsters.Foreach([this](Monster& o)-> xx::ForeachResult
		{
			if (o.Update()) return xx::ForeachResult::RemoveAndContinue;
			monsters.Update(o);
			return xx::ForeachResult::Continue;
		});

		playerBullets.ForeachLink([](PlayerBullet& o)-> xx::ForeachResult
		{
			if (o.Update()) return xx::ForeachResult::RemoveAndContinue;
			return xx::ForeachResult::Continue;
		});
		// ...
	}
}

void Scene::Draw()
{
	// init
	rendererChars->ClearInstances();
	rendererBullets->ClearInstances();
	rendererEffects->ClearInstances();
	FTransform t;

	// draw player( do not need crop )
	if (player)
	{
		player->Draw(t);
	}

	// calculate screen crop range
	screenMinX = camX + screenMinX_;
	screenMaxX = camX + screenMaxX_;
	screenMinY = camY + screenMinY_;
	screenMaxY = camY + screenMaxY_;

	monsters.Foreach([&](Monster& o)-> void
	{
		o.Draw(t);
	});

	playerBullets.ForeachLink([&](PlayerBullet& o)-> void
	{
		o.Draw(t);
	});
}

void Scene::Log(std::string_view sv)
{
	FString fs(sv.data(), sv.size());
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::White, fs);
}

/****************************************************************************************************/
/****************************************************************************************************/

void Player::Init(Scene* scene_, XY pos_, float radius_, float speed_)
{
	scene = scene_;
	frameNum = scene->sprites_player.Num();
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
		if (frameIndex >= frameNum)
		{
			frameIndex -= frameNum;
		}

		// todo: limit move range
	}

	// todo: shoot
	auto d = scene->mouseGridPos - pos;
	auto r = std::atan2(d.y, d.x);
	scene->playerBullets.Emplace().Init(xx::SharedFromThis(this), r, 14, 8);

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

	t.SetLocation({x, y, scene->originalZ});
	t.SetRotation(UE::Math::TQuat<double>::MakeFromEuler({rx, 0, rz}));
	t.SetScale3D({s, s, s});
	scene->rendererChars->AddInstance(t, scene->sprites_player[(int)frameIndex], false, FLinearColor::Blue);
}

/****************************************************************************************************/
/****************************************************************************************************/

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

		t.SetLocation({x, y, scene->originalZ});
		t.SetRotation(UE::Math::TQuat<double>::MakeFromEuler({rx, 0, rz}));
		t.SetScale3D({s, s, s});
		scene->rendererBullets->AddInstance(t, scene->sprites_bullets[1], false, FLinearColor::White);
	}
}

/****************************************************************************************************/
/****************************************************************************************************/

void Monster::Init(Scene* scene_, XY pos_, float radius_, float speed_)
{
	scene = scene_;
	frameIndexMax = scene->sprites_monster01.Num();
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

		t.SetLocation({x, y, scene->originalZ});
		t.SetRotation(UE::Math::TQuat<double>::MakeFromEuler({rx, 0, rz}));
		t.SetScale3D({s, s, s});
		scene->rendererChars->AddInstance(t, scene->sprites_monster01[(int)frameIndex], false, FLinearColor::White);
	}
}
