// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/MyPlayerSkillComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/Character.h"
#include "Character/Player/MyPlayer.h"

// Sets default values for this component's properties
UMyPlayerSkillComponent::UMyPlayerSkillComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//HP Up 이펙트
	HPUpEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("HPUP_EFFECT"));
	
	//static ConstructorHelpers::FObjectFinder<UParticleSystem> P_HPUP(TEXT("ParticleSystem'/Game/FX/P_Heal_Target.P_Heal_Target'"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_HPUP(TEXT("ParticleSystem'/Game/FX/Effect_EnemySpawn.Effect_EnemySpawn'"));
	if (P_HPUP.Succeeded())
	{
		HPUpEffect->SetTemplate(P_HPUP.Object);
		HPUpEffect->bAutoActivate = false;
	}

	if (ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()))
	{
		HPUpEffect->SetupAttachment(OwnerCharacter->GetMesh());
	}
	HPUpEffect->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	HPUpEffect->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.45f));

	Skill1_Cooltime = 5.0f;
	Skill1_CurrentCooltime = Skill1_Cooltime;
	
	Skill2_Cooltime = 10.0f;
	Skill2_CurrentCooltime = Skill2_Cooltime;

	Skill3_Cooltime = 10.0f;
	Skill3_CurrentCooltime = Skill3_Cooltime;
	

	bCanSkill1Activate = true;
	bCanSkill2Activate = true;
	bCanSkill3Activate = true;
	
	SetComponentTickInterval(1.0f); // 1초마다 Tick실행.
	// ...
}


// Called when the game starts
void UMyPlayerSkillComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerPlayer = Cast<AMyPlayer>(GetOwner());
	if (!OwnerPlayer) return;

	OwnerPlayer->OnStartDrinkPotion.AddUObject(this, &UMyPlayerSkillComponent::ExecuteSkill_1);
	// ...
	
}

void UMyPlayerSkillComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	//Super::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction);

	if (!bCanSkill1Activate)
	{
		Skill1_CurrentCooltime++; // 쿨타임값 변경.
		
		if (Skill1_CurrentCooltime == Skill1_Cooltime)// 쿨타임이 다 지나면 다시 스킬 Active.
		{
			bCanSkill1Activate = true; 
		}
		OnSkillCooltimeValueChangeEvent.Broadcast(1); //쿨타임값 변경을 알리는 이벤트.
	}

	if (!bCanSkill2Activate)
	{
		Skill2_CurrentCooltime++; // 쿨타임값 변경.
		
		if (Skill2_CurrentCooltime == Skill1_Cooltime)
		{
			bCanSkill2Activate = true; // 쿨타임이 다 지나면 다시 스킬 Active.
		}
		OnSkillCooltimeValueChangeEvent.Broadcast(2); //쿨타임값 변경을 알리는 이벤트.
	}

	if (!bCanSkill3Activate)
	{
		Skill3_CurrentCooltime++; // 쿨타임값 변경.
		
		if (Skill3_CurrentCooltime == Skill1_Cooltime)
		{
			bCanSkill3Activate = true; // 쿨타임이 다 지나면 다시 스킬 Active.
		}
		OnSkillCooltimeValueChangeEvent.Broadcast(3); //쿨타임값 변경을 알리는 이벤트.
	}

}

bool UMyPlayerSkillComponent::GetSkillCanActivate(uint8 _SkillID)
{
	bool result;
	switch (_SkillID)
	{
	case 1:
		result = bCanSkill1Activate;
		break;
	case 2:
		result = bCanSkill2Activate;
		break;
	case 3:
		result = bCanSkill3Activate;
	default:
		break;
	}

	return result;
}

void UMyPlayerSkillComponent::ExecuteSkill_1()
{
	HPUpEffect->Activate(true);
	bCanSkill1Activate = false; //쿨타임 시작.
	Skill1_CurrentCooltime = 0.0f;
}

void UMyPlayerSkillComponent::ExecuteSkill_2()
{
	bCanSkill2Activate = false;
	Skill2_CurrentCooltime = 0.0f;
}

void UMyPlayerSkillComponent::ExecuteSkill_3()
{
	bCanSkill3Activate = false;
	Skill3_CurrentCooltime = 0.0f;
}


float UMyPlayerSkillComponent::GetCooltimeRatio(uint8 _SkillID)
{
	float result = 0.0f;

	switch (_SkillID)
	{
	case 1:
		result = Skill1_CurrentCooltime / Skill1_Cooltime;
		break;
	case 2:
		result = Skill2_CurrentCooltime / Skill2_Cooltime;
		break;
	case 3:
		result = Skill3_CurrentCooltime / Skill3_Cooltime;
		break;
	default:
		break;
	}

	return 1.0f-result;
}

FText UMyPlayerSkillComponent::GetCooltimeText(uint8 _SkillID)
{
	FText result = FText::FromString("");

	switch (_SkillID)
	{
	case 1:
		if (Skill1_Cooltime== Skill1_CurrentCooltime) break;
		result = FText::AsNumber(Skill1_Cooltime-Skill1_CurrentCooltime);
		break;
	case 2:
		if (Skill2_Cooltime == Skill2_CurrentCooltime) break;
		result = FText::AsNumber(Skill2_Cooltime-Skill2_CurrentCooltime);
		break;
	case 3:
		if (Skill3_Cooltime == Skill3_CurrentCooltime) break;
		result = FText::AsNumber(Skill3_Cooltime-Skill3_CurrentCooltime);
		break;
	default:
		break;
	}

	return result;
}

