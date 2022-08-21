// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class UNREALCOOPAI_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool FocusOnFriend;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool FocusOnPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool FocusedOnFriend;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DefendingDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsDefending;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool Dead;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Health = 100.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool FoundPlayer;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void DefendEvent();

	void DefendEvent_Implementation();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};