// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "FWBPlayerST.h"
#include "WBGameState.generated.h"

/**
 * 
 */
class AWBMonsterGroup;
class AWBMonsterBase;
class AWBPlayerState;
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerHPChanged, int32, PlayerID, int32, NewHP);
UCLASS()
class WAVEBARRAGE_PROJECT_API AWBGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	AWBGameState();
	
	virtual void BeginPlay() override;

	UFUNCTION(NetMulticast, Reliable)
	void S2C_MGSetTargetPlayer(AWBMonsterGroup* MG, AActor* TargetPlayer);
	void S2C_MGSetTargetPlayer_Implementation(AWBMonsterGroup* MG, AActor* TargetPlayer);

	UFUNCTION(NetMulticast, Reliable)
	void S2C_MBSetTargetPlayer(AWBMonsterBase* MB, AActor* TargetPlayer);
	void S2C_MBSetTargetPlayer_Implementation(AWBMonsterBase* MB, AActor* TargetPlayer);

	UFUNCTION()
	void CheckAllPlayersReady();
	
	// �÷��̾� HP �迭
	UPROPERTY(EditAnywhere,BlueprintReadWrite,ReplicatedUsing = OnRep_PlayerStates)
	TArray<FWBPlayerST> PlayerStates;

	UFUNCTION(BlueprintCallable)
	int32 AssignPlayerID();

	// HP ���� �� ȣ��Ǵ� �Լ�
	UFUNCTION()
	void OnRep_PlayerStates();

	// �÷��̾� HP ������Ʈ �Լ�
	UFUNCTION(BlueprintCallable)
	void UpdatePlayerHP(int32 PlayerID, int32 Damage);

	UFUNCTION(BlueprintCallable)
	// ���� ���� ���� Ȯ��
	void EndGame();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	FTimerHandle FTimerHandle_HPHill;

	UFUNCTION()
	void UpdateAllPlayerHPHell();

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Replicated)
	int Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	int MaxExp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	int Exp;

	//// �÷��̾� HP ��ȭ ��������Ʈ
	//UPROPERTY(BlueprintAssignable, Category = "Events")
	//FOnPlayerHPChanged OnPlayerHPChanged;
};
