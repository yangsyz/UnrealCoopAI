// Fill out your copyright notice in the Description page of Project Settings.


#include "FriendAIV1Controller.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "UObject/ConstructorHelpers.h"
#include <UnrealCoopAI/Public/EnemyCharacter.h>
#include "UObject/UnrealType.h"
#include "UObject/UObjectGlobals.h"
#include "Perception/AIPerceptionTypes.h"


AFriendAIV1Controller::AFriendAIV1Controller()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> obj(TEXT("BehaviorTree'/Game/Character/CPPFriendV1/CPPFriendV1_BT.CPPFriendV1_BT'"));
	if (obj.Succeeded())
	{
		FriendV1BehaviorTree = obj.Object;
	}

	FriendV1BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	FriendV1Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception Component"));
	UAISenseConfig_Sight* sightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("AI Sight Config"));
	SetPerceptionComponent(*AIPerception);
	sightConfig->DetectionByAffiliation.bDetectEnemies = true;
	sightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AFriendAIV1Controller::SenseStuff);
	AIPerception->ConfigureSense(*sightConfig);
}

UBlackboardComponent* AFriendAIV1Controller::Get_blackboard() const
{
	return FriendV1Blackboard;
}

UBehaviorTreeComponent* AFriendAIV1Controller::Get_btComponent() const
{
	return FriendV1BehaviorTreeComponent;
}

ACPPFriendParentCharacter* AFriendAIV1Controller::Get_selfActor() const
{
	return TheFriendV1;
}

void AFriendAIV1Controller::OnPossess(APawn* const pawn)
{
	Super::OnPossess(pawn);

	TheFriendV1 = Cast<ACPPFriendParentCharacter>(pawn);
	if (FriendV1Blackboard)
	{
		FriendV1Blackboard->InitializeBlackboard(*FriendV1BehaviorTree->BlackboardAsset);
	}
}

void AFriendAIV1Controller::BeginPlay()
{
	Super::BeginPlay();

	RunBehaviorTree(FriendV1BehaviorTree);
	FriendV1BehaviorTreeComponent->StartTree(*FriendV1BehaviorTree);
}

void AFriendAIV1Controller::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float distance = 2000;
	int amount = 0;
	AEnemyCharacter* Emptyobject = NewObject<AEnemyCharacter>();
	AEnemyCharacter* EmptyEnemy = NewObject<AEnemyCharacter>(Emptyobject, "empty");
	AEnemyCharacter* DefendingEnemy = NewObject<AEnemyCharacter>(Emptyobject, "empty");
	AEnemyCharacter* NearestEnemy = NewObject<AEnemyCharacter>(Emptyobject, "empty");


	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT(" should not valid")));
	bool BeingFocused = false;

	TArray<AActor*> foundEnemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyCharacter::StaticClass(), foundEnemies);
	for (int i = 0; i < foundEnemies.Num(); i++)
	{
		AEnemyCharacter* enemies = Cast<AEnemyCharacter>(foundEnemies[i]);
		float currentdistance = UKismetMathLibrary::Vector_Distance(TheFriendV1->GetActorLocation(), enemies->GetActorLocation());
		if (currentdistance < 2000.0)
		{
			amount++;
		}

		FBoolProperty* FocusedOnFriendProperty = FindFProperty<FBoolProperty>(enemies->GetClass(), FName(TEXT("FocusedOnFriend")));
		bool FocusedOnFriendValue = FocusedOnFriendProperty->GetPropertyValue_InContainer(enemies);

		if (FocusedOnFriendValue)
		{
			BeingFocused = true;
		}

		FBoolProperty* IsDefendingProperty = FindFProperty<FBoolProperty>(enemies->GetClass(), FName(TEXT("IsDefending")));
		bool IsDefendingValue = IsDefendingProperty->GetPropertyValue_InContainer(enemies);

		if (IsDefendingValue)
		{
			DefendingEnemy = enemies;
		}
		else DefendingEnemy = NewObject<AEnemyCharacter>(Emptyobject, "empty");

		if (currentdistance <= distance)
		{
			distance = currentdistance;
			NearestEnemy = enemies;
		}
	}

	if (DefendingEnemy != EmptyEnemy)
	{
		Get_blackboard()->SetValueAsObject(FName(TEXT("TargetActor")), DefendingEnemy);
	}
	else {
		if (NearestEnemy != EmptyEnemy)
		{
			Get_blackboard()->SetValueAsObject(FName(TEXT("TargetActor")), NearestEnemy);
		}
	}

	Get_blackboard()->SetValueAsBool(FName(TEXT("ShouldEscaping")), BeingFocused);

	if (amount == 1) {
		AEnemyCharacter* target = Cast<AEnemyCharacter>(Get_blackboard()->GetValueAsObject(FName(TEXT("TargetActor"))));
		FBoolProperty* DeadProperty = FindFProperty<FBoolProperty>(target->GetClass(), FName(TEXT("Dead")));
		bool DeadValue = DeadProperty->GetPropertyValue_InContainer(target);
		if (DeadValue)
		{
			Get_blackboard()->SetValueAsBool(FName(TEXT("FoundEnemy")), false);
		}
	}
	
}

void AFriendAIV1Controller::SenseStuff(AActor* testActors, FAIStimulus stimulus)
{
	
	AEnemyCharacter* TheEnemy = Cast<AEnemyCharacter>(testActors);
	if (IsValid(TheEnemy))
	{
		Get_blackboard()->SetValueAsBool(FName(TEXT("FoundEnemy")), true);
	}
		
}