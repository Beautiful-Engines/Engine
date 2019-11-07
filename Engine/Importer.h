#ifndef __Importer_H_
#define __Importer_H_

#include "Globals.h"

class Importer
{
public:
	Importer() {};
	~Importer() {};

	virtual bool Init() { return true; }
	virtual bool CleanUp() { return true; }

	virtual bool Import(const char* file) { return true; }
	virtual bool Import(const char* file, std::string& output_file) { return true; }
	virtual bool Import(const void* buffer, uint size, std::string& output_file) { return true; }
};

#endif // !__Importer_H_
