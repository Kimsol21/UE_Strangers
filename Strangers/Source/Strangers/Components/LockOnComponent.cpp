// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/LockOnComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"


// Sets default values for this component's properties
ULockOnComponent::ULockOnComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	//���� Circle UI
	LockOnWidget = CreateDefaultSubobject <UWidgetComponent>(TEXT("LockOnWidget")); //���� ���� ����.
	LockOnWidget->SetWidgetSpace(EWidgetSpace::Screen); //�׻� ��ũ�� �ٶ󺸵��� ����.

	static ConstructorHelpers::FClassFinder<UUserWidget> UI_LOCKON(TEXT("WidgetBlueprint'/Game/UI/Monster/WG_LockOn.WG_LockOn_C'")); //���� Ŭ���� ã�Ƽ� ����.
	if (UI_LOCKON.Succeeded())
	{
		LockOnWidget->SetWidgetClass(UI_LOCKON.Class);
		LockOnWidget->SetDrawSize(FVector2D(35.0f, 35.0f)); 
	}
	LockOnWidget->SetVisibility(false); //���� ���ü� ����.
	LockOnWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 110.0f)); //���� ��ġ ����.

	if (ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()))
	{
		LockOnWidget->SetupAttachment(OwnerCharacter->GetMesh());
	}

	bIsLockOn = false;

	// ...
}


void ULockOnComponent::InitializeComponent()
{
	Super::InitializeComponent();

	
}

// Called when the game starts
void ULockOnComponent::BeginPlay()
{
	Super::BeginPlay();

	//GetOwner()->GetActorLocation
	// ...
	

	OnLockOnExcuteDelegate.AddLambda([this]()->void {
		LockOnWidget->SetVisibility(true); //���� ���ü� ����.
		bIsLockOn = true;
		});

	OnLockOnRemoveDelegate.AddLambda([this]()->void {
		LockOnWidget->SetVisibility(false); //���� ���ü� ����.
		bIsLockOn = false;
		});
}


