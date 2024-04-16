#pragma once
#include "..\engine\HLAengine.h"
#include<iostream>


class test :public Component {
	
	DECLARE_CLASS

public:
	test(){}
	~test(){}
	static void classInit();
	virtual void init() {};
	virtual void tick() override;
	virtual void simulation() override;
public:
	int index;
	vector<int> v;
	int count;
};

