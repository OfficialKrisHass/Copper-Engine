#pragma once

#include "Projects/Project.h"

#define BIT(x) (1 << x)

namespace Editor::ProjectChecker {

	enum ProjectIssue : Copper::uint16 {

		MissingAssets = BIT(0),
		MissingBinaries = BIT(1),

		MissingProjectDLL = BIT(2),
		MissingScriptingDLL = BIT(3),

		MissingProjectFile = BIT(4),

#ifdef CU_WINDOWS
		MissingSolution = BIT(5),
		MissingCSProj = BIT(6),
#elif CU_LINUX
		MissingMakefile = BIT(5),
		MissingPremake = BIT(6),
#endif

	};

	Copper::uint16 CheckProject(const Project& project);
	void FixProject(Project& project, const Copper::uint16 issueFlags);
	
}