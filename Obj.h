#pragma once
#include<stdlib.h>
#include<iostream>
#include<list>
#include<atomic>
#include<time.h>
#include<mutex>
#include<condition_variable>
#include<queue>
#include"EntityManager.h"
#include<string>
#include<map>
#include"attribute.h"
#include"Reflection.h"
#include"Entity.h"

using namespace std;

class Entity;
class EntityManager;

//CObj ���� ��������̳߳ص��������
//ҵ����ֻ��̳в���д �麯������
class CObj {

    DECLARE_CLASS

public:
	CObj() {
		lock_guard<mutex> lg(Entitymtx);
		j++;
		id = j;
	}
	virtual ~CObj() {}

	//run ��������
	static void run(void* arg);
	static void classInit();
	virtual void init() {}
	virtual void tick(){}
	virtual void simulation() {}
	virtual void Quit();

	virtual Attribute* GetState() ;

	//��Ա��������API
	int getMemberCount();
	member* getMember(int pos);
	member* getMember(const string& memberName);

	template<typename T>
	void getRefValue(const string& memberName, T& value) {
		member* mem = reflection::GetInstance().Get_Member(GetClassName(), memberName);
		size_t offset = mem->offset();
		value = *(T*)((unsigned char*)(this) + offset);
	}

	template<typename T>
	void setRefValue(const string& memberName, const T& value) {
		member* mem = reflection::GetInstance().Get_Member(GetClassName(), memberName);
		size_t offset = mem->offset();
		*(T*)((unsigned char*)(this) + offset) = value;
	}

	//static void ClassInit() {};
	EntityManager* GetManager();

	//ע�� entity�ۺ���ָ��
	void regEntity(Entity* v);

	//ע�� DataEntity,����״̬��
	virtual void regDataEntity(CObj* c);

	CObj* getDataEntity() { return m_DataEntity; }

	Entity* getEntity() { return m_Entity; }


public:
	int id = 0;
	static int j;
	static atomic_int ObjSum;

private:
	mutex Entitymtx;
	Entity* m_Entity;
	CObj* m_DataEntity;
};
