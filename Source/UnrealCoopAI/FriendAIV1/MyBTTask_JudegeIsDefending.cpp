// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTTask_JudegeIsDefending.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include <UnrealCoopAI/Public/EnemyCharacter.h>
#include <UnrealCoopAI/Public/CPPPlayerCharacter.h>
#include <UnrealCoopAI/Public/CPPFriendParentCharacter.h>

EBTNodeResult::Type UMyBTTask_JudegeIsDefending::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FriendController = Cast<AFriendAIV1Controller>(OwnerComp.GetAIOwner());
	ACPPFriendParentCharacter* TheFriend = Cast<ACPPFriendParentCharacter>(FriendController->Get_selfActor());

	AEnemyCharacter* TheEnemy = Cast<AEnemyCharacter>(FriendController->Get_blackboard()->GetValueAsObject(FName(TEXT("TargetActor"))));
	if (IsValid(TheEnemy))
	{
		FBoolProperty* IsDefendingProperty = FindFProperty<FBoolProperty>(TheEnemy->GetClass(), FName(TEXT("IsDefending")));
		bool IsDefendingValue = IsDefendingProperty->GetPropertyValue_InContainer(TheEnemy);

		FriendController->Get_blackboard()->SetValueAsBool(FName(TEXT("IsTargetDefending")), IsDefendingValue);
	}
	
	return EBTNodeResult::Succeeded;
}