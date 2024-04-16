#include"Entity.h"

void Entity::setEnttInfo(std::vector<std::string> info)
{
	m_enttInfo = info;
}

void Entity::addEntity(CObj* cobj)
{
	m_enttInfo.push_back(cobj->GetClassName());
	m_entt.push_back(cobj);
}

void Entity::removeEntity(CObj* cobj)
{
	for (int i = 0; i < m_entt.size(); ++i) {
		if (cobj == m_entt[i]) {
			m_entt.erase(m_entt.begin() + i);
			m_enttInfo.erase(m_enttInfo.begin() + i);
			break;
		}
	}
}

CObj* Entity::LookFromObject(string type)
{
	for (int i = 0; i < m_enttInfo.size(); ++i) {
		if (m_enttInfo[i] == type) {
			return m_entt[i];
		}
	}
	return nullptr;
}

vector<CObj*>& Entity::getCObjVector()
{
	return m_entt;
}
