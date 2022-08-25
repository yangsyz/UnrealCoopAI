// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTTask_Defending.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyAIController.h"
#include "ObjectEditorUtils.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include <UnrealCoopAI/Public/EnemyCharacter.h>
#include <UnrealCoopAI/Public/CPPPlayerCharacter.h>
#include <UnrealCoopAI/Public/CPPFriendParentCharacter.h>

EBTNodeResult::Type UMyBTTask_Defending::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EnemyController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	AEnemyCharacter* TheEnemy = Cast<AEnemyCharacter>(EnemyController->Get_selfActor());

	if (IsValid(TheEnemy)) 
	{
		TheEnemy->DefendEvent();

		FBoolProperty* IsDefendingProperty = FindFProperty<FBoolProperty>(TheEnemy->GetClass(), FName(TEXT("IsDefending")));
		IsDefending = IsDefendingProperty->GetPropertyValue_InContainer(TheEnemy);

		if (IsDefending)
		{
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, & UMyBTTask_Defending::WaitUntil, 0.1f, true);
			return EBTNodeResult::InProgress;
			
		}
		else
		{
			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::InProgress;
}

void UMyBTTask_Defending::WaitUntil()
{
	AEnemyCharacter* TheEnemy = Cast<AEnemyCharacter>(EnemyController->Get_selfActor());

	if (IsValid(TheEnemy))
	{
		FBoolProperty* IsDefendingProperty = FindFProperty<FBoolProperty>(TheEnemy->GetClass(), FName(TEXT("IsDefending")));
		IsDefending = IsDefendingProperty->GetPropertyValue_InContainer(TheEnemy);

		if (IsDefending)
		{
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UMyBTTask_Defending::WaitUntil, 0.1f, true);
		}
		else {
			FinishLatentTask(*EnemyController->Get_btComponent(), EBTNodeResult::Succeeded);
			GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		}
	}
	else {
		FinishLatentTask(*EnemyController->Get_btComponent(), EBTNodeResult::Succeeded);
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}
	
}