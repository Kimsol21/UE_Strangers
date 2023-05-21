// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT/BTService_Detect.h"
#include "AI/MyAIController.h"
#include "Character/Player/MyPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect"); //노드 이름 설정.
	Interval = 1.0f; // 실행 빈도 설정.
} 

//빈도마다 실행되는 함수.
void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds); //부모의 함수 먼저 실행.

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn(); //이 BT를 소유하고 있는 폰을 가져와 지역변수에 저장.
	if (nullptr == ControllingPawn) return; // 만약 가져오는데 실패했다면 return.

	UWorld* World = ControllingPawn->GetWorld(); //폰이 존재하는 월드를 지역 변수에 저장.
	if (nullptr == World) return;//월드가 유효하지 않다면 return.

	FVector Center = ControllingPawn->GetActorLocation(); //폰의 위치를 Vector값 지역 변수에 저장.
	float DetectRadius = 600.0f; //물체 감지 반경을 6미터로 설정.

	TArray<FOverlapResult> OverlapResults; //오버랩 결과를 저장할 배열 선언.
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
	
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(AMyAIController::TargetKey, nullptr); //블랙보드의 타겟값 nullptr로 초기화. 

	//만약 감지된 액터가 있다면,
	if (bResult)
	{	
		for (auto const& OverlapResult : OverlapResults) // 오버랩된 액터 배열 요소를 하나하나 꺼내보기.
		{
			AMyPlayer* MyPlayer = Cast<AMyPlayer>(OverlapResult.GetActor()); // 오버랩된 액터를 플레이어로 캐스팅.
			if (MyPlayer) // 만약 플레이어가 맞다면,
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(AMyAIController::TargetKey, MyPlayer); //해당 플레이어를 블랙보드의 TargetKey값에 저장.
				////인자 : (구를 그릴 공간, 그릴 위치, 구의 반지름, 조각수, 구의 색깔, 영구적인지 여부, 구의 수명)
				//DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f); // 구모양의 디버그 라인을 그린다. 색은 초록색.
				////인자 : (선을 그릴 공간, 그리기 시작할 위치, 마지막 위치, 선의 색깔, 영구적인지 여부, 선의 수명)
				//DrawDebugLine(World, ControllingPawn->GetActorLocation(), MyPlayer->GetActorLocation(), FColor::Blue, false, 0.2f); // 적에서 플레이어 사이에 파란색 디버그 라인을 그린다. 
				return;
			}
		}
	}

	//감지된 액터가 없다면, 
	//DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f); //위와 같은 구모양의 디버그라인을 그리지만 색은 빨간색이다.
	
}
