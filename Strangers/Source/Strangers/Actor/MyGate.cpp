#include "Actor/MyGate.h"
#include "Components/BoxComponent.h"
#include "Character/Player/MyPlayer.h"
#include "Character/Player/MyPlayerController.h"
#include "Character/Player/MyPlayerStatComponent.h"

AMyGate::AMyGate()
{
	PrimaryActorTick.bCanEverTick = false;

	// 플레이어 막는 콜리전 관련.
	BlockCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BlockCollisionBox")); 
	BlockCollisionBox->SetBoxExtent(FVector(300.0f, 300.0f, 200.0f)); 
	BlockCollisionBox->SetCollisionProfileName(TEXT("BlockAllDynamic")); 

	// 플레이어 감지 콜리전 관련.
	PlayerCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TargetCollisionBox")); 
	PlayerCollisionBox->SetBoxExtent(FVector(400.0f, 400.0f, 300.0f)); 
	PlayerCollisionBox->SetCollisionProfileName(TEXT("OverlapOnlyPawn")); 
	PlayerCollisionBox->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));	
}

void AMyGate::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PlayerCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AMyGate::OnPlayerCollisionBeginOverlap); //콜리전 오버랩 델리게이트에 함수 바인딩.
}

void AMyGate::OnPlayerCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//콜리전에 오버랩된 액터가 플레이어라면,
	AMyPlayer* OverlappedPlayer = Cast<AMyPlayer>(OtherActor);
	if (OverlappedPlayer)
	{
		//플레이어의 현재 레벨이 통과 가능 레벨보다 높으면 통과.
		if (PassableLevel < OverlappedPlayer->MyStat->GetLevel())
		{
			OpenGate();
		}
		else
		{
			// 조건 불충분 시 알림UI에 준비된 알림텍스트 출력시키기.
			OverlappedPlayer->GetPlayerController()->OnNoticeUpdate().Broadcast(NoticeString);
		}
	}
}

void AMyGate::OpenGate()
{
	BlockCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 콜리전 비활성화.
}

void AMyGate::CloseGate()
{
	BlockCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // 콜리전 활성화.
}

