#include "MyPawn.h"
#include "MyUserWidget.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "PaperSpriteComponent.h"
#include "PaperGroupedSpriteComponent.h"
#include "PaperGroupedSpriteActor.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "InputActionValue.h"
#include "Components/SceneCaptureComponent2D.h"
#include "GameFramework/Controller.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/SceneCapture2D.h"

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


	// todo: replace all paper sprites's default material ?
	// /Script/Engine.MaterialInstanceConstant'/Paper2D/MaskedLitSpriteMaterial.MaskedLitSpriteMaterial'
}

void AMyPawn::BeginPlay()
{
	Super::BeginPlay();
	auto w = GetWorld();

	// init display 
	auto us = GEngine->GetGameUserSettings();
	us->SetScreenResolution({1920, 1080});
	us->SetFullscreenMode(EWindowMode::Type::Windowed); // windows for debug break point
	us->ApplySettings(false);
	if (us->SupportsHDRDisplayOutput())
	{
		//us->EnableHDRDisplayOutput(true);	// not working...
		GEngine->Exec(w, TEXT("r.HDR.EnableHDROutput 1")); // ok
	}

	// input init 
	pc = Cast<APlayerController>(Controller);
	check(pc);
	const auto ei = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
	check(ei);
	ei->AddMappingContext(imc, 0);

	// show mouse
	pc->SetShowMouseCursor(true);

	// UI init
	check(hud_t);
	hud = CreateWidget<UMyUserWidget>(GetGameInstance(), hud_t);
	check(hud);
	hud->AddToViewport();

	// do not use this component. only for design
	//sprite->SetVisibility(false);
	sprite->DestroyComponent();
	sprite = {};

	// locate renderers( looks like order by ActorLabel asc )
	TArray<AActor*> renderers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APaperGroupedSpriteActor::StaticClass(), renderers);
	check(renderers.Num() >= 4);
	for (int i = 0, e = renderers.Num(); i < e; ++i)
	{
		auto r = (APaperGroupedSpriteActor*)renderers[i];	// GetActorName for editor only
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::White, r->GetName(), true, {1.5, 1.5});
	}

	// locate scene capture 2d
	TArray<AActor*> sceneCapture2ds;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASceneCapture2D::StaticClass(), sceneCapture2ds);
	check(sceneCapture2ds.Num() >= 1);
	sceneCapture2D = (ASceneCapture2D*)sceneCapture2ds[0];

	// fix editor select actor bug
	// auto& showOnlyActors = sceneCapture2D->GetCaptureComponent2D()->ShowOnlyActors;
	// showOnlyActors.Reset();
	// showOnlyActors.Add(renderers[0]);

	// scene init
	scene = std::make_unique<Scene>();
	scene->rendererMinimap = ((APaperGroupedSpriteActor*)renderers[0])->GetRenderComponent();
	scene->rendererChars = ((APaperGroupedSpriteActor*)renderers[1])->GetRenderComponent();
	scene->rendererBullets = ((APaperGroupedSpriteActor*)renderers[2])->GetRenderComponent();
	scene->rendererEffects = ((APaperGroupedSpriteActor*)renderers[3])->GetRenderComponent();
	scene->sprites_font = sprites_font;
	scene->sprites_explosions = sprites_explosions;
	scene->sprites_player = sprites_player;
	scene->sprites_bullets = sprites_bullets;
	scene->sprites_monster01 = sprites_monster01;
	scene->sprites_monster02 = sprites_monster02;
	scene->sprites_monster03 = sprites_monster03;
	scene->sprites_monster04 = sprites_monster04;
	scene->sprites_monster05 = sprites_monster05;
	scene->sprites_monster06 = sprites_monster06;
	scene->sprites_monster07 = sprites_monster07;
	scene->sprites_monster08 = sprites_monster08;
	scene->sprite_point = sprite_point;
	scene->Init();
}

