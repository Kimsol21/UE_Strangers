// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "MyAnimInstance.h"

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


	GetCharacterMovement()->JumpZVelocity = 800.0f; //점프 높이 지정
	bIsAttacking = false;//공격중이 아님으로 초기화.
	MaxCombo = 4; //최대 콤보 지정.
	AttackEndComboState(); //기타 AttackCombo관련 초기값 지정.

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

void AMyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	MyAnim = Cast<UMyAnimInstance>(GetMesh()->GetAnimInstance());
	if (nullptr == MyAnim)
	{
		UE_LOG(LogTemp, Error, TEXT("MyAnim is null!"));
	}
	MyAnim->OnMontageEnded.AddDynamic(this, &AMyCharacter::OnAttackMontageEnded); //AnimInstance의 델리게이트 OnMontageEnded에 My함수 바인딩.


	//OnNextAttackCheck 델리게이트에 바인딩할 함수를 람다함수로 간편하게 구현.
	//AnimInstance의 노티파이함수에서 BroadCast한 델리게이트에 의해 다음 람다함수가 실행.
	MyAnim->OnNextAttackCheck.AddLambda([this]() -> void {
		bCanNextCombo = false;
		if (bIsComboInputOn) //공격인풋이 들어왔을 시, 다음콤보 진행.
		{
			AttackStartComboState();//bCanNextCombo true로 바꿔주고, CurrentCombo+1해주는 함수.
			MyAnim->JumpToAttackMontageSection(CurrentCombo);//다음 콤보의 몽타주섹션 재생.
		}
		});
	
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
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);//캐릭터 무브먼트 컴포넌트에 내장되어있다.
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AMyCharacter::Attack);

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

void AMyCharacter::Attack()
{
	if (bIsAttacking)//현재 공격중이면,
	{
		if(bCanNextCombo) //다음콤보를 실행할 수 있다면
		{
			bIsComboInputOn = true; //콤보인풋 입력여부를 true로 바꿔준다.
		}
	}
	else//현재 공격중이 아니면,
	{
		AttackStartComboState();//다음 콤보로 진행가능하게 하고, 콤보+1해주는 함수
		MyAnim->PlayAttackMontage(); //공격 몽타주 재생.
		MyAnim->JumpToAttackMontageSection(CurrentCombo);//Current콤보의 몽타주 섹션 재생.
		bIsAttacking = true;//공격중임을 알림.
	}
}

void AMyCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)//AnimInstance에서 제공하는 OnMontageEnded델리게이트에 바인딩된 함수.
{
	if (bIsAttacking || CurrentCombo > 0)
	{
		UE_LOG(LogTemp, Error, TEXT("OnAttackMontageEnded Error"));
	}
	bIsAttacking = false;//공격이 끝났음을 알림.
	AttackEndComboState();//콤보 초기화, 변수초기화.
}

void AMyCharacter::AttackStartComboState()
{
	bCanNextCombo = true;
	bIsComboInputOn = false;
	CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo); //CurrentCombo+1이 1과 MaxCombo까지의 범위를 벗어나지 않게 한다.
}

void AMyCharacter::AttackEndComboState()
{
	bIsComboInputOn = false;
	bCanNextCombo = false;
	CurrentCombo = 0; //콤보공격 끝, 콤보 0으로 초기화.
}

