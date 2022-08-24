// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTTask_ChooseTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyAIController.h"
#include "Kismet/KismetMathLibrary.h"
#include <UnrealCoopAI/Public/EnemyCharacter.h>
#include <UnrealCoopAI/Public/CPPPlayerCharacter.h>
#include <UnrealCoopAI/Public/CPPFriendParentCharacter.h>

EBTNodeResult::Type UMyBTTask_ChooseTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyAIController* EnemyController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	AEnemyCharacter* TheEnemy = Cast<AEnemyCharacter> (EnemyController->Get_selfActor());

	ACPPPlayerCharacter* ThePlayer = Cast<ACPPPlayerCharacter>(EnemyController->Get_blackboard()->GetValueAsObject(FName(TEXT("PlayerActor"))));
	ACPPFriendParentCharacter* TheFriend = Cast<ACPPFriendParentCharacter>(EnemyController->Get_blackboard()->GetValueAsObject(FName(TEXT("FriendActor"))));

	float FriendDistance = 0;
	float PlayerDistance = 0;

	if (IsValid(ThePlayer))
	{
		
		FBoolProperty* DeathProperty = FindFProperty<FBoolProperty>(ThePlayer->GetClass(), FName(TEXT("Death")));
		bool DeathValue = DeathProperty->GetPropertyValue_InContainer(ThePlayer);

		
		if (!DeathValue)
		{
 			PlayerDistance = UKismetMathLibrary::Vector_Distance(TheEnemy->GetActorLocation(), ThePlayer->GetActorLocation());
		}

	}

	if (IsValid(TheFriend))
	{
		FriendDistance = UKismetMathLibrary::Vector_Distance(TheEnemy->GetActorLocation(), TheFriend->GetActorLocation());
	}

	


	if (IsValid(ThePlayer) && IsValid(TheFriend)) {
		if (PlayerDistance > FriendDistance) {
			EnemyController->Get_blackboard()->SetValueAsObject(FName(TEXT("TargetActor")), TheFriend);
			FBoolProperty* FocusedOnFriendProperty = FindFProperty<FBoolProperty>(TheEnemy->GetClass(), FName(TEXT("FocusedOnFriend")));
			FocusedOnFriendProperty->SetPropertyValue_InContainer(TheEnemy, true);
		}
		else
		{
			EnemyController->Get_blackboard()->SetValueAsObject(FName(TEXT("TargetActor")), ThePlayer);
			FBoolProperty* FocusedOnFriendProperty = FindFProperty<FBoolProperty>(TheEnemy->GetClass(), FName(TEXT("FocusedOnFriend")));
			FocusedOnFriendProperty->SetPropertyValue_InContainer(TheEnemy, false);
		}
	}

	if (IsValid(ThePlayer) && !IsValid(TheFriend))
	{
		EnemyController->Get_blackboard()->SetValueAsObject(FName(TEXT("TargetActor")), ThePlayer);
		FBoolProperty* FocusedOnFriendProperty = FindFProperty<FBoolProperty>(TheEnemy->GetClass(), FName(TEXT("FocusedOnFriend")));
		FocusedOnFriendProperty->SetPropertyValue_InContainer(TheEnemy, false);
	}

	return EBTNodeResult::Succeeded;
}