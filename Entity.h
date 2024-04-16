#pragma once
#include "Obj.h"
#include <vector>
using namespace std;

class CObj;

//�ۺ��� Entity�Ǿ���ʵ�壬����������װʵ��
class Entity final {
public:
	Entity(){}
	~Entity(){}
	
	//���þۺ�����Ϣ  ����
	void setEnttInfo(std::vector<std::string> info);
	//��Ӿۺ�������ģ��obj*
	void addEntity(CObj* cobj);
	//�Ƴ��ۺ�������е�ĳ��ģ��obj*
	void removeEntity(CObj* cobj);
	//�Ӹþۺ�������в���ĳ��ģ�������Ϣ
	CObj* LookFromObject(std::string type);

	vector<CObj*>& getCObjVector();

private:
	std::vector<std::string> m_enttInfo;
	std::vector<CObj*> m_entt;
};