// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BossHPWidget.h"
#include "Character/Boss/MyBoss.h"
#include "Components/ProgressBar.h"

void UBossHPWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//�̸��� ���� ProgressBar ���� �޾ƿ���.
	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PGBar_BossHPBar")));
	if (nullptr == HPProgressBar) return;

	HPProgressBar_SlowVer = Cast<UProgressBar>(GetWidgetFromName(TEXT("PGBar_BossHPBar_SlowVer")));
	if (nullptr == HPProgressBar_SlowVer) return;


	
	HPProgressBar->SetPercent(1.0f);//ó�� HP�� 100�ۼ�Ʈ�� ����.
	HPProgressBar_SlowVer->SetPercent(1.0f);


	bNeedInterpolation = false; //HP�� ������ �ʿ������� false�� ����.
	CurrentHPRatio = 1.0f;
	CurrentSlowHPRatio = CurrentHPRatio;
}

void UBossHPWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (bNeedInterpolation)
	{
		CurrentSlowHPRatio = FMath::FInterpConstantTo(CurrentSlowHPRatio, CurrentHPRatio, InDeltaTime, 0.2f); // ������ ������ ����.

		if (FMath::Abs(CurrentHPRatio - CurrentSlowHPRatio) < KINDA_SMALL_NUMBER) //�̼����� �̸����� �������� �پ��� ��������.
		{
			CurrentSlowHPRatio = CurrentHPRatio; //���� �Ϸᰪ ����.
			
			bNeedInterpolation = false; //���� ������ falseó��.
		}
		HPProgressBar_SlowVer->SetPercent(CurrentSlowHPRatio); // ������ ���� UI�� �ݿ������ֱ�.
		
	}
	
}

void UBossHPWidget::BindWidgetToBoss(AMyBoss* NewBoss)
{
	if (nullptr == NewBoss) return;
	
	CurrentBoss = NewBoss;
	CurrentBoss->OnBossHPChanged().AddLambda([this](float HPRatio)->void {
		HPProgressBar->SetPercent(HPRatio); //�̺�Ʈ�� ����� HP ���� �޾ƿ´��� UI�� �ݿ���Ű��.
		bNeedInterpolation = true;
		CurrentHPRatio = HPRatio; // ���� ��ǥ���� �ֱ�HP�� ����.
		});
}


