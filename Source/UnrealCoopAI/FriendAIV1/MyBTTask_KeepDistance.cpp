// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTTask_KeepDistance.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include <UnrealCoopAI/FriendAIV2/FriendAIV2Controller.h>
#include <UnrealCoopAI/Public/EnemyCharacter.h>
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UMyBTTask_KeepDistance::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FriendController = Cast<AFriendAIV1Controller>(OwnerComp.GetAIOwner());
	ACPPFriendParentCharacter* TheFriend = Cast<ACPPFriendParentCharacter>(FriendController->Get_selfActor());

	AEnemyCharacter* TheEnemy = Cast<AEnemyCharacter>(FriendController->Get_blackboard()->GetValueAsObject(FName(TEXT("TargetActor"))));

	if (IsValid(TheEnemy) && UKismetMathLibrary::Vector_Distance(FriendController->GetPawn()->GetActorLocation(), TheEnemy->GetActorLocation()) <= 400 && !FriendController->GetProxy())
	{
		FVector destination = UKismetMathLibrary::GetDirectionUnitVector(TheEnemy->GetActorLocation(), FriendController->GetPawn()->GetActorLocation()) * 450;
		EPathFollowingRequestResult::Type con = FriendController->MoveToLocation(destination, (float)5);

		if (con == EPathFollowingRequestResult::AlreadyAtGoal)
			return EBTNodeResult::Succeeded;
		else GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UMyBTTask_KeepDistance::OnArrivedTarget, 0.2f, true);

	}
	else return EBTNodeResult::Succeeded;


	return EBTNodeResult::InProgress;
}

void UMyBTTask_KeepDistance::OnArrivedTarget()
{
	ACPPFriendParentCharacter* TheFriend = Cast<ACPPFriendParentCharacter>(FriendController->Get_selfActor());

	AEnemyCharacter* TheEnemy = Cast<AEnemyCharacter>(FriendController->Get_blackboard()->GetValueAsObject(FName(TEXT("TargetActor"))));

	if (IsValid(TheEnemy) && UKismetMathLibrary::Vector_Distance(FriendController->GetPawn()->GetActorLocation(), TheEnemy->GetActorLocation()) <= 400 && !FriendController->GetProxy())
	{
		FVector destination = UKismetMathLibrary::GetDirectionUnitVector(TheEnemy->GetActorLocation(), FriendController->GetPawn()->GetActorLocation()) * 450;
		EPathFollowingRequestResult::Type con = FriendController->MoveToLocation(destination, (float)5);

		if (con == EPathFollowingRequestResult::AlreadyAtGoal)
		{
			FinishLatentTask(*FriendController->Get_btComponent(), EBTNodeResult::Succeeded);
			GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		}
		else GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UMyBTTask_KeepDistance::OnArrivedTarget, 0.2f, true);
	}
	else {
		FinishLatentTask(*FriendController->Get_btComponent(), EBTNodeResult::Failed);
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}
}
