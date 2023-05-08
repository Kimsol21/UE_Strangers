// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyGate.generated.h"

UCLASS()
class STRANGERS_API AMyGate : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyGate();

protected:
	virtual void PostInitializeComponents() override; //Actor ������Ʈ�� ������ �ʱ�ȭ�� ���� ȣ��ȴ�.���� �÷��� �߿��� ȣ��
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	//Collision OverlapEvent�� �߻����� �� ���ε��� �Լ�.
	UFUNCTION()
	void OnPlayerCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void OpenGate();
	void CloseGate();
	

private:
	UPROPERTY(VisibleAnywhere, Category = "Entrance")
	class UBoxComponent* PlayerCollisionBox; // �÷��̾ ������ �ݸ���.

	UPROPERTY(VisibleAnywhere, Category = "Entrance")
	class UBoxComponent* BlockCollisionBox; // �÷��̾ ���� �ݸ���.

	UPROPERTY(EditAnywhere, Category = "Entrance", meta = (AllowPrivateAccess = "true"))
	uint8 PassableLevel;

	UPROPERTY(EditAnywhere, Category = "Entrance", meta = (AllowPrivateAccess = "true"))
	FString NoticeString;

	

};
