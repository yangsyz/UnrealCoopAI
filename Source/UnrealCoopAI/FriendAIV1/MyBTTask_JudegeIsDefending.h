// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include <UnrealCoopAI/FriendAIV1/FriendAIV1Controller.h>
#include "MyBTTask_JudegeIsDefending.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class UNREALCOOPAI_API UMyBTTask_JudegeIsDefending : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadWrite)
		AFriendAIV1Controller* FriendController;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
