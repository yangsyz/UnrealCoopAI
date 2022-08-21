// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/BTTaskNode.h"
#include "MyBTTask_Defending.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class UNREALCOOPAI_API UMyBTTask_Defending : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:

	UMyBTTask_Defending();

	UPROPERTY(BlueprintReadWrite)
		bool FirstTime = true;

	UPROPERTY(BlueprintReadWrite)
		bool IsDefending;

	UPROPERTY(BlueprintReadWrite)
		FTimerHandle TimerHandle;

	UPROPERTY(BlueprintReadWrite)
		UBehaviorTreeComponent* DefendTree;

	UPROPERTY(BlueprintReadWrite)
		uint8 DefendNode;

	UFUNCTION()
		void WaitUntil();


	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
