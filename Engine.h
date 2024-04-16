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

	//以下四个函数是 API 加载初始化，运行 暂停 结束退出
	//引擎加载初始化 仅执行业务的初始化 ，未继续运行
	//此时引擎在做完前置工作和初始化后，将阻塞在跑起来的条件变量上
	//再调度RunEngine 才跑起来
	//v是类名的数组 用于完成对象的创建控制
	void StartEngine(std::vector<std::string>& v);

	//开始运行计算
	void RunEngine();

	//暂停
	void PauseEngine();

	//结束 退出
	void EndEngine();


public:
	//mtx cv ListIndex用于实现业务线程与主线程的任务调度同步
	//sun_count 仅用作计算 无实际意义 
	mutex mtx;
//	condition_variable cv;
//	int ListIndex;
	atomic_uint sun_count;
private:
	//一下两个 用作主线程调度时，引擎的开始暂停与退出
	condition_variable StartCV;
	mutex StartMtx;
	//开始暂停标志
	bool StartFlag = false;
	//退出引擎标志
	bool EndFlag = false;
};