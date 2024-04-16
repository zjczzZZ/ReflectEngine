#include "DataEntity.h"

IMPLEMENT_CLASS(DataEntity, classInit)

DataEntity::DataEntity() {
	m_arg = new Attribute();
	m_arg->m_id = j;
}

DataEntity::~DataEntity() {
	delete m_arg;
	m_arg = nullptr;
}

void DataEntity::classInit()
{
	REGISTER_CLASS_MEMBER(DataEntity, test, int)
}

Attribute* DataEntity::GetState()
{
	return m_arg;
}

void DataEntity::regDataEntity(CObj* v)
{
	CObj::regDataEntity(this);
}

