// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
class UCapsuleComponent;
class USpringArmComponent;
class UCameraComponent;
class UPaperSprite;
class UPaperSpriteComponent;
class UPaperGroupedSpriteComponent;
/*
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "PaperSpriteComponent.h"
#include "PaperGroupedSpriteComponent.h"
*/

#include "xx_space.h"
#include "xx_math.h"
#include "xx_string.h"
#include "xx_ptr.h"
#include "xx_task.h"
using XY = xx::XY;


struct Scene;
struct Player
{
	Scene* scene{};
	
	XY pos{};
	bool flipX{};
	float frameIndex{};
	float moveSpeed{10};
	float radius{100};
	
	void Init(Scene* scene_);
	bool Update();
	int Draw(double& x, double& y, double& rx, double& rz);
};

struct Monster
{
	Scene* scene{};
	
	float frameIndex{};
	float radius{100}, avoidanceRadius{500};
	float moveSpeed{5};
	XY pos{}, originalPos{}, lastPlayerPos{};
	
	void Init(Scene* scene_);
	bool Update();
	int Draw(double& x, double& y, double& rx, double& rz);
};

struct Scene
{
	static constexpr int cellSize = 32;
	static constexpr int numRows = 512;
	static constexpr int numCols = 512;
	static constexpr XY gridSize{numCols * cellSize, numRows * cellSize};
	static constexpr XY gridCenter{gridSize / 2};

	float timePool{}, delta{};
	int time{};

	// input states
	bool playerUsingKeyboard{}, playerMoving{};
	bool playerKBMovingUp{}, playerKBMovingDown{}, playerKBMovingLeft{}, playerKBMovingRight{};
	XY playerMoveValue{}, playerDirection{1, 0};
	void HandlePlayerInput();

	double camX{}, camY{};

	xx::Shared<Player> player;
	xx::SpaceGrid<Monster> monsters;

	void Init();
	void Update();
	void Draw(TObjectPtr<UPaperGroupedSpriteComponent> const& sprites, TArray<TObjectPtr<UPaperSprite>> const& papers, float z);

	void Log(std::string_view sv);
};
