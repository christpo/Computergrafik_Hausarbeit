#include "scene.h"
#include <fstream>

using namespace std;

Scene::Scene()
{
	this->m_root.setScaling(Vector(1, 1, 1));
	this->m_root.setParent(NULL);
}


Scene::~Scene()
{
	for (auto& map : this->m_Models) {
		delete(map.second);
	}
}


bool Scene::addSceneFile(const char* Scenefile) 
{
	ifstream objFile;
	objFile.open(Scenefile, std::ifstream::in);

	if (!objFile.good()) {
		objFile.close();
		return false;
	}

	string line;
	SceneNode* parent;

	while (!objFile.eof()) {

		getline(objFile, line);

		stringstream ss(line);
		string cmd;
		ss >> cmd;

		char Modelfile[256];
		char ModelID[256];

		Vector Pos, Scale, RotAxis;
		float Angle;
		char NodeID[256];
		char ParentID[256];


		if (cmd == "MODEL") {

			sscanf(line.c_str(), "MODEL ID=%s FILE=%s", ModelID, Modelfile);
			Model* m = new Model();

			if (!m->load(Modelfile, false, 100.0f, "", "")) {
				cout << "model " << Modelfile << " konnte nicht geladen werden" << endl;
			}
			this->m_Models.insert(make_pair(ModelID, m));
		}
		else if (cmd == "NODE") {

			sscanf(line.c_str(), "NODE ID=%s PARENTID=%s MODELID=%s TRANSLATION=%f %f %f ROTATIONAXIS=%f %f %f ROTATIONANGLE = %f SCALE = %f %f %f",
								NodeID, ParentID, ModelID,
								&Pos.X, &Pos.Y, &Pos.Z,
								&RotAxis.X, &RotAxis.Y, &RotAxis.Z,
								&Angle,
								&Scale.X, &Scale.Y, &Scale.Z);
			
			Model* model = this->m_Models[ModelID];
			string parentid(ParentID);
			
			if (parentid == "NULL") {

				SceneNode* node = new SceneNode(NodeID, Pos, RotAxis, Angle, Scale, &this->m_root, model);
				this->m_root.addChild(node);
				parent = node;
			}
			else {

				SceneNode* node = new SceneNode(NodeID, Pos, RotAxis, Angle, Scale, parent, model);
				parent->addChild(node);
				parent = node;
			}
		}
	}
	objFile.close();

	return true;
}


void Scene::draw() 
{
	//for (auto child : this->m_root.getChildren()) {
		this->draw(&this->m_root);
	//}
}


void Scene::draw(SceneNode* pNode)
{
	if (pNode->getModel() != NULL) {

		glPushMatrix();
		glMultMatrixf(pNode->getGlobalTransform(true));
		pNode->getModel()->drawTriangles();
		glPopMatrix();
	}

	for (auto child : pNode->getChildren()) {
		draw(child);
	}
}