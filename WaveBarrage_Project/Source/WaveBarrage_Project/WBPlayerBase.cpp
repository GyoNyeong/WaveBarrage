// Fill out your copyright notice in the Description page of Project Settings.


#include "WBPlayerBase.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"
#include "WBWeaponBase.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "WBGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "Kismet/KismetMathLibrary.h"
//#include "Engine/EngineTypes.h"
//#include "WBMonsterBase.h"


// Sets default values
AWBPlayerBase::AWBPlayerBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;


	//GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 400.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;
	GetCharacterMovement()->MaxAcceleration = 1000.0f;
	GetCharacterMovement()->BrakingFrictionFactor = 1.0f;
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;
	SpringArm->TargetArmLength = 4000.0f;
	SpringArm->SetRelativeRotation(FRotator(-60.0f, 0.0f, 0.0f));

	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = false;
	Camera->FieldOfView = 55.0f;


	WeaponPotionComponent = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponPotion"));
	WeaponPotionComponent->SetupAttachment(GetMesh());

	MonsterSpawnPosition1 = CreateDefaultSubobject<USceneComponent>(TEXT("Pos1"));
	MonsterSpawnPosition1->SetRelativeLocation(FVector(0, 3000, 0));
	MonsterSpawnPosition1->SetupAttachment(RootComponent);
	MonsterSpawnPositions.Emplace(MonsterSpawnPosition1);

	MonsterSpawnPosition2 = CreateDefaultSubobject<USceneComponent>(TEXT("Pos2"));
	MonsterSpawnPosition2->SetRelativeLocation(FVector(0, -3000, 0));
	MonsterSpawnPosition2->SetupAttachment(RootComponent);
	MonsterSpawnPositions.Emplace(MonsterSpawnPosition2);

	MonsterSpawnPosition3 = CreateDefaultSubobject<USceneComponent>(TEXT("Pos3"));
	MonsterSpawnPosition3->SetRelativeLocation(FVector(3000, 0, 0));
	MonsterSpawnPosition3->SetupAttachment(RootComponent);
	MonsterSpawnPositions.Emplace(MonsterSpawnPosition3);

	MonsterSpawnPosition4 = CreateDefaultSubobject<USceneComponent>(TEXT("Pos4"));
	MonsterSpawnPosition4->SetRelativeLocation(FVector(-3000, 0, 0));
	MonsterSpawnPosition4->SetupAttachment(RootComponent);
	MonsterSpawnPositions.Emplace(MonsterSpawnPosition4);

	bAutoMode = false;
	ClosestDistance = FLT_MAX;
}

// Called when the game starts or when spawned
void AWBPlayerBase::BeginPlay()
{
	Super::BeginPlay();

	ConfigureInputMapping();
	MyPlayerController = Cast<APlayerController>(GetController());
	
	if (MyPlayerController)
	{
		MyPlayerController->bShowMouseCursor = true;
	}

	if (WeaponPotionComponent && ChampionOnlyWeapon)
	{
		//UE_LOG(LogTemp, Error, TEXT("ChampionOnlyWeapon!!!!!!!!!!!!!!!!"));

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnedWeapon = GetWorld()->SpawnActor<AWBWeaponBase>(ChampionOnlyWeapon, GetActorLocation(), FRotator::ZeroRotator, SpawnParams);
		if (SpawnedWeapon)
		{
			SpawnedWeapon->AttachToComponent(WeaponPotionComponent, FAttachmentTransformRules::KeepWorldTransform);
			SpawnedWeapon->OwnerCharacter = this;
			EquippedWeapons.Add(SpawnedWeapon);
		}
	}

	AWBGameMode* GM = Cast<AWBGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (IsValid(GM))
	{
		GM->Players.Emplace(this);
	}

	//UE_LOG(LogTemp, Error, TEXT("1. Attack Check!!!!!!!"));
	if (HasAuthority())
	{
		GetWorld()->GetTimerManager().SetTimer(FTimerHandle_AttackFire, this, &AWBPlayerBase::AttackFire, 2.0f, true);
	}
}

// Called every frame
void AWBPlayerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWBPlayerBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

