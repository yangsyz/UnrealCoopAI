// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTTask_FriendAttack.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include <UnrealCoopAI/Public/EnemyCharacter.h>
#include <UnrealCoopAI/Public/CPPPlayerCharacter.h>
#include <UnrealCoopAI/Public/CPPFriendParentCharacter.h>

EBTNodeResult::Type UMyBTTask_FriendAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FriendController = Cast<AFriendAIV2Controller>(OwnerComp.GetAIOwner());
	ACPPFriendParentCharacter* TheFriend = Cast<ACPPFriendParentCharacter>(FriendController->Get_selfActor());

	TheFriend->PlayAnimMontage(AttackMontage, 1.0, NAME_None);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UMyBTTask_FriendAttack::OnAnimationFinished, 2.0f, true);

	return EBTNodeResult::InProgress;
}

void UMyBTTask_FriendAttack::OnAnimationFinished()
{
	FinishLatentTask(*FriendController->Get_btComponent(), EBTNodeResult::Succeeded);
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

UMyBTTask_FriendAttack::UMyBTTask_FriendAttack()
{
	AttackMontage = CreateDefaultSubobject<UAnimMontage>(TEXT("FriendAttackMontage"));
}
