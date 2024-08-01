#pragma once
#include "ue_test4.h"

#include "xx_space.h"
#include "xx_math.h"
#include "xx_string.h"
#include "xx_ptr.h"
#include "xx_task.h"
#include "xx_rnd.h"
using XY = xx::XY;

struct Scene;

struct Cfg // Scene : Cfg
{
	static constexpr XY designSize{ 1920, 1080 };
	static constexpr XY designSize_2{ designSize / 2 };
	
	static constexpr int framePerSeconds{60};
	static constexpr float frameDelaySeconds{1.f / framePerSeconds};
	static constexpr int cellSize{32};
	static constexpr int numRows{512};
	static constexpr int numCols{512};
	static constexpr XY gridSize{numCols * cellSize, numRows * cellSize};
	static constexpr XY gridCenter{gridSize / 2};
};

/****************************************************************************************************/
/****************************************************************************************************/

struct Player
{
	static constexpr float unitRadius{14}; // scale = radius / unitRadius
	Scene* scene{};

	XY pos{};
	float frameIndex{}, frameIndexMax{}, speed{}, radius{};
	bool flipX{};

	void Init(Scene* scene_, XY pos_, float radius_, float speed_);
	bool Update();
	void Draw(FTransform& t);
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
	float moveSpeed{}, radians{}, radius{}, speed{};
	XY pos{}, moveInc{};

	void Init(xx::Shared<Player> owner_, float radians_, float radius_, float speed_);
	bool Update();
	void Draw(FTransform& t);
};

/****************************************************************************************************/
/****************************************************************************************************/

struct Monster
{
	static constexpr float unitRadius{12}; // scale = radius / unitRadius
	Scene* scene{};

	float frameIndex{}, frameIndexMax{};
	float radius{};
	float speed{};
	XY pos{}, originalPos{}, lastPlayerPos{};

	void Init(Scene* scene_, XY pos_, float radius_, float speed_);
	bool Update();
	void Draw(FTransform& t);
};

/****************************************************************************************************/
/****************************************************************************************************/

struct EffectNumber
{
	static constexpr float incZ{30.f / Cfg::framePerSeconds};
	static constexpr int life{int(0.8 * Cfg::framePerSeconds)};
	Scene* scene{};

	UPaperSprite* sprites[12];
	int spritesLen{};
	int endLifeTime{};
	XY pos{};
	float z{}, scale{};
	bool criticalHit{};

	void Init(Scene* scene_, float x_, float y_, float scale_, double v_, bool criticalHit_);
	bool Update();
	void Draw(FTransform& t);
};

/****************************************************************************************************/
/****************************************************************************************************/

struct Scene : Cfg
{
	// scene states
	float timePool{};
	int time{};
	xx::Rnd rnd;
	xx::SpaceRingDiffuseData srdd;

	// mouse states
	FVector mouseLocation, mouseDirection;
	XY mousePos{}, mouseGridPos{};
	bool mouseIsReady{};
	bool mouseBtn1Pressed{}, mouseBtn2Pressed{};

	// keyboard & joy input states
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
	xx::Listi32<EffectNumber> effectNumbers;

	// fill these by pawm before call Init()
	// args for Draw()
	float screenWidth{}, screenGradient{}, screenMinY{}, screenMaxY{};
	UPaperGroupedSpriteComponent *rendererChars{}, *rendererBullets{}, *rendererEffects{};
	// sprite mappings( copy from Pawn )
	TArray<UPaperSprite*> sprites_font;
	TArray<UPaperSprite*> sprites_explosions;
	TArray<UPaperSprite*> sprites_player;
	TArray<UPaperSprite*> sprites_bullets;
	TArray<UPaperSprite*> sprites_monster01;
	TArray<UPaperSprite*> sprites_monster02;
	TArray<UPaperSprite*> sprites_monster03;
	TArray<UPaperSprite*> sprites_monster04;
	TArray<UPaperSprite*> sprites_monster05;
	TArray<UPaperSprite*> sprites_monster06;
	TArray<UPaperSprite*> sprites_monster07;
	TArray<UPaperSprite*> sprites_monster08;

	// life cycle functions
	void Init();
	void Update(float delta);
	void Draw();

	void Log(std::string_view sv);
};
