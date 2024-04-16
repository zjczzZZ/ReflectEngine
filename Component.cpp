#include"Component.h"

IMPLEMENT_CLASS(Component, classInit)

void Component::classInit()
{
}

Attribute* Component::GetState() {
	return getDataEntity()->GetState();
}

void Component::Quit()
{
	EntityManager::GetEntityManager()->QuitObject(this);
}

vector<CObj*> Component::GetStatePool()
{
	return EntityManager::GetEntityManager()->LookUpFrom("DataEntity");
}

int Component::GetID() {
	return this->id;
}

