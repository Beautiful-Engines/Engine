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

};

#endif // !__Importer_H_
