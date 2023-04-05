// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Item_Interactable.h"
#include "Manager/MyGameInstance.h"
#include "Character/Player/MyPlayer.h"
#include "Inventory/InventoryComponent.h"
#include "Character/Player/MyPlayerController.h"

// Sets default values
AItem_Interactable::AItem_Interactable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ITEM"));
	RootComponent = ItemMesh;
	
	//디폴트 스태틱메쉬 에셋 불러온 후 적용
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_ITEM(TEXT("StaticMesh'/Game/InfinityBladeGrassLands/Environments/Misc/Exo_Deco01/StaticMesh/SM_WoodenBarrel_Damaged.SM_WoodenBarrel_Damaged'"));
	if (SM_ITEM.Succeeded())
	{
		ItemMesh->SetStaticMesh(SM_ITEM.Object);
	}

	ItemMesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));

	

}

// Called when the game starts or when spawned
void AItem_Interactable::BeginPlay()
{
	Super::BeginPlay();

	if (SetItemData())
	{
		//데이터 테이블에서 로드할 Icon의 경로정보를 가져와 FString에 저장합니다. 
		FString TexturePath = ItemData->Icon;

		//경로정보를 바탕으로 FSoftObjectPath를 생성합니다.
		FSoftObjectPath SoftRef = FSoftObjectPath(TexturePath);

		//FSoftObjectPath를 로드하여 UTexture2D로 캐스팅합니다.
		IconTexture = Cast<UTexture2D>(SoftRef.TryLoad());
	}

}

void AItem_Interactable::Interact(AMyPlayerController* Controller)
{
	if (Controller)
	{
		Controller->GetPossessedPawn()->GetMyInventoryComponent()->AddItem(this);
	}
}

FString AItem_Interactable::GetUseText() const //여기서의 const는 해당 함수가 속해있는 멤버변수의 값을 바꾸지 않겠다는 의미이다. 
{ 
	return *FString::Printf(TEXT("%s : Press E to %s"), *ItemData->Name, *ItemData->Action) ;
}

bool AItem_Interactable::SetItemData()
{

	if (UMyGameInstance* GI = Cast<UMyGameInstance>(GetWorld()->GetGameInstance())) //게임 인스턴스 가져오기.
	{
		ItemData = GI->GetItemData(ID); //데이터 테이블 가져오기. 
		return true;
	}
	else return false;

}


