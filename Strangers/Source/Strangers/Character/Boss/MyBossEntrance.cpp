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

	PlayerCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TargetCollisionBox")); //콜리전 생성.
	PlayerCollisionBox->SetBoxExtent(FVector(400.0f, 400.0f, 300.0f)); //콜리전 감지 범위 설정.
	PlayerCollisionBox->SetCollisionProfileName(TEXT("OverlapOnlyPawn")); //콜리전 프리셋 설정.

}


void AMyBossEntrance::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PlayerCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AMyBossEntrance::OnPlayerCollisionBeginOverlap); //콜리전 오버랩 델리게이트에 함수 바인딩.
}

void AMyBossEntrance::OnPlayerCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMyPlayer* OverlappedPlayer = Cast<AMyPlayer>(OtherActor);
	if (OverlappedPlayer&&BossToFight)
	{
		OverlappedPlayer->GetPlayerController()->OnBossRoomEnter().Broadcast(BossToFight);// 캐릭터가 보스방 들어갔음을 알림. 
	}
}


