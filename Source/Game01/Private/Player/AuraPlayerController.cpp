// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}


void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(AuraContext);
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);
	Subsystem->AddMappingContext(AuraContext, 0);

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	
	
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
	
}

void AAuraPlayerController::PlayerTick(float DeltaTime) 
{
    Super::PlayerTick(DeltaTime);
    CursorTrace();
}
void AAuraPlayerController::CursorTrace() 
{
    FHitResult CursorHit;
    GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
    if (!CursorHit.bBlockingHit) return;
    /* 
    * A & E: Không có gì thay đổi (Cả hai đều rỗng hoặc là cùng một người) -> Không làm gì.

    B: Trước đó không có ai, giờ có kẻ địch -> Highlight ThisActor.

    C: Trước đó có kẻ địch, giờ rê chuột ra chỗ trống -> Bỏ Highlight LastActor.

    D: Rê chuột từ kẻ địch này sang kẻ địch khác -> Bỏ Highlight cũ, bật Highlight mới.
     */
    if (LastActor != ThisActor) 
    {
        // 1. Bỏ highlight đối tượng cũ (nếu có)
        if (LastActor) LastActor->UnHighLightActor();
        
        // 2. Bật highlight đối tượng mới (nếu có)
        if (ThisActor) ThisActor->HighLightActor();
    }

    // 3. Cập nhật LastActor cho khung hình tiếp theo
    LastActor = ThisActor;
    ThisActor = CursorHit.GetActor();
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	//const FRotator Rotation = InputActionValue.Get<FRotator>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
	
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection,InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection,InputAxisVector.X);
	}
}


