// Fill out your copyright notice in the Description page of Project Settings.


#include "WBGameMode.h"
#include "WBGameState.h"
#include "WBPlayerController.h"
#include "WBPlayerState.h"
#include "WBMonsterBase.h"
#include "WBPlayerBase.h"
#include "WBMonsterGroup.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
//#include "WBULobbyWidget.h"



AWBGameMode::AWBGameMode()
{

}

void AWBGameMode::BeginPlay()
{
	Super::BeginPlay();

	//if (LobbyWidgetClass)
	//{
	//	// 로비 위젯 생성 및 화면에 추가
	//	UWBULobbyWidget* LobbyWidget = CreateWidget<UWBULobbyWidget>(GetWorld(), LobbyWidgetClass);
	//	if (LobbyWidget)
	//	{
	//		LobbyWidget->AddToViewport();
	//	}
	//}

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
	if (MaxExp <= Exp)
	{
		Exp = 0;
		LevelUp();
		Level += 1;
		if (Level < 10)
		{
			MaxExp += 5;
		}
		else if (Level < 20)
		{
			MaxExp += 10;
		}
		else
		{
			MaxExp += 30;
		}

	}
}

void AWBGameMode::LevelUp()
{

	for (AActor* PlayerActor : Players)
	{
		AWBPlayerBase* Player = Cast<AWBPlayerBase>(PlayerActor);
		if (Player)
		{
			APlayerController* PlayerController = Cast<APlayerController>(Player->GetController());
			if (PlayerController)
			{
				AWBPlayerController* WBPlayerController = Cast<AWBPlayerController>(PlayerController);
				if(WBPlayerController)
				{
					WBPlayerController->ShowCardSelectionWidget();
				}
			}
		}
	}

}

//void AWBGameMode::MulticastLevelUp()
//{
//	for (auto It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
//	{
//		APlayerController* PC = It->Get();
//		if (PC && PC->IsLocalController())
//		{
//			if (AWBPlayerController* MyPlayer = Cast<AWBPlayerController>(PC))
//			{
//			
//			}
//		}
//	}
//}


