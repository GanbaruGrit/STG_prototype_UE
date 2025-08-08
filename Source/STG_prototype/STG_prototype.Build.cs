// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

namespace STG_prototypeNamespace // Added a namespace to avoid conflicts
{
    public class STG_prototype : ModuleRules
    {
        public STG_prototype(ReadOnlyTargetRules Target) : base(Target)
        {
            PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

            PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "Niagara", "GameplayTasks" });

            PrivateDependencyModuleNames.AddRange(new string[] { });

            // Uncomment if you are using Slate UI
            // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

            // Uncomment if you are using online features
            // PrivateDependencyModuleNames.Add("OnlineSubsystem");

            // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
        }
    }
}
