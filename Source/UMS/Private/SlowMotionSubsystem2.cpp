#include "SlowMotionSubsystem2.h"
#include "Kismet/GameplayStatics.h"
#include "Curves/CurveFloat.h"

void USlowMotionSubsystem::StartSlowMotion(float Duration, float TimeDilation)
{
    // Stop any existing slow-mo
    if (SlowMotionTimerHandle.IsValid())
    {
        GetWorld()->GetTimerManager().ClearTimer(SlowMotionTimerHandle);
    }

    // Load the curve asset (create one in UE5 Editor and assign it here)
    static ConstructorHelpers::FObjectFinder<UCurveFloat> CurveFinder(TEXT("/Game/Path/To/Your/CurveAsset"));
    if (CurveFinder.Succeeded())
    {
        SlowMotionCurve = CurveFinder.Object;
    }

    TargetTimeDilation = TimeDilation;

    // Start the timeline effect
    float ElapsedTime = 0.0f;
    GetWorld()->GetTimerManager().SetTimer(SlowMotionTimerHandle, [this, ElapsedTime, Duration]() mutable
        {
            ElapsedTime += GetWorld()->GetDeltaSeconds();
            float Alpha = FMath::Clamp(ElapsedTime / Duration, 0.0f, 1.0f);

            // Use the curve to interpolate smoothly
            float CurveValue = SlowMotionCurve ? SlowMotionCurve->GetFloatValue(Alpha) : Alpha;
            float CurrentTimeDilation = FMath::Lerp(1.0f, TargetTimeDilation, CurveValue);
            UGameplayStatics::SetGlobalTimeDilation(GetWorld(), CurrentTimeDilation);

            // Reset when finished
            if (Alpha >= 1.0f)
            {
                ResetTimeDilation();
            }
        }, GetWorld()->GetDeltaSeconds(), true);
}

void USlowMotionSubsystem::ResetTimeDilation()
{
    GetWorld()->GetTimerManager().ClearTimer(SlowMotionTimerHandle);
    UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
}