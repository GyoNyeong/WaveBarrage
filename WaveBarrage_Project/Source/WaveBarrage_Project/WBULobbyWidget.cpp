// Fill out your copyright notice in the Description page of Project Settings.


#include "WBULobbyWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"

void UWBULobbyWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (CreateSessionButton)
    {
        CreateSessionButton->OnClicked.AddDynamic(this, &UWBULobbyWidget::OnCreateSessionClicked);
    }

    if (JoinSessionButton)
    {
        JoinSessionButton->OnClicked.AddDynamic(this, &UWBULobbyWidget::OnJoinSessionClicked);
    }

    if (ProjectNameText)
    {
        ProjectNameText->SetText(FText::FromString(TEXT("Wave Barrage Project")));
    }

    // �߰�������, GameImage�� ���� ������ �� �� �ֽ��ϴ� (�̹����� �����ϴ� �ڵ�)
}

void UWBULobbyWidget::OnCreateSessionClicked()
{
    // ���� ���� ���� �߰�
    UE_LOG(LogTemp, Warning, TEXT("Create Session Clicked"));
}

void UWBULobbyWidget::OnJoinSessionClicked()
{
    // ���� ���� ���� �߰�
    UE_LOG(LogTemp, Warning, TEXT("Join Session Clicked"));
}
