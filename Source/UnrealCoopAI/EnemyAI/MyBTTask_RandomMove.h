// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/BTTaskNode.h"
#include "NavigationSystem.h"
#include "MyBTTask_RandomMove.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class UNREALCOOPAI_API UMyBTTask_RandomMove : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:


	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
