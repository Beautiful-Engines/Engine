#ifndef __ImportAnimation_H_
#define __ImportAnimation_H_

struct aiAnimation;
struct aiBone;
class ResourceAnimation;
class ResourceBone;
class ResourceAnimationGraph;

class ImportAnimation
{
public:
	ImportAnimation();
	~ImportAnimation();

	bool Init();
	bool CleanUp();

	uint Import(aiAnimation* _animation);
	bool CreateOurAnimation(ResourceAnimation* _animation);
	void LoadAnimationFromResource(ResourceAnimation* _animation);

	uint ImportBone(aiBone* _bone);
	bool CreateOurBone(ResourceBone* _bone);
	void LoadBoneFromResource(ResourceBone* _bone);

	bool CreateOurGraph(ResourceAnimationGraph* _graph);
	void LoadGraphFromResource(ResourceAnimationGraph* _graph);

};

#endif // !__ImportAnimation_H_
