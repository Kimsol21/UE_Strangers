// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/MyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "MyPlayer.generated.h"

class UInventoryComponent;

/**
 * 
 */



UCLASS()
class STRANGERS_API AMyPlayer : public AMyCharacter
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	AMyPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void AttackCheck() override;

	virtual void SetDamage(float _Damage) override;
	virtual float GetAttackPower() override;


public:
	class AMyPlayerController* MyPlayerController;

	UInventoryComponent* GetMyInventoryComponent() const{ return MyInventory; };
	

	void SetEXP(float _NewEXP);

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override; //Actor 컴포넌트가 완전히 초기화된 이후 호출된다.게임 플레이 중에만 호출
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;//액터가 받은 대미지를 처리하는 로직을 추가하기 위해 오버라이드.

	bool HasAnyWeapon();
	void SetWeapon(class AMyWeapon* NewWeapon);


	UPROPERTY(VisibleAnywhere, Category = Weapon)
	class AMyWeapon* CurrentWeapon;

	

	UPROPERTY(VisibleAnywhere, Category = Stat)
		class UMyPlayerStatComponent* MyStat;

	UPROPERTY(VisibleAnywhere, Category = Camera)
		USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
		UCameraComponent* Camera;

	void Attack();
	

	void UpDown(float NewAxisValue);
	void LeftRight(float NewAxisValue);
	void LookUp(float NewAxisValue);
	void Turn(float NewAxisValue);
	void ZoomIn();
	void ZoomOut();
	void Roll();


private:

	void CheckForInteractables();

	UPROPERTY(VisibleAnywhere, Category = Inventory)
	UInventoryComponent* MyInventory;

	void AttackStartComboState(); //공격이 시작할 때 관련 속성 지정.

	void MoveForward();

private:
	float MyMoveSpeed = 2.0f;

	UPROPERTY(VisibleAnywhere, Category = Movement)
		bool bCanMove = true; //움직일 수 있는지

	UPROPERTY(VisibleAnywhere, Category = Attack)
		bool bIsInvincible = false; //무적인지

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = SpringArm, Meta = (AllowPrivateAccess = true))
		FVector ExpectedAttackMovePos; //미세전진 목표위치.

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = SpringArm, Meta = (AllowPrivateAccess = true))
		bool bCanZoom; //선형보간 줌 가능한지

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = SpringArm, Meta = (AllowPrivateAccess = true))
		bool bCanMyMove; //AttackMove선형보간 가능한지

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = SpringArm, Meta = (AllowPrivateAccess = true))
		float ExpectedSpringArmLength; //스프링암 길이 목표값.

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float AttackMoveImpulse;//미세전진에 가할 힘

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool bIsAttacking;//현재 공격중인지 판단.

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool bCanNextCombo; //다음 콤보로의 이동 가능 여부

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool bIsComboInputOn; //콤보 입력여부

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		int32 CurrentCombo; //현재 실행 중인 콤보 카운터

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		int32 MaxCombo; //콤보 카운터의 최대치

	UPROPERTY()
		class UMyAnimInstance* MyAnim; //캐릭터클래스에서 애님 인스턴스는 자주 사용하므로 멤버변수로 선언한다.
};
