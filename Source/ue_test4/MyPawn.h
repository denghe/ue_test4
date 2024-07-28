#pragma once

#include "ue_test4.h"
#include "GameFramework/Pawn.h"
#include "MyPawn.generated.h"

class UMyUserWidget;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class UE_TEST4_API AMyPawn : public APawn
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UCapsuleComponent> capsule;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<USpringArmComponent> springArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UCameraComponent> camera;

	// editor only
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UPaperSpriteComponent> sprite;

	// sprite binds
	UPROPERTY(EditAnywhere, meta=(MakeEditWidget=true))
	TArray<UPaperSprite*> sprites_numbers;
	UPROPERTY(EditAnywhere, meta=(MakeEditWidget=true))
	TArray<UPaperSprite*> sprites_explosions;
	UPROPERTY(EditAnywhere, meta=(MakeEditWidget=true))
	TArray<UPaperSprite*> sprites_player;
	UPROPERTY(EditAnywhere, meta=(MakeEditWidget=true))
	TArray<UPaperSprite*> sprites_bullets;
	UPROPERTY(EditAnywhere, meta=(MakeEditWidget=true))
	TArray<UPaperSprite*> sprites_monster01;
	UPROPERTY(EditAnywhere, meta=(MakeEditWidget=true))
	TArray<UPaperSprite*> sprites_monster02;
	UPROPERTY(EditAnywhere, meta=(MakeEditWidget=true))
	TArray<UPaperSprite*> sprites_monster03;
	UPROPERTY(EditAnywhere, meta=(MakeEditWidget=true))
	TArray<UPaperSprite*> sprites_monster04;
	UPROPERTY(EditAnywhere, meta=(MakeEditWidget=true))
	TArray<UPaperSprite*> sprites_monster05;
	UPROPERTY(EditAnywhere, meta=(MakeEditWidget=true))
	TArray<UPaperSprite*> sprites_monster06;
	UPROPERTY(EditAnywhere, meta=(MakeEditWidget=true))
	TArray<UPaperSprite*> sprites_monster07;
	UPROPERTY(EditAnywhere, meta=(MakeEditWidget=true))
	TArray<UPaperSprite*> sprites_monster08;


	UPROPERTY(EditAnywhere)
	float originalZ{};

	UPROPERTY(EditAnywhere)
	TSubclassOf<UMyUserWidget> hud_t;

	UPROPERTY(EditAnywhere)
	UInputMappingContext* imc;
	UPROPERTY(EditAnywhere)
	UInputAction* iaKBMoveUp;
	UPROPERTY(EditAnywhere)
	UInputAction* iaKBMoveDown;
	UPROPERTY(EditAnywhere)
	UInputAction* iaKBMoveLeft;
	UPROPERTY(EditAnywhere)
	UInputAction* iaKBMoveRight;

	void InputHandle_KBMoveUpBegin(FInputActionValue const& av);
	void InputHandle_KBMoveUpEnd(FInputActionValue const& av);

	void InputHandle_KBMoveDownBegin(FInputActionValue const& av);
	void InputHandle_KBMoveDownEnd(FInputActionValue const& av);

	void InputHandle_KBMoveLeftBegin(FInputActionValue const& av);
	void InputHandle_KBMoveLeftEnd(FInputActionValue const& av);

	void InputHandle_KBMoveRightBegin(FInputActionValue const& av);
	void InputHandle_KBMoveRightEnd(FInputActionValue const& av);

	void InputHandle_GPMoveBegin(FInputActionValue const& av);
	void InputHandle_GPMoveEnd(FInputActionValue const& av);

	APlayerController* pc{};
	TObjectPtr<UMyUserWidget> hud;
	std::unique_ptr<Scene> scene;

	AMyPawn();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
