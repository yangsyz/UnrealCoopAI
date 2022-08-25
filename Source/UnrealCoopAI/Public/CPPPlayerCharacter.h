// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <UnrealCoopAI/Public/EnemyCharacter.h>
#include "CPPPlayerCharacter.generated.h"


UCLASS()
class UNREALCOOPAI_API ACPPPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACPPPlayerCharacter();
	
	UPROPERTY(BlueprintReadWrite, Category = ACPPPlayerCharacter)
		bool AttackCombo;

	UPROPERTY(BlueprintReadWrite, Category = ACPPPlayerCharacter)
		bool IsDodging;

	UPROPERTY(BlueprintReadWrite, Category = ACPPPlayerCharacter)
		int AttackCount;

	UPROPERTY(BlueprintReadWrite, Category = ACPPPlayerCharacter)
		bool IsJumping;

	UPROPERTY(BlueprintReadWrite, Category = ACPPPlayerCharacter)
		bool IsAttacking;

	UPROPERTY(BlueprintReadWrite, Category = ACPPPlayerCharacter)
		bool IsDefending;

	UPROPERTY(BlueprintReadWrite, Category = ACPPPlayerCharacter)
		float Health = 100.0;

	UPROPERTY(BlueprintReadWrite, Category = ACPPPlayerCharacter)
		bool Death;

	UPROPERTY(BlueprintReadWrite, Category = ACPPPlayerCharacter)
		int PlayWhichDodgeAnimation;

	UPROPERTY(BlueprintReadWrite, Category = ACPPPlayerCharacter)
		bool CanDodge;

	UPROPERTY(BlueprintReadWrite, Category = ACPPPlayerCharacter)
		bool DodgeColdDown;

	UPROPERTY(BlueprintReadWrite, Category = ACPPPlayerCharacter)
		bool InMenu;

	UPROPERTY(BlueprintReadWrite, Category = ACPPPlayerCharacter)
		bool InFight;

	UPROPERTY(BlueprintReadWrite, Category = ACPPPlayerCharacter)
		bool CameraLock;

	UPROPERTY(BlueprintReadWrite, Category = ACPPPlayerCharacter)
		APawn* LockedEnemy;

	UPROPERTY(BlueprintReadWrite, Category = ACPPPlayerCharacter)
		float DistanceToLockedEnemy;

	UPROPERTY(BlueprintReadWrite, Category = ACPPPlayerCharacter)
		float FlashColdDown;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
