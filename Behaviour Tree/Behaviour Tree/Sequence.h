#pragma once
#include "Composite.h"

class Sequence : 
	public Composite
{
public:
	virtual bool run() override;
};

