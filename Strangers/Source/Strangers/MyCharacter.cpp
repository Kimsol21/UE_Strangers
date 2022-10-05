// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//언리얼 오브젝트 생성 및 세부사항 설정.
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0, -90.0f, 0.0f));
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));


	//에셋 불러오기.
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM_KWANG(TEXT("SkeletalMesh'/Game/Assets/ParagonKwang/Characters/Heroes/Kwang/Meshes/KwangRosewood.KwangRosewood'"));
	if (SM_KWANG.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SM_KWANG.Object);
	}
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance> ABP_KWANG(TEXT("AnimBlueprint'/Game/Animations/Kwang_AnimBlueprint.Kwang_AnimBlueprint_C'")); //클래스정보 가져오기.
	if (ABP_KWANG.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(ABP_KWANG.Class);
	}

	//GTA식 ControlMode 설정.
	SpringArm->TargetArmLength = 450.0f;
	SpringArm->SetRelativeRotation(FRotator::ZeroRotator);
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritRoll = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bDoCollisionTest = true;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;//캐릭터가 움직이는 방향으로 캐릭터를 자동 회전시켜주는 기능.
	GetCharacterMovement()->bUseControllerDesiredRotation = false;//컨트롤 회전이 가리키는 방향으로 캐릭터가 부드럽게 회전.
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);

}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 설정한 입력 세팅을 Pawn의 함수와 Binding시키기.
	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AMyCharacter::UpDown); //TEXT 안의 값은 입력 세팅의 이름이다.
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AMyCharacter::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AMyCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AMyCharacter::Turn);

}

void AMyCharacter::UpDown(float NewAxisValue)
{
	AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::X), NewAxisValue);//언리얼 엔진에서 시선방향은 X축, 우측 방향은 Y축을 의미한다.
}

void AMyCharacter::LeftRight(float NewAxisValue)
{
	AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::Y), NewAxisValue);//좌우로 폰이동.
}
 void AMyCharacter::LookUp(float NewAxisValue)
{
	AddControllerPitchInput(NewAxisValue); 
}

void AMyCharacter::Turn(float NewAxisValue)
{
	AddControllerYawInput(NewAxisValue);//마우스 입력 신호 값을 PlayerController의 ControlRotation값으로 변환하는 명령.
}

