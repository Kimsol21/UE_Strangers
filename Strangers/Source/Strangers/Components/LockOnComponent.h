// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LockOnComponent.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnLockOnExcuteDelegate);
DECLARE_MULTICAST_DELEGATE(FOnLockOnRemoveDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STRANGERS_API ULockOnComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULockOnComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//Actor ������Ʈ�� ������ �ʱ�ȭ�� ���� ȣ��ȴ�.���� �÷��� �߿��� ȣ��
	virtual void InitializeComponent() override;

public:	
	//��������Ʈ Get�Լ�.
	FOnLockOnExcuteDelegate& OnLockOnExcute() { return OnLockOnExcuteDelegate; };
	FOnLockOnRemoveDelegate& OnLockOnRemove() { return OnLockOnRemoveDelegate; };

	bool GetIsLockOn() const { return bIsLockOn; };
private:
	//���� UI������ ���� ������ ���� ����.
	UPROPERTY(VisibleAnywhere, Category = UI)
	class UWidgetComponent* LockOnWidget;

	//��������Ʈ ����.
	FOnLockOnExcuteDelegate OnLockOnExcuteDelegate;
	FOnLockOnRemoveDelegate OnLockOnRemoveDelegate;

	//���� ���µǾ��ִ����� ���� ����.
	bool bIsLockOn;

		
};
