#include "MyPawn.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "PaperSpriteComponent.h"
#include "PaperGroupedSpriteComponent.h"

AMyPawn::AMyPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	capsule = CreateDefaultSubobject<UCapsuleComponent>("capsule");
	SetRootComponent(capsule);

	springArm = CreateDefaultSubobject<USpringArmComponent>("springArm");
	springArm->SetupAttachment(RootComponent);

	camera = CreateDefaultSubobject<UCameraComponent>("camera");
	camera->SetupAttachment(springArm, USpringArmComponent::SocketName);

	sprite = CreateDefaultSubobject<UPaperSpriteComponent>("sprite");
	sprite->SetupAttachment(RootComponent);

	sprites = CreateDefaultSubobject<UPaperGroupedSpriteComponent>("sprites");
	sprites->SetupAttachment(RootComponent);
}

void AMyPawn::BeginPlay()
{
	Super::BeginPlay();

	// create 50000 monsters
	monsters.Init(Cfg::numRows, Cfg::numCols, Cfg::cellSize);
	for (int i = 0; i < 50000; ++i)
	{
		monsters.EmplaceInit();
	}
}

void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// update all monsters
	timePool += DeltaTime;
	while (timePool >= 1.f / 60)
	{
		timePool -= 1.f / 60;

		monsters.Foreach([this](Monster& o)-> xx::ForeachResult
		{
			if (o.Update()) return xx::ForeachResult::RemoveAndContinue;
			monsters.Update(o);
			return xx::ForeachResult::Continue;
		});
	}

	// draw all monsters
	sprites->ClearInstances();
	auto t = GetTransform();
	int paperIndex{};
	monsters.Foreach([&](Monster& o)-> void
	{
		o.Draw(t, paperIndex);
		const auto s = papers[paperIndex];
		sprites->AddInstance(t, s);
	});
}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
