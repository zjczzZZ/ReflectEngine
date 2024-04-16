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

//ʵ������� ����ά���Ͳ��ҹ�����������
//ҵ�����г�Ա���� ���ò�ѯ�� 
//������������߳��У�����ɾ��Ҫɾ���Ķ���ָ��
class EntityManager {
private:
	static EntityManager* m_EntityManager;
	EntityManager() :cFlag(false){}
	EntityManager(const EntityManager& e){}
	EntityManager& operator=(const EntityManager& e){}
	void searchALL();

	//��������obj�����ɷ��䷵��
	CObj* CreateObj(string ss);
	void Quit(CObj* t);

public:
	static EntityManager* GetEntityManager() {
		return m_EntityManager;
	}

	std::vector<CObj*> LookUpFrom(std::string ss);

	void Refresh();

	//�����ۺ϶��󣬲����ظþۺ϶���
	Entity* CreateObject(string ss);

	void QuitObject(CObj* cj);

	std::vector<CObj*>& GetAllObj();

	int GetALLObjNum();

	std::unordered_map<std::string, std::vector<CObj*>>& GetObjMap();

	void SetConvergeInfo(std::unordered_map<std::string, std::vector<std::string>> enttinfo);

	vector<Entity*>& GetObjectVector();
private:
	//�򴴽�����pushʱ���Ƕ��̳߳���������
	std::mutex createListMtx;
	std::mutex quitListMtx;

	int AllObjNum;
	bool cFlag;

	//�����ۺ����� �ۺ�����+�������
	std::unordered_map<std::string, std::vector<std::string>> m_EnttInfo;

	std::unordered_map<std::string, std::vector<CObj*>> ObjMap;
	std::vector<CObj*> CreateVector;
	std::vector<CObj*> QuitVector;
	std::vector<CObj*> AllVector;
	std::vector<Entity*> QuitObjectVector;
	std::vector<Entity*> CreateObjectVector;
	std::vector<Entity*> ObjectVector;
};