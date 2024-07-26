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
	player->Init(this, gridCenter, 28, 10);

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
		monsters.EmplaceInit(this, XY{gridCenter.x + (float)i.x * 16, gridCenter.y + (float)i.y * 16}, 14, 3);
	}
}

void Scene::Update()
{
	HandlePlayerInput();

	timePool += delta;
	while (timePool >= 1.f / 60)
	{
		timePool -= 1.f / 60;
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

		// ...
	}
}

void Scene::Draw(TObjectPtr<UPaperGroupedSpriteComponent> const& sprites,
                 TArray<TObjectPtr<UPaperSprite>> const& papers, float z)
{
	sprites->ClearInstances();
	FTransform t;
	double x{}, y{}, rx{}, rz{}, s{};

	if (player)
	{
		const auto paperIndex = player->Draw(x, y, rx, rz, s);
		t.SetLocation({x, y, z});
		t.SetRotation(UE::Math::TQuat<double>::MakeFromEuler({rx, 0, rz}));
		t.SetScale3D({s, s, s});
		sprites->AddInstance(t, papers[paperIndex], false, FLinearColor::Blue);

		// camera follow player
		camX = x;
		camY = y;
	}

	// calculate screen cut range
	auto minX = camX - 1100;
	auto maxX = camX + 1100;
	auto minY = camY - 1000;
	auto maxY = camY + 300;

	monsters.Foreach([&](Monster& o)-> void
	{
		const auto paperIndex = o.Draw(x, y, rx, rz, s);
		if (paperIndex >= 0)
		{
			if (x < minX || x > maxX || y < minY || y > maxY)
			{
				// do nothing
			}
			else
			{
				t.SetLocation({x, y, z});
				t.SetRotation(UE::Math::TQuat<double>::MakeFromEuler({rx, 0, rz}));
				t.SetScale3D({s, s, s});
				sprites->AddInstance(t, papers[paperIndex], false, FLinearColor::White);
			}
		}
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
		if (frameIndex >= 10)
		{
			frameIndex -= 10;
		}

		// todo: limit move range
	}

	// alive
	return false;
}

int Player::Draw(double& x, double& y, double& rx, double& rz, double& s)
{
	x = (double)(pos.x - Scene::gridCenter.x);
	y = (double)(pos.y - Scene::gridCenter.y);
	if (flipX) rz = 180;
	else rz = 0;
	rx = 0;
	s = radius / unitRadius;
	return (int)frameIndex + 1;
}

/****************************************************************************************************/
/****************************************************************************************************/

void Monster::Init(Scene* scene_, XY pos_, float radius_, float speed_)
{
	scene = scene_;
	pos = pos_;
	radius = radius_;
	speed = speed_;
	originalPos = pos;
}

bool Monster::Update()
{
	// step animation
	frameIndex += 1.f / 5;
	if (frameIndex >= 10)
	{
		frameIndex -= 10;
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

int Monster::Draw(double& x, double& y, double& rz, double& rx, double& s)
{
	x = (double)(pos.x - Scene::gridCenter.x);
	y = (double)(pos.y - Scene::gridCenter.y);
	rz = 0;
	rx = 0;
	s = radius / unitRadius;
	return (int)frameIndex + 1;
}
