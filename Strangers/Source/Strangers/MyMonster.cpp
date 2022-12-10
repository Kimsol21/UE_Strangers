// Fill out your copyright notice in the Description page of Project Settings.


#include "MyMonster.h"
#include "Components/WidgetComponent.h"
#include "MyAIController.h"
#include "MyMonsterWidget.h"

AMyMonster::AMyMonster()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	HPBarWidget = CreateDefaultSubobject <UWidgetComponent>(TEXT("HPBARWIDGET")); //HP�� ���� ������Ʈ ����.
	HPBarWidget->SetupAttachment(GetMesh());

	

	//���� �ҷ�����.
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM_CORPSE(TEXT("SkeletalMesh'/Game/CityofBrass_Enemies/Meshes/Enemy/Corpse_Basic/Corpse_Basic.Corpse_Basic'"));
	if (SM_CORPSE.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SM_CORPSE.Object);
	}
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance> ABP_KWANG(TEXT("AnimBlueprint'/Game/Animations/Monster_AnimBlueprint.Monster_AnimBlueprint_C'")); //Ŭ�������� ��������.
	if (ABP_KWANG.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(ABP_KWANG.Class);
	}

	//���� ������Ʈ ��ġ ���� & ������ �������Ʈ �ּ��� Ŭ���� ������ ���� ������Ʈ�� widgetClass�� ���.
	HPBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 220.0f));
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);//�׻� �÷��̾� �ٶ󺸱�.

	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("WidgetBlueprint'/Game/UI/UI_HPBar.UI_HPBar_C'"));
	if (UI_HUD.Succeeded())
	{
		HPBarWidget->SetWidgetClass(UI_HUD.Class);
		HPBarWidget->SetDrawSize(FVector2D(150.0f, 50.0f));
	}

	//AI ���� �ɼ� ���� : ������ ��ġ�ϰų� ���Ӱ� �����Ǵ� MyMonster��ü���� MyAIController���� ����.
	AIControllerClass = AMyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCharacterMovement()->MaxWalkSpeed = 300.0f; //�̵��ӵ� ����

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("MyMonster"));//���� ���� �ݸ��� ������ ���.
}

void AMyMonster::BeginPlay()
{
	Super::BeginPlay();

	//ĳ���� ���� �޾ƿ���.
	auto CharacterWidget = Cast<UMyMonsterWidget>(HPBarWidget->GetUserWidgetObject());
	if (nullptr != CharacterWidget)
	{
		CharacterWidget->BindCharacterStat(MonsterStat);
	}

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0, 0.0f, 0.0f)); //�޽� ��ġ����.
}

void AMyMonster::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	MonsterAnim = Cast<UMonster_AnimInstance>(GetMesh()->GetAnimInstance());
	if (nullptr == MonsterAnim)
	{
		UE_LOG(LogTemp, Error, TEXT("MyAnim is null!"));
		return;
	}
	
}

void AMyMonster::Attack()
{
	if (MonsterAnim)
	{
		MonsterAnim->PlayAttackMontage(); //���� ��Ÿ�� ���.
	}
	
}
