#ifndef __EngineWindow_H__
#define __EngineWindow_H__

class EngineWindow
{
public:

	EngineWindow();
	virtual ~EngineWindow();

	virtual bool Draw() = 0;

	virtual inline void Show_NotShow();

protected:

	bool enabled = false;
};

#endif // !__EngineWindow_H__