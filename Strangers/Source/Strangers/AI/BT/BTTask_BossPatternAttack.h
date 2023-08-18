#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_BossPatternAttack.generated.h"

 // 보스 패턴에 관한 Enum.
UENUM(BlueprintType)
enum class EBossPattern : uint8
{
	BOSSPATTERN_NORMAL_ATTACK1 UMETA(DisplayName = "NormalAttack1"),
	BOSSPATTERN_NORMAL_ATTACK2 UMETA(DisplayName = "NormalAttack2"),
	BOSSPATTERN_NORMAL_ATTACK3 UMETA(DisplayName = "NormalAttack3"),
	BOSSPATTERN_COMBO_ATTACK UMETA(DisplayName = "ComboAttack"),
	BOSSPATTERN_STAB UMETA(DisplayName = "Stab"),
	BOSSPATTERN_KICK UMETA(DisplayName = "Kick")
};

UCLASS()
class STRANGERS_API UBTTask_BossPatternAttack : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_BossPatternAttack();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Attack Pattern")
	EBossPattern EPatternToExcute;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	class AMyBoss* OwnerPawn;
};
