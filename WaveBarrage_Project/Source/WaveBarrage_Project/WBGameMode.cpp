// Fill out your copyright notice in the Description page of Project Settings.


#include "WBGameMode.h"
#include "WBGameState.h"
#include "WBPlayerController.h"
#include "WBPlayerState.h"
#include "WBMonsterBase.h"
#include "WBPlayerBase.h"
#include "WBMonsterGroup.h"
#include "Kismet/GameplayStatics.h"
//#include "WBULobbyWidget.h"


AWBGameMode::AWBGameMode()
{
	Level = 1;
	MaxExp = 10;
}

void AWBGameMode::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle THandle;
	GetWorld()->GetTimerManager().SetTimer(THandle, this, &AWBGameMode::UpdateTargetPlayer, 2.0f, true);
	//if (LobbyWidgetClass)
	//{
	//	// �κ� ���� ���� �� ȭ�鿡 �߰�
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
	UE_LOG(LogTemp, Warning, TEXT("EXP : %i"), Exp);
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


//void AWBGameMode::ApplyCardEffect(AWBPlayerController* PlayerController, int32 CardIndex)
//{
//	UE_LOG(LogTemp, Warning, TEXT("ApplayCardEffect"));
//		AWBPlayerBase* Player = Cast<AWBPlayerBase>(PlayerController->GetCharacter());
//		if (Player)
//		{
//			UE_LOG(LogTemp, Warning, TEXT("PlayerController: %s, Player: %s"),
//				*PlayerController->GetName(), *Player->GetName());
//			AWBPlayerState* PlayerState = Player->GetPlayerState<AWBPlayerState>();
//			if (!PlayerState)
//			{
//				return;
//			}
//
//			// ���� ī�� ó�� (0-4)
//			if (CardIndex >= 0 && CardIndex <= 4)
//			{
//				FString WeaponTypeString;
//				switch (static_cast<EWeaponType>(CardIndex))
//				{
//				case EWeaponType::WeaponJinx:
//					WeaponTypeString = "Jinx";
//					break;
//				case EWeaponType::WeaponWhirlwind:
//					WeaponTypeString = "Whirlwind";
//					break;
//				case EWeaponType::WeaponPoisonFootprint:
//					WeaponTypeString = "PoisonFootprint";
//					break;
//				case EWeaponType::WeaponBoomerang:
//					WeaponTypeString = "Boomerang";
//					break;
//				case EWeaponType::WeaponCuteLauncher:
//					WeaponTypeString = "CuteLauncher";
//					break;
//				default:
//					return; // ��ȿ���� ���� ���� Ÿ���� ��� ����
//				}
//
//				// ���� ���� ��������
//				int16* CurrentLevelPtr = PlayerState->ItemLevel.Find(WeaponTypeString);
//				int WeaponLevel = (CurrentLevelPtr) ? FMath::Min((*CurrentLevelPtr) + 1, 5) : 1;
//
//				// ������ ���̺��� �ش� ����� ������ �ɷ�ġ�� ��������
//				FName RowName = FName(*FString::Printf(TEXT("%s_%d"), *WeaponTypeString, WeaponLevel));
//				FWeaponData* WeaponData = WeaponDataTable->FindRow<FWeaponData>(RowName, TEXT(""));
//				if (WeaponData)
//				{
//					// �α� : ���� ������Ʈ �� ������ ���
//					for (AWBWeaponBase* Weapon : Player->EquippedWeapons)
//					{
//						if (Weapon && Weapon->GetWeaponType() == static_cast<EWeaponType>(CardIndex))
//						{
//							UE_LOG(LogTemp, Warning, TEXT("Before Update - Weapon: %s, Damage: %d"), *WeaponTypeString, Weapon->Damage);
//						}
//					}
//
//					bool bWeaponExists = false;
//
//					// ���� ������Ʈ
//					for (AWBWeaponBase* Weapon : Player->EquippedWeapons)
//					{
//						if (Weapon && Weapon->GetWeaponType() == static_cast<EWeaponType>(CardIndex))
//						{
//							Weapon->WeaponLevel = WeaponData->WeaponLevel;
//							Weapon->Damage = WeaponData->Damage;
//							Weapon->SkillAcceleration = WeaponData->SkillAcceleration;
//							Weapon->CoolDown = WeaponData->CoolDown;
//							Weapon->CriticalChance = WeaponData->CriticalChance;
//							Weapon->ProjectileCount = WeaponData->ProjectileCount;
//							Weapon->ProjectileClass = WeaponData->ProjectileClass;
//
//							// ���� ������Ʈ �� ������ ���
//							UE_LOG(LogTemp, Warning, TEXT("After Update - Weapon: %s, Damage: %d"), *WeaponTypeString, Weapon->Damage);
//
//							bWeaponExists = true;
//							break;
//						}
//					}
//
//					if (!bWeaponExists)
//					{
//						// ���ο� ���� ����
//						FActorSpawnParameters SpawnParams;
//						SpawnParams.Owner = Player;
//						AWBWeaponBase* NewWeapon = GetWorld()->SpawnActor<AWBWeaponBase>(Player->WeaponAttachBoxes[CardIndex], Player->GetActorLocation(), FRotator::ZeroRotator, SpawnParams);
//						if (NewWeapon)
//						{
//							NewWeapon->AttachToComponent(Player->GetMesh(), FAttachmentTransformRules::KeepWorldTransform);
//							NewWeapon->OwnerCharacter = Player;
//							Player->EquippedWeapons.Add(NewWeapon);
//						}
//					}
//
//
//					// ���� ���� ������Ʈ
//					PlayerState->ItemLevel.Add(WeaponTypeString, WeaponLevel);
//				}
//			}
//			// �нú� ī�� ó�� (5-9)
//			else if (CardIndex >= 5 && CardIndex <= 9)
//			{
//				// �нú� ó�� ������ ������ �����մϴ�.
//				FString PassiveKey = FString::FromInt(CardIndex);
//				int16* CurrentLevelPtr = PlayerState->ItemLevel.Find(PassiveKey);
//				int PassiveLevel = (CurrentLevelPtr) ? (*CurrentLevelPtr) + 1 : 1;
//
//				switch (CardIndex)
//				{
//				case 5:
//					PlayerState->Damage += 10; // ������ +10%, �ִ� 50%
//					break;
//				case 6:
//					PlayerState->CriticalHitChance += 8; // ������ ġ��Ÿ Ȯ�� 8% ����, �ִ� +40%
//					break;
//				case 7:
//					PlayerState->SkillAcceleration += 10; // ������ ��ų ���� 10 ����, �ִ� +50
//					break;
//				case 8:
//					PlayerState->MovementSpeed += 8; // ������ �̵� �ӵ� 8% ����, �ִ� +40%
//					break;
//				case 9:
//					PlayerState->MaxHealth += 4; //������ �ʴ� ü�� ��� 4 ����, �ִ� +20
//					break;
//				default:
//					break;
//				}
//
//				// �нú� ���� ������Ʈ
//				PlayerState->ItemLevel.Add(FString::FromInt(CardIndex), 1); // �нú� ���� �߰� (�⺻ ���� 1)
//			}
//		}
//	
//}

