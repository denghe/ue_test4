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
	static constexpr float unitRadius{14}; // scale = radius / unitRadius
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
	static constexpr float unitRadius{14}; // scale = radius / unitRadius
	Scene* scene{};
	xx::Weak<Player> owner;

	int lifeEndTime{};
	float frameIndex{}, moveSpeed{}, radians{}, radius{}, speed{};
	XY pos{}, moveInc{};

	void Init(xx::Shared<Player> owner_, float radians_, float radius_, float speed_);
	bool Update();
	int Draw(double& x, double& y, double& rx, double& rz, double& s);
};

/****************************************************************************************************/
/****************************************************************************************************/

struct Monster
{
	static constexpr float unitRadius{14}; // scale = radius / unitRadius
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
	static constexpr int framePerSeconds{60};
	static constexpr float frameDelaySeconds{1.f / framePerSeconds};
	static constexpr int cellSize{32};
	static constexpr int numRows{512};
	static constexpr int numCols{512};
	static constexpr XY gridSize{numCols * cellSize, numRows * cellSize};
	static constexpr XY gridCenter{gridSize / 2};

	// scene states
	float timePool{};
	int time{};
	xx::Rnd rnd;
	xx::SpaceRingDiffuseData srdd;

	// mouse states
	FVector mouseLocation, mouseDirection;
	XY mousePos{}, mouseGridPos{};
	bool mouseIsReady{};

	// input states
	bool playerUsingKeyboard{}, playerMoving{};
	bool playerKBMovingUp{}, playerKBMovingDown{}, playerKBMovingLeft{}, playerKBMovingRight{};
	XY playerMoveValue{}, playerDirection{1, 0};
	void HandlePlayerInput();

	// camera location
	double camX{}, camY{};

	// scene objects
	xx::Shared<Player> player;
	xx::SpaceGrid<Monster> monsters;
	xx::BlockLink<PlayerBullet, xx::BlockLinkVINPT> playerBullets;

	// fill these by pawm before call Init()
	// args for Draw()
	float screenMinX{}, screenMaxX{}, screenMinY{}, screenMaxY{};
	UPaperGroupedSpriteComponent *rendererChars{}, *rendererBullets{}, *rendererEffects{};
	UPaperSprite** papers{};
	int papersCount{};
	float originalZ{};

	// life cycle functions
	void Init();
	void Update(float delta);
	void Draw();

	void Log(std::string_view sv);
};
