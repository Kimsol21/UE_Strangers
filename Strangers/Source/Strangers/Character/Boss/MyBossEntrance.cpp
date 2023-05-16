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
	
	// �÷��̾� ���� �ݸ��� ����.
	PlayerCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TargetCollisionBox")); //�ݸ��� ����.
	PlayerCollisionBox->SetBoxExtent(FVector(400.0f, 400.0f, 300.0f)); //�ݸ��� ���� ���� ����.
	PlayerCollisionBox->SetCollisionProfileName(TEXT("OverlapOnlyPawn")); //�ݸ��� ������ ����.
	//PlayerCollisionBox->SetCollisionProfileName(TEXT("NoCollision")); //�ݸ��� ������ ����.
}


void AMyBossEntrance::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PlayerCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AMyBossEntrance::OnPlayerCollisionBeginOverlap); //�ݸ��� ������ ��������Ʈ�� �Լ� ���ε�.
}

void AMyBossEntrance::BeginPlay()
{
	Super::BeginPlay();

	//�ó׸�ƽ ��ŵ���� ���ε�.
	AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());
	if (MyPlayerController)
	{
		MyPlayerController->OnExitKeyPressed().AddLambda([this]()->void {
			SequencePlayer->GoToEndAndStop();
			});
	}

	//���忡 ��ġ�� AMyBoss Ÿ���� ���� ������ �ο� ������ ����.
	for (AMyBoss* BossActor : TActorRange<AMyBoss>(GetWorld()))
	{
		if (BossActor)
		{
			BossToFight = BossActor;
		}
	}
	//������ ���Ͱ� null�̸� ����Ʈ ���������� ����.
	if (nullptr == BossStartSequenceActor)
	{
		FStringAssetReference DefaultSequenceName("LevelSequence'/Game/Cinematic/Boss/Boss_BloodHand_StartScene.Boss_BloodHand_StartScene'");
		BossStartSequenceActor = Cast<ALevelSequenceActor>(DefaultSequenceName.TryLoad());
	}

	// ��� �⺻ �ɼ� ����.
	FMovieSceneSequencePlaybackSettings LevelSequencePlaySettings;
	LevelSequencePlaySettings.PlayRate = 0.6f; // ��� �ӵ� ���߱�.
	LevelSequencePlaySettings.bDisableMovementInput = true; // ��� ���� ĳ���� ��ǲ ���� �� ���� ����.

	//������ ���Ͱ� null�� �ƴϰ� ������ �÷��̾ null�϶� �÷��̾� ����.
	if (BossStartSequenceActor && SequencePlayer == nullptr)
	{
		//�÷��̾ CD�ֱ� �۾�.
		SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), BossStartSequenceActor->GetSequence(), LevelSequencePlaySettings, BossStartSequenceActor);
	}
	
	if (SequencePlayer)
	{
		//���� �������� ������ ���� ��������Ʈ�� �Լ� ���ε�.
		//SequencePlayer->OnFinished.AddDynamic(this, &AMyBossEntrance::OnStartCinematicFinished);
		SequencePlayer->OnStop.AddDynamic(this, &AMyBossEntrance::OnStartCinematicFinished);
	}  

	//PlayerCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision); //�ӽ÷� �ݸ��� ��ȿȭ. (�׽�Ʈ��)

}

void AMyBossEntrance::OnStartCinematicFinished()
{
	// �÷��̾� ��Ʈ�ѷ����� �����뿡 ���������� �˸�.
	AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());
	if (MyPlayerController)
	{
		MyPlayerController->OnBossFightStart().Broadcast(BossToFight);
	}
}

void AMyBossEntrance::OnPlayerCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//�ݸ����� �������� ���Ͱ� �÷��̾���,
	AMyPlayer* OverlappedPlayer = Cast<AMyPlayer>(OtherActor);
	if (OverlappedPlayer)
	{
		//�ó׸�ƽ�� ����ϱ� �� ���͸� ����.
		OverlappedPlayer->MyPlayerController->OnLevelSequenceStart().Broadcast();

		//���� �������� �����. 
		if (SequencePlayer)
		{
			SequencePlayer->Play();
		}
		
	}

	PlayerCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision); // �ѹ� �� �ݸ����� ��Ȱ��ȭ.
}