//void AWBGameMode::ApplyCardEffect_Implementation(AWBPlayerController* PlayerController, int32 CardIndex)
//{
//	if (PlayerController->IsLocalPlayerController())
//	{
//		UE_LOG(LogTemp, Warning, TEXT("ApplayCardEffect"));
//		AWBPlayerBase* Player = Cast<AWBPlayerBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
//		if (Player)
//		{
//			UE_LOG(LogTemp, Warning, TEXT("PlayerController: %s, Player: %s"),
//				*PlayerController->GetName(), *Player->GetName());
//			AWBPlayerState* PlayerState = Player->GetPlayerState<AWBPlayerState>();
//			if (!PlayerState)
//			{
//				return;
//			}
//
//			// ���� ī�� ó�� (0-4)
//			if (CardIndex >= 0 && CardIndex <= 4)
//			{
//				FString WeaponTypeString;
//				switch (static_cast<EWeaponType>(CardIndex))
//				{
//				case EWeaponType::WeaponJinx:
//					WeaponTypeString = "Jinx";
//					break;
//				case EWeaponType::WeaponWhirlwind:
//					WeaponTypeString = "Whirlwind";
//					break;
//				case EWeaponType::WeaponPoisonFootprint:
//					WeaponTypeString = "PoisonFootprint";
//					break;
//				case EWeaponType::WeaponBoomerang:
//					WeaponTypeString = "Boomerang";
//					break;
//				case EWeaponType::WeaponCuteLauncher:
//					WeaponTypeString = "CuteLauncher";
//					break;
//				default:
//					return; // ��ȿ���� ���� ���� Ÿ���� ��� ����
//				}
//
//				// ���� ���� ��������
//				int16* CurrentLevelPtr = PlayerState->ItemLevel.Find(WeaponTypeString);
//				int WeaponLevel = (CurrentLevelPtr) ? FMath::Min((*CurrentLevelPtr) + 1, 5) : 1;
//
//				// ������ ���̺��� �ش� ����� ������ �ɷ�ġ�� ��������
//				FName RowName = FName(*FString::Printf(TEXT("%s_%d"), *WeaponTypeString, WeaponLevel));
//				FWeaponData* WeaponData = WeaponDataTable->FindRow<FWeaponData>(RowName, TEXT(""));
//				if (WeaponData)
//				{
//					// �α� : ���� ������Ʈ �� ������ ���
//					for (AWBWeaponBase* Weapon : Player->EquippedWeapons)
//					{
//						if (Weapon && Weapon->GetWeaponType() == static_cast<EWeaponType>(CardIndex))
//						{
//							UE_LOG(LogTemp, Warning, TEXT("Before Update - Weapon: %s, Damage: %d"), *WeaponTypeString, Weapon->Damage);
//						}
//					}
//
//					bool bWeaponExists = false;
//
//					// ���� ������Ʈ
//					for (AWBWeaponBase* Weapon : Player->EquippedWeapons)
//					{
//						if (Weapon && Weapon->GetWeaponType() == static_cast<EWeaponType>(CardIndex))
//						{
//							Weapon->WeaponLevel = WeaponData->WeaponLevel;
//							Weapon->Damage = WeaponData->Damage;
//							Weapon->SkillAcceleration = WeaponData->SkillAcceleration;
//							Weapon->CoolDown = WeaponData->CoolDown;
//							Weapon->CriticalChance = WeaponData->CriticalChance;
//							Weapon->ProjectileCount = WeaponData->ProjectileCount;
//							Weapon->ProjectileClass = WeaponData->ProjectileClass;
//
//							// ���� ������Ʈ �� ������ ���
//							UE_LOG(LogTemp, Warning, TEXT("After Update - Weapon: %s, Damage: %d"), *WeaponTypeString, Weapon->Damage);
//
//							bWeaponExists = true;
//							break;
//						}
//					}
//
//					if (!bWeaponExists)
//					{
//						// ���ο� ���� ����
//						FActorSpawnParameters SpawnParams;
//						SpawnParams.Owner = Player;
//						AWBWeaponBase* NewWeapon = GetWorld()->SpawnActor<AWBWeaponBase>(Player->WeaponAttachBoxes[CardIndex], Player->GetActorLocation(), FRotator::ZeroRotator, SpawnParams);
//						if (NewWeapon)
//						{
//							NewWeapon->AttachToComponent(Player->GetMesh(), FAttachmentTransformRules::KeepWorldTransform);
//							NewWeapon->OwnerCharacter = Player;
//							Player->EquippedWeapons.Add(NewWeapon);
//						}
//					}
//
//
//					// ���� ���� ������Ʈ
//					PlayerState->ItemLevel.Add(WeaponTypeString, WeaponLevel);
//				}
//			}
//			// �нú� ī�� ó�� (5-9)
//			else if (CardIndex >= 5 && CardIndex <= 9)
//			{
//				// �нú� ó�� ������ ������ �����մϴ�.
//				FString PassiveKey = FString::FromInt(CardIndex);
//				int16* CurrentLevelPtr = PlayerState->ItemLevel.Find(PassiveKey);
//				int PassiveLevel = (CurrentLevelPtr) ? (*CurrentLevelPtr) + 1 : 1;
//
//				switch (CardIndex)
//				{
//				case 5:
//					PlayerState->Damage += 10; // ������ +10%, �ִ� 50%
//					break;
//				case 6:
//					PlayerState->CriticalHitChance += 8; // ������ ġ��Ÿ Ȯ�� 8% ����, �ִ� +40%
//					break;
//				case 7:
//					PlayerState->SkillAcceleration += 10; // ������ ��ų ���� 10 ����, �ִ� +50
//					break;
//				case 8:
//					PlayerState->MovementSpeed += 8; // ������ �̵� �ӵ� 8% ����, �ִ� +40%
//					break;
//				case 9:
//					PlayerState->MaxHealth += 4; //������ �ʴ� ü�� ��� 4 ����, �ִ� +20
//					break;
//				default:
//					break;
//				}
//
//				// �нú� ���� ������Ʈ
//				PlayerState->ItemLevel.Add(FString::FromInt(CardIndex), 1); // �нú� ���� �߰� (�⺻ ���� 1)
//			}
//		}
//	}
//}

