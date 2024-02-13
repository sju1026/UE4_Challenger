/*
* Writer : seo jae ung
*
* Function
*  - 적 캐릭터를 제어하는 AI 컨트롤러입니다. 
*  - 이 AI 컨트롤러는 행동 트리 (Behavior Tree)를 사용하여 적의 행동을 관리합니다.
*  - Behavior Tree 및 Blackboard 구성 요소를 초기화하고, 인식 시스템을 설정하여 적이 플레이어를 감지하고 추적하는 기능을 수행합니다. 
*  - 게임이 시작되면 행동 트리를 실행하고, 플레이어를 감지하면 블랙보드에 관련 정보를 업데이트하여 상태를 조절합니다. 
*
* UpdateRate : 2024 - 02 - 13
*/


#include "EnemyAIController.h"
#include <BehaviorTree/BehaviorTreeComponent.h>
#include <BehaviorTree/BehaviorTree.h>
#include <BehaviorTree/BlackboardComponent.h>
#include <BehaviorTree/Blackboard/BlackboardKeyType_Object.h>
#include <UObject/ConstructorHelpers.h>
#include <Perception/AIPerceptionComponent.h>
#include <Perception/AISenseConfig_Sight.h>
#include <Perception/AIPerceptionStimuliSourceComponent.h>
#include <blackboard_keys.h>
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <Engine/World.h>
#include <PlayerCharacter.h>
#include <Enemy.h>
#include <Engine.h>

AEnemyAIController::AEnemyAIController(FObjectInitializer const& object_initializer) {
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UBehaviorTree>obj(TEXT("BehaviorTree'/Game/_My/AI/BT_Enemy.BT_Enemy'"));
	if (obj.Succeeded()) {
		btree = obj.Object;
	}

	behavior_tree_comp = object_initializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComp"));
	blackboard = object_initializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComp"));

	setup_perception_system();
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	RunBehaviorTree(btree);
	behavior_tree_comp->StartTree(*btree);
}

void AEnemyAIController::OnPossess(APawn* pawn)
{
	Super::OnPossess(pawn);
	if (blackboard) {
		blackboard->InitializeBlackboard(*btree->BlackboardAsset);
	}
}

void AEnemyAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	AEnemy* owner = Cast<AEnemy>(GetPawn());
	if (owner != nullptr && owner->isDeath == true) {
		btree = nullptr;
	}
}

FRotator AEnemyAIController::GetControlRotation() const
{
	if (GetPawn() == nullptr) {
		return FRotator(0.f, 0.f, 0.f);
	}

	return FRotator(0.f, GetPawn()->GetActorRotation().Yaw, 0.0f);
}

void AEnemyAIController::on_target_detected(AActor* actor, FAIStimulus const stimulus)
{
	if (auto const ch = Cast<APlayerCharacter>(actor)) {
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "Find Player!!");
		get_blackboard()->SetValueAsBool(bb_keys::can_see_player, stimulus.WasSuccessfullySensed());
	}
}

UBlackboardComponent* AEnemyAIController::get_blackboard() const
{
	return blackboard;
}

void AEnemyAIController::setup_perception_system()
{
	sight_config = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));

	sight_config->SightRadius = 1500.0f;
	sight_config->LoseSightRadius = sight_config->SightRadius + 50.0f;
	sight_config->PeripheralVisionAngleDegrees = 270.0f;
	sight_config->SetMaxAge(5.0f);
	sight_config->AutoSuccessRangeFromLastSeenLocation = 520.0f;
	sight_config->DetectionByAffiliation.bDetectEnemies = true;
	sight_config->DetectionByAffiliation.bDetectFriendlies = true;
	sight_config->DetectionByAffiliation.bDetectNeutrals = true;

	GetPerceptionComponent()->SetDominantSense(*sight_config->GetSenseImplementation());
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::on_target_detected);
	GetPerceptionComponent()->ConfigureSense(*sight_config);
}
