// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

class UBhaviourTree;

/**
 * 
 */
UCLASS()
class UNREALCOOPAI_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		UBehaviorTree* CPPEnemy_BT;

	virtual void BeginPlay() override;
};
