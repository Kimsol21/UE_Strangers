// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/MyGate.h"
#include "Components/BoxComponent.h"
#include "Character/Player/MyPlayer.h"
#include "Character/Player/MyPlayerController.h"
#include "Character/Player/MyPlayerStatComponent.h"

// Sets default values
AMyGate::AMyGate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// �÷��̾� ���� �ݸ��� ����.
	BlockCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BlockCollisionBox")); //�ݸ��� ����.
	BlockCollisionBox->SetBoxExtent(FVector(300.0f, 300.0f, 200.0f)); //�ݸ��� ���� ���� ����.
	BlockCollisionBox->SetCollisionProfileName(TEXT("BlockAllDynamic")); //�ݸ��� ������ ����.

	// �÷��̾� ���� �ݸ��� ����.
	PlayerCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TargetCollisionBox")); //�ݸ��� ����.
	PlayerCollisionBox->SetBoxExtent(FVector(400.0f, 400.0f, 300.0f)); //�ݸ��� ���� ���� ����.
	PlayerCollisionBox->SetCollisionProfileName(TEXT("OverlapOnlyPawn")); //�ݸ��� ������ ����.

	PlayerCollisionBox->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	

	
}

void AMyGate::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PlayerCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AMyGate::OnPlayerCollisionBeginOverlap); //�ݸ��� ������ ��������Ʈ�� �Լ� ���ε�.
}

// Called when the game starts or when spawned
void AMyGate::BeginPlay()
{
	Super::BeginPlay();
	

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

