// Fill out your copyright notice in the Description page of Project Settings.


#include "WBPlayerController.h"
#include "WBGameState.h"
#include "WBPlayerState.h"
#include "WBMonsterBase.h"
#include "WBPlayerBase.h"
#include "WBMonsterGroup.h"
#include "Kismet/GameplayStatics.h"
#include "WBGameMode.h"
//void AWBPlayerController::ShowCardSelectionWidget()
//{
	//if (widgetClass)
	//{
 //       if (!CardSelectionWidgetInstance)
 //       {
 //           CardSelectionWidgetInstance = CreateWidget<UUserWidget>(this, widgetClass);
 //           if (CardSelectionWidgetInstance)
 //           {
 //               CardSelectionWidgetInstance->AddToViewport();
 //           }
 //       }
	//}
//}

AWBPlayerController::AWBPlayerController()
{
	WeaponDataTable = CreateDefaultSubobject<UDataTable>("WeaponDataTable");
}

void AWBPlayerController::BeginPlay()
{
	Super::BeginPlay();


}

void AWBPlayerController::CardClicked()
{
	C2S_SetPlayerReady();
}

void AWBPlayerController::C2S_SetPlayerReady_Implementation()
{
	AWBPlayerState* MyPlayerState = GetPlayerState<AWBPlayerState>();
	if (MyPlayerState)
	{
		MyPlayerState->SetPlayerState(EPlayerState::Ready);
	}

	AWBGameState* MyGameState = GetWorld()->GetGameState<AWBGameState>();
	if (MyGameState)
	{
		MyGameState->CheckAllPlayersReady();
	}
}

