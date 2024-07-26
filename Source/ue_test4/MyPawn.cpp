#include "MyPawn.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "PaperSpriteComponent.h"
#include "PaperGroupedSpriteComponent.h"
#include "MyUserWidget.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/Controller.h"
#include "GameFramework/GameUserSettings.h"

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

	// set window mode 1920x1080
	auto us = GEngine->GetGameUserSettings();
	us->SetScreenResolution({1920, 1080});
	us->SetFullscreenMode(EWindowMode::Type::Windowed);
	us->ApplyResolutionSettings(false);
	// if(us->SupportsHDRDisplayOutput())
	// {
	// 	us->EnableHDRDisplayOutput(true);
	// }
}

void AMyPawn::BeginPlay()
{
	Super::BeginPlay();

	// map input 
	const auto pc = Cast<APlayerController>(Controller);
	check(pc);
	const auto ei = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
	check(ei);
	ei->AddMappingContext(imc, 0);

	// init UI
	check(hud_t);
	hud = CreateWidget<UMyUserWidget>(GetGameInstance(), hud_t);
	check(hud);
	hud->AddToViewport();

	// init scene
	scene = std::make_unique<Scene>();
	scene->Init(); // todo
	originalTrans = GetTransform();
	originalLocation = originalTrans.GetLocation();
	sprite->SetVisibility(false);
}

void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	hud->SetFps(1.f / DeltaTime);
	scene->delta = DeltaTime;
	scene->Update();
	scene->Draw(sprites, papers, originalZ);

	// camera follow player
	originalTrans.SetLocation({
		originalLocation.X - scene->camX, originalLocation.Y - scene->camY, originalZ
	});
	sprites->SetRelativeTransform(originalTrans);
	SetActorLocation({scene->camX, scene->camY, originalZ});
}

void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (const auto eic = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		eic->BindAction(iaKBMoveUp, ETriggerEvent::Started, this, &AMyPawn::InputHandle_KBMoveUpBegin);
		eic->BindAction(iaKBMoveUp, ETriggerEvent::Canceled, this, &AMyPawn::InputHandle_KBMoveUpEnd);
		eic->BindAction(iaKBMoveUp, ETriggerEvent::Completed, this, &AMyPawn::InputHandle_KBMoveUpEnd);

		eic->BindAction(iaKBMoveDown, ETriggerEvent::Started, this, &AMyPawn::InputHandle_KBMoveDownBegin);
		eic->BindAction(iaKBMoveDown, ETriggerEvent::Canceled, this, &AMyPawn::InputHandle_KBMoveDownEnd);
		eic->BindAction(iaKBMoveDown, ETriggerEvent::Completed, this, &AMyPawn::InputHandle_KBMoveDownEnd);

		eic->BindAction(iaKBMoveLeft, ETriggerEvent::Started, this, &AMyPawn::InputHandle_KBMoveLeftBegin);
		eic->BindAction(iaKBMoveLeft, ETriggerEvent::Canceled, this, &AMyPawn::InputHandle_KBMoveLeftEnd);
		eic->BindAction(iaKBMoveLeft, ETriggerEvent::Completed, this, &AMyPawn::InputHandle_KBMoveLeftEnd);

		eic->BindAction(iaKBMoveRight, ETriggerEvent::Started, this, &AMyPawn::InputHandle_KBMoveRightBegin);
		eic->BindAction(iaKBMoveRight, ETriggerEvent::Canceled, this, &AMyPawn::InputHandle_KBMoveRightEnd);
		eic->BindAction(iaKBMoveRight, ETriggerEvent::Completed, this, &AMyPawn::InputHandle_KBMoveRightEnd);
	}
}

void AMyPawn::InputHandle_KBMoveUpBegin(FInputActionValue const& av)
{
	scene->playerUsingKeyboard = true;
	scene->playerKBMovingUp = true;
}

void AMyPawn::InputHandle_KBMoveUpEnd(FInputActionValue const& av)
{
	scene->playerUsingKeyboard = true;
	scene->playerKBMovingUp = false;
}

void AMyPawn::InputHandle_KBMoveDownBegin(FInputActionValue const& av)
{
	scene->playerUsingKeyboard = true;
	scene->playerKBMovingDown = true;
}

void AMyPawn::InputHandle_KBMoveDownEnd(FInputActionValue const& av)
{
	scene->playerUsingKeyboard = true;
	scene->playerKBMovingDown = false;
}

void AMyPawn::InputHandle_KBMoveLeftBegin(FInputActionValue const& av)
{
	scene->playerUsingKeyboard = true;
	scene->playerKBMovingLeft = true;
}

void AMyPawn::InputHandle_KBMoveLeftEnd(FInputActionValue const& av)
{
	scene->playerUsingKeyboard = true;
	scene->playerKBMovingLeft = false;
}

void AMyPawn::InputHandle_KBMoveRightBegin(FInputActionValue const& av)
{
	scene->playerUsingKeyboard = true;
	scene->playerKBMovingRight = true;
}

void AMyPawn::InputHandle_KBMoveRightEnd(FInputActionValue const& av)
{
	scene->playerUsingKeyboard = true;
	scene->playerKBMovingRight = false;
}

void AMyPawn::InputHandle_GPMoveBegin(FInputActionValue const& av)
{
	scene->playerUsingKeyboard = false;
	scene->playerMoving = true;
	const auto v = av.Get<FVector2d>();
	scene->playerMoveValue.x = v.X;
	scene->playerMoveValue.y = v.Y;
};

void AMyPawn::InputHandle_GPMoveEnd(FInputActionValue const& av)
{
	scene->playerUsingKeyboard = false;
	scene->playerMoving = false;
	scene->playerMoveValue = {};
};
