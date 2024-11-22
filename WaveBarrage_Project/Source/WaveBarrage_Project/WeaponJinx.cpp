// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponJinx.h"
#include "WBPlayerState.h"
#include "Kismet/KismetSystemLibrary.h"
#include "WBPlayerBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ProCuteLauncher.h"
#include "Kismet/GameplayStatics.h"
#include "WBPlayerController.h"

AWeaponJinx::AWeaponJinx()
{
	// ��Ʈ ������Ʈ�� ����
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	// ����ü ���� ����Ʈ ������Ʈ
	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>("ProjectileSpawnPoint");
	ProjectileSpawnPoint->SetupAttachment(RootComponent);
	//RootComponent = ProjectileSpawnPoint;

	// ����ü ���� ����Ʈ�� ���� ��ġ�� ȸ�� ����
	//ProjectileSpawnPoint->SetRelativeLocation(FVector(100.0f, 0.0f, 0.0f)); // ĳ���� �������� 100 ���� �̵�
	//ProjectileSpawnPoint->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f)); // �ʿ信 ���� ȸ�� ����

	SetReplicates(true);
	SetReplicateMovement(true);

	WeaponType = EWeaponType::WeaponJinx;
	WeaponLevel = 1;
	Damage = 20;
	ProjectileCount = 3;
	MaxProjectileCnt = 0;
	CurProjectileCnt = 0;


}

void AWeaponJinx::BeginPlay()
{
	Super::BeginPlay();

	if (OwnerCharacter == nullptr && GetOwner())
	{
		OwnerCharacter = Cast<AWBPlayerBase>(GetOwner());

	}


	if (OwnerCharacter && ProjectileSpawnPoint)
	{
		// ���� ��Ģ�� �����մϴ�.
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);

		// `ProjectileSpawnPoint`�� ĳ���� �޽��� �����մϴ�.
		ProjectileSpawnPoint->AttachToComponent(OwnerCharacter->GetMesh(), AttachmentRules);
	}
	// ���� ȸ���� �����Ǵ��� Ȯ���ϱ� ���� �α� ���
	UE_LOG(LogTemp, Warning, TEXT("Weapon Rotation on %s: %s"), HasAuthority() ? TEXT("Server") : TEXT("Client"), *GetActorRotation().ToString());
}

void AWeaponJinx::Fire()
{
	Super::Fire();

	// Get projectile count from player state and update max projectile count
	if (HasAuthority() && OwnerCharacter)
	{
		AWBPlayerState* PlayerState = Cast<AWBPlayerState>(OwnerCharacter->GetPlayerState());
		if (PlayerState)
		{
			MaxProjectileCnt = ProjectileCount + PlayerState->ProjectileCounts;
			
		}
	}

	// Set timer to repeatedly call SpawnProjectile
	UKismetSystemLibrary::K2_SetTimer(this, TEXT("SpawnProjectile"), 0.1f, true);
}

void AWeaponJinx::SpawnProjectile()
{

	if (ProjectileClass && OwnerCharacter && ProjectileSpawnPoint)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Cast<APawn>(OwnerCharacter);
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FVector SpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
		FRotator SpawnRotation = ProjectileSpawnPoint->GetComponentRotation();

		//FRotator CharacterMeshRotation = OwnerCharacter->GetMesh()->GetComponentRotation();
		//FRotator SpawnRotation = CharacterMeshRotation;
		//SpawnRotation.Yaw += 90.0f;

		// Spawn the projectile actor
		AProCuteLauncher* SpawnedProjectile = GetWorld()->SpawnActor<AProCuteLauncher>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
		if (SpawnedProjectile)
		{
			// Increment current projectile count
			CurProjectileCnt++;
			int32 FinalDamage = CalculateFinalDamage();
			CanCritialAttack(FinalDamage);
			SpawnedProjectile->SetDamage(FinalDamage);
			SpawnedProjectile->CanCollision = true;

			// �α� �߰�
			UE_LOG(LogTemp, Warning, TEXT("Weapon Rotation on %s: %s"), HasAuthority() ? TEXT("Server") : TEXT("Client"), *GetActorRotation().ToString());
			UE_LOG(LogTemp, Warning, TEXT("ProjectileSpawnPoint Rotation: %s"), *ProjectileSpawnPoint->GetComponentRotation().ToString());
		}
	}

	// Check if the current projectile count has reached the max
	if (CurProjectileCnt >= MaxProjectileCnt)
	{

		// Clear the timer to stop spawning projectiles
		UKismetSystemLibrary::K2_ClearTimer(this, TEXT("SpawnProjectile"));

		// Reset current projectile count
		CurProjectileCnt = 0;

		if(HasAuthority())
		{
			OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
			OwnerCharacter->GetMesh()->SetWorldRotation(FRotator(0.0f, OwnerCharacter->GetActorRotation().Yaw - 90.0f, 0.0f));
			OwnerCharacter->ServerSetOrientation(OwnerCharacter->GetActorRotation().Yaw, true);
		}
		

		// �������� �÷��̾��� ���� �������� ȸ�� ����
		if (AWBPlayerBase* PlayerBase = Cast<AWBPlayerBase>(OwnerCharacter))
		{
			PlayerBase->ServerSetOrientation(OwnerCharacter->GetActorRotation().Yaw, true);
		}

	}
}


