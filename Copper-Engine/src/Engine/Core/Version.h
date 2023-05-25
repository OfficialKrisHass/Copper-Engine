#pragma once

namespace Copper {

	struct Version {

		uint32_t major = 0;
		uint32_t minor = 0;
		uint32_t patch = 0;
		uint32_t tweak = 0;

		uint32_t sceneVersion = 0;

		Version() = default;
		Version(uint32_t major, uint32_t minor, uint32_t patch = 0, uint32_t tweak = 0) : major(major), minor(minor), patch(patch), tweak(tweak) {}

		bool operator==(const Version& other) const { return major == other.major && minor == other.minor && patch == other.patch && tweak == other.tweak; }

	};

	inline std::ostream& operator<<(std::ostream& os, const Version& version) {

		return os << version.major << "." << version.minor << "." << version.patch << "." << version.tweak;

	}

}