#pragma once
#include "Obj.h"
#include <vector>
using namespace std;

class CObj;

//聚合类 Entity是具体实体，包含各种组装实体
class Entity final {
public:
	Entity(){}
	~Entity(){}
	
	//设置聚合类信息  别用
	void setEnttInfo(std::vector<std::string> info);
	//添加聚合类对象的模型obj*
	void addEntity(CObj* cobj);
	//移除聚合类对象中的某个模型obj*
	void removeEntity(CObj* cobj);
	//从该聚合类对象中查找某个模型类的信息
	CObj* LookFromObject(std::string type);

	vector<CObj*>& getCObjVector();

private:
	std::vector<std::string> m_enttInfo;
	std::vector<CObj*> m_entt;
};