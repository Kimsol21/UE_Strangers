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

	// 플레이어 감지 콜리전 관련.
	PlayerCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TargetCollisionBox")); //콜리전 생성.
	PlayerCollisionBox->SetBoxExtent(FVector(400.0f, 400.0f, 300.0f)); //콜리전 감지 범위 설정.
	PlayerCollisionBox->SetCollisionProfileName(TEXT("OverlapOnlyPawn")); //콜리전 프리셋 설정.
}

// Called when the game starts or when spawned
void AMyPortal::BeginPlay()
{
	Super::BeginPlay();
	
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

