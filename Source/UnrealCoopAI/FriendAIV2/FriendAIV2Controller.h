// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include <UnrealCoopAI/Public/CPPFriendParentCharacter.h>
#include "FriendAIV2Controller.generated.h"

/**
 * 
 */
UCLASS()
class UNREALCOOPAI_API AFriendAIV2Controller : public AAIController
{
	GENERATED_BODY()
	
public:
	AFriendAIV2Controller();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
		UBehaviorTree* FriendV2BehaviorTree;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
		UBehaviorTreeComponent* FriendV2BehaviorTreeComponent;

	UPROPERTY(BlueprintReadWrite)
		UAIPerceptionComponent* AIPerception;

	UPROPERTY(BlueprintReadWrite)
		UBlackboardComponent* FriendV2Blackboard;

	UPROPERTY(BlueprintReadWrite)
		ACPPFriendParentCharacter* TheFriendV2;

	UFUNCTION()
		void SenseStuff(AActor* testActors, FAIStimulus stimulus);

	UFUNCTION()
		UBlackboardComponent* Get_blackboard() const;

	UFUNCTION()
		UBehaviorTreeComponent* Get_btComponent() const;

	UFUNCTION()
		ACPPFriendParentCharacter* Get_selfActor() const;

	virtual void OnPossess(APawn* const pawn) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
};
