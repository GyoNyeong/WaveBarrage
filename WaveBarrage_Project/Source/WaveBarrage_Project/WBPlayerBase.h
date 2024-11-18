// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "WBPlayerBase.generated.h"

class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class UBoxComponent;
class AWBMonsterBase;
class AActor;
class AWBWeaponBase;

UCLASS()
class WAVEBARRAGE_PROJECT_API AWBPlayerBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AWBPlayerBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void ConfigureInputMapping();

	// ���� ��� �Լ�
	UFUNCTION()
	void AutomaticAiming();

	UFUNCTION()
	void AttackFire();

	UFUNCTION()
	void CursorHitAiming();


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Box1;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Box2;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Box3;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Box4;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Box5;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> MonsterSpawnPosition1;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> MonsterSpawnPosition2;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> MonsterSpawnPosition3;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> MonsterSpawnPosition4;

	UPROPERTY(VisibleAnywhere)
	TArray<USceneComponent*> MonsterSpawnPositions;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> InputMapping;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> IA_Move;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> IA_Skill_E;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> IA_Skill_R;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> IA_AutoMode;

	void Move(const FInputActionValue& Value);

	void SkillE();

	void SkillR();

	void ToggleAutoMode();

	UPROPERTY()
	TObjectPtr<APlayerController> MyPlayerController;

	// ĳ���� �� ���� ����
	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf<AWBWeaponBase> ChampionOnlyWeapon;

	// �Ϲ� ������� ������ Box �迭 (Blueprint���� ���� ����)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TArray<TSubclassOf<AWBWeaponBase>> WeaponAttachBoxes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bAutoMode;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ClosestDistance;

	UPROPERTY()
	TObjectPtr<AActor> ClosestEnemy;

	UPROPERTY()
	AWBWeaponBase* SpawnedWeapon;

	UPROPERTY()
	AWBWeaponBase* GeneralSpawnedWeapon;

	UPROPERTY()
	TArray<AWBWeaponBase*> EquippedWeapons;

	FTimerHandle FTimerHandle_AttackFire;
	FTimerHandle FTimerHandle_AutomaticAiming;
	FTimerHandle FTimerHandle_CursorAiming;
};
