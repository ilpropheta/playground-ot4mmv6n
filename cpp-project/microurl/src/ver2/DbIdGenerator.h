#pragma once

class DbIdGenerator
{
	static long id;
public:
	long GenerateId(const char* token);
};