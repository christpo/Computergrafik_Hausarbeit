#pragma once
#include <string>
#include <map>
#include "scenenode.h"
#include "Model.h"
#include <set>

class Scene
{
public:
	Scene();
	~Scene();
	bool addSceneFile(const char* Scenefile);
	void draw();

protected:
	void draw(SceneNode* pNode);

	SceneNode m_root;
	std::map<std::string, Model*> m_Models;
};

