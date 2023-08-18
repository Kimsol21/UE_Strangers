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
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, ControllingPawn); //탐색 방법에 대한 설정 값을 모아둔 구조체. 
	/*
	* 첫번째 인자 (TraceTag) : Trace 디버깅을 위한 추가 정보 또는 필터링을 제공하는 데 사용되는 태그(예: Collision Analyzer)
	* 두번째 인자 (bTraceComplex) : 복잡한 충돌에 대해 추적해야 하는지 여부.
	* 세번째 인자 (IgnoreActor) : Trace하는 동안 무시해야 하는 엑터.
	*/

	bool bResult = World->OverlapMultiByChannel(//트레이스 채널을 통해 오버랩 여부를 가리는 함수.
		OverlapResults, //오버랩된 액터들을 저장할 배열.
		Center, // 탐색을 시작할 위치.
		FQuat::Identity, // 탐색에 사용할 도형의 회전값.
		ECollisionChannel::ECC_GameTraceChannel2, // 물리 감지에 사용할 콜리전 채널 정보.
		FCollisionShape::MakeSphere(DetectRadius), // 탐색에 사용할 기본 도형 정보. (구체, 캡슐, 박스 등) 
		CollisionQueryParam //탐색 방법에 대한 설정 값을 모아둔 구조체.
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
				//인자 : (구를 그릴 공간, 그릴 위치, 구의 반지름, 조각수, 구의 색깔, 영구적인지 여부, 구의 수명)
				//DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f); // 구모양의 디버그 라인을 그린다. 색은 초록색.
				//인자 : (선을 그릴 공간, 그리기 시작할 위치, 마지막 위치, 선의 색깔, 영구적인지 여부, 선의 수명)
				//DrawDebugLine(World, ControllingPawn->GetActorLocation(), MyPlayer->GetActorLocation(), FColor::Blue, false, 0.2f); // 적에서 플레이어 사이에 파란색 디버그 라인을 그린다. 
				return;
			}
		}
	}	
}
