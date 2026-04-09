// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "AuraEffectActor.generated.h"

class UGameplayEffect;

UENUM(BlueprintType)
enum class EEffectApplicationPolicy : uint8 {
    ApplyOnOverlap,
    ApplyOnEndOverlap,
    DoNotApply
};

UENUM(BlueprintType)
enum class EEffectRemovalPolicy : uint8 {
    RemoveOnEndOverlap,
    DoNotRemove
};

UCLASS()
class GAME01_API AAuraEffectActor : public AActor {
    GENERATED_BODY()

  public:
    AAuraEffectActor();

  protected:
    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable)
    void ApplyEffectToTarget(AActor *TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass);

    UFUNCTION(BlueprintCallable)
    void OnOverlap(AActor *TargetActor);

    UFUNCTION(BlueprintCallable)
    void OnEndOverlap(AActor *TargetActor);

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
    bool bDestroyOnEffectRemoval = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Instant Effects")
    TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Instant Effects")
    EEffectApplicationPolicy InstantEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Duration Effects")
    TSubclassOf<UGameplayEffect> DurationGameplayEffectClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Duration Effects")
    EEffectApplicationPolicy DurationEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Infinite Effects")
    TSubclassOf<UGameplayEffect> InfiniteGameplayEffectClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Infinite Effects")
    EEffectApplicationPolicy InfiniteEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Infinite Effects")
    EEffectRemovalPolicy InfiniteEffectRemovalPolicy = EEffectRemovalPolicy::RemoveOnEndOverlap;

    TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent *> ActiveEffectHandles;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
    float EffectLevel = 1.f;
};
