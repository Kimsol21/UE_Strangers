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

	//박스 오브젝트 생성 (콜라이더, 스태틱메쉬) & 파티클
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));
	Box = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BOX"));
	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EFFECT"));

	RootComponent = Trigger;
	Box->SetupAttachment(RootComponent);
	Effect->SetupAttachment(RootComponent);

	Trigger->SetBoxExtent(FVector(40.0f, 42.0f, 30.0f));

	//스태틱메쉬 에셋 불러온 후 적용
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BOX(TEXT("StaticMesh'/Game/InfinityBladeGrassLands/Environments/Breakables/StaticMesh/Box/SM_Env_Breakables_Box1.SM_Env_Breakables_Box1'"));
	if (SM_BOX.Succeeded())
	{
		Box->SetStaticMesh(SM_BOX.Object);
	}

	Box->SetRelativeLocation(FVector(0.0f, -3.5f, -30.0f));

	//파티클 에셋 불러온 후 적용
	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_CHESTOPEN(TEXT("ParticleSystem'/Game/InfinityBladeGrassLands/Effects/FX_Treasure/Chest/P_TreasureChest_Open_Mesh.P_TreasureChest_Open_Mesh'"));
	if (P_CHESTOPEN.Succeeded())
	{
		Effect->SetTemplate(P_CHESTOPEN.Object);
		Effect->bAutoActivate = false;
	}

	Effect->SetRelativeLocation(FVector(-40.0f, 0.0f, -25.0f));
	Effect->SetRelativeScale3D(FVector(1.0, 0.6, 1.0));

	//콜리전 설정
	Trigger->SetCollisionProfileName(TEXT("ItemBox"));
	Box->SetCollisionProfileName(TEXT("NoCollision"));

	//기본 클래스 값 지정
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

	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AMyItemBox::OnCharacterOverlap); //델리게이트 바인딩
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
		auto NewWeapon = GetWorld()->SpawnActor<AMyWeapon>(WeaponItemClass, FVector::ZeroVector, FRotator::ZeroRotator); //무기액터 월드에 소환후
		MyPlayer->SetWeapon(NewWeapon); //캐릭터에 장착

		Effect->Activate(true); //파티클 재생
		SetActorEnableCollision(false); //더이상 아이템 획득 못하도록 콜리전 봉인
		Effect->OnSystemFinished.AddDynamic(this, &AMyItemBox::OnEffectFinished); //델리게이트로 함수바인딩.
	}
}

void AMyItemBox::OnEffectFinished(UParticleSystemComponent* PSystem)
{
	Destroy();
}



