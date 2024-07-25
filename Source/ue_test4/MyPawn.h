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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UPaperSpriteComponent> sprite;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(MakeEditWidget=true))
	TArray<TObjectPtr<UPaperSprite>> papers;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UPaperGroupedSpriteComponent> sprites;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float originalZ{};

	UPROPERTY(EditAnywhere)
	TSubclassOf<UMyUserWidget> hud_t;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputMappingContext* imc;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction* iaKBMoveUp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction* iaKBMoveDown;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction* iaKBMoveLeft;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
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

	TObjectPtr<UMyUserWidget> hud;
	std::unique_ptr<Scene> scene;
	FTransform originalTrans;
	UE::Math::TVector<double> originalLocation; 

	AMyPawn();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
