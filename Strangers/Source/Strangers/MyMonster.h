// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyCharacter.h"
#include "Monster_AnimInstance.h"
#include "MyMonster.generated.h"

/**
 * 
 */
UCLASS()
class STRANGERS_API AMyMonster : public AMyCharacter
{
	GENERATED_BODY()

public:
	AMyMonster();
	void Attack();

	UPROPERTY(VisibleAnywhere, Category = UI)
		class UWidgetComponent* HPBarWidget;

	UPROPERTY(VisibleAnywhere, Category = Stat)
		class UMyMonsterStatComponent* MonsterStat;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override; //Actor 컴포넌트가 완전히 초기화된 이후 호출된다.게임 플레이 중에만 호출
	

private:
	UPROPERTY()
	class UMonster_AnimInstance* MonsterAnim; //캐릭터클래스에서 애님 인스턴스는 자주 사용하므로 멤버변수로 선언한다.

};
