#include "scenenode.h"
#include <stdexcept>



SceneNode::SceneNode()
{
}


SceneNode::~SceneNode()
{
	for (set<SceneNode*>::iterator i = this->m_Children.begin(); i != this->m_Children.end(); i++) {
		delete(*i);
	}
}

SceneNode::SceneNode(const std::string& Name, const Vector& Translation, const Vector& RotationAxis,
		float RotationAngle, const Vector& Scale, SceneNode* pParent, Model* pModel)
{
	this->m_name = Name;
	this->m_Scaling = Scale;
	this->m_pParent = pParent;
	this->m_pModel = pModel;
	
	this->setLocalTransform(Translation,RotationAxis, RotationAngle);
}


const std::string& SceneNode::getName() const
{
	return this->m_name;
}


const Matrix& SceneNode::getLocalTransform() const
{
	return this->m_LocalTransform;
}


Matrix SceneNode::getGlobalTransform(bool scaleing) const
{
	Matrix g = this->m_LocalTransform;
	
	if (this->m_pParent->getParent() != NULL) {

		g = this->m_pParent->getGlobalTransform(false) * g;
	}	

	if (scaleing) {
		Matrix scale;
		scale.scale(this->getScaling());
		g = g * scale;
	}
	
	return g;
}


const Vector& SceneNode::getScaling() const
{
	return this->m_Scaling;
}


const SceneNode* SceneNode::getParent() const
{
	return this->m_pParent;
}


const std::set<SceneNode*>& SceneNode::getChildren() const
{
	return this->m_Children;
}


const Model* SceneNode::getModel() const
{
	return this->m_pModel;
}


void SceneNode::setName(const std::string& Name)
{
	this->m_name = Name;
}


void SceneNode::setModel(Model* pModel)
{
	this->m_pModel = pModel;
}


void SceneNode::setLocalTransform(const Vector& Translation, const Vector& RotationAxis, const float RotationAngle)
{
	Matrix rot, trans;
	trans.translation(Translation);
	rot.rotationAxis(RotationAxis, RotationAngle);
	this->m_LocalTransform = rot * trans;
}


void SceneNode::setLocalTransform(const Matrix& LocalTransform)
{
	this->m_LocalTransform = LocalTransform;
}


void SceneNode::setScaling(const Vector& Scaling)
{
	this->m_Scaling = Scaling;
}


void SceneNode::setParent(SceneNode* pNode)
{
	this->m_pParent = pNode;
}


void SceneNode::addChild(SceneNode* pChild)
{
	this->m_Children.insert(pChild);
}


void SceneNode::removeChild(SceneNode* pChild)
{
	this->m_Children.erase(pChild);
}