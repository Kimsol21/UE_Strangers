// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Item_Interactable.h"
#include "Manager/MyGameInstance.h"
#include "Character/Player/MyPlayer.h"
#include "Inventory/InventoryComponent.h"

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

	if (!SetItemData())
	{
		UE_LOG(LogTemp, Error, TEXT("게임 인스턴스를 불러오지 못했습니다. 아이템 데이터를 Set 할 수 없습니다."));
	}


	////스태틱메쉬 에셋 불러온 후 적용
	//static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_ITEM( *ItemDataTable->Mesh );
	//if (SM_ITEM.Succeeded())
	//{
	//	ItemMesh->SetStaticMesh(SM_ITEM.Object);
	//}
	
}

void AItem_Interactable::Interact(APlayerController* Controller)
{
	if (AMyPlayer* p = Cast<AMyPlayer>(Controller->GetCharacter()))
	{
		p->MyInventory->AddItem(this); //컨트롤러에서 캐릭터의 인벤토리 컴포넌트 불러와서 그곳에 이 아이템 주솟값 저장하기.
	}
}

FString AItem_Interactable::GetUseText() const //여기서의 const는 해당 함수가 속해있는 멤버변수의 값을 바꾸지 않겠다는 의미이다. 
{ 
	return *FString::Printf(TEXT("%s : Press E to %s"), *ItemDataTable->Name, *ItemDataTable->Action) ;
}

bool AItem_Interactable::SetItemData()
{

	if (UMyGameInstance* GI = Cast<UMyGameInstance>(GetWorld()->GetGameInstance())) //게임 인스턴스 가져오기.
	{
		ItemDataTable = GI->GetItemData(ID); //데이터 테이블 가져오기. 
		return true;
	}
	else return false;

}


