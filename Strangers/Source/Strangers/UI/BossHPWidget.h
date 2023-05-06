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
	void BindWidgetToBoss(AMyBoss* NewBoss); //���Ϳ� UI�� �����Ų��. 

protected:
	virtual void NativeConstruct() override; //AddToViewport �� ȣ��ȴ�.
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

private:
	TWeakObjectPtr<AMyBoss> CurrentBoss; //�������� ���

	UPROPERTY()
	class UProgressBar* HPProgressBar; //HP ProgressBar UI.

	UPROPERTY()
	UProgressBar* HPProgressBar_SlowVer; // õõ�� �پ��� HP��

	bool bNeedInterpolation; // HP ������ �ʿ�����.
	float CurrentHPRatio; // �ֱ� HP������.
	float CurrentSlowHPRatio; //���� HP ������.
};
