#include "Application.h"
#include "ModuleTimeManager.h"

ModuleTimeManager::ModuleTimeManager(bool start_enabled) : Module(start_enabled)
{
	name = "TimeManager";
}

ModuleTimeManager::~ModuleTimeManager()
{
}

bool ModuleTimeManager::Init()
{
	game_time.Stop();
	return true;
}
void ModuleTimeManager::PrepareUpdate()
{
	frameCount++;

	// Dt
	realDt = App->GetDt();
	switch (state)
	{
	case WANTS_PLAY:
		LOG("PLAY");
		StartGameClock();
		saved = true;
		game_dt = realDt * timeScale;
		state = PLAY;
		break;
	case WANTS_PAUSE:
		LOG("PAUSE");
		PauseGameClock();
		state = PAUSE;
		break;
	case WANTS_EDITOR:
		LOG("EDITOR");
		StopGameClock();
		game_dt = 0.0f;
		state = EDITOR;
		break;
	default:
		break;
	}
}

void ModuleTimeManager::SetTimeScale(float timeScale)
{
	this->timeScale = timeScale;

	if (this->timeScale > MAX_TIME_SCALE)
		this->timeScale = MAX_TIME_SCALE;
}

float ModuleTimeManager::GetTimeScale() const
{
	return timeScale;
}

float ModuleTimeManager::GetDt() const
{
	return game_dt;
}

int ModuleTimeManager::GetFrameCount() const
{
	return frameCount;
}

float ModuleTimeManager::GetRealDt() const
{
	return realDt;
}

float ModuleTimeManager::GetTimeSincePlay()
{
	return game_time.Read() / 1000.0f;
}

float ModuleTimeManager::GetRealTimeSinceStartup()
{
	return real_time.Read() / 1000.0f;
}

void ModuleTimeManager::PauseGameClock()
{
	game_time.Pause();
}

void ModuleTimeManager::StopGameClock()
{
	game_time.Stop();
}

void ModuleTimeManager::StartGameClock()
{
	game_time.Start();
}
