// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include <UnrealCoopAI/FriendAIV1/FriendAIV1Controller.h>
#include "MyBTTask_FriendAttackV1.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class UNREALCOOPAI_API UMyBTTask_FriendAttackV1 : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:

	UMyBTTask_FriendAttackV1();

	UPROPERTY(BlueprintReadWrite)
		FTimerHandle TimerHandle;

	UPROPERTY(BlueprintReadWrite)
		AFriendAIV1Controller* FriendController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		UAnimMontage* AttackMontage;

	UFUNCTION()
		void OnAnimationFinished();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
