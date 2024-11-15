// Fill out your copyright notice in the Description page of Project Settings.


#include "WBGameMode.h"
#include "WBMonsterBase.h"
#include "WBPlayerBase.h"
#include "WBMonsterGroup.h"

void AWBGameMode::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle Handle;
	FTimerManagerTimerParameters Para;
	GetWorld()->GetTimerManager().SetTimer(Handle, this, &AWBGameMode::SpawnMonsterGroup, 1.0f, false);
	SpawnMonsterGroup();
}

void AWBGameMode::AddExp(int Value)
{
	Exp += Value;
}

void AWBGameMode::LevelUp()
{

}

void AWBGameMode::SetTargetPlayer()
{

}

void AWBGameMode::SpawnMonsterGroup()
{
	UClass* MC = MonsterClass1->GetClass();
	FActorSpawnParameters SpawnPara;
	SpawnPara.Owner = this;
	SpawnPara.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	UE_LOG(LogTemp, Warning, TEXT("Players Num : %d"), Players.Num());
	for (AWBPlayerBase* Player : Players)
	{
		if (IsValid(Player))
		{
			for (USceneComponent* Comp : Player->MonsterSpawnPositions)
			{
				if (MC)
				{
					AWBMonsterGroup* Spawned = GetWorld()->SpawnActor<AWBMonsterGroup>(AWBMonsterGroup::StaticClass(), Comp->GetComponentLocation(), Comp->GetComponentRotation(), SpawnPara);
					if (Spawned)
					{
						UE_LOG(LogTemp, Warning, TEXT("Spawn End"));
						Spawned->TargetPlayer = Player;
						Spawned->MosterClass = MC;
						Spawned->SpawnCount = 3;
						MonsterGroups.Emplace(Spawned);
					}

					UE_LOG(LogTemp, Warning, TEXT("Group Spawned"));
				}
			}
		}
	}

}
