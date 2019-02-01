#pragma once
#include "Object.h"

class ObjectFactory
{
public:
	ObjectFactory();

	Object CreateTriangle() const;
	Object CreateCube() const;
	Object CreateTeapot() const;
	Object CreateSuzanne() const;
	Object CreateAfrican() const;
	Object CreateEyesInner() const;
	Object CreateEyesOuter() const;
	Object CreateDiablo() const;
	Object CreateBMax() const;
	Object CreateChest() const;
};


