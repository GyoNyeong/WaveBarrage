// Fill out your copyright notice in the Description page of Project Settings.


#include "WBGameState.h"
#include "WBMonsterBase.h"
#include "WBMonsterGroup.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "WBPlayerState.h"
void AWBGameState::S2C_MGSetTargetPlayer_Implementation(AWBMonsterGroup* MG, AActor* TargetPlayer)
{
	if (IsValid(MG) && IsValid(TargetPlayer))
	{
		MG->TargetPlayer = TargetPlayer;
	}
}

void AWBGameState::S2C_MBSetTargetPlayer_Implementation(AWBMonsterBase* MB, AActor* TargetPlayer)
{
	MB->SetTargetPlayer(TargetPlayer);
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

