// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BossHPWidget.generated.h"

/**
 * 
 */
class AMyBoss;

UCLASS()
class STRANGERS_API UBossHPWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void BindWidgetToBoss(AMyBoss* NewBoss); //몬스터와 UI를 연결시킨다. 

protected:
	virtual void NativeConstruct() override; //AddToViewport 시 호출된다.
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

private:
	TWeakObjectPtr<AMyBoss> CurrentBoss; //약포인터 사용

	UPROPERTY()
	class UProgressBar* HPProgressBar; //HP ProgressBar UI.

	UPROPERTY()
	UProgressBar* HPProgressBar_SlowVer; // 천천히 줄어드는 HP바

	bool bNeedInterpolation; // HP 보간이 필요한지.
	float CurrentHPRatio; // 최근 HP비율값.
	float CurrentSlowHPRatio; //보간 HP 비율값.
};
