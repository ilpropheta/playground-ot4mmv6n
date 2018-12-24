#pragma once

class IIdGenerator
{
public:
	virtual long Generate(const char* token) = 0;
};