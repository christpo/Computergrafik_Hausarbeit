#pragma once
#include <string>
#include "vector.h"
#include "Model.h"
#include "Matrix.h"
#include <set>

class SceneNode
{
public:
	SceneNode();
	~SceneNode();
	SceneNode(const std::string& Name, const Vector& Translation, const Vector& RotationAxis, float RotationAngle, const Vector& Scale, SceneNode* pParent, Model* pModel);
	
	const std::string& getName() const;
	const Matrix& getLocalTransform() const;
	Matrix getGlobalTransform(bool scaling) const;
	const Vector& getScaling() const;
	const SceneNode* getParent() const;
	const std::set<SceneNode*>& getChildren() const;
	const Model* getModel() const;

	void setName(const std::string& Name);
	void setModel(Model* pModel);
	void setLocalTransform(const Vector& Translation, const Vector& RotationAxis, const float RotationAngle);
	void setLocalTransform(const Matrix& LocalTransform);
	void setScaling(const Vector& Scaling);
	void setParent(SceneNode* pNode);
	void addChild(SceneNode* pChild);
	void removeChild(SceneNode* pChild);

protected:
	std::string m_name;
	Model* m_pModel;
	SceneNode* m_pParent;
	std::set<SceneNode*> m_Children;
	Matrix m_LocalTransform;
	Vector m_Scaling;
};