void AWBGameMode::ApplyCardEffect(AWBPlayerController* PlayerController, int32 CardIndex)
{
	AWBPlayerBase* Player = Cast<AWBPlayerBase>(PlayerController->GetCharacter());
	if (Player)
	{
		AWBPlayerState* PlayerState = Player->GetPlayerState<AWBPlayerState>();
		if (!PlayerState)
		{
			return;
		}

		// 무기 카드 처리 (0-4)
		if (CardIndex >= 0 && CardIndex <= 4)
		{
			FString WeaponTypeString;
			switch (static_cast<EWeaponType>(CardIndex))
			{
			case EWeaponType::WeaponJinx:
				WeaponTypeString = "Jinx";
				break;
			case EWeaponType::WeaponWhirlwind:
				WeaponTypeString = "Whirlwind";
				break;
			case EWeaponType::WeaponPoisonFootprint:
				WeaponTypeString = "PoisonFootprint";
				break;
			case EWeaponType::WeaponBoomerang:
				WeaponTypeString = "Boomerang";
				break;
			case EWeaponType::WeaponCuteLauncher:
				WeaponTypeString = "CuteLauncher";
				break;
			default:
				return; // 유효하지 않은 무기 타입인 경우 리턴
			}

			// 무기 레벨 가져오기
			int16* CurrentLevelPtr = PlayerState->ItemLevel.Find(WeaponTypeString);
			int WeaponLevel = (CurrentLevelPtr) ? FMath::Min((*CurrentLevelPtr) + 1, 5) : 1;

			// 데이터 테이블에서 해당 무기와 레벨의 능력치를 가져오기
			FName RowName = FName(*FString::Printf(TEXT("%s_%d"), *WeaponTypeString, WeaponLevel));
			FWeaponData* WeaponData = WeaponDataTable->FindRow<FWeaponData>(RowName, TEXT(""));
			if (WeaponData)
			{
				// 로그 : 무기 업데이트 전 데미지 출력
				for (AWBWeaponBase* Weapon : Player->EquippedWeapons)
				{
					if (Weapon && Weapon->GetWeaponType() == static_cast<EWeaponType>(CardIndex))
					{
						UE_LOG(LogTemp, Warning, TEXT("Before Update - Weapon: %s, Damage: %d"), *WeaponTypeString, Weapon->Damage);
					}
				}
				
				bool bWeaponExists = false;

				// 무기 업데이트
				for (AWBWeaponBase* Weapon : Player->EquippedWeapons)
				{
					if (Weapon && Weapon->GetWeaponType() == static_cast<EWeaponType>(CardIndex))
					{
						Weapon->WeaponLevel = WeaponData->WeaponLevel;
						Weapon->Damage = WeaponData->Damage;
						Weapon->SkillAcceleration = WeaponData->SkillAcceleration;
						Weapon->CoolDown = WeaponData->CoolDown;
						Weapon->CriticalChance = WeaponData->CriticalChance;
						Weapon->ProjectileCount = WeaponData->ProjectileCount;
						Weapon->ProjectileClass = WeaponData->ProjectileClass;

						// 무기 업데이트 후 데미지 출력
						UE_LOG(LogTemp, Warning, TEXT("After Update - Weapon: %s, Damage: %d"), *WeaponTypeString, Weapon->Damage);

						bWeaponExists = true;
						break;
					}
				}

				if (!bWeaponExists)
				{
					// 새로운 무기 생성
					FActorSpawnParameters SpawnParams;
					SpawnParams.Owner = Player;
					AWBWeaponBase* NewWeapon = GetWorld()->SpawnActor<AWBWeaponBase>(Player->WeaponAttachBoxes[CardIndex], Player->GetActorLocation(), FRotator::ZeroRotator, SpawnParams);
					if (NewWeapon)
					{
						NewWeapon->AttachToComponent(Player->GetMesh(), FAttachmentTransformRules::KeepWorldTransform);
						NewWeapon->OwnerCharacter = Player;
						Player->EquippedWeapons.Add(NewWeapon);
					}
				}


				// 무기 레벨 업데이트
				PlayerState->ItemLevel.Add(WeaponTypeString, WeaponLevel);
			}
		}
		// 패시브 카드 처리 (5-9)
		else if (CardIndex >= 5 && CardIndex <= 9)
		{
			// 패시브 처리 로직은 이전과 동일합니다.
			FString PassiveKey = FString::FromInt(CardIndex);
			int16* CurrentLevelPtr = PlayerState->ItemLevel.Find(PassiveKey);
			int PassiveLevel = (CurrentLevelPtr) ? (*CurrentLevelPtr) + 1 : 1;

			switch (CardIndex)
			{
			case 5:
				PlayerState->Damage += 10; // 레벨당 +10%, 최대 50%
				break;
			case 6:
				PlayerState->CriticalHitChance += 8; // 레벨당 치명타 확률 8% 증가, 최대 +40%
				break;
			case 7:
				PlayerState->SkillAcceleration += 10; // 레벨당 스킬 가속 10 증가, 최대 +50
				break;
			case 8:
				PlayerState->MovementSpeed += 8; // 레벨당 이동 속도 8% 증가, 최대 +40%
				break;
			case 9:
				PlayerState->MaxHealth += 4; //레벨당 초당 체력 재생 4 증가, 최대 +20
				break;
			default:
				break;
			}

			// 패시브 레벨 업데이트
			PlayerState->ItemLevel.Add(FString::FromInt(CardIndex), 1); // 패시브 레벨 추가 (기본 레벨 1)
		}
	}
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
							AWBGameState* GS = Cast<AWBGameState>(GameState);
							if (IsValid(GS))
							{
								GS->S2C_MGSetTargetPlayer(Spawned, Player);
							}
							Spawned->TargetPlayer = Player;
							Spawned->TargetPlayer = Player;
							Spawned->MonsterClass = MonsterClass;
							Spawned->SpawnCount = SpawnCount;
							MonsterGroups.Emplace(Spawned);
							Spawned->SpawnRandomPositionMonster();
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
						AWBGameState* GS = Cast<AWBGameState>(GameState);
						if (IsValid(GS))
						{
							GS->S2C_MGSetTargetPlayer(Spawned, Player);
						}
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
			if (IsValid(Players[0]))
			{

				AWBMonsterGroup* Spawned = GetWorld()->SpawnActor<AWBMonsterGroup>(AWBMonsterGroup::StaticClass(), Players[0]->GetActorLocation(), FRotator::ZeroRotator, SpawnPara);
				if (Spawned)
				{
					AWBGameState* GS = Cast<AWBGameState>(GameState);
					if (IsValid(GS))
					{
						GS->S2C_MGSetTargetPlayer(Spawned, Players[0]);
					}
					Spawned->TargetPlayer = Players[0];
					Spawned->MonsterClass = MonsterClass;
					Spawned->SpawnCount = SpawnCount;
					MonsterGroups.Emplace(Spawned);
					Spawned->SpawnCirclePositionMonster();
				}
			}
		}
		break;
	case ESpawnType::Elite:
		{
			AWBMonsterBase* Spawned = GetWorld()->SpawnActor<AWBMonsterBase>(MonsterClass, FVector(x, y, 0), FRotator::ZeroRotator, SpawnPara);
			AWBGameState* GS = Cast<AWBGameState>(GameState);
			if (IsValid(GS))
			{
				GS->S2C_MBSetTargetPlayer(Spawned, GetNearPlayer(Spawned));
			}
			Spawned->SetTargetPlayer(GetNearPlayer(Spawned));
			
		}
		break;
	case ESpawnType::Boss:
		{
			AWBMonsterBase* Spawned = GetWorld()->SpawnActor<AWBMonsterBase>(MonsterClass, FVector(x, y, 0), FRotator::ZeroRotator, SpawnPara);
			Spawned->SetTargetPlayer(GetNearPlayer(Spawned));
		}
		break;
	default:
		break;
	}



}

AActor* AWBGameMode::GetNearPlayer(AActor* Monster)
{
	float Near = 9999.9999f;
	AActor* NearPlayer = nullptr;
	for (AActor* Player : Players)
	{
		float Dist = FVector::Dist(Monster->GetActorLocation(), Player->GetActorLocation());
		if (Near > Dist)
		{
			Near = Dist;
			NearPlayer = Player;
		}
	}

	return NearPlayer;
}
