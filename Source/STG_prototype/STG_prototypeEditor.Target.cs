using UnrealBuildTool;
using System.Collections.Generic;

namespace STG_prototype // Added a namespace to avoid conflicts
{
    public class STG_prototypeEditorTarget : TargetRules
    {
        public STG_prototypeEditorTarget(TargetInfo Target) : base(Target)
        {
            Type = TargetType.Editor;
            DefaultBuildSettings = BuildSettingsVersion.V5; // Suppress upgrade warnings
            IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_5; // Use supported include order
            CppStandard = CppStandardVersion.Cpp20; // Use supported C++ standard

            // Use a unique build environment to allow property overrides
            BuildEnvironment = TargetBuildEnvironment.Unique;

            ExtraModuleNames.Add("STG_prototype");
        }
    }
}