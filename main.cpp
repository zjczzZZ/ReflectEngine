#include"HLAengine.h"

using namespace std;

int main() {

	unordered_map<string, vector<string>> ss;
	ss["plane"].push_back("ForcePlane");
	ss["car"].push_back("test");
	vector<std::string> v;
	for (int i = 0; i < 60; ++i) {
		v.push_back("car");
	}
	v.push_back("plane");
	auto te = EntityManager::GetEntityManager()->GetObjMap();
	EntityManager::GetEntityManager()->SetConvergeInfo(ss);
//	EntityManager::GetEntityManager()->CreateObject("test");
	//�������߳����������أ��������̻߳ᱻ��ס
	thread t(StartEngine,std::ref(v));
	//t.detch() ���Ի���t.join() ��������main�����̣߳���Ҫ��
	//t.join()����RunEnging()֮�󣬱�֤����һֱ��

	//�ӳ������ ������
	//this_thread::sleep_for(chrono::seconds(1));
	Engine::GetEngine().RunEngine();

	////�ӳ������ ��ͣ
	//this_thread::sleep_for(chrono::seconds(1));
	//Engine::GetEngine().PauseEngine();

	////�ӳ������ �ٴ�������
	//this_thread::sleep_for(chrono::seconds(1));
	//Engine::GetEngine().RunEngine();

	//�ӳ������ �˳���ǰ����join���� ��һֱ����join �����˳�
	//Ҫ�˳� ��joinע�ͣ�����detch����ʹ��
	this_thread::sleep_for(chrono::seconds(20));
	Engine::GetEngine().EndEngine();

	t.join();

	this_thread::sleep_for(chrono::seconds(5));

	return 0;
}