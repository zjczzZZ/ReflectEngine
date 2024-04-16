#include"Engine.h"

//�����main�����У�������һ���߳���StartEngine ��main��������ȥ���ƿ�ʼ��ͣ �˳�
void StartEngine(std::vector<std::string>& v)
{
	Engine::GetEngine().StartEngine(v);
}

void Engine::StartEngine(std::vector<std::string>& v) {
	
	//������ʽ�������� �ӵ�����������
	for (int i = 0; i < v.size(); ++i) {
		GetManager()->CreateObject(v[i]);
	}
	
	//���������е�ָ��ȫ����ӵ����ض�������
	EntityManager::GetEntityManager()->Refresh();
	 
	//��ʱ
	clock_t starttime, endtime;
	starttime = clock();
	//�����̳߳� �����С�߳�������10
	ThreadPool pool(16, 20);

	while (1) {

		//�ж� ��ʼ���б�־λ ��false �������ȴ�
		unique_lock<mutex> startlk(StartMtx);
		while (StartFlag != true) {
			StartCV.wait(startlk);
		}

		//ˢ�¡��� ��ɾ�������еĶ��� ��ȫ�ֶ���������ɾ��
		EntityManager::GetEntityManager()->Refresh();
		//GetManager()->Refresh();
		//����¼ ���̻߳����˶��ٴΣ�ҵ�������ȫ��ִ����Ϊһ�֣����˼���
		++sun_count;
		endtime = clock();
		std::cout << CObj::ObjSum <<" ------���� ��" + to_string(sun_count) + "��  ----time��" + to_string(endtime/1000.0) << std::endl;
		
		//��������ֵ �������
		CObj::ObjSum =  EntityManager::GetEntityManager()->GetALLObjNum();
		
		for (auto it = EntityManager::GetEntityManager()->GetAllObj().begin(); it != EntityManager::GetEntityManager()->GetAllObj().end(); ++it) {
			pool.CommitTask(Task(CObj::run, (*it)));
		}

		while (CObj::ObjSum != 0) {
			this_thread::yield();
			//cout << "========== YIELD ==========="<<endl;
		}

		//�˳���־λ
		if (EndFlag == true) {
			break;
		}
	}
	//��ȷ�ȴ��̳߳ؽ������˳�ǰ��ҵ��������
	while (CObj::ObjSum != 0);
	//�˳�while��1�� �����˳������ˣ��Ѷ�������������
	for (auto &c : EntityManager::GetEntityManager()->GetAllObj()) {
		delete c;
		c = nullptr;
	}
	EntityManager::GetEntityManager()->GetAllObj().clear();
	std::cout << to_string(sun_count)<<"-------------end-------------" << std::endl;
}

void Engine::RunEngine() {
	//��־λ��true
	StartFlag = true;
	//����������startcv���̣߳��ù�����ҵ���ֻ�������
	StartCV.notify_one();
}

void Engine::PauseEngine() {
	//��ʼ��־λ��false �����̻߳�������StartCV��
	//��Ҫ�ٴε���RunEngine ��������
	StartFlag = false;
}

void Engine::EndEngine() {
	//�˳���־λ��true
	EndFlag = true;
}


EntityManager* GetManager() {
	return EntityManager::GetEntityManager();
}

