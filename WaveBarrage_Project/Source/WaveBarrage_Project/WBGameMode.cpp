// Fill out your copyright notice in the Description page of Project Settings.


#include "WBGameMode.h"
#include "WBMonsterBase.h"
#include "WBPlayerBase.h"
#include "WBMonsterGroup.h"

AWBGameMode::AWBGameMode()
{

}

void AWBGameMode::BeginPlay()
{
	Super::BeginPlay();

	TArray<FSpawnData*> arr;
	SpawnDataTable->GetAllRows<FSpawnData>(TEXT("GetAllRows"), arr);

	for (FSpawnData* Data : arr)
	{
		FTimerDelegate TimerDel;
		TimerDel.BindUFunction(this, FName("SpawnMonster"), Data->SpawnType, Data->MonsterClass, Data->SpawnCount, Data->x, Data->y);

		FTimerHandle Handle;
		FTimerManagerTimerParameters Para;
		GetWorld()->GetTimerManager().SetTimer(Handle, TimerDel, Data->SpawnTime, false);
	}


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

void AWBGameMode::SpawnMonster(ESpawnType SpawnType, TSubclassOf<AWBMonsterBase> MonsterClass, int SpawnCount, float x, float y)
{
	FActorSpawnParameters SpawnPara;
	SpawnPara.Owner = this;
	SpawnPara.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	switch (SpawnType)
	{
	case ESpawnType::Normal:
		{
			for (AWBPlayerBase* Player : Players)
			{
				if (IsValid(Player))
				{
					for (USceneComponent* Comp : Player->MonsterSpawnPositions)
					{
						AWBMonsterGroup* Spawned = GetWorld()->SpawnActor<AWBMonsterGroup>(AWBMonsterGroup::StaticClass(), Comp->GetComponentLocation(), FRotator::ZeroRotator, SpawnPara);
						if (Spawned)
						{
							Spawned->TargetPlayer = Player;
							Spawned->MonsterClass = MonsterClass;
							Spawned->SpawnCount = SpawnCount;
							MonsterGroups.Emplace(Spawned);
							Spawned->SpawnMonster();
						}
					}
				}
			}
		}
		break;
	case ESpawnType::Minion:
		{
			for (AWBPlayerBase* Player : Players)
			{
				if (IsValid(Player))
				{

					AWBMonsterGroup* Spawned = GetWorld()->SpawnActor<AWBMonsterGroup>(AWBMonsterGroup::StaticClass(), Player->GetActorLocation() + FVector(x, y, 0), FRotator::ZeroRotator, SpawnPara);
					if (Spawned)
					{
						Spawned->TargetPlayer = Player;
						Spawned->MonsterClass = MonsterClass;
						Spawned->SpawnCount = SpawnCount;
						MonsterGroups.Emplace(Spawned);
						Spawned->SpawnMonster();
					}
				}
			}
		}
		break;
	case ESpawnType::Circle:
		{
			for (AWBPlayerBase* Player : Players)
			{
				if (IsValid(Player))
				{

					AWBMonsterGroup* Spawned = GetWorld()->SpawnActor<AWBMonsterGroup>(AWBMonsterGroup::StaticClass(), Player->GetActorLocation(), FRotator::ZeroRotator, SpawnPara);
					if (Spawned)
					{
						Spawned->TargetPlayer = Player;
						Spawned->MonsterClass = MonsterClass;
						Spawned->SpawnCount = SpawnCount;
						MonsterGroups.Emplace(Spawned);
						Spawned->SpawnMonster();
					}
				}
			}
		}
		break;
	case ESpawnType::Elite:
		{
			AWBMonsterBase* Spawned = GetWorld()->SpawnActor<AWBMonsterBase>(MonsterClass, FVector(x, y, 0), FRotator::ZeroRotator, SpawnPara);
		}
		break;
	case ESpawnType::Boss:
		{
			AWBMonsterBase* Spawned = GetWorld()->SpawnActor<AWBMonsterBase>(MonsterClass, FVector(x, y, 0), FRotator::ZeroRotator, SpawnPara);
		}
		break;
	default:
		break;
	}



}
