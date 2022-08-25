// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTTask_FriendAttackV1.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include <UnrealCoopAI/FriendAIV1/FriendAIV1Controller.h>
#include <UnrealCoopAI/Public/EnemyCharacter.h>
#include <UnrealCoopAI/Public/CPPPlayerCharacter.h>
#include <UnrealCoopAI/Public/CPPFriendParentCharacter.h>


EBTNodeResult::Type UMyBTTask_FriendAttackV1::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FriendController = Cast<AFriendAIV1Controller>(OwnerComp.GetAIOwner());
	ACPPFriendParentCharacter* TheFriend = Cast<ACPPFriendParentCharacter>(FriendController->Get_selfActor());

	if (IsValid(TheFriend))
	{
		TheFriend->PlayAnimMontage(AttackMontage, 1.0, NAME_None);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UMyBTTask_FriendAttackV1::OnAnimationFinished, 1.2f, true);
	}
	else return EBTNodeResult::Succeeded;
	

	return EBTNodeResult::InProgress;
}

void UMyBTTask_FriendAttackV1::OnAnimationFinished()
{
	FinishLatentTask(*FriendController->Get_btComponent(), EBTNodeResult::Succeeded);
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

UMyBTTask_FriendAttackV1::UMyBTTask_FriendAttackV1()
{
	AttackMontage = CreateDefaultSubobject<UAnimMontage>(TEXT("FriendAttackMontage"));
}
