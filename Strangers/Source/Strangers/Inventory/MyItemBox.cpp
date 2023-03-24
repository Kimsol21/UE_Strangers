// Fill out your copyright notice in the Description page of Project Settings.


#include "MyItemBox.h"
#include "Inventory/MyWeapon.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/BoxComponent.h"
#include "Character/Player/MyPlayer.h"


// Sets default values
AMyItemBox::AMyItemBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//�ڽ� ������Ʈ ���� (�ݶ��̴�, ����ƽ�޽�) & ��ƼŬ
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));
	Box = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BOX"));
	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EFFECT"));

	RootComponent = Trigger;
	Box->SetupAttachment(RootComponent);
	Effect->SetupAttachment(RootComponent);

	Trigger->SetBoxExtent(FVector(40.0f, 42.0f, 30.0f));

	//����ƽ�޽� ���� �ҷ��� �� ����
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BOX(TEXT("StaticMesh'/Game/InfinityBladeGrassLands/Environments/Breakables/StaticMesh/Box/SM_Env_Breakables_Box1.SM_Env_Breakables_Box1'"));
	if (SM_BOX.Succeeded())
	{
		Box->SetStaticMesh(SM_BOX.Object);
	}

	Box->SetRelativeLocation(FVector(0.0f, -3.5f, -30.0f));

	//��ƼŬ ���� �ҷ��� �� ����
	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_CHESTOPEN(TEXT("ParticleSystem'/Game/InfinityBladeGrassLands/Effects/FX_Treasure/Chest/P_TreasureChest_Open_Mesh.P_TreasureChest_Open_Mesh'"));
	if (P_CHESTOPEN.Succeeded())
	{
		Effect->SetTemplate(P_CHESTOPEN.Object);
		Effect->bAutoActivate = false;
	}

	Effect->SetRelativeLocation(FVector(-40.0f, 0.0f, -25.0f));
	Effect->SetRelativeScale3D(FVector(1.0, 0.6, 1.0));

	//�ݸ��� ����
	Trigger->SetCollisionProfileName(TEXT("ItemBox"));
	Box->SetCollisionProfileName(TEXT("NoCollision"));

	//�⺻ Ŭ���� �� ����
	WeaponItemClass = AMyWeapon::StaticClass();

}

// Called when the game starts or when spawned
void AMyItemBox::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMyItemBox::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AMyItemBox::OnCharacterOverlap); //��������Ʈ ���ε�
}

void AMyItemBox::OnCharacterOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto MyPlayer = Cast<AMyPlayer>(OtherActor);

	if (nullptr != MyPlayer && nullptr != WeaponItemClass)
	{
		if (MyPlayer->HasAnyWeapon())
		{
			MyPlayer->CurrentWeapon->Destroy();
		}
		auto NewWeapon = GetWorld()->SpawnActor<AMyWeapon>(WeaponItemClass, FVector::ZeroVector, FRotator::ZeroRotator); //������� ���忡 ��ȯ��
		MyPlayer->SetWeapon(NewWeapon); //ĳ���Ϳ� ����

		Effect->Activate(true); //��ƼŬ ���
		SetActorEnableCollision(false); //���̻� ������ ȹ�� ���ϵ��� �ݸ��� ����
		Effect->OnSystemFinished.AddDynamic(this, &AMyItemBox::OnEffectFinished); //��������Ʈ�� �Լ����ε�.
	}
}

void AMyItemBox::OnEffectFinished(UParticleSystemComponent* PSystem)
{
	Destroy();
}



