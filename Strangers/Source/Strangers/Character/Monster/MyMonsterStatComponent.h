// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyMonsterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpIsZeroDelegate); //델리게이트 매크로 선언
DECLARE_MULTICAST_DELEGATE(FOnHpChangedDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STRANGERS_API UMyMonsterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMyMonsterStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

public:
	UFUNCTION(BlueprintCallable)
	void SetNewMonster(int32 ID); //몬스터ID는 이 함수를 통해서만 값변경을 할 수 있게 한다.

	void SetHP(float NewHP); //HP설정
	void SetDamage(float NewDamage); //MyMonster의 TakeDamage함수에서 호출.

	float GetAttackPower(); //공격력 내보내기
	float GetEXPGive();//경험치 내보내기
	float GetHPRatio(); //현재 HP비율 내보내기
	float GetMoveSpeed(); //현재 이동속도 내보내기

	FOnHpIsZeroDelegate OnHPIsZero; //델리게이트 선언
	FOnHpChangedDelegate OnHPChanged;

private:

	struct FMyMonsterData* CurrentStatData = nullptr;

	UPROPERTY(EditInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	int32 ID;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	float CurrentHP;

		
};
