#include "AI/BT/BTService_Detect.h"
#include "AI/MyAIController.h"
#include "Character/Player/MyPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect"); 
	Interval = 1.0f; 
} 

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds); 

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn(); 
	if (nullptr == ControllingPawn) return; 

	UWorld* World = ControllingPawn->GetWorld(); 
	if (nullptr == World) return;

	FVector Center = ControllingPawn->GetActorLocation(); 
	float DetectRadius = 600.0f; 

	TArray<FOverlapResult> OverlapResults; 
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, ControllingPawn); //Ž�� ����� ���� ���� ���� ��Ƶ� ����ü. 
	/*
	* ù��° ���� (TraceTag) : Trace ������� ���� �߰� ���� �Ǵ� ���͸��� �����ϴ� �� ���Ǵ� �±�(��: Collision Analyzer)
	* �ι�° ���� (bTraceComplex) : ������ �浹�� ���� �����ؾ� �ϴ��� ����.
	* ����° ���� (IgnoreActor) : Trace�ϴ� ���� �����ؾ� �ϴ� ����.
	*/

	bool bResult = World->OverlapMultiByChannel(//Ʈ���̽� ä���� ���� ������ ���θ� ������ �Լ�.
		OverlapResults, //�������� ���͵��� ������ �迭.
		Center, // Ž���� ������ ��ġ.
		FQuat::Identity, // Ž���� ����� ������ ȸ����.
		ECollisionChannel::ECC_GameTraceChannel2, // ���� ������ ����� �ݸ��� ä�� ����.
		FCollisionShape::MakeSphere(DetectRadius), // Ž���� ����� �⺻ ���� ����. (��ü, ĸ��, �ڽ� ��) 
		CollisionQueryParam //Ž�� ����� ���� ���� ���� ��Ƶ� ����ü.
	);
	
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(AMyAIController::TargetKey, nullptr); 

	if (bResult)
	{	
		for (auto const& OverlapResult : OverlapResults) 
		{
			AMyPlayer* MyPlayer = Cast<AMyPlayer>(OverlapResult.GetActor()); 
			if (MyPlayer) 
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(AMyAIController::TargetKey, MyPlayer); 
				//���� : (���� �׸� ����, �׸� ��ġ, ���� ������, ������, ���� ����, ���������� ����, ���� ����)
				//DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f); // ������� ����� ������ �׸���. ���� �ʷϻ�.
				//���� : (���� �׸� ����, �׸��� ������ ��ġ, ������ ��ġ, ���� ����, ���������� ����, ���� ����)
				//DrawDebugLine(World, ControllingPawn->GetActorLocation(), MyPlayer->GetActorLocation(), FColor::Blue, false, 0.2f); // ������ �÷��̾� ���̿� �Ķ��� ����� ������ �׸���. 
				return;
			}
		}
	}	
}
