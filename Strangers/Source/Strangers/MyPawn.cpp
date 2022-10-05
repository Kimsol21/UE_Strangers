// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPawn.h"

// Sets default values
AMyPawn::AMyPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	

	//언리얼 오브젝트 생성 및 세부사항 설정.
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CAPSULE"));
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MESH"));
	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MOVEMENT"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	RootComponent = Capsule;
	Mesh->SetupAttachment(Capsule);
	SpringArm->SetupAttachment(Capsule);
	Camera->SetupAttachment(SpringArm);

	Capsule->SetCapsuleHalfHeight(88.0f);
	Capsule->SetCapsuleRadius(34.0f);
	Mesh->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));

	//캐릭터 에셋 적용.
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SM_KWANG(TEXT("SkeletalMesh'/Game/Assets/ParagonKwang/Characters/Heroes/Kwang/Meshes/KwangRosewood.KwangRosewood'"));
	if (SM_KWANG.Succeeded())
	{
		Mesh->SetSkeletalMesh(SM_KWANG.Object);
	}

	//애니메이션 블루프린트 등록.
	Mesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance> WARRIOR_ANIM(TEXT("AnimBlueprint'/Game/Animations/Kwang_AnimBlueprint.Kwang_AnimBlueprint_C'")); //클래스정보 가져오기.
	if (WARRIOR_ANIM.Succeeded())
	{
		Mesh->SetAnimInstanceClass(WARRIOR_ANIM.Class);
	}
}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 설정한 입력 세팅을 Pawn의 함수와 Binding시키기.
	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AMyPawn::UpDown); //TEXT 안의 값은 입력 세팅의 이름이다.
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AMyPawn::LeftRight);

}

void AMyPawn::UpDown(float NewAxisValue)
{
	AddMovementInput(GetActorForwardVector(), NewAxisValue);//상하로 폰이동.
}

void AMyPawn::LeftRight(float NewAxisValue)
{
	AddMovementInput(GetActorRightVector(), NewAxisValue); //좌우로 폰이동.
}

