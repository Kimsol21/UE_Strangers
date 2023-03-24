// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/MyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "MyPlayer.generated.h"

/**
 * 
 */

DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate); //���� ������ ȣ���� ��������Ʈ

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


	

	void SetEXP(float _NewEXP);

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override; //Actor ������Ʈ�� ������ �ʱ�ȭ�� ���� ȣ��ȴ�.���� �÷��� �߿��� ȣ��
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;//���Ͱ� ���� ������� ó���ϴ� ������ �߰��ϱ� ���� �������̵�.

	bool HasAnyWeapon();
	void SetWeapon(class AMyWeapon* NewWeapon);


	UPROPERTY(VisibleAnywhere, Category = Weapon)
	class AMyWeapon* CurrentWeapon;

	UPROPERTY(VisibleAnywhere, Category = Inventory)
		class UInventoryComponent* MyInventory;

	UPROPERTY(VisibleAnywhere, Category = Stat)
		class UMyPlayerStatComponent* MyStat;

	UPROPERTY(VisibleAnywhere, Category = Camera)
		USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
		UCameraComponent* Camera;

	void Attack();
	FOnAttackEndDelegate OnAttackEnd;

	void UpDown(float NewAxisValue);
	void LeftRight(float NewAxisValue);
	void LookUp(float NewAxisValue);
	void Turn(float NewAxisValue);
	void ZoomIn();
	void ZoomOut();


private:

	void CheckForInteractables();

	

	UFUNCTION()
		void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);//AnimInstance�� OnMontageEnded ��������Ʈ�� ���ε��� �Լ�.

	void AttackStartComboState(); //������ ������ �� ���� �Ӽ� ����.
	void AttackEndComboState();//������ �����Ҷ� ���.

private:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = SpringArm, Meta = (AllowPrivateAccess = true))
		FVector ExpectedAttackMovePos; //�̼����� ��ǥ��ġ.

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = SpringArm, Meta = (AllowPrivateAccess = true))
		bool bCanZoom; //�������� �� ��������

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = SpringArm, Meta = (AllowPrivateAccess = true))
		bool bCanAttackMove; //AttackMove�������� ��������

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = SpringArm, Meta = (AllowPrivateAccess = true))
		float ExpectedSpringArmLength; //�������� ���� ��ǥ��.

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float AttackMoveImpulse;//�̼������� ���� ��

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool bIsAttacking;//���� ���������� �Ǵ�.

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool bCanNextCombo; //���� �޺����� �̵� ���� ����

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool bIsComboInputOn; //�޺� �Է¿���

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		int32 CurrentCombo; //���� ���� ���� �޺� ī����

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		int32 MaxCombo; //�޺� ī������ �ִ�ġ

	UPROPERTY()
		class UMyAnimInstance* MyAnim; //ĳ����Ŭ�������� �ִ� �ν��Ͻ��� ���� ����ϹǷ� ��������� �����Ѵ�.
};
