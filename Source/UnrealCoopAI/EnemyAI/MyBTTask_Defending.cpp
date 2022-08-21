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
	DefendTree = &OwnerComp;
	DefendNode = *NodeMemory;

	AEnemyAIController* EnemyController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	AEnemyCharacter* TheEnemy = Cast<AEnemyCharacter>(EnemyController->Get_selfActor());

	if (IsValid(TheEnemy)) 
	{
		if(FirstTime)
			TheEnemy->DefendEvent();

		FBoolProperty* IsDefendingProperty = FindFProperty<FBoolProperty>(TheEnemy->GetClass(), FName(TEXT("IsDefending")));
		IsDefending = IsDefendingProperty->GetPropertyValue_InContainer(TheEnemy);

		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, UKismetStringLibrary::Conv_BoolToString(IsDefending));
		if (IsDefending)
		{
			FirstTime = false;
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT(" IsDefending ")));
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, & UMyBTTask_Defending::WaitUntil, 0.1f, true);
			return EBTNodeResult::InProgress;
			
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT(" NotDefending ")));
			FirstTime = true;
			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::InProgress;
}

void UMyBTTask_Defending::WaitUntil()
{
	if (IsDefending) {
		UMyBTTask_Defending::ExecuteTask(*DefendTree, &DefendNode);
	}
	else {
		FirstTime = true;
		FinishLatentTask(*DefendTree, EBTNodeResult::Succeeded);
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}
	
}

UMyBTTask_Defending::UMyBTTask_Defending()
{
	DefendTree = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("Defend Tree Component"));
}