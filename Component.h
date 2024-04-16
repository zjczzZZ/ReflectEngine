#pragma once
#include"Obj.h"

class Component :public CObj {

	DECLARE_CLASS

public:
	Component() {}
	virtual ~Component() {}

public:
	static void classInit();

	virtual void tick() {};

	virtual void simulation() {};

	virtual void init() {};

	virtual Attribute* GetState() override;

	virtual void Quit() override;

	vector<CObj*> GetStatePool();

	int GetID();

private:

	int count = 0;
};