void AWBGameMode::UpdateTargetPlayer()
{
	for (AWBMonsterGroup* MG : MonsterGroups)
	{
		AWBGameState* GS = Cast<AWBGameState>(GameState);
		if (IsValid(GS) && IsValid(MG))
		{
			AActor* NearPlayer = GetNearPlayer(MG);
			MG->TargetPlayer = NearPlayer;
			MG->UpdateTargetPlayer();
			GS->S2C_MGSetTargetPlayer(MG, NearPlayer);
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
			if (IsValid(Players[0]))
			{
				for (USceneComponent* Comp : Players[0]->MonsterSpawnPositions)
				{
					AWBMonsterGroup* Spawned = GetWorld()->SpawnActor<AWBMonsterGroup>(AWBMonsterGroup::StaticClass(), Comp->GetComponentLocation(), FRotator::ZeroRotator, SpawnPara);
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
						Spawned->SpawnRandomPositionMonster();
					}
				}
			}
		}
		break;
	case ESpawnType::Minion:
		{
			if (IsValid(Players[0]))
			{

				AWBMonsterGroup* Spawned = GetWorld()->SpawnActor<AWBMonsterGroup>(AWBMonsterGroup::StaticClass(), Players[0]->GetActorLocation() + FVector(x, y, 0), FRotator::ZeroRotator, SpawnPara);
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
					Spawned->SpawnMonster();
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
