#pragma once
#include<list>
#include<iostream>
#include"Obj.h"
#include<unordered_map>
#include"Entity.h"

using namespace std;

class CObj;
class Entity;
class ClassInfo;

//实体管理类 负责维护和查找公共对象链表
//业务类中成员函数 调用查询， 
//引擎任务调度线程中，负责删除要删除的对象指针
class EntityManager {
private:
	static EntityManager* m_EntityManager;
	EntityManager() :cFlag(false){}
	EntityManager(const EntityManager& e){}
	EntityManager& operator=(const EntityManager& e){}
	void searchALL();

	//创建单个obj对象，由反射返回
	CObj* CreateObj(string ss);
	void Quit(CObj* t);

public:
	static EntityManager* GetEntityManager() {
		return m_EntityManager;
	}

	std::vector<CObj*> LookUpFrom(std::string ss);

	void Refresh();

	//创建聚合对象，并返回该聚合对象
	Entity* CreateObject(string ss);

	void QuitObject(CObj* cj);

	std::vector<CObj*>& GetAllObj();

	int GetALLObjNum();

	std::unordered_map<std::string, std::vector<CObj*>>& GetObjMap();

	void SetConvergeInfo(std::unordered_map<std::string, std::vector<std::string>> enttinfo);

	vector<Entity*>& GetObjectVector();
private:
	//向创建队列push时，是多线程场景，加锁
	std::mutex createListMtx;
	std::mutex quitListMtx;

	int AllObjNum;
	bool cFlag;

	//创建聚合类型 聚合类名+组件类名
	std::unordered_map<std::string, std::vector<std::string>> m_EnttInfo;

	std::unordered_map<std::string, std::vector<CObj*>> ObjMap;
	std::vector<CObj*> CreateVector;
	std::vector<CObj*> QuitVector;
	std::vector<CObj*> AllVector;
	std::vector<Entity*> QuitObjectVector;
	std::vector<Entity*> CreateObjectVector;
	std::vector<Entity*> ObjectVector;
};