// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBoomerang.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "WBPlayerState.h"
#include "ProBoomerang.h"

AWeaponBoomerang::AWeaponBoomerang()
{
	SpawnDelay = UKismetMathLibrary::RandomFloatInRange(0.033f, 0.528f);
	WeaponType = EWeaponType::WeaponBoomerang;

	WeaponLevel = 1;
	CoolDown = 6.0f;
	Damage = 40;
	ProjectileCount = 1;

	SetReplicates(true);
	SetReplicateMovement(true);

}

void AWeaponBoomerang::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(FTimerHandle_Fire, this, &AWeaponBoomerang::Fire, CoolDown, true);
	
	ACharacter* MyCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (MyCharacter)
	{
		AWBPlayerState* MyPs = Cast<AWBPlayerState>(MyCharacter->GetPlayerState());
		if (MyPs)
		{
			MaxProjectileCnt = ProjectileCount + MyPs->ProjectileCounts;
		}
	}

}

void AWeaponBoomerang::Fire()
{
	Super::Fire();
	if (CurProjectileCnt == 0)
	{
		GetWorld()->GetTimerManager().SetTimer(FTimerHandle_SpawnProjectile, this, &AWeaponBoomerang::SpawnProjectile, SpawnDelay, true);
	}

	

}

void AWeaponBoomerang::SpawnProjectile()
{
	Super::SpawnProjectile();
	if (ProjectileClass)
	{
		FTransform SpawnTransform = FTransform (ProjectileSpawnPoint->GetComponentRotation(), ProjectileSpawnPoint->GetComponentLocation());
		AProBoomerang* SpawnedProjectile = GetWorld()->SpawnActor<AProBoomerang>(ProjectileClass, SpawnTransform);

		CurProjectileCnt++;
		if (CurProjectileCnt >= MaxProjectileCnt)
		{
		 GetWorld()->GetTimerManager().ClearTimer(FTimerHandle_SpawnProjectile);
			CurProjectileCnt = 0;

			int32 FinalDamage = CalculateFinalDamage();
			SpawnedProjectile->SetDamage(FinalDamage);
			SpawnedProjectile->CanCollision = true;

		}
		
	}
}
