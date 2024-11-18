// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponCuteLauncher.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "WBMonsterBase.h"
#include "Engine/World.h"
#include "WBPlayerState.h"
#include "Kismet/KismetMathLibrary.h"

// MaxProjectileCnt = ProjectileCount + PlayerState->ProjectileCounts
// ProjectileCount : 1/1/2/2/3
// Damage : 45/50/55/60/65
AWeaponCuteLauncher::AWeaponCuteLauncher()
{
	WeaponType = EWeaponType::WeaponCuteLauncher;

	ClosestDistance = FLT_MAX;
	CoolDown = 1.0f;
	Damage = 45;
	ProjectileCount = 1;

	if (OwnerCharacter)
	{
		AWBPlayerState* PlayerState = Cast<AWBPlayerState>(OwnerCharacter->GetPlayerState());
		if (PlayerState)
		{
			MaxProjectileCnt = ProjectileCount + PlayerState->ProjectileCounts;
		}
	}
}

void AWeaponCuteLauncher::BeginPlay()
{
	//Super::BeginPlay();

	if(!GetWorld()->GetTimerManager().IsTimerActive(FTimerHandle_AttackFire))
	{
		GetWorld()->GetTimerManager().SetTimer(FTimerHandle_AttackFire, this, &AWeaponCuteLauncher::Fire, CoolDown, true);
	}
}

void AWeaponCuteLauncher::Fire()
{
	CuteLauncherAutomaticAiming();

	if (ProjectileClass && ProjectileSpawnPoint)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Cast<APawn>(OwnerCharacter);

		// Get spawn location and rotation from ProjectileSpawnPoint
		FVector SpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
		FRotator SpawnRotation = ProjectileSpawnPoint->GetComponentRotation();

		// Spawn the projectile actor
		AActor* SpawnedProjectile = GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
		if (SpawnedProjectile)
		{
			// Increment current projectile count
			CurProjectileCnt++;
			UE_LOG(LogTemp, Error, TEXT("%d CuteLauncher"), CurProjectileCnt);

		}
	}
	if (CurProjectileCnt >= MaxProjectileCnt)
	{
		return;
	}
}

void AWeaponCuteLauncher::SpawnProjectile()
{

}

void AWeaponCuteLauncher::CuteLauncherAutomaticAiming()
{
	ClosestDistance = FLT_MAX;
	ClosestEnemy = nullptr;

	FVector Start = GetActorLocation();
	FVector End = Start + GetActorForwardVector() * 1000.0f;
	float Radius = 1200.0f;

	TArray<FHitResult> OutHits;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->SweepMultiByChannel(
		OutHits,
		Start,
		End,
		FQuat::Identity,
		ECC_GameTraceChannel1,
		FCollisionShape::MakeSphere(Radius),
		CollisionParams
	);

	// ����׿� ��ü ǥ�� (�� ������ ���� ǥ��)
	DrawDebugSphere(GetWorld(), Start, Radius, 12, FColor::Red, false, -1.f, 0, 1.0f);

	if (bHit)
	{
		for (const FHitResult& Hit : OutHits)
		{
			AActor* HitActor = Hit.GetActor();

			if (AWBMonsterBase* TargetMonster = Cast<AWBMonsterBase>(HitActor))
			{
				UE_LOG(LogTemp, Error, TEXT("Actor : %s"), *TargetMonster->GetName());

				float Distance = FVector::Dist(TargetMonster->GetActorLocation(), GetActorLocation());
				if (Distance < ClosestDistance)
				{
					ClosestDistance = Distance;
					ClosestEnemy = TargetMonster;
				}
			}
		}
	}
	if (ClosestEnemy)
	{
		FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), ClosestEnemy->GetActorLocation());
		SetActorRotation(FRotator(0.0f, TargetRotation.Yaw, 0.0f));
	}
}
