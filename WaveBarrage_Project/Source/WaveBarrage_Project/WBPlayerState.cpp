// Fill out your copyright notice in the Description page of Project Settings.


#include "WBPlayerState.h"

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
