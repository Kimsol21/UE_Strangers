// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Boss/MyBossEntrance.h"
#include "Components/BoxComponent.h"
#include "Character/Player/MyPlayer.h"
#include "Character/Player/MyPlayerController.h"

// Sets default values
AMyBossEntrance::AMyBossEntrance()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	PlayerCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TargetCollisionBox")); //�ݸ��� ����.
	PlayerCollisionBox->SetBoxExtent(FVector(400.0f, 400.0f, 300.0f)); //�ݸ��� ���� ���� ����.
	PlayerCollisionBox->SetCollisionProfileName(TEXT("OverlapOnlyPawn")); //�ݸ��� ������ ����.

}


void AMyBossEntrance::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PlayerCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AMyBossEntrance::OnPlayerCollisionBeginOverlap); //�ݸ��� ������ ��������Ʈ�� �Լ� ���ε�.
}

void AMyBossEntrance::OnPlayerCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMyPlayer* OverlappedPlayer = Cast<AMyPlayer>(OtherActor);
	if (OverlappedPlayer&&BossToFight)
	{
		OverlappedPlayer->GetPlayerController()->OnBossRoomEnter().Broadcast(BossToFight);// ĳ���Ͱ� ������ ������ �˸�. 
	}
}


