// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "MyPlayerSkillComponent.generated.h"


DECLARE_EVENT_OneParam(UMyPlayerSkillComponent, FOnSkillCooltimeValueChangeEvent, uint8);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STRANGERS_API UMyPlayerSkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMyPlayerSkillComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:	
	void ExecuteSkill_1();
	void ExecuteSkill_2();
	void ExecuteSkill_3();

	float GetCooltimeRatio(uint8 _SkillID);
	FText GetCooltimeText(uint8 _SkillID);

	FOnSkillCooltimeValueChangeEvent& OnSkillCooltimeValueChange() { return OnSkillCooltimeValueChangeEvent; };
	bool GetSkillCanActivate(uint8 _SkillID);
private:
	bool bCanSkill1Activate;
	bool bCanSkill2Activate;
	bool bCanSkill3Activate;
	FOnSkillCooltimeValueChangeEvent OnSkillCooltimeValueChangeEvent;

	UPROPERTY(VisibleAnywhere, Category = "Player")
	class AMyPlayer* OwnerPlayer;
	
	//HP회복 스킬 이펙트
	UPROPERTY(VisibleAnywhere, Category = Effect)
	UParticleSystemComponent* HPUpEffect;

	float Skill1_CurrentCooltime;
	float Skill1_Cooltime;

	float Skill2_CurrentCooltime;
	float Skill2_Cooltime;

	float Skill3_CurrentCooltime;
	float Skill3_Cooltime;

		
};
