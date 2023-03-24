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
	
	//����Ʈ ����ƽ�޽� ���� �ҷ��� �� ����
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
		UE_LOG(LogTemp, Error, TEXT("���� �ν��Ͻ��� �ҷ����� ���߽��ϴ�. ������ �����͸� Set �� �� �����ϴ�."));
	}


	////����ƽ�޽� ���� �ҷ��� �� ����
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
		p->MyInventory->AddItem(this); //��Ʈ�ѷ����� ĳ������ �κ��丮 ������Ʈ �ҷ��ͼ� �װ��� �� ������ �ּڰ� �����ϱ�.
	}
}

FString AItem_Interactable::GetUseText() const //���⼭�� const�� �ش� �Լ��� �����ִ� ��������� ���� �ٲ��� �ʰڴٴ� �ǹ��̴�. 
{ 
	return *FString::Printf(TEXT("%s : Press E to %s"), *ItemDataTable->Name, *ItemDataTable->Action) ;
}

bool AItem_Interactable::SetItemData()
{

	if (UMyGameInstance* GI = Cast<UMyGameInstance>(GetWorld()->GetGameInstance())) //���� �ν��Ͻ� ��������.
	{
		ItemDataTable = GI->GetItemData(ID); //������ ���̺� ��������. 
		return true;
	}
	else return false;

}


