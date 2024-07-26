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
#include "xx_rnd.h"
using XY = xx::XY;

struct Scene;

/****************************************************************************************************/
/****************************************************************************************************/

struct Player
{
	static constexpr float unitRadius{14};	// scale = radius / unitRadius
	Scene* scene{};

	XY pos{};
	bool flipX{};
	float frameIndex{}, speed{}, radius{};

	void Init(Scene* scene_, XY pos_, float radius_, float speed_);
	bool Update();
	int Draw(double& x, double& y, double& rx, double& rz, double& s);
};

/****************************************************************************************************/
/****************************************************************************************************/

// todo: aim by mouse & shoot 
struct PlayerBullet
{
	Scene* scene{};
	xx::Weak<Player> owner;

	float frameIndex{}, moveSpeed{}, radius{};
	XY pos{}, moveInc{};

	void Init(xx::Shared<Player> owner_, float radians_, float radius_, float speed_);
	bool Update();
	int Draw(double& x, double& y, double& rx, double& rz);
};

/****************************************************************************************************/
/****************************************************************************************************/

struct Monster
{
	static constexpr float unitRadius{14};	// scale = radius / unitRadius
	Scene* scene{};

	float frameIndex{};
	float radius{};
	float speed{};
	XY pos{}, originalPos{}, lastPlayerPos{};

	void Init(Scene* scene_, XY pos_, float radius_, float speed_);
	bool Update();
	int Draw(double& x, double& y, double& rx, double& rz, double& s);
};

/****************************************************************************************************/
/****************************************************************************************************/

struct Scene
{
	static constexpr int cellSize = 32;
	static constexpr int numRows = 512;
	static constexpr int numCols = 512;
	static constexpr XY gridSize{numCols * cellSize, numRows * cellSize};
	static constexpr XY gridCenter{gridSize / 2};

	float timePool{}, delta{};
	int time{};
	xx::Rnd rnd;

	// input states
	bool playerUsingKeyboard{}, playerMoving{};
	bool playerKBMovingUp{}, playerKBMovingDown{}, playerKBMovingLeft{}, playerKBMovingRight{};
	XY playerMoveValue{}, playerDirection{1, 0};
	void HandlePlayerInput();

	double camX{}, camY{};

	xx::Shared<Player> player;
	xx::SpaceGrid<Monster> monsters;
	xx::SpaceRingDiffuseData srdd;

	void Init();
	void Update();
	void Draw(TObjectPtr<UPaperGroupedSpriteComponent> const& sprites, TArray<TObjectPtr<UPaperSprite>> const& papers,
	          float z);

	void Log(std::string_view sv);
};
