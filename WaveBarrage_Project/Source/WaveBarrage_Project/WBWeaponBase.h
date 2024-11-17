// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WBPlayerBase.h"
#include "WBWeaponBase.generated.h"

class USceneComponent;

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


	UFUNCTION()
	virtual void Fire();

	UFUNCTION()
	virtual void SpawnProjectile();

	// ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 WeaponLevel;

	// ������
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Damage;

	// ��ų ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SkillAcceleration;

	// ��ų �⺻ �ӵ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CoolDown;

	// ũ��Ƽ�� Ȯ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CriticalChance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
	AWBPlayerBase* OwnerCharacter;

	// �߻�ü
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<AActor> ProjectileClass;

	// �߻�ü ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ProjectileCount;

	// ���� �߻�ü ���� (Default : 0)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurProjectileCnt;

	// �ִ� �߻�ü ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxProjectileCnt;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<USceneComponent> ProjectileSpawnPoint;



};
