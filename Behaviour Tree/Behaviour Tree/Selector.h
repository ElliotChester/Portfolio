#pragma once
#include "Composite.h"

class Selector : 
	public Composite
{
	virtual bool run() override;
};

