// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTTask_RandomMove.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyAIController.h"
#include <UnrealCoopAI/Public/EnemyCharacter.h>
#include <UnrealCoopAI/Public/CPPPlayerCharacter.h>
#include <UnrealCoopAI/Public/CPPFriendParentCharacter.h>
#include "Engine/World.h"
#include "NavigationSystem.h"

EBTNodeResult::Type UMyBTTask_RandomMove::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyAIController* EnemyController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	AEnemyCharacter* TheEnemy = Cast<AEnemyCharacter>(EnemyController->Get_selfActor());

	FNavLocation resultLocation = FNavLocation::FNavLocation();

	UNavigationSystemV1* const Navisystem = UNavigationSystemV1::GetCurrent(GetWorld());

	while(!Navisystem->GetRandomPointInNavigableRadius(TheEnemy->GetActorLocation(), (float)1500.0, resultLocation));

	EnemyController->MoveToLocation(resultLocation.Location, (float)50, true, true, true, false, NULL, true);

	return EBTNodeResult::Succeeded;
}