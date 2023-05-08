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
class AMyPlayerController;
class AMyNPC;

DECLARE_MULTICAST_DELEGATE(FOnStartDrinkPotionDelegate);
DECLARE_MULTICAST_DELEGATE(FOnDialogueFinishedDelegate);
//DECLARE_EVENT(AMyPlayer, FOnPlayerMeetNPCEvent);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerMeetNPCDelegate, AMyNPC*);



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
	AMyPlayerController* GetPlayerController() const{ return MyPlayerController; };
	AMyPlayerController* MyPlayerController;

	UInventoryComponent* GetMyInventoryComponent() const{ return MyInventory; };
	bool GetDoingSomething() const { return bDoingSomething; };
	void SetDoingSomething(bool _bDoingSomething) { bDoingSomething = _bDoingSomething; };
	bool GetIsDrinkPotion() const { return bIsDrinkPotion; };

	void SetPlayerHidden(bool _bHidden);

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
	void DrinkPotion();
	void LockOn();


	FOnStartDrinkPotionDelegate OnStartDrinkPotion; // 포션 먹기 시작했음을 알리는 델리게이트.


	UFUNCTION()
	void OnTargetCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnTargetCollisionEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	FOnPlayerMeetNPCDelegate& OnPlayerMeetNPC() { return OnPlayerMeetNPCDelegate; };
	
	bool GetIsPlayerTalking() const{ return bIsPlayerTalking; };

	FOnDialogueFinishedDelegate& OnDialogueFinished() { return OnDialogueFinishedDelegate; };

private:
	FOnDialogueFinishedDelegate OnDialogueFinishedDelegate;

	//플레이어가 대화를 시작했는지.
	bool bIsPlayerTalking;

	UPROPERTY()
	class AMyNPC* CurrentNPC;

	FOnPlayerMeetNPCDelegate OnPlayerMeetNPCDelegate;

	//레벨업 이펙트
	UPROPERTY(VisibleAnywhere, Category = Effect)
	UParticleSystemComponent* LevelupEffect;


	void CheckForInteractables();

	UPROPERTY(VisibleAnywhere, Category = Inventory)
	UInventoryComponent* MyInventory;

	void AttackStartComboState(); //공격이 시작할 때 관련 속성 지정.

	void MoveForward();

private:

	UPROPERTY(VisibleAnywhere, Category = "LockOn")
	class UBoxComponent* TargetCollisionBox; // 락온할 타겟을 감지할 콜리전.
	
	bool bIsLockedOn; // 현재 락온상태인지.
	float TargettingHeightOffset; // 락온일때 높이 오프셋값. 

	UPROPERTY(VisibleAnywhere, Category = "LockOn")
	ACharacter* LockedOnCharacter; // 현재 락온중인 캐릭터.

	UPROPERTY(VisibleAnywhere, Category = "LockOn")
	TArray<ACharacter*> LockOnCandidates; //락온범위에 감지된 지원자들.



	//스테미나 관련 변수
	float CurrentStamina; //현재 스테미나 값.
	float MaxStamina; // 스테미나 최대값.
	bool bCanStaminaRecharge; // 스테미나가 재 충전 될 수 있는지 여부.
	bool bIsSprinting; // 전력질주 중인지 여부.




	bool bIsDrinkPotion = false;

	float MyMoveSpeed = 2.0f;

	UPROPERTY(VisibleAnywhere, Category = Movement)
		bool bDoingSomething = false; // 걷기, 달리기, 아이들상태 외에 다른 동작을 하고 있는지 여부.

	UPROPERTY(VisibleAnywhere, Category = Movement)
		bool isDoingMovableAction = false; //점프, 포션먹기와 같은 이동인풋을 받으며 동작해야하는 행동인지 여부.

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
