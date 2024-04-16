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
	//单独开线程跑启动加载，否则主线程会被卡住
	thread t(StartEngine,std::ref(v));
	//t.detch() 可以换成t.join() 但会阻塞main的主线程，需要将
	//t.join()放在RunEnging()之后，保证引擎一直跑

	//延迟两秒后 跑起来
	//this_thread::sleep_for(chrono::seconds(1));
	Engine::GetEngine().RunEngine();

	////延迟两秒后 暂停
	//this_thread::sleep_for(chrono::seconds(1));
	//Engine::GetEngine().PauseEngine();

	////延迟两秒后 再次跑起来
	//this_thread::sleep_for(chrono::seconds(1));
	//Engine::GetEngine().RunEngine();

	//延迟两秒后 退出，前面有join（） 会一直卡在join 不会退出
	//要退出 将join注释，搭配detch（）使用
	this_thread::sleep_for(chrono::seconds(20));
	Engine::GetEngine().EndEngine();

	t.join();

	this_thread::sleep_for(chrono::seconds(5));

	return 0;
}