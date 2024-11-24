// Fill out your copyright notice in the Description page of Project Settings.


#include "WBGameState.h"
#include "WBMonsterBase.h"
#include "WBMonsterGroup.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "WBPlayerState.h"
#include "WBGameMode.h"
#include "FWBPlayerST.h"
#include "Engine/Engine.h"

AWBGameState::AWBGameState()
{
    // ���������� �ʱ�ȭ
    if (HasAuthority())
    {
        // �ʱ�ȭ�� �÷��̾� ���� ���� PlayerStates �迭�� ä��ϴ�.
        // ���� ���, �ִ� 4���� �÷��̾���:
        PlayerStates.SetNum(4);
        for (int32 i = 0; i < PlayerStates.Num(); ++i)
        {
            PlayerStates[i] = FWBPlayerST(0, 1000);
        }
    }

    //GetWorld()->GetTimerManager().SetTimer(FTimerHandle_HPHill, this, &AWBGameState::UpdateAllPlayerHPHell, 3.0f, true);
}

void AWBGameState::S2C_MGSetTargetPlayer_Implementation(AWBMonsterGroup* MG, AActor* TargetPlayer)
{
	if (IsValid(MG) && IsValid(TargetPlayer))
	{
		MG->TargetPlayer = TargetPlayer;
	}
}

void AWBGameState::S2C_MBSetTargetPlayer_Implementation(AWBMonsterBase* MB, AActor* TargetPlayer)
{
    if (IsValid(MB) && IsValid(TargetPlayer))
    {
        MB->SetTargetPlayer(TargetPlayer);
    }
}

void AWBGameState::CheckAllPlayersReady()
{
    for (APlayerState* PlayerState : PlayerArray)
    {
        AWBPlayerState* MyPlayerState = Cast<AWBPlayerState>(PlayerState);
        if (MyPlayerState && MyPlayerState->CurrentState != EPlayerState::Ready)
        {
            return; // �� ���̶� Ready�� �ƴϸ� ����
        }
    }
        // ��� �÷��̾ �غ�Ǿ��� ���
        UGameplayStatics::SetGamePaused(GetWorld(), false);
       
        for (APlayerState* PlayerState : PlayerArray)
        {
            AWBPlayerState* MyPlayerState = Cast<AWBPlayerState>(PlayerState);
            MyPlayerState->SetPlayerState(EPlayerState::Waiting);
        }
}

int32 AWBGameState::AssignPlayerID()
{
    for (int32 i = 0; i < PlayerStates.Num(); ++i)
    {
        if (PlayerStates[i].PlayerID == 0)
        {
            PlayerStates[i].PlayerID = i + 1; // 1���� ����
            PlayerStates[i].HP = 1000;
            return PlayerStates[i].PlayerID;
        }
    }
    return -1; // �Ҵ��� �� ���� ���
}

void AWBGameState::OnRep_PlayerStates()
{
    // PlayerStates�� ����Ǿ��� �� Ŭ���̾�Ʈ���� ó���� ������ ���⿡ �ۼ�
    // ��: UI ������Ʈ ��
    for (const auto& PlayerState : PlayerStates)
    {
        UE_LOG(LogTemp, Log, TEXT("PlayerID: %d, HP: %d"), PlayerState.PlayerID, PlayerState.HP);
    }
}

void AWBGameState::UpdatePlayerHP(int32 PlayerID, int32 Damage)
{
    if (!HasAuthority())
    {
        UE_LOG(LogTemp, Warning, TEXT("UpdatePlayerHP not Server."));
        return;
    }

    for (auto& PlayerState : PlayerStates)
    {
        if (PlayerState.PlayerID == PlayerID)
        {
            PlayerState.HP -= Damage;
            PlayerState.HP = FMath::Max(PlayerState.HP, 0);
            UE_LOG(LogTemp, Warning, TEXT("PlayerID %d took %d damage, HP now %d"), PlayerID, Damage, PlayerState.HP);

            if (PlayerState.HP <= 0)
            {
                EndGame();
            }

            // �迭 ��ü�� �ٽ� �����ϵ��� MarkItemDirty�� ȣ������ �ʰ�, PlayerStates �迭�� �����߱� ������ OnRep_PlayerStates�� ȣ��˴ϴ�.
            // ���� ������ ���� ȣ���� �ʿ����� �ʽ��ϴ�.
            break;
        }
    }
}

void AWBGameState::EndGame()
{
    if (!HasAuthority())
    {
        UE_LOG(LogTemp, Warning, TEXT("EndGame Client Call Err."));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("EndGame."));

    // ���� ���� ���� ����
    // ��: ���� ��忡 ���� ��ȣ ������, ��� ȭ������ ��ȯ ��
    // ���÷� ��� �÷��̾�� ���� ���� �޽����� ��ε�ĳ��Ʈ�մϴ�.
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        APlayerController* PC = It->Get();
        if (PC)
        {
            // Ŭ���̾�Ʈ���� ���� ���� �˸��� �����ϴ�.
            // Blueprint�� �ٸ� ������� Ŭ���̾�Ʈ UI�� ������Ʈ�� �� �ֽ��ϴ�.
            PC->ClientMessage(TEXT("Game End!!!."));
        }
    }
}

void AWBGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AWBGameState, PlayerStates);
}

void AWBGameState::UpdateAllPlayerHPHell()
{

}
