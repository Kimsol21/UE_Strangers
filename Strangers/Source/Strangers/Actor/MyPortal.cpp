#include "Actor/MyPortal.h"
#include "Components/BoxComponent.h"
#include "Character/Player/MyPlayer.h"
#include "Character/Player/MyPlayerController.h"
#include "Kismet/GameplayStatics.h"

AMyPortal::AMyPortal()
{
	PrimaryActorTick.bCanEverTick = false;

	PlayerCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TargetCollisionBox")); 
	PlayerCollisionBox->SetBoxExtent(FVector(400.0f, 400.0f, 300.0f));
	PlayerCollisionBox->SetCollisionProfileName(TEXT("OverlapOnlyPawn")); 
}

void AMyPortal::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PlayerCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AMyPortal::OnPlayerCollisionBeginOverlap); //콜리전 오버랩 델리게이트에 함수 바인딩.
}

void AMyPortal::OnPlayerCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//콜리전에 오버랩된 액터가 플레이어라면,
	AMyPlayer* OverlappedPlayer = Cast<AMyPlayer>(OtherActor);
	if (OverlappedPlayer&& TransgerLevelName!="")
	{
		UGameplayStatics::OpenLevel(this, TransgerLevelName); // 다른 레벨로 이동.
	}


}

