// Fill out your copyright notice in the Description page of Project Settings.


#include "WBWeaponBase.h"
#include "Components/SceneComponent.h"
#include "WBPlayerBase.h"
#include "WBPlayerState.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "WBProjectileBase.h"


// Sets default values
AWBWeaponBase::AWBWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// ����ü ���� ����Ʈ ������Ʈ
	//ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>("ProjectileSpawnPoint");
	//RootComponent = ProjectileSpawnPoint;

	// �⺻ ���� Ÿ�� ���� (�ʿ信 ���� ���� ����)
	WeaponType = EWeaponType::WeaponJinx;

	// Initialize default values
	OwnerCharacter = nullptr;
	WeaponLevel = 1;
	Damage = 10;
	SkillAcceleration = 0;
	CriticalChance = 0; // Default critical chance is 5%
	ProjectileCount = 1;
	CurProjectileCnt = 0;
	MaxProjectileCnt = 10; // Default maximum projectile count

}

// Called when the game starts or when spawned
void AWBWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	if (OwnerCharacter == nullptr && GetOwner())
	{
		OwnerCharacter = Cast<AWBPlayerBase>(GetOwner());
	}
	
}

// Called every frame
void AWBWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWBWeaponBase::Fire()
{
	if (ProjectileClass && OwnerCharacter)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;

		FVector SpawnLocation = GetActorLocation();
		FRotator SpawnRotation = GetActorRotation();

		// ����ü ����
		AWBProjectileBase* SpawnedProjectile = GetWorld()->SpawnActor<AWBProjectileBase>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
		if (SpawnedProjectile)
		{
			// ������ ����
			int32 FinalDamage = CalculateFinalDamage();
			SpawnedProjectile->SetDamage(FinalDamage);
		}
	}
}

void AWBWeaponBase::SpawnProjectile()
{

}

EWeaponType AWBWeaponBase::GetWeaponType() const
{
	return WeaponType;
}

int32 AWBWeaponBase::CalculateFinalDamage()
{
	// ������ �⺻ �������� ���
	int32 WeaponBaseDamage = Damage;

	// OwnerCharacter�� PlayerState���� �нú� ������ ������ ��������
	if (OwnerCharacter)
	{
		AWBPlayerState* PlayerState = OwnerCharacter->GetPlayerState<AWBPlayerState>();
		if (PlayerState)
		{
			int32 DamageMultiplier = Damage + ((Damage*PlayerState->Damage)/100);
			return DamageMultiplier;
		}
	}

	return WeaponBaseDamage;
}

