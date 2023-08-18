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

	PlayerCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AMyPortal::OnPlayerCollisionBeginOverlap); //�ݸ��� ������ ��������Ʈ�� �Լ� ���ε�.
}

void AMyPortal::OnPlayerCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//�ݸ����� �������� ���Ͱ� �÷��̾���,
	AMyPlayer* OverlappedPlayer = Cast<AMyPlayer>(OtherActor);
	if (OverlappedPlayer&& TransgerLevelName!="")
	{
		UGameplayStatics::OpenLevel(this, TransgerLevelName); // �ٸ� ������ �̵�.
	}


}

