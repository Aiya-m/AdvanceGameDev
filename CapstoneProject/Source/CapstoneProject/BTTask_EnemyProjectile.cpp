#include "BTTask_EnemyProjectile.h"

#include "CapstoneProjectCharacter.h"
#include "AIController.h"
#include "C_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "GenericTeamAgentInterface.h"

UBTTask_EnemyProjectile::UBTTask_EnemyProjectile()
{
	NodeName = TEXT("Ranged Attack");
}

EBTNodeResult::Type UBTTask_EnemyProjectile::ExecuteTask(UBehaviorTreeComponent& OwnerComp,uint8* NodeMemory)
{
	AC_AIController* AICon = Cast<AC_AIController>(OwnerComp.GetAIOwner());

	if (!AICon || !AICon->HasAuthority()){return EBTNodeResult::Failed;}

	ACapstoneProjectCharacter* Me = Cast<ACapstoneProjectCharacter>(AICon->GetPawn());

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();

	if (!Me || !BB){return EBTNodeResult::Failed;}

	AActor* Target = Cast<AActor>(BB->GetValueAsObject(TargetActorKey));

	if (!IsValid(Target)){return EBTNodeResult::Failed;}

	const IGenericTeamAgentInterface* TargetTeam = Cast<IGenericTeamAgentInterface>(Target);

	if (!TargetTeam || TargetTeam->GetGenericTeamId().GetId() != 0)
	{
		return EBTNodeResult::Failed;
	}

	const float Distance = FVector::Dist(Me->GetActorLocation(),Target->GetActorLocation());

	if (Distance > AttackRange)
	{
		return EBTNodeResult::Failed;
	}

	Me->FireProjectileAt(Target);

	return EBTNodeResult::Succeeded;
}