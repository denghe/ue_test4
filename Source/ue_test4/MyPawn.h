#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MyPawn.generated.h"

class UCapsuleComponent;
class USpringArmComponent;
class UCameraComponent;
class UPaperSprite;
class UPaperSpriteComponent;
class UPaperGroupedSpriteComponent;

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

	// todo: more containers
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UPaperGroupedSpriteComponent> sprites;

	UPaperSprite* ps{};
	TArray<FTransform> ts;

	AMyPawn();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
