#include"Obj.h"

IMPLEMENT_CLASS(CObj, classInit)

int CObj::j = 0;
atomic_int CObj::ObjSum = 1;

void CObj::run(void* arg) {
	CObj* c = static_cast<CObj*>(arg);
	c->simulation();
	c->tick();
	--ObjSum;
	//cout<< ObjSum <<endl;
}

EntityManager* CObj::GetManager() {
	return EntityManager::GetEntityManager();
}

void CObj::Quit() {
	GetManager()->QuitObject(this);
}

void CObj::classInit() {
	REGISTER_CLASS_MEMBER(CObj,id,int)
}

Attribute* CObj::GetState() {
	return nullptr;
}

int CObj::getMemberCount()
{
	return reflection::GetInstance().Get_Member_Count(GetClassName());
}

member* CObj::getMember(int pos)
{
	return reflection::GetInstance().Get_Member(GetClassName(),pos);
}

member* CObj::getMember(const string& memberName)
{
	return reflection::GetInstance().Get_Member(GetClassName(), memberName);
}

void CObj::regEntity(Entity* v) {
	m_Entity = v;
}

void CObj::regDataEntity(CObj* v) {
	m_DataEntity = v;
}