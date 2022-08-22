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

	if (ThePlayer)
	{
		
		FBoolProperty* DeathProperty = FindFProperty<FBoolProperty>(ThePlayer->GetClass(), FName(TEXT("Death")));
		bool DeathValue = DeathProperty->GetPropertyValue_InContainer(ThePlayer);

		
		if (!DeathValue)
		{
 			PlayerDistance = UKismetMathLibrary::Vector_Distance(TheEnemy->GetActorLocation(), ThePlayer->GetActorLocation());
		}

	}

	ACPPFriendParentCharacter* TheFriend = Cast<ACPPFriendParentCharacter>(EnemyController->Get_blackboard()->GetValueAsObject(FName(TEXT("FriendActor"))));

	if (TheFriend)
	{
		FriendDistance = UKismetMathLibrary::Vector_Distance(TheEnemy->GetActorLocation(), TheFriend->GetActorLocation());
	}



	if (FriendDistance != 0 && PlayerDistance > FriendDistance) {
		EnemyController->Get_blackboard()->SetValueAsObject(FName(TEXT("TargetActor")), TheFriend);
		//if (GEngine)
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT(" friend target")));
	}
	else
	{
		EnemyController->Get_blackboard()->SetValueAsObject(FName(TEXT("TargetActor")), ThePlayer);
		//if (GEngine)
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT(" player target ")));
	}

	return EBTNodeResult::Succeeded;
}