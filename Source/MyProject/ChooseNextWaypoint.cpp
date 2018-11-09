// Fill out your copyright notice in the Description page of Project Settings.

#include "ChooseNextWaypoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "PatrolRoute.h"

EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	// TODO protect against empty patrol routes and empty pawns

	// Get patrol points
	auto Guard = OwnerComp.GetAIOwner()->GetPawn();
	auto PatrolRoute = Guard->FindComponentByClass<UPatrolRoute>();
	if (!ensure(PatrolRoute)) { return EBTNodeResult::Failed; }

	TArray<AActor*> PatrolPoints = PatrolRoute->GetPatrolPoints();
	if (PatrolPoints.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Empty patrol route!"))
		return EBTNodeResult::Failed;
	}

	// Set the next waypoint
	auto BlackBoardComp = OwnerComp.GetBlackboardComponent();
	auto IndexNumber = BlackBoardComp->GetValueAsInt(Index.SelectedKeyName);
	BlackBoardComp->SetValueAsObject(Waypoint.SelectedKeyName, PatrolPoints[IndexNumber]);

	// Cycle the index
	BlackBoardComp->SetValueAsInt(Index.SelectedKeyName, (IndexNumber + 1) % PatrolPoints.Num());

	return EBTNodeResult::Succeeded;
}
