#ifndef __ImportAnimation_H_
#define __ImportAnimation_H_

struct aiAnimation;
class ResourceAnimation;

class ImportAnimation
{
public:
	ImportAnimation();
	~ImportAnimation();

	bool Init();
	bool CleanUp();

	void Import(aiAnimation* animation);
	bool CreateOurAnimation(ResourceAnimation* animation);
	void LoadAnimationFromResource(ResourceAnimation* animation);

};

#endif // !__ImportAnimation_H_
