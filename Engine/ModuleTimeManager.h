#ifndef __MODULE_TIME_MANAGER_H__
#define __MODULE_TIME_MANAGER_H__

#include "Module.h"
#include "Timer.h"

enum STATE {
	PLAY,
	PAUSE,
	EDITOR,
	WANTS_PLAY,
	WANTS_PAUSE,
	WANTS_EDITOR,
};

#define MAX_TIME_SCALE 2.0f

class ModuleTimeManager : public Module
{
public:

	ModuleTimeManager(bool start_enabled = true);
	~ModuleTimeManager();

	bool Init() override;
	void PrepareUpdate();

	void SetTimeScale(float timeScale);
	float GetTimeScale() const;

	float GetDt() const;

	int GetFrameCount() const;
	float GetRealDt() const;

	Timer GetGameTimer();

	float GetTimeSincePlay();

	float GetRealTimeSinceStartup();

	void PauseGameClock();

	void StopGameClock();

	void StartGameClock();

	void ChechState();

	bool play = false, pause = false, saved = false, load = false;
	
	STATE state;

private:

	// Game Clock
	float timeScale = 1.0f; // Scale at which time is passing
	Timer game_time;
	float game_dt = 0.0f;

	// Real Time Clock
	int frameCount = 0; // App graphics frames since game start
	Timer real_time;
	float realDt = 0.0f; // Last frame time expressed in seconds
};

#endif

