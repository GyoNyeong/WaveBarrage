// Fill out your copyright notice in the Description page of Project Settings.


#include "WBFSMComponent.h"

// Sets default values for this component's properties
UWBFSMComponent::UWBFSMComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWBFSMComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UWBFSMComponent::IdleState()
{
	// ��� ���
}

void UWBFSMComponent::MoveState()
{
	//Ÿ�� �÷��̾� �������� �̵�
	//�̵� �� CanAttack �� true �̰� �Ÿ��� ��Ÿ� ���̶�� AttackState
	//�ƴϸ鼭 ���� �ð��� ������ IdleState
}

void UWBFSMComponent::AttackState()
{
	//���Ÿ� , ���� ���� ����
	//����ü �߻� �� ���� �ð� ������ IdleState
}

void UWBFSMComponent::SkillState()
{
	// ���� ����
	// ���� ���� ���� �ݸ��� �˻��ؼ� ���� ���� �÷��̾�� ������
	// ���� �ð� ������ IdleState
}

void UWBFSMComponent::DieState()
{
	// Destroy Actor �����ϴ� ���� �������� �ʵ��� �ϱ� ����
}


// Called every frame
void UWBFSMComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UWBFSMComponent::ChangeState()
{
}

