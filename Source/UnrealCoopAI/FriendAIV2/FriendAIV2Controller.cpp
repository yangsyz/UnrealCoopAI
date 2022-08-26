// Fill out your copyright notice in the Description page of Project Settings.


#include "FriendAIV2Controller.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Controller.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include <UnrealCoopAI/Public/EnemyCharacter.h>
#include <UnrealCoopAI/Public/CPPPlayerCharacter.h>
#include <UnrealCoopAI/Public/CPPFriendParentCharacter.h>
#include "UObject/ConstructorHelpers.h"
#include "UObject/UnrealType.h"
#include "Perception/AIPerceptionTypes.h"

AFriendAIV2Controller::AFriendAIV2Controller()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> obj(TEXT("BehaviorTree'/Game/Character/CPPFriendV2/CPPFriendV2_BT.CPPFriendV2_BT'"));
	if (obj.Succeeded())
	{
		FriendV2BehaviorTree = obj.Object;
	}

	FriendV2BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	FriendV2Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception Component"));
	UAISenseConfig_Sight* sightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("AI Sight Config"));
	SetPerceptionComponent(*AIPerception);
	sightConfig->DetectionByAffiliation.bDetectEnemies = true;
	sightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AFriendAIV2Controller::SenseStuff);
	AIPerception->ConfigureSense(*sightConfig);
}

UBlackboardComponent* AFriendAIV2Controller::Get_blackboard() const
{
	if (IsValid(FriendV2Blackboard))
		return FriendV2Blackboard;
	else return NULL;
}

UBehaviorTreeComponent* AFriendAIV2Controller::Get_btComponent() const
{
	return FriendV2BehaviorTreeComponent;
}
ACPPFriendParentCharacter* AFriendAIV2Controller::Get_selfActor() const
{
	return TheFriendV2;
}

void AFriendAIV2Controller::OnPossess(APawn* const pawn)
{
	Super::OnPossess(pawn);

	TheFriendV2 = Cast<ACPPFriendParentCharacter>(pawn);
	if (FriendV2Blackboard)
	{
		FriendV2Blackboard->InitializeBlackboard(*FriendV2BehaviorTree->BlackboardAsset);
	}
}

void AFriendAIV2Controller::BeginPlay()
{
	Super::BeginPlay();

	RunBehaviorTree(FriendV2BehaviorTree);
	FriendV2BehaviorTreeComponent->StartTree(*FriendV2BehaviorTree);
}


void AFriendAIV2Controller::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	int amount = 0;
	float distance = 2000;
	AEnemyCharacter* TheNearestEnemy = NewObject<AEnemyCharacter>();


	TArray<AActor*> foundEnemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyCharacter::StaticClass(), foundEnemies);

	for (int i = 0; i < foundEnemies.Num(); i++) {
		AEnemyCharacter* enemies = Cast<AEnemyCharacter>(foundEnemies[i]);
		float currentdistance = UKismetMathLibrary::Vector_Distance(TheFriendV2->GetActorLocation(), enemies->GetActorLocation());
		if (currentdistance <= 2000.0)
		{
			amount++;
		}
		if (currentdistance <= distance) {
			distance = currentdistance;
			TheNearestEnemy = enemies;
		}
	}

	if (IsValid(TheNearestEnemy))
	{
		Get_blackboard()->SetValueAsObject(FName(TEXT("TargetActor")), TheNearestEnemy);
		if (amount == 1)
		{
			FBoolProperty* DeadProperty = FindFProperty<FBoolProperty>(TheNearestEnemy->GetClass(), FName(TEXT("Dead")));
			bool DeathValue = DeadProperty->GetPropertyValue_InContainer(TheNearestEnemy);

			if (DeathValue)
			{
				Get_blackboard()->SetValueAsBool(FName(TEXT("FoundEnemy")), false);
			}
		}
	}
}


void AFriendAIV2Controller::SenseStuff(AActor* testActors, FAIStimulus stimulus)
{
	
	AEnemyCharacter* TheEnemy = Cast<AEnemyCharacter>(testActors);
	if (IsValid(TheEnemy)) 
	{
		Get_blackboard()->SetValueAsBool(FName(TEXT("FoundEnemy")), true);
	}

}