void AWBPlayerController::ApplyCardEffect_Implementation(AWBPlayerController* PlayerController, int32 CardIndex)
{
	if (PlayerController->IsLocalPlayerController())
	{
		AWBPlayerBase* MyPlayer = Cast<AWBPlayerBase>(PlayerController->GetCharacter());
		if (MyPlayer)
		{
			AWBPlayerState* MyPlayerState = MyPlayer->GetPlayerState<AWBPlayerState>();
			if (!MyPlayerState)
			{
				return;
			}

			// ���� ī�� ó�� (0-4)
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
					return; // ��ȿ���� ���� ���� Ÿ���� ��� ����
				}
				// ���� ���� ��������
				int16* CurrentLevelPtr = MyPlayerState->ItemLevel.Find(WeaponTypeString);
				int MyWeaponLevel = (CurrentLevelPtr) ? FMath::Min((*CurrentLevelPtr) + 1, 5) : 1;

				// ������ ���̺��� �ش� ����� ������ �ɷ�ġ�� ��������
				FName RowName = FName(*FString::Printf(TEXT("%s_%d"), *WeaponTypeString, MyWeaponLevel));
				FWeaponData* WeaponData = WeaponDataTable->FindRow<FWeaponData>(RowName, TEXT(""));
				if (WeaponData)
				{
					// �α� : ���� ������Ʈ �� ������ ���
/*					for (AWBWeaponBase* Weapon : MyPlayer->EquippedWeapons)
					{
						if (Weapon && Weapon->GetWeaponType() == static_cast<EWeaponType>(CardIndex))
						{
							UE_LOG(LogTemp, Warning, TEXT(" CurWeapon: %s"), *WeaponTypeString);
						}
					}*/

					bool bWeaponExists = false;


					// ���� ������Ʈ
					for (AWBWeaponBase* Weapon : MyPlayer->EquippedWeapons)
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


							// ���� ������Ʈ �� ������ ���
							UE_LOG(LogTemp, Warning, TEXT("After Update - Weapon: %s, Level: %d"), *WeaponTypeString, Weapon->WeaponLevel);

							bWeaponExists = true;

							break;
						}
					}
					UE_LOG(LogTemp, Warning, TEXT("bWeaponExists value: %s"), bWeaponExists ? TEXT("true") : TEXT("false"));

					//if (!bWeaponExists)
					//{
					//	UE_LOG(LogTemp, Warning, TEXT("ReqWeaponCreate"));
					//	C2S_SpawnWeapon(CardIndex, MyPlayer);
					//}
					if (!bWeaponExists)
					{
						//// ���ο� ���� ����
						//FActorSpawnParameters SpawnParams;
						//SpawnParams.Owner = MyPlayer;
						//AWBWeaponBase* NewWeapon = GetWorld()->SpawnActor<AWBWeaponBase>(MyPlayer->WeaponAttachBoxes[CardIndex], MyPlayer->GetActorLocation(), FRotator::ZeroRotator, SpawnParams);
						//if (NewWeapon)
						//{
						//	NewWeapon->AttachToComponent(MyPlayer->GetMesh(), FAttachmentTransformRules::KeepWorldTransform);
						//	NewWeapon->OwnerCharacter = MyPlayer;
						//	MyPlayer->EquippedWeapons.Add(NewWeapon);
						//}
						
						C2S_SpawnWeapon(CardIndex, MyPlayer);
						
					}


					// ���� ���� ������Ʈ
					MyPlayerState->ItemLevel.Add(WeaponTypeString, MyWeaponLevel);
				}

			}
			// �нú� ī�� ó�� (5-9)
			else if (CardIndex >= 5 && CardIndex <= 9)
			{

				// �нú� ó�� ������ ������ �����մϴ�.
				FString PassiveKey = FString::FromInt(CardIndex);
				int16* CurrentLevelPtr = MyPlayerState->ItemLevel.Find(PassiveKey);
				int PassiveLevel = (CurrentLevelPtr) ? (*CurrentLevelPtr) + 1 : 1;

				switch (CardIndex)
				{
				case 5:
					MyPlayerState->Damage += 10; // ������ +10%, �ִ� 50%
					break;
				case 6:
					MyPlayerState->CriticalHitChance += 8; // ������ ġ��Ÿ Ȯ�� 8% ����, �ִ� +40%
					break;
				case 7:
					MyPlayerState->SkillAcceleration += 10; // ������ ��ų ���� 10 ����, �ִ� +50
					break;
				case 8:
					MyPlayerState->MovementSpeed += 8; // ������ �̵� �ӵ� 8% ����, �ִ� +40%
					break;
				case 9:
					MyPlayerState->MaxHealth += 4; //������ �ʴ� ü�� ��� 4 ����, �ִ� +20
					break;
				default:
					break;
				}

				// �нú� ���� ������Ʈ
				MyPlayerState->ItemLevel.Add(FString::FromInt(CardIndex), 1); // �нú� ���� �߰� (�⺻ ���� 1)
			}
		}
	}
}

void AWBPlayerController::C2S_ApplyCardEffect_Implementation(AWBPlayerController* PlayerController, int32 CardIndex)
{
	if (HasAuthority())
	{
		ApplyCardEffect(PlayerController, CardIndex);
	}
}

void AWBPlayerController::C2S_SpawnWeapon_Implementation(int32 CardIndex, AWBPlayerBase* MyPlayer)
{
	UE_LOG(LogTemp, Warning, TEXT("WeaponCreateStart"));
	if (HasAuthority()) // ���������� ����
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = MyPlayer;

		AWBWeaponBase* NewWeapon = GetWorld()->SpawnActor<AWBWeaponBase>(
			MyPlayer->WeaponAttachBoxes[CardIndex],
			MyPlayer->GetActorLocation(),
			FRotator::ZeroRotator,
			SpawnParams);

		if (NewWeapon)
		{
			NewWeapon->AttachToComponent(MyPlayer->GetMesh(), FAttachmentTransformRules::KeepWorldTransform);
			NewWeapon->OwnerCharacter = MyPlayer;
			MyPlayer->EquippedWeapons.Add(NewWeapon);

		}
	}

}