// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include <UnrealCoopAI/FriendAIV2/FriendAIV2Controller.h>
#include "MyBTTask_FriendAttack.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class UNREALCOOPAI_API UMyBTTask_FriendAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:

	UMyBTTask_FriendAttack();

	UPROPERTY(BlueprintReadWrite)
		FTimerHandle TimerHandle;

	UPROPERTY(BlueprintReadWrite)
		AFriendAIV2Controller* FriendController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		UAnimMontage* AttackMontage;

	UFUNCTION()
		void OnAnimationFinished();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
