#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SlowMotionSubsystem.generated.h"

// Forward declaration for the timeline curve
class UCurveFloat;

UCLASS()
class YOURPROJECT_API USlowMotionSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    // Starts the slow motion effect
    UFUNCTION(BlueprintCallable, Category = "Slow Motion")
    void StartSlowMotion(float Duration, float TimeDilation = 0.2f);

private:
    // The curve asset to control time dilation interpolation
    UPROPERTY()
    UCurveFloat* SlowMotionCurve;

    // Timeline to handle the interpolation
    FTimerHandle SlowMotionTimerHandle;

    // Current target time dilation (e.g., 0.2 for slow-mo)
    float TargetTimeDilation;

    // Callback to update time dilation
    void UpdateTimeDilation(float Alpha);

    // Resets time dilation to 1.0
    void ResetTimeDilation();
};