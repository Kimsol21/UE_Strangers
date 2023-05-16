// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Boss/MyBossEntrance.h"
#include "Components/BoxComponent.h"
#include "Character/Player/MyPlayer.h"
#include "Character/Player/MyPlayerController.h"
#include "Character/Boss/MyBoss.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "EngineUtils.h"



// Sets default values
AMyBossEntrance::AMyBossEntrance()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	// 플레이어 감지 콜리전 관련.
	PlayerCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TargetCollisionBox")); //콜리전 생성.
	PlayerCollisionBox->SetBoxExtent(FVector(400.0f, 400.0f, 300.0f)); //콜리전 감지 범위 설정.
	PlayerCollisionBox->SetCollisionProfileName(TEXT("OverlapOnlyPawn")); //콜리전 프리셋 설정.
	//PlayerCollisionBox->SetCollisionProfileName(TEXT("NoCollision")); //콜리전 프리셋 설정.
}


void AMyBossEntrance::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PlayerCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AMyBossEntrance::OnPlayerCollisionBeginOverlap); //콜리전 오버랩 델리게이트에 함수 바인딩.
}

void AMyBossEntrance::BeginPlay()
{
	Super::BeginPlay();

	//시네마틱 스킵관련 바인딩.
	AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());
	if (MyPlayerController)
	{
		MyPlayerController->OnExitKeyPressed().AddLambda([this]()->void {
			SequencePlayer->GoToEndAndStop();
			});
	}

	//월드에 배치된 AMyBoss 타입의 액터 가져와 싸울 보스에 저장.
	for (AMyBoss* BossActor : TActorRange<AMyBoss>(GetWorld()))
	{
		if (BossActor)
		{
			BossToFight = BossActor;
		}
	}
	//시퀀스 액터가 null이면 디폴트 시퀀스액터 적용.
	if (nullptr == BossStartSequenceActor)
	{
		FStringAssetReference DefaultSequenceName("LevelSequence'/Game/Cinematic/Boss/Boss_BloodHand_StartScene.Boss_BloodHand_StartScene'");
		BossStartSequenceActor = Cast<ALevelSequenceActor>(DefaultSequenceName.TryLoad());
	}

	// 재생 기본 옵션 설정.
	FMovieSceneSequencePlaybackSettings LevelSequencePlaySettings;
	LevelSequencePlaySettings.PlayRate = 0.6f; // 재생 속도 낮추기.
	LevelSequencePlaySettings.bDisableMovementInput = true; // 재생 도중 캐릭터 인풋 받을 수 없게 설정.

	//시퀀스 액터가 null이 아니고 시퀀스 플레이어가 null일때 플레이어 생성.
	if (BossStartSequenceActor && SequencePlayer == nullptr)
	{
		//플레이어에 CD넣기 작업.
		SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), BossStartSequenceActor->GetSequence(), LevelSequencePlaySettings, BossStartSequenceActor);
	}
	
	if (SequencePlayer)
	{
		//레벨 시퀀스가 끝났을 때의 델리게이트에 함수 바인딩.
		//SequencePlayer->OnFinished.AddDynamic(this, &AMyBossEntrance::OnStartCinematicFinished);
		SequencePlayer->OnStop.AddDynamic(this, &AMyBossEntrance::OnStartCinematicFinished);
	}  

	//PlayerCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision); //임시로 콜리전 무효화. (테스트용)

}

void AMyBossEntrance::OnStartCinematicFinished()
{
	// 플레이어 컨트롤러에게 보스룸에 입장했음을 알림.
	AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());
	if (MyPlayerController)
	{
		MyPlayerController->OnBossFightStart().Broadcast(BossToFight);
	}
}

void AMyBossEntrance::OnPlayerCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//콜리전에 오버랩된 액터가 플레이어라면,
	AMyPlayer* OverlappedPlayer = Cast<AMyPlayer>(OtherActor);
	if (OverlappedPlayer)
	{
		//시네마틱을 재생하기 전 액터를 숨김.
		OverlappedPlayer->MyPlayerController->OnLevelSequenceStart().Broadcast();

		//레벨 시퀀스를 재생함. 
		if (SequencePlayer)
		{
			SequencePlayer->Play();
		}
		
	}

	PlayerCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 한번 들어간 콜리전은 비활성화.
}


