// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTTask_Retreat.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include <UnrealCoopAI/FriendAIV1/FriendAIV1Controller.h>
#include <UnrealCoopAI/Public/EnemyCharacter.h>
#include <UnrealCoopAI/Public/CPPPlayerCharacter.h>
#include <UnrealCoopAI/Public/CPPFriendParentCharacter.h>

EBTNodeResult::Type UMyBTTask_Retreat::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FriendController = Cast<AFriendAIV1Controller>(OwnerComp.GetAIOwner());
	ACPPFriendParentCharacter* TheFriend = Cast<ACPPFriendParentCharacter>(FriendController->Get_selfActor());

	if (IsValid(TheFriend))
	{
		TheFriend->PlayAnimMontage(AttackMontage, 1.0, NAME_None);
		FriendController->SetProxy(true);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UMyBTTask_Retreat::OnAnimationFinished, 1.2f, true);
	}
	else return EBTNodeResult::Succeeded;

	


	return EBTNodeResult::InProgress;
}

UMyBTTask_Retreat::UMyBTTask_Retreat()
{
	AttackMontage = CreateDefaultSubobject<UAnimMontage>(TEXT("Retreat Montage"));
}

void UMyBTTask_Retreat::OnAnimationFinished()
{
	FriendController->SetProxy(false);
	FinishLatentTask(*FriendController->Get_btComponent(), EBTNodeResult::Succeeded);
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}