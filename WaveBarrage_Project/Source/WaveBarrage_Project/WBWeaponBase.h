// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WBWeaponBase.generated.h"

class USceneComponent;
class AWBPlayerState;
class AWBPlayerBase;
class AWBProjectileBase;


// �� ������ Ÿ���� �ĺ�
UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	WeaponJinx UMETA(DisplayName = "Jinx"),
	WeaponWhirlwind UMETA(DisplayName = "Whirlwind"),
	WeaponPoisonFootprint UMETA(DisplayName = "PoisonFootprint"),
	WeaponBoomerang UMETA(DisplayName = "Boomerang"),
	WeaponCuteLauncher UMETA(DisplayName = "CuteLauncher")
};


UCLASS()
class WAVEBARRAGE_PROJECT_API AWBWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWBWeaponBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UFUNCTION(NetMulticast, Reliable)
	virtual void Fire(); // Fire �Լ��� ��Ƽĳ��Ʈ�� ����

	UFUNCTION()
	virtual void SpawnProjectile();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	EWeaponType GetWeaponType() const;

	virtual int32 CalculateFinalDamage();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon",Replicated)
	EWeaponType WeaponType; // ���� Ÿ���� ��Ÿ���� ����

public:

	// ���� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Replicated)
	int32 WeaponLevel;

	// ������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 Damage;

	// ��ų ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 SkillAcceleration;

	// ��ų �⺻ �ӵ�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float CoolDown;

	// ũ��Ƽ�� Ȯ��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 CriticalChance;

	// �߻�ü
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<AActor> ProjectileClass;

	// �߻�ü ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 ProjectileCount;

	// ���� �߻�ü ���� (Default : 0)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 CurProjectileCnt;

	// �ִ� �߻�ü ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 MaxProjectileCnt;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USceneComponent> ProjectileSpawnPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
	AWBPlayerBase* OwnerCharacter;

	UFUNCTION()
	virtual void CalculateAttackStatus();

	UFUNCTION()
	int32 CanCritialAttack(int32 WeaponBaseDamage);
};
