// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTTask_MoveToTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "EnemyAIController.h"
#include "Kismet/KismetMathLibrary.h"
#include <UnrealCoopAI/Public/EnemyCharacter.h>
#include <UnrealCoopAI/Public/CPPPlayerCharacter.h>
#include <UnrealCoopAI/Public/CPPFriendParentCharacter.h>
#include <Runtime/Engine/Classes/Kismet/KismetStringLibrary.h>

EBTNodeResult::Type UMyBTTask_MoveToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EnemyController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	AEnemyCharacter* TheEnemy = Cast<AEnemyCharacter>(EnemyController->Get_selfActor());

	UObject* TargetActor = EnemyController->Get_blackboard()->GetValueAsObject(FName(TEXT("TargetActor")));
	AActor* TheActor = Cast<AActor>(TargetActor);

	if (IsValid(TheActor))
	{
		EnemyController->SetFocus(TheActor, EAIFocusPriority::Gameplay);

		//EPathFollowingRequestResult::Type con = EnemyController->MoveToActor(TheActor, (float)50);
		EnemyController->Get_blackboard()->SetValueAsBool(FName(TEXT("HasArrived")), false);
		UAIBlueprintHelperLibrary::CreateMoveToProxyObject(EnemyController, nullptr, TheActor->GetActorLocation(), TheActor, (float)50, true);

		/*if (con == EPathFollowingRequestResult::AlreadyAtGoal) {
			GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
			FinishLatentTask(*EnemyController->Get_btComponent(), EBTNodeResult::Succeeded);
			return EBTNodeResult::Succeeded;
		}
		else {
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UMyBTTask_MoveToTarget::OnArrivedTarget, 0.1f, true);
		}*/
		//FinishLatentTask(*EnemyController->Get_btComponent(), EBTNodeResult::Succeeded);
		//return EBTNodeResult::Succeeded;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UMyBTTask_MoveToTarget::OnArrivedTarget, 0.1f, true);
		return EBTNodeResult::InProgress;
		
	}
	else {
		FinishLatentTask(*EnemyController->Get_btComponent(), EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}
	

	return EBTNodeResult::InProgress;
}

void UMyBTTask_MoveToTarget::OnArrivedTarget()
{
	/*UObject* TargetActor = EnemyController->Get_blackboard()->GetValueAsObject(FName(TEXT("TargetActor")));
	AActor* TheActor = Cast<AActor>(TargetActor);

	if (IsValid(TheActor))
	{
		UAIBlueprintHelperLibrary::CreateMoveToProxyObject(EnemyController, nullptr, TheActor->GetActorLocation());
		//EnemyController->MoveToActor(TheActor, (float)50);

		if (UKismetMathLibrary::Vector_Distance(EnemyController->GetPawn()->GetActorLocation(), TheActor->GetActorLocation()) <= 150)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("arrived")));
			GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
			FinishLatentTask(*EnemyController->Get_btComponent(), EBTNodeResult::Succeeded);
		}
		else
		{
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UMyBTTask_MoveToTarget::OnArrivedTarget, 0.1f, true);
		}
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		FinishLatentTask(*EnemyController->Get_btComponent(), EBTNodeResult::Succeeded);
	}*/
	UObject* TargetActor = EnemyController->Get_blackboard()->GetValueAsObject(FName(TEXT("TargetActor")));
	AActor* TheActor = Cast<AActor>(TargetActor);
	if (IsValid(TheActor)) {
		bool arrived = EnemyController->Get_blackboard()->GetValueAsBool(FName(TEXT("HasArrived")));
		if (arrived)
		{
			FinishLatentTask(*EnemyController->Get_btComponent(), EBTNodeResult::Succeeded);
			GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		}
		else GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UMyBTTask_MoveToTarget::OnArrivedTarget, 0.1f, true);
	}
	else {
		FinishLatentTask(*EnemyController->Get_btComponent(), EBTNodeResult::Succeeded);
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}
}