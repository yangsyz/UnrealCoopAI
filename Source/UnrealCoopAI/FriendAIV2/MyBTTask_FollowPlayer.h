// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include <UnrealCoopAI/FriendAIV2/FriendAIV2Controller.h>
#include "MyBTTask_FollowPlayer.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class UNREALCOOPAI_API UMyBTTask_FollowPlayer : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadWrite)
		FTimerHandle TimerHandle;

	UPROPERTY(BlueprintReadWrite)
		AFriendAIV2Controller* FriendController;

	UFUNCTION()
		void OnArrivedTarget();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
