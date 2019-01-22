#pragma once
class Blackboard
{
public:
	Blackboard();
	~Blackboard();

	//the blackboard holds variables used in the tree

	bool riverCrossed = false;
	bool hasPlank = false;
	bool plankOverRiver = false;

	float distanceFromPlank = 10;

	static Blackboard* _instance;

	static Blackboard* Instance() 
	{
		if (!_instance) {
			_instance = new Blackboard;
		}
		return _instance;
	}
};

