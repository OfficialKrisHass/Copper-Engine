#pragma once

namespace Copper {

	struct Version {

		std::string stage = "Invalid";
		uint32_t major = 0;
		uint32_t minor = 0;
		uint32_t dev = 0;

		uint32_t sceneVersion = 0;

		Version() = default;
		Version(const std::string& stage, uint32_t major, uint32_t minor, uint32_t dev) : stage(stage), major(major), minor(minor), dev(dev) {}

		bool operator==(const Version& other) const { return stage == other.stage && major == other.major && minor == other.minor && dev == other.dev; }

	};

	inline std::ostream& operator<<(std::ostream& os, const Version& version) {

		return os << version.stage << " " << version.major << "." << version.minor << "." << version.dev;

	}

}