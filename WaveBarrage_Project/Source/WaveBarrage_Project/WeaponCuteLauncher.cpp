// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponCuteLauncher.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "WBMonsterBase.h"
#include "Engine/World.h"
#include "WBPlayerState.h"
#include "GameFramework/Character.h"

// MaxProjectileCnt = ProjectileCount + PlayerState->ProjectileCounts
// ProjectileCount : 1/1/2/2/3
// Damage : 45/50/55/60/65
AWeaponCuteLauncher::AWeaponCuteLauncher()
{
	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>("ProjectileSpawnPoint");
	RootComponent = ProjectileSpawnPoint;

	WeaponType = EWeaponType::WeaponCuteLauncher;

	ClosestDistance = FLT_MAX;
	Damage = 45;
	CoolDown = 1.0f;
	ProjectileCount = 1;


}

void AWeaponCuteLauncher::BeginPlay()
{
	Super::BeginPlay();

	ACharacter* MyCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (MyCharacter)
	{
		AWBPlayerState* MyPs = Cast<AWBPlayerState>(MyCharacter->GetPlayerState());
		if (MyPs)
		{
			MaxProjectileCnt = ProjectileCount + MyPs->ProjectileCounts;
		}
	}

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

		// Get spawn location and rotation from ProjectileSpawnPoint
		FVector SpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
		FRotator SpawnRotation = ProjectileSpawnPoint->GetComponentRotation();

		// Spawn the projectile actor
		AActor* SpawnedProjectile = GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnLocation, SpawnRotation);
		if (SpawnedProjectile)
		{
			// Increment current projectile count
			CurProjectileCnt++;
			//UE_LOG(LogTemp, Error, TEXT("%d CuteLauncher"), CurProjectileCnt);

		}
	}
	if (CurProjectileCnt >= MaxProjectileCnt)
	{
		return;
	}
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
				//UE_LOG(LogTemp, Error, TEXT("Actor : %s"), *TargetMonster->GetName());

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
