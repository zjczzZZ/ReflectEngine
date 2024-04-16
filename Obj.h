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

//CObj 基类 与引擎的线程池的任务耦合
//业务类只需继承并重写 虚函数即可
class CObj {

    DECLARE_CLASS

public:
	CObj() {
		lock_guard<mutex> lg(Entitymtx);
		j++;
		id = j;
	}
	virtual ~CObj() {}

	//run 包裹函数
	static void run(void* arg);
	static void classInit();
	virtual void init() {}
	virtual void tick(){}
	virtual void simulation() {}
	virtual void Quit();

	virtual Attribute* GetState() ;

	//成员变量反射API
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

	//注册 entity聚合类指针
	void regEntity(Entity* v);

	//注册 DataEntity,用于状态池
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
