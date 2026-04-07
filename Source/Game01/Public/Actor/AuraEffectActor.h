// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AuraEffectActor.generated.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

class UGameplayEffect;

UCLASS()
class GAME01_API AAuraEffectActor : public AActor {
    GENERATED_BODY()

  public:
    AAuraEffectActor();

  protected:
    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable)
    void ApplyEffectToTarget(AActor *TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass);

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
    TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;
};
