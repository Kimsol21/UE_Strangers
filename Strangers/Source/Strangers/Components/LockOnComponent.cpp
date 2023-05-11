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

	//락온 Circle UI
	LockOnWidget = CreateDefaultSubobject <UWidgetComponent>(TEXT("LockOnWidget")); //락온 위젯 생성.
	LockOnWidget->SetWidgetSpace(EWidgetSpace::Screen); //항상 스크린 바라보도록 설정.

	static ConstructorHelpers::FClassFinder<UUserWidget> UI_LOCKON(TEXT("WidgetBlueprint'/Game/UI/Monster/WG_LockOn.WG_LockOn_C'")); //위젯 클래스 찾아서 적용.
	if (UI_LOCKON.Succeeded())
	{
		LockOnWidget->SetWidgetClass(UI_LOCKON.Class);
		LockOnWidget->SetDrawSize(FVector2D(35.0f, 35.0f)); 
	}
	LockOnWidget->SetVisibility(false); //위젯 가시성 설정.
	LockOnWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 110.0f)); //위젯 위치 설정.

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
		LockOnWidget->SetVisibility(true); //위젯 가시성 설정.
		bIsLockOn = true;
		});

	OnLockOnRemoveDelegate.AddLambda([this]()->void {
		LockOnWidget->SetVisibility(false); //위젯 가시성 설정.
		bIsLockOn = false;
		});
}


