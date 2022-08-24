// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTTask_Escape.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include <UnrealCoopAI/FriendAIV1/FriendAIV1Controller.h>
#include <UnrealCoopAI/Public/EnemyCharacter.h>
#include <UnrealCoopAI/Public/CPPPlayerCharacter.h>
#include <UnrealCoopAI/Public/CPPFriendParentCharacter.h>


EBTNodeResult::Type UMyBTTask_Escape::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FriendController = Cast<AFriendAIV1Controller>(OwnerComp.GetAIOwner());
	ACPPFriendParentCharacter* TheFriend = Cast<ACPPFriendParentCharacter>(FriendController->Get_selfActor());

	TheFriend->PlayAnimMontage(AttackMontage, 1.0, NAME_None);
	
	AEnemyCharacter* TargetEnemy = Cast<AEnemyCharacter>(FriendController->Get_blackboard()->GetValueAsObject(FName(TEXT("TargetActor"))));

	if (IsValid(TargetEnemy) && IsValid(TheFriend))
	{
		/*FVector Destination = UKismetMathLibrary::GetDirectionUnitVector(TargetEnemy->GetActorLocation(), TheFriend->GetActorLocation()) * 700.0;
		EPathFollowingRequestResult::Type con = FriendController->MoveToLocation(Destination, (float)20);
		if(con == EPathFollowingRequestResult::AlreadyAtGoal)
			return EBTNodeResult::Succeeded;
		else GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UMyBTTask_Escape::OnAnimationFinished, 0.1f, true);*/
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UMyBTTask_Escape::OnAnimationFinished, 1.5f, true);
	}
	else return EBTNodeResult::Succeeded;

	return EBTNodeResult::InProgress;
}

UMyBTTask_Escape::UMyBTTask_Escape()
{
	AttackMontage = CreateDefaultSubobject<UAnimMontage>(TEXT("Escape Montage"));
}

void UMyBTTask_Escape::OnAnimationFinished()
{
	/*
	ACPPFriendParentCharacter* TheFriend = Cast<ACPPFriendParentCharacter>(FriendController->Get_selfActor());

	AEnemyCharacter* TargetEnemy = Cast<AEnemyCharacter>(FriendController->Get_blackboard()->GetValueAsObject(FName(TEXT("TargetActor"))));

	if (IsValid(TargetEnemy) && IsValid(TheFriend)) {
		
		FVector Destination = UKismetMathLibrary::GetDirectionUnitVector(TargetEnemy->GetActorLocation(), TheFriend->GetActorLocation()) * 700.0;
		EPathFollowingRequestResult::Type con = FriendController->MoveToLocation(Destination, (float)20);
		if (con == EPathFollowingRequestResult::AlreadyAtGoal) {
			FinishLatentTask(*FriendController->Get_btComponent(), EBTNodeResult::Succeeded);
			GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		}
		else GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UMyBTTask_Escape::OnAnimationFinished, 0.1f, true);
	}
	else
	{*/
		FinishLatentTask(*FriendController->Get_btComponent(), EBTNodeResult::Succeeded);
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	//}

}