// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTTask_DodegeEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include <UnrealCoopAI/Public/EnemyCharacter.h>
#include <UnrealCoopAI/Public/CPPPlayerCharacter.h>
#include <UnrealCoopAI/Public/CPPFriendParentCharacter.h>
#include <UnrealCoopAI/FriendAIV1/FriendAIV1Controller.h>
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>


void UMyBTTask_DodegeEnemy::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FriendController = Cast<AFriendAIV1Controller>(OwnerComp.GetAIOwner());
	ACPPFriendParentCharacter* TheFriend = Cast<ACPPFriendParentCharacter>(FriendController->Get_selfActor());

	AEnemyCharacter* TheEnemy = Cast<AEnemyCharacter>(FriendController->Get_blackboard()->GetValueAsObject(FName(TEXT("TargetActor"))));

	ACPPPlayerCharacter* ThePlayer = Cast<ACPPPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	FVector destination;

	if (IsValid(TheEnemy) && IsValid(ThePlayer))
	{ 
		float distance = UKismetMathLibrary::Vector_Distance(TheEnemy->GetActorLocation(), ThePlayer->GetActorLocation());
		FVector direction = UKismetMathLibrary::GetDirectionUnitVector(ThePlayer->GetActorLocation(), TheEnemy->GetActorLocation());
		if (distance >= 300.0)
		{
			destination = TheEnemy->GetActorLocation() - ThePlayer->GetActorLocation();
		}
		else destination = direction * 600.0;
		FriendController->MoveToLocation(ThePlayer->GetActorLocation() - destination, (float)20);
	}

	FinishLatentTask(*FriendController->Get_btComponent(), EBTNodeResult::Succeeded);
}

EBTNodeResult::Type UMyBTTask_DodegeEnemy::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FriendController = Cast<AFriendAIV1Controller>(OwnerComp.GetAIOwner());
	ACPPFriendParentCharacter* TheFriend = Cast<ACPPFriendParentCharacter>(FriendController->Get_selfActor());

	AEnemyCharacter* TheEnemy = Cast<AEnemyCharacter>(FriendController->Get_blackboard()->GetValueAsObject(FName(TEXT("TargetActor"))));

	if(IsValid(TheEnemy))
		FriendController->SetFocus(TheEnemy, EAIFocusPriority::Gameplay);

	return EBTNodeResult::InProgress;
}