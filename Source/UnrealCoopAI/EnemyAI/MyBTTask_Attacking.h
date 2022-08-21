// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "MyBTTask_Attacking.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class UNREALCOOPAI_API UMyBTTask_Attacking : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:

	UMyBTTask_Attacking();

	UPROPERTY(EditAnywhere)
		UBehaviorTreeComponent* OwnerTree;

	UPROPERTY(BlueprintReadWrite)
		FTimerHandle TimerHandle;

	UFUNCTION(BlueprintCallable)
		void PlayAnimation();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
			UAnimMontage* AttackMontage;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
