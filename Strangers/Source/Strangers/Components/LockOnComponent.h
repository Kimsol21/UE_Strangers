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

	//Actor 컴포넌트가 완전히 초기화된 이후 호출된다.게임 플레이 중에만 호출
	virtual void InitializeComponent() override;

public:	
	//델리게이트 Get함수.
	FOnLockOnExcuteDelegate& OnLockOnExcute() { return OnLockOnExcuteDelegate; };
	FOnLockOnRemoveDelegate& OnLockOnRemove() { return OnLockOnRemoveDelegate; };

	bool GetIsLockOn() const { return bIsLockOn; };
private:
	//락온 UI위젯을 담을 포인터 변수 선언.
	UPROPERTY(VisibleAnywhere, Category = UI)
	class UWidgetComponent* LockOnWidget;

	//델리게이트 선언.
	FOnLockOnExcuteDelegate OnLockOnExcuteDelegate;
	FOnLockOnRemoveDelegate OnLockOnRemoveDelegate;

	//현재 락온되어있는지에 대한 변수.
	bool bIsLockOn;

		
};
