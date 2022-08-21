// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/BTTaskNode.h"
#include "MyBTTask_MoveToTarget.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class UNREALCOOPAI_API UMyBTTask_MoveToTarget : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:

	UMyBTTask_MoveToTarget();

	UPROPERTY(BlueprintReadWrite)
		uint8 MoveNode;

	UPROPERTY(BlueprintReadWrite)
		UBehaviorTreeComponent* MoveTree;

	UPROPERTY(BlueprintReadWrite)
		FTimerHandle TimerHandle;

	UFUNCTION()
		void OnArrivedTarget();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
