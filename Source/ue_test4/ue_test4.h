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

struct Cfg
{
	static constexpr int cellSize = 32;
	static constexpr int numRows = 512;
	static constexpr int numCols = 512;
	static constexpr xx::XY gridSize{numCols * cellSize, numRows * cellSize};
	static constexpr xx::XY gridCenter{gridSize / 2};
};

struct Monster
{
	xx::XY pos{};
	float frameIndex{};
	void Init();
	bool Update();
	void Draw(FTransform& t, int& paperIndex);
};
