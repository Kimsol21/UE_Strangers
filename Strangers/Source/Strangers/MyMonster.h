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
	virtual void PostInitializeComponents() override; //Actor ������Ʈ�� ������ �ʱ�ȭ�� ���� ȣ��ȴ�.���� �÷��� �߿��� ȣ��
	

private:
	UPROPERTY()
	class UMonster_AnimInstance* MonsterAnim; //ĳ����Ŭ�������� �ִ� �ν��Ͻ��� ���� ����ϹǷ� ��������� �����Ѵ�.

};
