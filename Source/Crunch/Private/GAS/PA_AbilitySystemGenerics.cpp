#include "GAS/PA_AbilitySystemGenerics.h"

const FRealCurve* UPA_AbilitySystemGenerics::GetExperienceCurve() const
{
    return ExperienceCurveTable->FindCurve(ExperienceRowName, "");
}
