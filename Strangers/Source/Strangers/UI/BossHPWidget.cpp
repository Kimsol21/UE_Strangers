// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BossHPWidget.h"
#include "Character/Boss/MyBoss.h"
#include "Components/ProgressBar.h"

void UBossHPWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//이름을 통해 ProgressBar 위젯 받아오기.
	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PGBar_BossHPBar")));
	if (nullptr == HPProgressBar) return;

	HPProgressBar_SlowVer = Cast<UProgressBar>(GetWidgetFromName(TEXT("PGBar_BossHPBar_SlowVer")));
	if (nullptr == HPProgressBar_SlowVer) return;


	
	HPProgressBar->SetPercent(1.0f);//처음 HP는 100퍼센트로 설정.
	HPProgressBar_SlowVer->SetPercent(1.0f);


	bNeedInterpolation = false; //HP바 보간이 필요한지는 false로 설정.
	CurrentHPRatio = 1.0f;
	CurrentSlowHPRatio = CurrentHPRatio;
}

void UBossHPWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (bNeedInterpolation)
	{
		CurrentSlowHPRatio = FMath::FInterpConstantTo(CurrentSlowHPRatio, CurrentHPRatio, InDeltaTime, 0.2f); // 보간값 변수에 저장.

		if (FMath::Abs(CurrentHPRatio - CurrentSlowHPRatio) < KINDA_SMALL_NUMBER) //미세오차 미만으로 보간값이 줄어들면 보간종료.
		{
			CurrentSlowHPRatio = CurrentHPRatio; //보간 완료값 저장.
			
			bNeedInterpolation = false; //보간 끝나면 false처리.
		}
		HPProgressBar_SlowVer->SetPercent(CurrentSlowHPRatio); // 보간값 실제 UI에 반영시켜주기.
		
	}
	
}

void UBossHPWidget::BindWidgetToBoss(AMyBoss* NewBoss)
{
	if (nullptr == NewBoss) return;
	
	CurrentBoss = NewBoss;
	CurrentBoss->OnBossHPChanged().AddLambda([this](float HPRatio)->void {
		HPProgressBar->SetPercent(HPRatio); //이벤트로 변경된 HP 비율 받아온다음 UI에 반영시키기.
		bNeedInterpolation = true;
		CurrentHPRatio = HPRatio; // 보간 목표값에 최근HP값 저장.
		});
}


