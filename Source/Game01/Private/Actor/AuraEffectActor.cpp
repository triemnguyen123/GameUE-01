// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor/AuraEffectActor.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Components/SphereComponent.h"

AAuraEffectActor::AAuraEffectActor() {

    PrimaryActorTick.bCanEverTick = false;
    SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}

void AAuraEffectActor::BeginPlay() {
    Super::BeginPlay();
}

void AAuraEffectActor::ApplyEffectToTarget(AActor *TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass) {
    UAbilitySystemComponent *TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
    if (TargetASC == nullptr)
        return;

    check(GameplayEffectClass);
    // tạo context gửi thông tin đi
    FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
    EffectContextHandle.AddSourceObject(this);
    // Bạn đưa vào GameplayEffectClass (ví dụ: bình mana).
    // Trong Blueprint của bình mana đó, bạn đã chọn Attribute: Mana.
    // Khi bạn gọi dòng dưới đây, hệ thống sẽ tìm đến Attribute "Mana" trong bình mana đó
    // và cộng thêm 50 vào giá trị hiện tại của người chơi.
    FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, 1.f, EffectContextHandle);
    // bơm dữ liệu vào Actor đã được gắn
    TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
}
