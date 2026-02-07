// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "OverlayWidgetController.generated.h"


struct FOnAttributeChangeData;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHeathChangedSignature, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxHeathChangedSignature, float, NewMaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnManaChangedSignature, float, NewMana);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxManaChangedSignature, float, NewMaxMana);
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class GAME01_API UOverlayWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
public:
    virtual void BroadcastInitialValues() override;
    virtual void BindCallBackstoDepencies() override;
    UPROPERTY(BlueprintAssignable, Category= "GAS|Attributes")
    FOnHeathChangedSignature OnHealthChanged;
    
    UPROPERTY(BlueprintAssignable, Category= "GAS|Attributes")
    FOnMaxHeathChangedSignature OnMaxHealthChanged;
    
    UPROPERTY(BlueprintAssignable, Category= "GAS|Attributes")
    FOnManaChangedSignature OnManaChanged;
    
    UPROPERTY(BlueprintAssignable, Category= "GAS|Attributes")
    FOnMaxManaChangedSignature OnMaxManaChanged;

protected:
    void HealthChanged(const FOnAttributeChangeData& Data) const;
    void MaxHealthChanged(const FOnAttributeChangeData& Data) const;
    void ManaChanged(const FOnAttributeChangeData& Data) const;
    void MaxManaChanged(const FOnAttributeChangeData& Data) const;
};
