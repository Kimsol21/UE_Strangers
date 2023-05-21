// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "Animation/MyAnimInstance.h"
#include "DrawDebugHelpers.h" 


// Sets default values
AMyCharacter::AMyCharacter()
{
	//공격범위 관련
	AttackRange = 200.0f; //구가 지나갈 길이.
	AttackRadius = 50.0f;//구 반지름.
}

float AMyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) //피해자에게 호출될 함수.
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	UE_LOG(LogTemp, Warning, TEXT("Actor : %s took Damage : %f"), *GetName(), FinalDamage);

	SetDamage(FinalDamage);

	return FinalDamage;
}

void AMyCharacter::AttackCheck() //OnAttackCheck 델리게이트에서 호출할 함수.
{
	FHitResult HitResult; //충돌경우 관련 정보 담을 구조체.
	FName temp = NAME_None;
	FCollisionQueryParams Params(NAME_None, false, this);//탐색 방법에 대한 설정 값을 모아둔 구조체.
	/*
	* 첫번째 인자 (TraceTag) : Trace 디버깅을 위한 추가 정보 또는 필터링을 제공하는 데 사용되는 태그(예: Collision Analyzer)
	* 두번째 인자 (bTraceComplex) : 복잡한 충돌에 대해 추적해야 하는지 여부.
	* 세번째 인자 (IgnoreActor) : Trace하는 동안 무시해야 하는 엑터.
	*/

	bool bResult = GetWorld()->SweepSingleByChannel( //트레이스 체널을 사용해 물리적 충돌여부를 가리는 함수.
		HitResult,
		GetActorLocation(),//탐색시작위치.
		GetActorLocation() + GetActorForwardVector() * AttackRange,//탐색 종료 위치.
		FQuat::Identity,//탐색에 사용할 도형의 회전.
		ECollisionChannel::ECC_GameTraceChannel2,//물리 충돌 감지에 사용할 트레이스 채널 정보.
		FCollisionShape::MakeSphere(AttackRadius),//탐색에 사용할 기본 도형 정보.(구체,캡슐,박스 등)
		Params
	);

//#if ENABLE_DRAW_DEBUG
//	FVector TraceVec = GetActorForwardVector() * AttackRange;
//	FVector Center = GetActorLocation() + TraceVec * 0.5f;
//	float HalfHeight = AttackRange * 0.5f + AttackRadius;
//	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();//캡슐의 Z벡터를 캐릭터 시선방향으로 회전.
//	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
//	float DebugLifeTime = 5.0f;
//
//	DrawDebugCapsule //DrawDebugHelpers에서 제공하는 캡슐그리기 함수.
//	(
//		GetWorld(),//그릴월드
//		Center,//위치
//		HalfHeight,//캡슐길이
//		AttackRadius,//반지름
//		CapsuleRot,//캡슐회전
//		DrawColor, //색깔
//		false,//지속여부
//		DebugLifeTime //지속시간
//	);
//#endif

	if (bResult)//충돌이 감지되면
	{
		if (HitResult.Actor.IsValid())
		{
			UE_LOG(LogTemp, Error, TEXT("Hit Actor Name : %s"), *HitResult.Actor->GetName());
			FDamageEvent DamageEvent;
			HitResult.Actor->TakeDamage(GetAttackPower(), DamageEvent, GetController(), this);//AActor에서 제공하는 함수. (전달할 데미지 세기, 데미지종류, 가해자, 직접피해를입힌 Actor)
		}
	}
}
