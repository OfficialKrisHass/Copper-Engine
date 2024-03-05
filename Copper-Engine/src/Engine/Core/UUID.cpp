#include "cupch.h"
#include "UUID.h"

namespace Copper {

	static UUIDv4::UUIDGenerator<std::mt19937_64> generator;

	UUID GetUUID() { return generator.getUUID(); }

}