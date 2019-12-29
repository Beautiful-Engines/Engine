#ifndef _WindowProject_H_
#define _WindowProject_H_

#include "WindowEngine.h"

struct ProjectNode
{
	std::string name;
	ProjectNode* parent;
	std::vector<ProjectNode*> childs;
};

class WindowProject : public WindowEngine
{
public:
	WindowProject();
	~WindowProject();

	bool Draw();
	std::string FitTextToImage(std::string text);

private:
	uint columns, image_size, spacing, offset;
	std::string folder = ASSETS_FOLDER;
};


#endif // !_PANEL_PROJECT_H_