#include "_Scene.h"

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
	player->Init(this, gridCenter, 28);

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
	srdd.Init(std::min(256, numRows / 2), cellSize);

	// create monsters by search data
	for (auto& i : srdd.idxs)
	{
		monsters.EmplaceInit(this, XY{
			                     gridCenter.x + (float)i.x * Monster::cUnitRadius * 2.1,
			                     gridCenter.y + (float)i.y * Monster::cUnitRadius * 2.1
		                     }, Monster::cUnitRadius, 4);
		//if (monsters.Count() >= 50000) break;	// limit for test
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

			// // generate some effect numbers for test
			// for (int i = 0; i < 150; ++i)
			// {
			// 	auto p = player->pos;
			// 	auto x = p.x + rnd.Next<float>(-Cfg::designSize_2.x, Cfg::designSize_2.x);
			// 	auto y = p.y + rnd.Next<float>(-designSize_2.y, Cfg::designSize_2.y);
			// 	auto v = rnd.Next<double>() * std::pow(10., rnd.Next<int>(2, 300 - 10));
			// 	effectNumbers.Emplace().Init(this, x, y, 1, v, rnd.Next<bool>());
			// }
			//Log(xx::ToString("effectNumbers.len = ", effectNumbers.len));

#if 0
			// slowly when too many items?
			monsters.Foreach([this](Monster& o)-> xx::ForeachResult
			{
				if (o.Update()) return xx::ForeachResult::RemoveAndContinue;
				monsters.Update(o);
				return xx::ForeachResult::Continue;
			});
#else
			// faster?
			monsters.ForeachByRange(srdd, player->pos.x, player->pos.y, screenWidth * 2,
			                        [this](Monster& o)-> xx::ForeachResult
			                        {
				                        if (o.Update()) return xx::ForeachResult::RemoveAndContinue;
				                        monsters.Update(o);
				                        return xx::ForeachResult::Continue;
			                        });
#endif
		}

		playerBullets.ForeachLink([](PlayerBullet& o)-> xx::ForeachResult
		{
			if (o.Update()) return xx::ForeachResult::RemoveAndContinue;
			return xx::ForeachResult::Continue;
		});

		for (int i = effectNumbers.len - 1; i >= 0; --i)
		{
			if (effectNumbers[i].Update())
			{
				effectNumbers.SwapRemoveAt(i);
			}
		}

		// ...
	}
}

void Scene::Draw()
{
	// init
	rendererChars->ClearInstances();
	rendererBullets->ClearInstances();
	rendererEffects->ClearInstances();
	if (((time & 2) == 0))
	{
		rendererMinimap->ClearInstances();
	}
	FTransform t;

	// draw player( do not need crop )
	if (player)
	{
		player->Draw(t);
	}

#if 0
	monsters.Foreach([&](Monster& o)-> void
	{
		o.Draw(t);
		o.DrawMini(t);
	});
#else
	monsters.ForeachByRange(srdd, player->pos.x, player->pos.y, screenWidth * 2, [&](Monster& o)-> void
	{
		o.Draw(t);
	});
	monsters.ForeachByRange(srdd, player->pos.x, player->pos.y, 3000, [&](Monster& o)-> void
	{
		o.DrawMini(t);
	});
#endif

	playerBullets.ForeachLink([&](PlayerBullet& o)-> void
	{
		o.Draw(t);
	});

	for (auto& en : effectNumbers)
	{
		en.Draw(t);
	}

	// ...
	//Log(xx::ToString( "rendererMinimap->GetInstanceCount() = ", rendererMinimap->GetInstanceCount() ));
}

void Scene::Log(std::string_view sv)
{
	FString fs(sv.data(), sv.size());
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::White, fs, true, {1.5, 1.5});
}
