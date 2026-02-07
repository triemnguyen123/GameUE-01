# Architecture Prompt - Game01 (Aura RPG)

Unreal Engine 5 RPG game architecture using Gameplay Ability System (GAS).

---

## Core Design Foundation

### Root: AuraCharacterBase (Abstract Base Class)
This is the foundation - all characters (Player & Enemy) inherit from this base.

**AuraCharacterBase** contains:
- `UAbilitySystemComponent* AbilitySystemComponent` (pointer reference)
- `UAttributeSet* AttributeSet` (pointer reference)
- `USkeletalMeshComponent* Weapon` (created in constructor)
- Implements `IAbilitySystemInterface`

From this base, the architecture splits into 2 character types with different GAS ownership patterns:

---

## Branch 1: Player Character Flow (GAS owned by PlayerState)

```
AuraCharacterBase (Abstract Root)
    ↓ inherits
AuraCharacter (Player Character)
    ↓ on PossessedBy (Server) / OnRep_PlayerState (Client)
    ↓ calls InitAbilityActorInfo()
    ↓
    ├─→ Get AuraPlayerState
    │       ↓ owns (created in constructor)
    │       ├─→ AuraAbilitySystemComponent (replication mode: Mixed)
    │       └─→ AuraAttributeSet
    │       ↓
    │   PlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(PlayerState, Character)
    │       ↓
    │   Character stores references:
    │       ├─→ AbilitySystemComponent = PlayerState's ASC
    │       └─→ AttributeSet = PlayerState's AttributeSet
    │
    └─→ Get AuraPlayerController
            ↓ get HUD
        AuraHUD
            ↓ calls InitOverlay()
            ↓ creates
            ├─→ OverlayWidgetController
            │       ↓ receives FWidgetControllerParams
            │       ├─→ PlayerController
            │       ├─→ PlayerState
            │       ├─→ AbilitySystemComponent (from PlayerState)
            │       └─→ AttributeSet (from PlayerState)
            │       ↓
            │       ├─→ BindCallBacksToDepencies() (listen to ASC attribute changes)
            │       └─→ BroadcastInitialValues() (send initial data to UI)
            │
            └─→ AuraUserWidget
                    ↓ receives
                OverlayWidgetController
                    ↓ displays
                Health/Mana bars
```

**Key Point**: Player's GAS components live in PlayerState (persist across respawns)

---

## Branch 2: Enemy Character Flow (GAS owned by Enemy itself)

```
AuraCharacterBase (Abstract Root)
    ↓ inherits
AuraEnemy (Enemy Character)
    ↓ in constructor
    ↓ creates directly
    ├─→ AuraAbilitySystemComponent (replication mode: Minimal)
    └─→ AuraAttributeSet
    ↓
    ↓ on BeginPlay()
    ↓ calls
AbilitySystemComponent->InitAbilityActorInfo(this, this)
    (Owner = this, Avatar = this)
    
AuraEnemy
    ↓ implements
IEnemyInterface
    ├─→ HighLightActor() (enable custom depth stencil)
    └─→ UnHighLightActor() (disable custom depth stencil)
```

**Key Point**: Enemy's GAS components are owned by the enemy actor itself (destroyed with actor)

---

## Player Input & Enemy Interaction Flow

```
AuraPlayerController
    ↓ every frame (PlayerTick)
    ↓ performs CursorTrace()
    ↓ line trace from cursor
    ↓ hit actor?
    ↓
    ├─→ Implements IEnemyInterface?
    │       ↓ Yes
    │   Call HighLightActor()
    │       ↓
    │   Enemy->GetMesh()->SetRenderCustomDepth(true)
    │   Enemy->GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED = 250)
    │   Enemy->Weapon->SetRenderCustomDepth(true)
    │
    └─→ No interface or no hit
        Call UnHighLightActor() on last actor
```

---

## UI Update Flow (Attribute Changes)

```
Gameplay Effect Applied
    ↓
AbilitySystemComponent (from PlayerState or Enemy)
    ↓ modifies
AttributeSet (Health, Mana, MaxHealth, MaxMana)
    ↓ triggers replication
OnRep_Health() / OnRep_Mana() / OnRep_MaxHealth() / OnRep_MaxMana()
    ↓ broadcasts attribute change event
OverlayWidgetController (listening via BindCallBacksToDepencies)
    ↓ receives callback
HealthChanged(Data) / MaxHealthChanged(Data)
    ↓ broadcasts to Blueprint
OnHealthChanged / OnMaxHealthChanged delegates
    ↓ received by
AuraUserWidget (Blueprint)
    ↓ updates
Visual UI elements (progress bars, text)
```

---

## Key Architectural Decisions

### 1. Abstract Base Pattern
**AuraCharacterBase** is the root - enforces that all characters must have GAS integration.

### 2. Different GAS Ownership Models
- **Player**: GAS owned by PlayerState (survives respawn, network-friendly)
- **Enemy**: GAS owned by self (simpler, destroyed with actor)

### 3. Initialization Timing
- **Player**: InitAbilityActorInfo called in `PossessedBy()` (server) and `OnRep_PlayerState()` (client)
- **Enemy**: InitAbilityActorInfo called in `BeginPlay()`

### 4. UI Initialization Chain
```
Character possessed → InitAbilityActorInfo() → Get PlayerController → Get HUD → InitOverlay() → Create WidgetController → Bind to ASC → Create Widget → Display
```

### 5. MVC Pattern for UI
- **Model**: AttributeSet (data)
- **Controller**: OverlayWidgetController (logic, binds to ASC events)
- **View**: AuraUserWidget (Blueprint visual)

---

## Class Hierarchy Summary

```
ACharacter (Unreal Engine)
    ↓
AuraCharacterBase (Abstract - Root of character system)
    ├─→ AuraCharacter (Player)
    └─→ AuraEnemy (Enemy + IEnemyInterface)

APlayerState (Unreal Engine)
    ↓
AuraPlayerState (owns GAS for player)
    ├─→ AuraAbilitySystemComponent
    └─→ AuraAttributeSet

AHUD (Unreal Engine)
    ↓
AuraHUD (manages UI creation)
    ├─→ creates OverlayWidgetController
    └─→ creates AuraUserWidget

UObject
    ↓
AuraWidgetController (base)
    ↓
OverlayWidgetController (binds to GAS)
```

---

Use Unreal Engine 5 and Gameplay Ability System infrastructure where applicable.
