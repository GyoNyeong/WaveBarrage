// Fill out your copyright notice in the Description page of Project Settings.


#include "WBPlayerState.h"
#include "Net/UnrealNetwork.h"

AWBPlayerState::AWBPlayerState()
{
    Armor = 0;
    StaminaRegeneration = 4;
}


int32 AWBPlayerState::GetDamageMultiplier() const
{
    // �⺻ ������ ������ 1.0 (��, ������ ���� ����)
    int32 DamageMultiplier = 1;

    // ������ ���� �ʿ��� "Damage" Ű�� �ش��ϴ� ������ ã��, ������ �⺻�� 0���� ����
    const int16* DamageLevelPtr = ItemLevel.Find("Damage");
    int16 DamageLevel = (DamageLevelPtr) ? *DamageLevelPtr : 0;

    // �� ������ 10%�� ������ ����
    DamageMultiplier += 0.1f * DamageLevel;

    return DamageMultiplier;
}

void AWBPlayerState::SetPlayerState(EPlayerState NewState)
{
    if (HasAuthority())
    {
        CurrentState = NewState;
        OnRep_PlayerState();
    }
}

void AWBPlayerState::OnRep_PlayerState()
{

}

void AWBPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AWBPlayerState, CurrentState);
}
