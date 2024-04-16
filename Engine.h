#pragma once
#include<stdlib.h>
#include"EntityManager.h"
#include"threadpool.h"
#include"Component.h"
#include<vector>
#include<time.h>

using namespace std;
class CObj;

EntityManager* GetManager();
void StartEngine(std::vector<std::string>& v);

class Engine {

private:
	Engine() = default;
	Engine(const Engine& e) = delete;
	Engine& operator=(const Engine& e) = delete;

public:
	static Engine& GetEngine() {
		static Engine m_engine;
		return m_engine;
	}

	//�����ĸ������� API ���س�ʼ�������� ��ͣ �����˳�
	//������س�ʼ�� ��ִ��ҵ��ĳ�ʼ�� ��δ��������
	//��ʱ����������ǰ�ù����ͳ�ʼ���󣬽�������������������������
	//�ٵ���RunEngine ��������
	//v������������ ������ɶ���Ĵ�������
	void StartEngine(std::vector<std::string>& v);

	//��ʼ���м���
	void RunEngine();

	//��ͣ
	void PauseEngine();

	//���� �˳�
	void EndEngine();


public:
	//mtx cv ListIndex����ʵ��ҵ���߳������̵߳��������ͬ��
	//sun_count ���������� ��ʵ������ 
	mutex mtx;
//	condition_variable cv;
//	int ListIndex;
	atomic_uint sun_count;
private:
	//һ������ �������̵߳���ʱ������Ŀ�ʼ��ͣ���˳�
	condition_variable StartCV;
	mutex StartMtx;
	//��ʼ��ͣ��־
	bool StartFlag = false;
	//�˳������־
	bool EndFlag = false;
};