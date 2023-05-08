// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/MyPortal.h"
#include "Components/BoxComponent.h"
#include "Character/Player/MyPlayer.h"
#include "Character/Player/MyPlayerController.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AMyPortal::AMyPortal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// �÷��̾� ���� �ݸ��� ����.
	PlayerCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TargetCollisionBox")); //�ݸ��� ����.
	PlayerCollisionBox->SetBoxExtent(FVector(400.0f, 400.0f, 300.0f)); //�ݸ��� ���� ���� ����.
	PlayerCollisionBox->SetCollisionProfileName(TEXT("OverlapOnlyPawn")); //�ݸ��� ������ ����.
}

// Called when the game starts or when spawned
void AMyPortal::BeginPlay()
{
	Super::BeginPlay();
	
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

