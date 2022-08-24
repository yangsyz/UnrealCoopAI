// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTTask_Attacking.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyAIController.h"
#include "Kismet/KismetSystemLibrary.h"
#include <UnrealCoopAI/Public/EnemyCharacter.h>
#include <UnrealCoopAI/Public/CPPPlayerCharacter.h>
#include <UnrealCoopAI/Public/CPPFriendParentCharacter.h>

EBTNodeResult::Type UMyBTTask_Attacking::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("attacking")));
	EnemyController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	AEnemyCharacter* TheEnemy = Cast<AEnemyCharacter>(EnemyController->Get_selfActor());

	if (IsValid(TheEnemy))
	{
		FBoolProperty* DeadProperty = FindFProperty<FBoolProperty>(TheEnemy->GetClass(), FName(TEXT("Dead")));
		bool DeadValue = DeadProperty->GetPropertyValue_InContainer(TheEnemy);

		if (DeadValue) {
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, & UMyBTTask_Attacking::PlayAnimation, 2.5f, true);
			return EBTNodeResult::InProgress;
		}
		else
		{
			TheEnemy->PlayAnimMontage(AttackMontage, 1.0, NAME_None);
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UMyBTTask_Attacking::PlayAnimation, 1.5f, true);
			return EBTNodeResult::InProgress;
		}
	}
	
	return EBTNodeResult::InProgress;
}

UMyBTTask_Attacking::UMyBTTask_Attacking()
{
	AttackMontage = CreateDefaultSubobject<UAnimMontage>(TEXT("AttackMontage"));
}

void UMyBTTask_Attacking::PlayAnimation()
{
	FinishLatentTask(*EnemyController->Get_btComponent() , EBTNodeResult::Succeeded);
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}