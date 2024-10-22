#ifndef __Timer_H__
#define __Timer_H__

#include "Globals.h"
#include "SDL\include\SDL.h"

class Timer
{
public:

	// Constructor
	Timer();

	void Start();
	void Stop();

	void Pause();

	void SetRunning(bool runn);

	Uint32 Read();
	Uint32 ReadSec();

private:

	bool	paused = false;
	bool	running;
	Uint32	started_at;
	Uint32	stopped_at;
};

#endif // !__Timer_H__