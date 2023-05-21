// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT/BTService_Detect.h"
#include "AI/MyAIController.h"
#include "Character/Player/MyPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect"); //��� �̸� ����.
	Interval = 1.0f; // ���� �� ����.
} 

//�󵵸��� ����Ǵ� �Լ�.
void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds); //�θ��� �Լ� ���� ����.

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn(); //�� BT�� �����ϰ� �ִ� ���� ������ ���������� ����.
	if (nullptr == ControllingPawn) return; // ���� �������µ� �����ߴٸ� return.

	UWorld* World = ControllingPawn->GetWorld(); //���� �����ϴ� ���带 ���� ������ ����.
	if (nullptr == World) return;//���尡 ��ȿ���� �ʴٸ� return.

	FVector Center = ControllingPawn->GetActorLocation(); //���� ��ġ�� Vector�� ���� ������ ����.
	float DetectRadius = 600.0f; //��ü ���� �ݰ��� 6���ͷ� ����.

	TArray<FOverlapResult> OverlapResults; //������ ����� ������ �迭 ����.
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
	
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(AMyAIController::TargetKey, nullptr); //�������� Ÿ�ٰ� nullptr�� �ʱ�ȭ. 

	//���� ������ ���Ͱ� �ִٸ�,
	if (bResult)
	{	
		for (auto const& OverlapResult : OverlapResults) // �������� ���� �迭 ��Ҹ� �ϳ��ϳ� ��������.
		{
			AMyPlayer* MyPlayer = Cast<AMyPlayer>(OverlapResult.GetActor()); // �������� ���͸� �÷��̾�� ĳ����.
			if (MyPlayer) // ���� �÷��̾ �´ٸ�,
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(AMyAIController::TargetKey, MyPlayer); //�ش� �÷��̾ �������� TargetKey���� ����.
				////���� : (���� �׸� ����, �׸� ��ġ, ���� ������, ������, ���� ����, ���������� ����, ���� ����)
				//DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f); // ������� ����� ������ �׸���. ���� �ʷϻ�.
				////���� : (���� �׸� ����, �׸��� ������ ��ġ, ������ ��ġ, ���� ����, ���������� ����, ���� ����)
				//DrawDebugLine(World, ControllingPawn->GetActorLocation(), MyPlayer->GetActorLocation(), FColor::Blue, false, 0.2f); // ������ �÷��̾� ���̿� �Ķ��� ����� ������ �׸���. 
				return;
			}
		}
	}

	//������ ���Ͱ� ���ٸ�, 
	//DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f); //���� ���� ������� ����׶����� �׸����� ���� �������̴�.
	
}
