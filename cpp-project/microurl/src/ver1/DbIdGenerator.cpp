#include "DbIdGenerator.h"

long DbIdGenerator::id = 100;

long DbIdGenerator::GenerateId(const char* token)
{
	return id++;
}
