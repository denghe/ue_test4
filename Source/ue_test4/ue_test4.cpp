// Copyright Epic Games, Inc. All Rights Reserved.

#include "ue_test4.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE(FDefaultGameModuleImpl, ue_test4, "ue_test4");

void Monster::Init()
{
	this->pos = Cfg::gridCenter;
}

bool Monster::Update()
{
	// change position
	pos.x = Cfg::gridCenter.x + FMath::FRandRange(-2000., 2000.);
	pos.y = Cfg::gridCenter.y + FMath::FRandRange(-2000., 2000.);

	// step animation
	frameIndex += 1.f / 5;
	if (frameIndex >= 10)
	{
		frameIndex -= 10;
	}

	// alive
	return false;
}

void Monster::Draw(FTransform& t, int& paperIndex)
{
	// convert pos to camera pos
	t.SetLocation(FVector{(double)(pos.x - Cfg::gridCenter.x), (double)(pos.y - Cfg::gridCenter.y), t.GetLocation().Z});
	paperIndex = (int)frameIndex + 1;
}
