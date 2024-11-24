// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "WBWeaponBase.h"
#include "WBWeaponDataTable.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FWeaponData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	// ���� ����
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 WeaponLevel=0;

	// ������
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Damage=0;

	// ��ų ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SkillAcceleration=0;

	// ��ų �⺻ �ӵ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CoolDown=0;

	// ũ��Ƽ�� Ȯ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CriticalChance=0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ProjectileCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Duration = 0;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite ,meta = (ExposeOnSpawn = "true"))
	//AWBPlayerBase* OwnerCharacter;

	//// �߻�ü
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//TSubclassOf<AActor> ProjectileClass;

	//// ���� �߻�ü ���� (Default : 0)
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//int32 CurProjectileCnt;

	//// �ִ� �߻�ü ����
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//int32 MaxProjectileCnt;

};

UCLASS()
class WAVEBARRAGE_PROJECT_API UWBWeaponDataTable : public UDataTable
{
	GENERATED_BODY()
	
};
