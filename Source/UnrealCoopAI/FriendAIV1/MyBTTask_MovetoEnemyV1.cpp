// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTTask_MovetoEnemyV1.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include <UnrealCoopAI/FriendAIV2/FriendAIV2Controller.h>
#include <UnrealCoopAI/Public/EnemyCharacter.h>
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UMyBTTask_MovetoEnemyV1::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FriendController = Cast<AFriendAIV1Controller>(OwnerComp.GetAIOwner());
	ACPPFriendParentCharacter* TheFriend = Cast<ACPPFriendParentCharacter>(FriendController->Get_selfActor());

	TargetEnemy = Cast<AEnemyCharacter>(FriendController->Get_blackboard()->GetValueAsObject(FName(TEXT("TargetActor"))));

	TArray<AActor*> foundEnemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyCharacter::StaticClass(), foundEnemies);

	if (foundEnemies.Contains(TargetEnemy))
	{
		if (IsValid(TargetEnemy))
		{
			FBoolProperty* DeadProperty = FindFProperty<FBoolProperty>(TargetEnemy->GetClass(), FName(TEXT("Dead")));
			bool DeadValue = DeadProperty->GetPropertyValue_InContainer(TargetEnemy);
			if (!DeadValue)
			{
				FriendController->ClearFocus(EAIFocusPriority::Gameplay);
				FriendController->SetFocus(TargetEnemy, EAIFocusPriority::Gameplay);

				FriendController->Get_blackboard()->SetValueAsBool(FName(TEXT("HasArrived")), false);
				if (!FriendController->GetProxy())
				{
					UAIBlueprintHelperLibrary::CreateMoveToProxyObject(FriendController, nullptr, TargetEnemy->GetActorLocation(), TargetEnemy, (float)100, true);
					FriendController->SetProxy(true);
					GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UMyBTTask_MovetoEnemyV1::OnArrivedTarget, 0.2f, true);
				}
				else return EBTNodeResult::Succeeded;
				
				return EBTNodeResult::InProgress;
			}
			else return EBTNodeResult::Failed;
			
		}
		else return EBTNodeResult::Failed;
	}
	else return EBTNodeResult::Failed;

	return EBTNodeResult::InProgress;
}

void UMyBTTask_MovetoEnemyV1::OnArrivedTarget()
{
	TargetEnemy = Cast<AEnemyCharacter>(FriendController->Get_blackboard()->GetValueAsObject(FName(TEXT("TargetActor"))));

	if (IsValid(TargetEnemy))
	{
		bool arrived = FriendController->Get_blackboard()->GetValueAsBool(FName(TEXT("HasArrived")));
		if (arrived)
		{
			FinishLatentTask(*FriendController->Get_btComponent(), EBTNodeResult::Succeeded);
			GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		}
		else GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UMyBTTask_MovetoEnemyV1::OnArrivedTarget, 0.2f, true);
	}
	else {
		FinishLatentTask(*FriendController->Get_btComponent(), EBTNodeResult::Succeeded);
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}
}