void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// handle mouse input
	pc->GetMousePosition(scene->mousePos.x, scene->mousePos.y);
	scene->mouseIsReady = pc->DeprojectMousePositionToWorld(scene->mouseLocation, scene->mouseDirection);
	if (scene->mouseIsReady)
	{
		auto p = FMath::RayPlaneIntersection(scene->mouseLocation, scene->mouseDirection,
		                                     FPlane(FVector(0, 0, 0), FVector::UpVector));
		scene->mouseGridPos.x = p.X + scene->gridCenter.x;
		scene->mouseGridPos.y = p.Y + scene->gridCenter.y;
	}

	scene->Update(DeltaTime);

	scene->screenMinY = scene->camY + screenMinY;
	scene->screenMaxY = scene->camY + screenMaxY;
	scene->screenWidth = screenWidth;
	scene->screenGradient = screenGradient;

	scene->miniMinX = scene->camX - 2000;
	scene->miniMaxX = scene->camX + 2000;
	scene->miniMinY = scene->camY - 2000;
	scene->miniMaxY = scene->camY + 2000;
	
	scene->Draw();

	// camera follow player
	SetActorLocation({scene->camX, scene->camY, 0});
	sceneCapture2D->SetActorLocation({scene->camX, scene->camY, 300});

	//scene->Log( xx::ToString(scene->rendererEffects->GetNumMaterials()) );

	++drawCounter;
	auto nowSecs = xx::NowEpochSeconds();
	if (auto elapsedSecs = nowSecs - lastSecs; elapsedSecs >= 1)
	{
		lastSecs = nowSecs;
		hud->SetFps(drawCounter / elapsedSecs);
		drawCounter = 0;

		scene->Log(xx::ToString("num monsters = ", scene->monsters.Count()
		                        , " ( ", scene->rendererChars->GetInstanceCount()
		                        , " displayed ) num bullets =  ", scene->playerBullets.Count()
		                        , " ( ", scene->rendererBullets->GetInstanceCount()
		                        , " displayed ) num numbers =  ", scene->effectNumbers.Count()
		                        , " ( ", scene->rendererEffects->GetInstanceCount()
		                        , " displayed )"
		));
	}
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

		// eic->BindAction(ia????, ETriggerEvent::Started, this, &AMyPawn::InputHandle_GPMoveBegin);
		// eic->BindAction(ia????, ETriggerEvent::Canceled, this, &AMyPawn::InputHandle_GPMoveEnd);
		// eic->BindAction(ia????, ETriggerEvent::Completed, this, &AMyPawn::InputHandle_GPMoveEnd);

		eic->BindAction(iaMBtn1, ETriggerEvent::Started, this, &AMyPawn::InputHandle_MBtn1Begin);
		eic->BindAction(iaMBtn1, ETriggerEvent::Canceled, this, &AMyPawn::InputHandle_MBtn1End);
		eic->BindAction(iaMBtn1, ETriggerEvent::Completed, this, &AMyPawn::InputHandle_MBtn1End);

		eic->BindAction(iaMBtn2, ETriggerEvent::Started, this, &AMyPawn::InputHandle_MBtn2Begin);
		eic->BindAction(iaMBtn2, ETriggerEvent::Canceled, this, &AMyPawn::InputHandle_MBtn2End);
		eic->BindAction(iaMBtn2, ETriggerEvent::Completed, this, &AMyPawn::InputHandle_MBtn2End);
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

void AMyPawn::InputHandle_MBtn1Begin(FInputActionValue const& av)
{
	scene->mouseBtn1Pressed = true;
};

void AMyPawn::InputHandle_MBtn1End(FInputActionValue const& av)
{
	scene->mouseBtn1Pressed = false;
};

void AMyPawn::InputHandle_MBtn2Begin(FInputActionValue const& av)
{
	scene->mouseBtn2Pressed = true;
};

void AMyPawn::InputHandle_MBtn2End(FInputActionValue const& av)
{
	scene->mouseBtn2Pressed = false;
};
