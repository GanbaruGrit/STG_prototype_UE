using UnrealBuildTool;
using System.Collections.Generic;

namespace STG_prototype
{
    public class STG_prototypeTarget : TargetRules
    {
        public STG_prototypeTarget(TargetInfo Target) : base(Target)
        {
            Type = TargetType.Game;
            DefaultBuildSettings = BuildSettingsVersion.V5; // Suppress upgrade warnings
            IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_5; // Use supported include order
            CppStandard = CppStandardVersion.Cpp20; // Use supported C++ standard

            ExtraModuleNames.Add("STG_prototype");
        }
    }
}