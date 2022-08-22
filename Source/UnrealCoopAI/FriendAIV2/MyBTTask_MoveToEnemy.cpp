// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTTask_MoveToEnemy.h"
#include "AIController.h"
#include <UnrealCoopAI/FriendAIV2/FriendAIV2Controller.h>
#include <UnrealCoopAI/Public/EnemyCharacter.h>
#include "BehaviorTree/BlackboardComponent.h"


EBTNodeResult::Type UMyBTTask_MoveToEnemy::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FriendController = Cast<AFriendAIV2Controller>(OwnerComp.GetAIOwner());
	ACPPFriendParentCharacter* TheFriend = Cast<ACPPFriendParentCharacter>(FriendController->Get_selfActor());

	AEnemyCharacter* TheEnemy = Cast<AEnemyCharacter>(FriendController->Get_blackboard()->GetValueAsObject(FName(TEXT("TargetActor"))));
	
	FriendController->ClearFocus(EAIFocusPriority::Gameplay);
	FriendController->SetFocus(TheEnemy, EAIFocusPriority::Gameplay);
	EPathFollowingRequestResult::Type con = FriendController->MoveToActor(TheEnemy, (float)50, true, true, true, NULL, true);

	if (con == EPathFollowingRequestResult::AlreadyAtGoal)
		return EBTNodeResult::Succeeded;
	else GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UMyBTTask_MoveToEnemy::OnArrivedTarget, 0.2f, true);

	return EBTNodeResult::InProgress;
}

void UMyBTTask_MoveToEnemy::OnArrivedTarget()
{
	AEnemyCharacter* TheEnemy = Cast<AEnemyCharacter>(FriendController->Get_blackboard()->GetValueAsObject(FName(TEXT("TargetActor"))));
	EPathFollowingRequestResult::Type con = FriendController->MoveToActor(TheEnemy, (float)50, true, true, true, NULL, true);

	if (con == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		FinishLatentTask(*FriendController->Get_btComponent(), EBTNodeResult::Succeeded);
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}
	else GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UMyBTTask_MoveToEnemy::OnArrivedTarget, 0.2f, true);
}