#include "cupch.h"
#include "UUID.h"

namespace Copper {

	static UUIDv4::UUIDGenerator<std::mt19937_64> generator;
	static const UUID emptyUUID = UUID::fromStrFactory("00000000-0000-0000-0000-000000000000");

	UUID GetUUID() { return generator.getUUID(); }
	const UUID& EmptyUUID() { return emptyUUID; }

}