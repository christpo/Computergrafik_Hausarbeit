//
//  Material.cpp
//  RealtimeRending
//
//  Created by Philipp Lensing on 27.10.14.
//  Copyright (c) 2014 Philipp Lensing. All rights reserved.
//

#include "Material.h"

Material::Material( Vector kd, Vector ks, Vector ka, float ns, std::string name )
{
	this->m_DiffuseColor.R = kd.X;
	this->m_DiffuseColor.G = kd.Y;
	this->m_DiffuseColor.B = kd.Z;

	this->m_SpecularColor.R = ks.X;
	this->m_SpecularColor.G = ks.Y;
	this->m_SpecularColor.B = ks.Z;

	this->m_SpecularExp = ns;

	this->m_AmbientColor.R = ka.X;
	this->m_AmbientColor.G = ka.Y;
	this->m_AmbientColor.B = ka.Z;

	this->m_Name = name;
}

Material::Material()
{
}

Material::~Material()
{
    
}

const Color& Material::getDiffuseColor() const
{
    return m_DiffuseColor;
}

const Color& Material::getSpecularColor() const
{
    return m_SpecularColor;
}

const Color& Material::getAmbientColor() const
{
    return m_AmbientColor;
}

float Material::getSpecularExponent() const
{
    return m_SpecularExp;
}

const std::string& Material::getName() const
{
    return m_Name;
}

void Material::setDiffuseColor( const Color& color)
{
    m_DiffuseColor = color;
}

void Material::setSpecularColor( const Color& color)
{
    m_SpecularColor = color;
}

void Material::setAmbientColor( const Color& color)
{
    m_AmbientColor = color;
}

void Material::setSpecularExponent( const float exp)
{
    m_SpecularExp = exp;
}

void Material::setName( const std::string& name)
{
    m_Name = name;
}

void Material::setDiffuseTexture( const char* Filename)
{
    m_DiffuseTexture.LoadFromBMP(Filename);
}

void Material::appyTexture() const {

	if (this->m_DiffuseTexture.isValid()) {

		this->m_DiffuseTexture.apply();
	}
}

bool Material::isValidTexture()const {
	return this->m_DiffuseTexture.isValid();
}



