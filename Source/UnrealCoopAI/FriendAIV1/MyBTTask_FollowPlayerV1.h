// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include <UnrealCoopAI/FriendAIV1/FriendAIV1Controller.h>
#include "MyBTTask_FollowPlayerV1.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class UNREALCOOPAI_API UMyBTTask_FollowPlayerV1 : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadWrite)
		FTimerHandle TimerHandle;

	UPROPERTY(BlueprintReadWrite)
		AFriendAIV1Controller* FriendController;

	UFUNCTION()
		void OnArrivedTarget();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
