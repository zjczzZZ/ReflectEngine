#pragma once
#include"Obj.h"
class DataEntity : public CObj
{
	DECLARE_CLASS
public:
	DataEntity();
	~DataEntity();
	static void classInit();
	virtual void tick() { m_arg->m_id = id; }
	virtual void simulation() {}
	virtual void init() {}
	
	virtual Attribute* GetState() override;
	virtual void regDataEntity(CObj* v) override;

	int test;
private:
	// µÃÂ Ù–‘
	Attribute* m_arg;
};

