// Fill out your copyright notice in the Description page of Project Settings.


#include "WBMonsterGroup.h"
#include "WBMonsterBase.h"
#include "WBGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AWBMonsterGroup::AWBMonsterGroup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
}

// Called when the game starts or when spawned
void AWBMonsterGroup::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWBMonsterGroup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (SpawnEnd && Monsters.Num() > 0)
	{
		if (IsValid(Monsters[0]))
		{
			SetActorLocation(Monsters[0]->GetActorLocation());
		}
	}
	else
	{
		// ���߿� �����Ǹ� ��������Ʈ�� ����
		AWBGameMode* GM = Cast<AWBGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		if (IsValid(GM))
		{
			GM->MonsterGroups.Remove(this);
		}
		Destroy();
		//FVector(FMath::RandRange(1, 500), FMath::RandRange(1, 500),0)
	}
}

void AWBMonsterGroup::SpawnMonster()
{
	FActorSpawnParameters SpawnPara;
	SpawnPara.Owner = this;
	SpawnPara.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	if (MonsterClass)
	{
		for (int i = 0; i < SpawnCount; i++)
		{
			AWBMonsterBase* Spawned = GetWorld()->SpawnActor<AWBMonsterBase>(MonsterClass, GetActorLocation(), GetActorRotation(), SpawnPara);
			Spawned->SetTargetPlayer(TargetPlayer);
			Monsters.Emplace(Spawned);
		}
		SpawnEnd = true;
	}
}

void AWBMonsterGroup::SpawnRandomPositionMonster()
{
	FActorSpawnParameters SpawnPara;
	SpawnPara.Owner = this;
	SpawnPara.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	if (MonsterClass)
	{
		for (int i = 0; i < SpawnCount; i++)
		{
			AWBMonsterBase* Spawned = GetWorld()->SpawnActor<AWBMonsterBase>(MonsterClass, GetActorLocation() + FVector(FMath::RandRange(1, 1500), FMath::RandRange(1, 1500), 0), GetActorRotation(), SpawnPara);
			Spawned->SetTargetPlayer(TargetPlayer);
			Monsters.Emplace(Spawned);
		}
		SpawnEnd = true;
	}
}

