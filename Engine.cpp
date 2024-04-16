#include"Engine.h"

//最好在main函数中，单独开一个线程跑StartEngine 在main函数中再去控制开始暂停 退出
void StartEngine(std::vector<std::string>& v)
{
	Engine::GetEngine().StartEngine(v);
}

void Engine::StartEngine(std::vector<std::string>& v) {
	
	//反射形式创建对象 加到创建队列中
	for (int i = 0; i < v.size(); ++i) {
		GetManager()->CreateObject(v[i]);
	}
	
	//将创建队列的指针全部添加到本地对象链表
	EntityManager::GetEntityManager()->Refresh();
	 
	//计时
	clock_t starttime, endtime;
	starttime = clock();
	//创建线程池 最大最小线程数都是10
	ThreadPool pool(16, 20);

	while (1) {

		//判断 开始运行标志位 若false 则阻塞等待
		unique_lock<mutex> startlk(StartMtx);
		while (StartFlag != true) {
			StartCV.wait(startlk);
		}

		//刷新。。 将删除队列中的对象 从全局对象链表中删除
		EntityManager::GetEntityManager()->Refresh();
		//GetManager()->Refresh();
		//仅记录 主线程唤醒了多少次，业务与管理全部执行完为一轮，跑了几轮
		++sun_count;
		endtime = clock();
		std::cout << CObj::ObjSum <<" ------唤醒 ：" + to_string(sun_count) + "次  ----time：" + to_string(endtime/1000.0) << std::endl;
		
		//链表索引值 添加任务
		CObj::ObjSum =  EntityManager::GetEntityManager()->GetALLObjNum();
		
		for (auto it = EntityManager::GetEntityManager()->GetAllObj().begin(); it != EntityManager::GetEntityManager()->GetAllObj().end(); ++it) {
			pool.CommitTask(Task(CObj::run, (*it)));
		}

		while (CObj::ObjSum != 0) {
			this_thread::yield();
			//cout << "========== YIELD ==========="<<endl;
		}

		//退出标志位
		if (EndFlag == true) {
			break;
		}
	}
	//正确等待线程池将引擎退出前的业务并行跑完
	while (CObj::ObjSum != 0);
	//退出while（1） 表明退出引擎了，把对象链表给处理好
	for (auto &c : EntityManager::GetEntityManager()->GetAllObj()) {
		delete c;
		c = nullptr;
	}
	EntityManager::GetEntityManager()->GetAllObj().clear();
	std::cout << to_string(sun_count)<<"-------------end-------------" << std::endl;
}

void Engine::RunEngine() {
	//标志位置true
	StartFlag = true;
	//唤醒阻塞在startcv的线程，让管理与业务轮回跑起来
	StartCV.notify_one();
}

void Engine::PauseEngine() {
	//开始标志位置false 管理线程会阻塞在StartCV上
	//需要再次调度RunEngine 才跑起来
	StartFlag = false;
}

void Engine::EndEngine() {
	//退出标志位置true
	EndFlag = true;
}


EntityManager* GetManager() {
	return EntityManager::GetEntityManager();
}

