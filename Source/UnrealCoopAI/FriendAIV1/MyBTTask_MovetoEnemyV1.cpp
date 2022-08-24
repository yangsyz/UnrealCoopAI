// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTTask_MovetoEnemyV1.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include <UnrealCoopAI/FriendAIV2/FriendAIV2Controller.h>
#include <UnrealCoopAI/Public/EnemyCharacter.h>
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UMyBTTask_MovetoEnemyV1::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FriendController = Cast<AFriendAIV1Controller>(OwnerComp.GetAIOwner());
	ACPPFriendParentCharacter* TheFriend = Cast<ACPPFriendParentCharacter>(FriendController->Get_selfActor());

	AEnemyCharacter* TheEnemy = Cast<AEnemyCharacter>(FriendController->Get_blackboard()->GetValueAsObject(FName(TEXT("TargetActor"))));

	TArray<AActor*> foundEnemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyCharacter::StaticClass(), foundEnemies);

	if (foundEnemies.Contains(TheEnemy)) 
	{
		if (IsValid(TheEnemy))
		{
			FriendController->ClearFocus(EAIFocusPriority::Gameplay);
			FriendController->SetFocus(TheEnemy, EAIFocusPriority::Gameplay);

			FriendController->Get_blackboard()->SetValueAsBool(FName(TEXT("HasArrived")), false);
			UAIBlueprintHelperLibrary::CreateMoveToProxyObject(FriendController, nullptr, TheEnemy->GetActorLocation(), TheEnemy, (float)100, false);

			/*EPathFollowingRequestResult::Type con = FriendController->MoveToActor(TheEnemy, (float)70, true, true, true, NULL, true);

			if (con == EPathFollowingRequestResult::AlreadyAtGoal)
				return EBTNodeResult::Succeeded;
			else GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UMyBTTask_MovetoEnemyV1::OnArrivedTarget, 0.2f, true);*/
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UMyBTTask_MovetoEnemyV1::OnArrivedTarget, 0.2f, true);
			return EBTNodeResult::InProgress;
		}
		else return EBTNodeResult::Succeeded;
	}
	else return EBTNodeResult::Succeeded;
	
	

	return EBTNodeResult::InProgress;
}

void UMyBTTask_MovetoEnemyV1::OnArrivedTarget()
{
	AEnemyCharacter* TheEnemy = Cast<AEnemyCharacter>(FriendController->Get_blackboard()->GetValueAsObject(FName(TEXT("TargetActor"))));

	if (IsValid(TheEnemy))
	{
		/*
		EPathFollowingRequestResult::Type con = FriendController->MoveToActor(TheEnemy, (float)50, true, true, true, NULL, true);

		if (con == EPathFollowingRequestResult::AlreadyAtGoal)
		{
			FinishLatentTask(*FriendController->Get_btComponent(), EBTNodeResult::Succeeded);
			GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		}
		else GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UMyBTTask_MovetoEnemyV1::OnArrivedTarget, 0.2f, true);*/

		bool arrived = FriendController->Get_blackboard()->GetValueAsBool(FName(TEXT("HasArrived")));
		if (arrived)
		{
			FinishLatentTask(*FriendController->Get_btComponent(), EBTNodeResult::Succeeded);
			GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		}
		else GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UMyBTTask_MovetoEnemyV1::OnArrivedTarget, 0.2f, true);
	}
	else {
		FinishLatentTask(*FriendController->Get_btComponent(), EBTNodeResult::Succeeded);
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}
}
