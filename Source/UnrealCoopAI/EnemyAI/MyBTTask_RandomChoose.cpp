// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTTask_RandomChoose.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyAIController.h"
#include "Kismet/KismetMathLibrary.h"
#include <UnrealCoopAI/Public/EnemyCharacter.h>
#include <UnrealCoopAI/Public/CPPPlayerCharacter.h>
#include <UnrealCoopAI/Public/CPPFriendParentCharacter.h>

EBTNodeResult::Type UMyBTTask_RandomChoose::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyAIController* EnemyController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	AEnemyCharacter* TheEnemy = Cast<AEnemyCharacter>(EnemyController->Get_selfActor());

	ACPPPlayerCharacter* ThePlayer = Cast<ACPPPlayerCharacter>(EnemyController->Get_blackboard()->GetValueAsObject(FName(TEXT("TargetActor"))));
	int n = UKismetMathLibrary::RandomIntegerInRange(0, 5);
	
	if (ThePlayer) 
	{
		EnemyController->Get_blackboard()->SetValueAsBool(FName(TEXT("RandomChoose")), n <= 3);
	}
	else
	{
		EnemyController->Get_blackboard()->SetValueAsBool(FName(TEXT("RandomChoose")), true);
	}

	return EBTNodeResult::Succeeded;
}