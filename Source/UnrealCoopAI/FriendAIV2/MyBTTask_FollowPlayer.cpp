// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTTask_FollowPlayer.h"
#include "Engine/World.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include <UnrealCoopAI/Public/CPPPlayerCharacter.h>
#include <UnrealCoopAI/Public/CPPFriendParentCharacter.h>
#include <UnrealCoopAI/FriendAIV2/FriendAIV2Controller.h>
#include "Kismet/GameplayStatics.h"
#include <Runtime/AIModule/Classes/AIController.h>

EBTNodeResult::Type UMyBTTask_FollowPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FriendController = Cast<AFriendAIV2Controller>(OwnerComp.GetAIOwner());
	ACPPFriendParentCharacter* TheFriend = Cast<ACPPFriendParentCharacter>(FriendController->Get_selfActor());

	ACPPPlayerCharacter* ThePlayer = Cast<ACPPPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (IsValid(ThePlayer))
	{
		FriendController->SetFocus(ThePlayer, EAIFocusPriority::Gameplay);

		EPathFollowingRequestResult::Type con = FriendController->MoveToActor(ThePlayer, (float)200, true, true, true, NULL, true);

		if (con == EPathFollowingRequestResult::AlreadyAtGoal)
			return EBTNodeResult::Succeeded;
		else
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UMyBTTask_FollowPlayer::OnArrivedTarget, 0.1f, true);
	}
	else return EBTNodeResult::Succeeded;
	
	
	return EBTNodeResult::InProgress;
}

void UMyBTTask_FollowPlayer::OnArrivedTarget()
{
	ACPPPlayerCharacter* ThePlayer = Cast<ACPPPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (IsValid(ThePlayer)) {
		EPathFollowingRequestResult::Type con = FriendController->MoveToActor(ThePlayer, (float)200, true, true, true, NULL, true);

		if (con == EPathFollowingRequestResult::AlreadyAtGoal)
		{
			FinishLatentTask(*FriendController->Get_btComponent(), EBTNodeResult::Succeeded);
			GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		}
		else
		{
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UMyBTTask_FollowPlayer::OnArrivedTarget, 0.1f, true);
		}
	}
	else
	{
		FinishLatentTask(*FriendController->Get_btComponent(), EBTNodeResult::Succeeded);
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}
	
}