// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/BTTaskNode.h"
#include "MyBTTask_ChooseTarget.generated.h"


/**
 * 
 */
UCLASS(Blueprintable)
class UNREALCOOPAI_API UMyBTTask_ChooseTarget : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float PlayerDistance = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float FriendDistance = 0;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
