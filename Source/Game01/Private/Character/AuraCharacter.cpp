// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUB/AuraHUD.h"

AAuraCharacter::AAuraCharacter()
{
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 400.0f, 0.0f);
    GetCharacterMovement()->bConstrainToPlane = true;
    GetCharacterMovement()->bSnapToPlaneAtStart = true;
    
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;
}

void AAuraCharacter::PossessedBy(AController *NewController) 
{
    Super::PossessedBy(NewController);
    //~ Init Ability On Server
    InitAbilityActorInfo();
}

void AAuraCharacter::OnRep_PlayerState() 
{
    Super::OnRep_PlayerState();
    //~ Init Ability On Client
    InitAbilityActorInfo();
}

void AAuraCharacter::InitAbilityActorInfo() 
{
    AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
    check(AuraPlayerState);
    AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState, this);
    AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
    AttributeSet = AuraPlayerState->GetAttributeSet();
    
    // 4. Lấy PlayerController và Cast sang lớp AuraPlayerController của mình
    if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
    {
        // 5. Từ Controller, lấy lớp HUD và Cast sang AAuraHUD
        if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
        {
            // 6. Gọi hàm khởi tạo UI và truyền vào 4 "mảnh ghép" quan trọng
            // PC: AuraPlayerController
            // PS: AuraPlayerState
            // ASC: AbilitySystemComponent
            // AS: AttributeSet
            AuraHUD->InitOverlay(AuraPlayerController, AuraPlayerState, AbilitySystemComponent, AttributeSet);
        }
    }
    
}
