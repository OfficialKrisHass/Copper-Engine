#pragma once

#include <Copper.h>

namespace Editor::ProjectChecker {

	enum ProjectIssue : Copper::uint16 {

		MissingAssets = FLAG(0),
		MissingBinaries = FLAG(1),

		MissingProjectDLL = FLAG(2),
		MissingScriptingDLL = FLAG(3),

		MissingProjectFile = FLAG(4),

#ifdef CU_WINDOWS
		MissingSolution = FLAG(5),
		MissingCSProj = FLAG(6),
#elif CU_LINUX
		MissingMakefile = FLAG(5),
		MissingPremake = FLAG(6),
#endif

	};

	Copper::uint16 CheckProject(const Project& project);
	void FixProject(Project& project, const Copper::uint16 issueFlags);
	
}
