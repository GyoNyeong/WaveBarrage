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

	// 무기 레벨
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 WeaponLevel=0;

	// 데미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Damage=0;

	// 스킬 가속
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SkillAcceleration=0;

	// 스킬 기본 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CoolDown=0;

	// 크리티컬 확율
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CriticalChance=0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ProjectileCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Duration = 0;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite ,meta = (ExposeOnSpawn = "true"))
	//AWBPlayerBase* OwnerCharacter;

	//// 발사체
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//TSubclassOf<AActor> ProjectileClass;

	//// 현재 발사체 개수 (Default : 0)
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//int32 CurProjectileCnt;

	//// 최대 발사체 개수
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//int32 MaxProjectileCnt;

};

UCLASS()
class WAVEBARRAGE_PROJECT_API UWBWeaponDataTable : public UDataTable
{
	GENERATED_BODY()
	
};
