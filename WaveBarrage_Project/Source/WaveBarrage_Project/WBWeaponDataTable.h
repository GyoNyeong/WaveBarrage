// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "WBWeaponBase.h"
#include "WBWeaponDataTable.generated.h"

/**
 * 
 */
USTRUCT()
struct FWeaponData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	// ���� ����
	UPROPERTY(EditAnywhere)
	int32 WeaponLevel;

	// ������
	UPROPERTY(EditAnywhere)
	int32 Damage;

	// ��ų ����
	UPROPERTY(EditAnywhere)
	int32 SkillAcceleration;

	// ��ų �⺻ �ӵ�
	UPROPERTY(EditAnywhere)
	float CoolDown;

	// ũ��Ƽ�� Ȯ��
	UPROPERTY(EditAnywhere)
	int32 CriticalChance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite ,meta = (ExposeOnSpawn = "true"))
	AWBPlayerBase* OwnerCharacter;

	// �߻�ü
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ProjectileClass;

	// �߻�ü ����
	UPROPERTY(EditAnywhere)
	int32 ProjectileCount;

	// ���� �߻�ü ���� (Default : 0)
	UPROPERTY(EditAnywhere)
	int32 CurProjectileCnt;

	// �ִ� �߻�ü ����
	UPROPERTY(EditAnywhere)
	int32 MaxProjectileCnt;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> ProjectileSpawnPoint;
};

UCLASS()
class WAVEBARRAGE_PROJECT_API UWBWeaponDataTable : public UDataTable
{
	GENERATED_BODY()
	
};
