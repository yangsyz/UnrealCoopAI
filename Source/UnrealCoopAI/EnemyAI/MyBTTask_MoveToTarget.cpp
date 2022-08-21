// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTTask_MoveToTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyAIController.h"
#include "Kismet/KismetMathLibrary.h"
#include <UnrealCoopAI/Public/EnemyCharacter.h>
#include <UnrealCoopAI/Public/CPPPlayerCharacter.h>
#include <UnrealCoopAI/Public/CPPFriendParentCharacter.h>

EBTNodeResult::Type UMyBTTask_MoveToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EnemyController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	AEnemyCharacter* TheEnemy = Cast<AEnemyCharacter>(EnemyController->Get_selfActor());

	UObject* TargetActor = EnemyController->Get_blackboard()->GetValueAsObject(FName(TEXT("TargetActor")));
	AActor* TheActor = Cast<AActor>(TargetActor);

	EnemyController->SetFocus(TheActor, EAIFocusPriority::Gameplay);

	EPathFollowingRequestResult::Type con = EnemyController->MoveToActor(TheActor, (float)100, true, true, true, NULL, true);

	if (con == EPathFollowingRequestResult::AlreadyAtGoal) {
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		FinishLatentTask(*EnemyController->Get_btComponent(), EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}
	else {
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UMyBTTask_MoveToTarget::OnArrivedTarget, 0.1f, true);
	}

	return EBTNodeResult::InProgress;
}

void UMyBTTask_MoveToTarget::OnArrivedTarget()
{
	UObject* TargetActor = EnemyController->Get_blackboard()->GetValueAsObject(FName(TEXT("TargetActor")));
	AActor* TheActor = Cast<AActor>(TargetActor);
	EPathFollowingRequestResult::Type con = EnemyController->MoveToActor(TheActor, (float)100, true, true, true, NULL, true);

	if (con == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		FinishLatentTask(*EnemyController->Get_btComponent(), EBTNodeResult::Succeeded);
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UMyBTTask_MoveToTarget::OnArrivedTarget, 0.1f, true);
	}

}