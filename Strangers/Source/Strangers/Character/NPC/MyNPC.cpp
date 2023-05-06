// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NPC/MyNPC.h"
#include "Manager/DialogueManagerComponent.h"
#include "Components/BoxComponent.h"
#include "Character/Player/MyPlayer.h"

// Sets default values
AMyNPC::AMyNPC()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//다이얼로그 컴포넌트 부착.
	DialogueManager = CreateDefaultSubobject<UDialogueManagerComponent>(TEXT("DIALOGUE"));

	//스켈레탈 메쉬 에셋 불러오기.
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM_DEFAULT(TEXT("SkeletalMesh'/Game/Animations/NPC/JapanGirl/Kachujin_G_Rosales.Kachujin_G_Rosales'"));
	if (SM_DEFAULT.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SM_DEFAULT.Object);
	}
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FRotator(0.0, -90.0f, 0.0f)); // Rotator ( pitch , yaw,  roll )

	//애님 블루프린트 에셋 불러오기.
	static ConstructorHelpers::FClassFinder<UAnimInstance> ABP_NPC(TEXT("AnimBlueprint'/Game/Animations/NPC/JapanGirl/AnimBP_JapanGirl.AnimBP_JapanGirl_C'")); //클래스정보 가져오기.
	if (ABP_NPC.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(ABP_NPC.Class);
	}

	//플레이어를 감지할 콜리전 설정.
	PlayerOverlapCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("PlayerOverlapCollision"));//생성.
	PlayerOverlapCollision->SetBoxExtent(FVector(76.0f, 31.0f, 53.0f));//범위.
	PlayerOverlapCollision->SetRelativeLocation(FVector(116.0f, 0.0f, 0.0f));
	PlayerOverlapCollision->SetCollisionProfileName(TEXT("ItemBox"));//콜리전 이름.
	PlayerOverlapCollision->SetupAttachment(GetCapsuleComponent());
}

void AMyNPC::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//콜리전 오버랩 이벤트에 함수 바인딩.
	PlayerOverlapCollision->OnComponentBeginOverlap.AddDynamic(this, &AMyNPC::OnPlayerCollisionBeginOverlap);
}


// Called when the game starts or when spawned
void AMyNPC::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMyNPC::OnPlayerCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//OnFoundPlayerEvent.Broadcast();
	AMyPlayer* MyPlayer = Cast<AMyPlayer>(OtherActor); // 감지된 플레이어 받아오기.
	if (MyPlayer)
	{
		MyPlayer->OnPlayerMeetNPC().Broadcast(this);
		DialogueManager->StartDialogue();
	}
	
	
}

// Called every frame
void AMyNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyNPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

