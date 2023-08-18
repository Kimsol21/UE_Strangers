#include "Actor/MyGate.h"
#include "Components/BoxComponent.h"
#include "Character/Player/MyPlayer.h"
#include "Character/Player/MyPlayerController.h"
#include "Character/Player/MyPlayerStatComponent.h"

AMyGate::AMyGate()
{
	PrimaryActorTick.bCanEverTick = false;

	// �÷��̾� ���� �ݸ��� ����.
	BlockCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BlockCollisionBox")); 
	BlockCollisionBox->SetBoxExtent(FVector(300.0f, 300.0f, 200.0f)); 
	BlockCollisionBox->SetCollisionProfileName(TEXT("BlockAllDynamic")); 

	// �÷��̾� ���� �ݸ��� ����.
	PlayerCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TargetCollisionBox")); 
	PlayerCollisionBox->SetBoxExtent(FVector(400.0f, 400.0f, 300.0f)); 
	PlayerCollisionBox->SetCollisionProfileName(TEXT("OverlapOnlyPawn")); 
	PlayerCollisionBox->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));	
}

void AMyGate::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PlayerCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AMyGate::OnPlayerCollisionBeginOverlap); //�ݸ��� ������ ��������Ʈ�� �Լ� ���ε�.
}

void AMyGate::OnPlayerCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//�ݸ����� �������� ���Ͱ� �÷��̾���,
	AMyPlayer* OverlappedPlayer = Cast<AMyPlayer>(OtherActor);
	if (OverlappedPlayer)
	{
		//�÷��̾��� ���� ������ ��� ���� �������� ������ ���.
		if (PassableLevel < OverlappedPlayer->MyStat->GetLevel())
		{
			OpenGate();
		}
		else
		{
			// ���� ����� �� �˸�UI�� �غ�� �˸��ؽ�Ʈ ��½�Ű��.
			OverlappedPlayer->GetPlayerController()->OnNoticeUpdate().Broadcast(NoticeString);
		}
	}
}

void AMyGate::OpenGate()
{
	BlockCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision); // �ݸ��� ��Ȱ��ȭ.
}

void AMyGate::CloseGate()
{
	BlockCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // �ݸ��� Ȱ��ȭ.
}

