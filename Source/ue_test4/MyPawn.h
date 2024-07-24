#pragma once

#include "ue_test4.h"
#include "GameFramework/Pawn.h"
#include "MyPawn.generated.h"

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

	//TArray<FTransform> ts;
	float timePool{};
	int time{};
	xx::SpaceGrid<Monster> monsters;
	

	AMyPawn();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
