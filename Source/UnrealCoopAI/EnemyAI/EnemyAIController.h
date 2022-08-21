// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include <UnrealCoopAI/Public/EnemyCharacter.h>
#include "EnemyAIController.generated.h"

class UBhaviourTree;

/**
 * 
 */
UCLASS()
class UNREALCOOPAI_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
	

public:

	AEnemyAIController();


	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "AI")
		UBehaviorTree* EnemyBehaviorTree;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
		UBehaviorTreeComponent* EnemyBehaviorTreeComponent;

	UPROPERTY(BlueprintReadWrite)
		UObject* PlayerActor;

	UPROPERTY(BlueprintReadWrite)
		UObject* FriendActor;

	UPROPERTY(BlueprintReadWrite)
		AEnemyCharacter* TheEnemy;

	UPROPERTY(BlueprintReadWrite)
		UAIPerceptionComponent* AIPerception;

	UPROPERTY(BlueprintReadWrite)
		UBlackboardComponent* EnemyBlackboard;

	UFUNCTION()
		void SenseStuff(AActor* testActors, FAIStimulus stimulus);

	UFUNCTION()
		UBlackboardComponent* Get_blackboard() const;

	UFUNCTION()
		UBehaviorTreeComponent* Get_btComponent() const;

	UFUNCTION()
		AEnemyCharacter* Get_selfActor() const;

	virtual void OnPossess(APawn* const pawn) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
};