// Called to bind functionality to input
void AWBPlayerBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (IA_Move)
		{
			EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AWBPlayerBase::Move);
		}
		if (IA_Skill_E)
		{
			EnhancedInputComponent->BindAction(IA_Skill_E, ETriggerEvent::Started, this, &AWBPlayerBase::SkillE);
		}
		if (IA_Skill_R)
		{
			EnhancedInputComponent->BindAction(IA_Skill_R, ETriggerEvent::Started, this, &AWBPlayerBase::SkillR);
		}
		if (IA_AutoMode)
		{
			EnhancedInputComponent->BindAction(IA_AutoMode, ETriggerEvent::Started, this, &AWBPlayerBase::ToggleAutoMode);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AWBPlayerBase::ConfigureInputMapping()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	if (IsValid(PlayerController))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			if (InputMapping)
			{
				Subsystem->AddMappingContext(InputMapping, 0);
			}
		}
	}
}


void AWBPlayerBase::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator MyRotation = GetControlRotation();
		const FRotator YawRotation(0, MyRotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		//AddMovementInput(ForwardDirection, MovementVector.Y);
		//AddMovementInput(RightDirection, MovementVector.X);
		FVector Direction = ForwardDirection * MovementVector.Y + RightDirection * MovementVector.X;

		// ������ �̵� ��û
		if (HasAuthority())
		{
			AddMovementInput(Direction); // ������ ��쿡�� ���� ó��
		}
		else
		{
			ServerMoveCharacter(Direction); // Ŭ���̾�Ʈ���� ������ �̵� ��û
		}
	}
}


void AWBPlayerBase::ServerMoveCharacter_Implementation(const FVector& Direction)
{
	AddMovementInput(Direction);

	MulticastMove(Direction);
}


void AWBPlayerBase::MulticastMove_Implementation(const FVector& Direction)
{
	if (!HasAuthority())
	{
		AddMovementInput(Direction);
	}
}

void AWBPlayerBase::SkillE()
{
	// Skill E logic here
}

void AWBPlayerBase::SkillR()
{
	// Skill R logic here
}

void AWBPlayerBase::ToggleAutoMode()
{
	//UE_LOG(LogTemp, Error, TEXT("ToggleAutoMode!!!!!!!!!!!!!!!"));

	if (bAutoMode)
	{
		bAutoMode = false;
	} 
	else
	{
		bAutoMode = true;
	}
	//DefaultAttackSettings();
}

// ���� ����� ���� �ڵ� Ÿ����
void AWBPlayerBase::AutomaticAiming()
{
	if (!bAutoMode)
	{
		return;
	}

	ClosestDistance = FLT_MAX;
	ClosestEnemy = nullptr;

	FVector Start = GetActorLocation();
	FVector End = Start + GetActorForwardVector() * 1500.0f;
	float Radius = 1500.0f;

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

	UE_LOG(LogTemp, Error, TEXT("DrawDebugSphere"));

	// ����׿� ��ü ǥ��
	DrawDebugSphere(GetWorld(), Start, Radius, 12, FColor::Red, false, 1.f);
	// ����׿� ��ü ǥ�� (�� ������ ���� ǥ��)
	//DrawDebugSphere(GetWorld(), Start, Radius, 12, FColor::Red, false, -1.f, 0, 1.0f);


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


		if (ClosestEnemy)
		{
			FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), ClosestEnemy->GetActorLocation());
			GetMesh()->SetWorldRotation(FRotator(0.0f, TargetRotation.Yaw - 90.f, 0.0f));
		}
	}
}



void AWBPlayerBase::AttackFire()
{
	if (!bAutoMode)
	{
		//UE_LOG(LogTemp, Error, TEXT("false bAutoMode"));
		GetCharacterMovement()->bOrientRotationToMovement = false;
		CursorHitAiming();
	}
	else if(bAutoMode)
	{
		//UE_LOG(LogTemp, Error, TEXT("true bAutoMode"));
		GetCharacterMovement()->bOrientRotationToMovement = false;
		AutomaticAiming();
	}

	if (SpawnedWeapon)
	{
		SpawnedWeapon->Fire();
	}
}

// ���콺 Ŀ�� ��ġ
void AWBPlayerBase::CursorHitAiming()
{
	if (!bAutoMode)
	{
		//UE_LOG(LogTemp, Error, TEXT("4. CursorHitAiming Check!!!!!!!"));

		if (MyPlayerController)
		{
			FHitResult HitResult;
			
			if (MyPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, HitResult))
			{
				FVector TargetLocation = HitResult.Location;
				FVector ActorLocation = GetActorLocation();
	
				FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(FVector(ActorLocation.X, ActorLocation.Y, 0.0f), FVector(TargetLocation.X, TargetLocation.Y, 0.0f));
				GetMesh()->SetWorldRotation(FRotator(0.0f, NewRotation.Yaw - 90.f, 0.0f));

			}
		}
	}

}
