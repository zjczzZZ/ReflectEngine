#include"EntityManager.h"

EntityManager* EntityManager::m_EntityManager = new EntityManager();


std::vector<CObj*> EntityManager::LookUpFrom(std::string ss)
{
	if (ObjMap.find(ss) == ObjMap.end()) {
		std::vector<CObj*> a{};
		return a;
	}
	return ObjMap[ss];
}

void EntityManager::Quit(CObj* t)
{
	lock_guard<mutex> lgmtx(quitListMtx);
	QuitVector.push_back(t);
}

void EntityManager::Refresh()
{
	if (!cFlag) {
		return;
	}
	if (CreateVector.size() != 0) {
		for (auto iter = CreateVector.begin(); iter != CreateVector.end();++iter) {		
			ObjMap[(*iter)->GetClassName()].push_back(*iter);
		}
		CreateVector.clear();
	}

	if (CreateObjectVector.size() != 0) {
		for (auto it = CreateObjectVector.begin(); it != CreateObjectVector.end(); ++it) {
			ObjectVector.push_back((*it));
		}
		CreateObjectVector.clear();
	}

	if (QuitVector.size() != 0) {
		for (auto it = QuitVector.begin(); it != QuitVector.end(); ++it) {

			if (ObjMap.find((*it)->GetClassName()) == ObjMap.end()) {
				continue;
			}
			string name = (*it)->GetClassName();
			for (auto lit = ObjMap[name].begin(); lit != ObjMap[name].end() ; ) {
				if ((*lit) == (*it)) {
					auto item = (*lit);
					lit = ObjMap[name].erase(lit);
					delete item;
					(*it) = nullptr;
					if (ObjMap[name].size() == 0) {
						ObjMap.erase(ObjMap.find(name));
					}
				}
				else {
					lit++;
				}
			}
		}
		QuitVector.clear();
	}

	if (QuitObjectVector.size() != 0) {
		for (auto it = CreateObjectVector.begin(); it != CreateObjectVector.end(); ++it) {
			ObjectVector.erase(find(ObjectVector.begin(), ObjectVector.end(), (*it)));
		}
		CreateObjectVector.clear();
	}

	cout << " $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ " << endl;
	AllObjNum = GetALLObjNum();
	AllVector.clear();
	AllVector.reserve(AllObjNum);
	searchALL();
	cFlag = false;
}

CObj* EntityManager::CreateObj(std::string ss)
{
	std::unordered_map<std::string, ClassInfo*>::const_iterator iter =
		reflection::GetInstance().GetClassInfoMap().find(ss);
	if (reflection::GetInstance().GetClassInfoMap().end() != iter) {
		CObj* tmp = iter->second->CreateObject();
		//tmp->init();
		{
			lock_guard<mutex> lgmtx(createListMtx);
			CreateVector.push_back(tmp);
		}
		return tmp;
	}
	return nullptr;
}

void EntityManager::searchALL()
{
	int count = 0;
	for (auto it = ObjMap.begin(); it != ObjMap.end(); ++it) {
		copy((*it).second.begin(), (*it).second.end(), back_inserter(AllVector));
		count += (*it).second.size();
	}
}

Entity* EntityManager::CreateObject(string ss)
{
	if (m_EnttInfo.find(ss) == m_EnttInfo.end()) {
		return nullptr;
	}
	cFlag = true;
	Entity* ent = new Entity();			//ent聚合类对象
	CObj* dataobj = CreateObj("DataEntity");//数据obj
	dataobj->regEntity(ent);				//注册
	dataobj->regDataEntity(dataobj);
	ent->addEntity(dataobj);

	for (int i = 0; i < m_EnttInfo[ss].size(); ++i) {
		if (m_EnttInfo[ss][i] != "DataEntity") {
			CObj* modelobj = CreateObj(m_EnttInfo[ss][i]);
			modelobj->regDataEntity(dataobj);
			modelobj->regEntity(ent);
			ent->addEntity(modelobj);
		}
	}
	//统一初始化
	for (auto it = ent->getCObjVector().begin(); it != ent->getCObjVector().end(); ++it) {
		(*it)->init();
	}

	CreateObjectVector.push_back(ent);
	return ent;
}

void EntityManager::QuitObject(CObj* cj)
{
	cFlag = true;
	Entity* temp = cj->getEntity();
	auto v = temp->getCObjVector();
	for (auto it = v.begin(); it != v.end(); ++it) {
		(*it)->Quit();
		temp->removeEntity((*it));
	}
	QuitObjectVector.push_back(temp);
}

std::vector<CObj*>&  EntityManager::GetAllObj()
{
	return AllVector;
}

int EntityManager::GetALLObjNum()
{
	int count = 0;
	auto it = ObjMap.begin();
	for (; it != ObjMap.end(); ++it) {
		count += (*it).second.size();
	}
	return count;
}

std::unordered_map<std::string, std::vector<CObj*>>& EntityManager::GetObjMap()
{
	return ObjMap;
}

void EntityManager::SetConvergeInfo(std::unordered_map<std::string, std::vector<std::string>> enttinfo)
{
	m_EnttInfo = enttinfo;
}

vector<Entity*>& EntityManager::GetObjectVector()
{
	return ObjectVector;
}
