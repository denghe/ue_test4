#include "MyPawn.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "PaperSpriteComponent.h"
#include "PaperGroupedSpriteComponent.h"

#include <string>

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

	ps = sprite->GetSprite();
	auto t = GetTransform();
	FVector v{};
	ts.Reserve(50000);
	for (int i = 0; i < 50000; ++i)
	{
		v.X = FMath::FRandRange(-100., 100.);
		v.Y = FMath::FRandRange(-100., 100.);
		t.SetTranslation(v);
		ts.Emplace(t);
	}

}

void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	sprites->ClearInstances();
	for(int n = ts.Num(), i = 0; i < n; ++i)
	{
		sprites->AddInstance(ts[i], ps);
	}
	
	// if (auto n = sprites->GetInstanceCount(); n < 50000)
	// {
	// 	auto t = GetTransform();
	// 	const auto s = sprite->GetSprite();
	// 	FVector v{};
	//
	// 	for (int i = 0; i < 100; ++i)
	// 	{
	// 		v.X = FMath::FRandRange(-100., 100.);
	// 		v.Y = FMath::FRandRange(-100., 100.);
	// 		t.SetTranslation(v);
	// 		sprites->AddInstance(t, s);
	// 	}
	// 	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow
	// 	                                 , (std::string("sprites->GetInstanceCount() == ") + std::to_string(n)).
	// 	                                 c_str());
	// } else
	// {
	// 	// todo: change all pos random
	// 	sprites->MarkRenderStateDirty();
	// }
}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
