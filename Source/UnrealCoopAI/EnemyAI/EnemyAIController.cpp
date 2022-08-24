// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "GameFramework/Controller.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include <UnrealCoopAI/Public/EnemyCharacter.h>
#include <UnrealCoopAI/Public/CPPPlayerCharacter.h>
#include <UnrealCoopAI/Public/CPPFriendParentCharacter.h>
#include "UObject/ConstructorHelpers.h"
#include "UObject/UnrealType.h"
#include "UObject/UObjectGlobals.h"
#include "Perception/AIPerceptionTypes.h"
#include <Runtime/AIModule/Classes/BehaviorTree/BTTaskNode.h>

AEnemyCharacter* AEnemyAIController::Get_selfActor() const
{
	return TheEnemy;
}

UBlackboardComponent* AEnemyAIController::Get_blackboard() const
{
	return EnemyBlackboard;
}

void AEnemyAIController::OnPossess(APawn* const pawn)
{
	Super::OnPossess(pawn);

	TheEnemy = Cast<AEnemyCharacter>(pawn);

	if (EnemyBlackboard)
	{
		EnemyBlackboard->InitializeBlackboard(*EnemyBehaviorTree->BlackboardAsset);
	}
}

AEnemyAIController::AEnemyAIController()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> obj(TEXT("BehaviorTree'/Game/Character/CPPEnemyAI/CPPEnemy_BT.CPPEnemy_BT'"));
	if (obj.Succeeded())
	{
		EnemyBehaviorTree = DuplicateObject(obj.Object, NULL );
	}

	EnemyBehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	EnemyBlackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception Component"));
	UAISenseConfig_Sight* sightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("AI Sight Config"));
	SetPerceptionComponent(*AIPerception);
	sightConfig->DetectionByAffiliation.bDetectEnemies = true;
	sightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::SenseStuff);
	AIPerception->ConfigureSense(*sightConfig);

}

void AEnemyAIController::BeginPlay() 
{
	Super::BeginPlay();

	RunBehaviorTree(EnemyBehaviorTree);
	EnemyBehaviorTreeComponent->StartTree(*EnemyBehaviorTree);
}

void AEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	PlayerActor = Get_blackboard()->GetValueAsObject(FName(TEXT("PlayerActor")));
	FriendActor = Get_blackboard()->GetValueAsObject(FName(TEXT("FriendActor")));

	if (IsValid(PlayerActor)) {
		//if (GEngine)
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT(" have both player and friend ")));
		APawn* EnemyObject = AController::K2_GetPawn();
		if (IsValid(EnemyObject)) {
			TheEnemy = (AEnemyCharacter*)EnemyObject;

			FBoolProperty* FocusOnPlayerProperty = FindFProperty<FBoolProperty>(TheEnemy->GetClass(), FName(TEXT("FocusOnPlayer")));
			bool FocusOnPlayerValue = FocusOnPlayerProperty->GetPropertyValue_InContainer(TheEnemy);

			if (FocusOnPlayerValue)
			{
				Get_blackboard()->SetValueAsObject(FName(TEXT("TargetActor")), PlayerActor);
				Get_blackboard()->SetValueAsBool(FName(TEXT("FocusedOnFriend")), false);
			}

			if (IsValid(FriendActor))
			{
				FBoolProperty* FocusOnFriendProperty = FindFProperty<FBoolProperty>(TheEnemy->GetClass(), FName(TEXT("FocusOnFriend")));
				bool FocusOnFriendValue = FocusOnFriendProperty->GetPropertyValue_InContainer(TheEnemy);

				if (FocusOnFriendValue)
				{
					Get_blackboard()->SetValueAsObject(FName(TEXT("TargetActor")), FriendActor);
					Get_blackboard()->SetValueAsBool(FName(TEXT("FocusedOnFriend")), true);
				}
			}
			ACPPPlayerCharacter* ThePlayer = (ACPPPlayerCharacter*)PlayerActor;
			FBoolProperty* DeathProperty = FindFProperty<FBoolProperty>(ThePlayer->GetClass(), FName(TEXT("Death")));
			bool DeathValue = DeathProperty->GetPropertyValue_InContainer(ThePlayer);

			if (DeathValue) {
				AAIController::ClearFocus(EAIFocusPriority::Gameplay);

				if(!FriendActor)
					Get_blackboard()->SetValueAsBool(FName(TEXT("FoundPlayer")), false);
			}

		}

	}

}

void AEnemyAIController::SenseStuff(AActor* testActors, FAIStimulus stimulus)
{
	
	ACPPPlayerCharacter* ThePlayer = Cast<ACPPPlayerCharacter>(testActors);
	if (ThePlayer) {
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT(" Found Player ")));
		Get_blackboard()->SetValueAsBool(FName(TEXT("FoundPlayer")), true);

		if (IsValid(TheEnemy))
		{
			FBoolProperty* FoundPlayerProperty = FindFProperty<FBoolProperty>(TheEnemy->GetClass(), FName(TEXT("FoundPlayer")));
			FoundPlayerProperty->SetPropertyValue_InContainer(TheEnemy, true);
		}

		Get_blackboard()->SetValueAsObject(FName(TEXT("PlayerActor")), ThePlayer);
	}

	ACPPFriendParentCharacter* TheFriend = Cast<ACPPFriendParentCharacter>(testActors);
	if (TheFriend)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT(" Found Friend ")));
		Get_blackboard()->SetValueAsObject(FName(TEXT("FriendActor")), TheFriend);
	}
		
}

UBehaviorTreeComponent* AEnemyAIController::Get_btComponent() const
{
	return EnemyBehaviorTreeComponent;
}

void AEnemyAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) {
	
	if (Result.IsSuccess())
	{
		Get_blackboard()->SetValueAsBool(FName(TEXT("HasArrived")), true);
	}
	
}