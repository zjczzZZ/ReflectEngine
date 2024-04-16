#include"test.h"

IMPLEMENT_CLASS(test,classInit)

void test::classInit()
{
	REGISTER_CLASS_MEMBER(test, index, int)
	REGISTER_CLASS_MEMBER(test, v, vector<int>)
}

void test::tick() {
	//cout << "id = " + to_string(id) << endl;
	vector<CObj*> a = GetManager()->LookUpFrom("Plane");

	for (auto it = a.begin(); it != a.end(); ++it) {
		if (this->id != (*it)->id) {
			int count_temp = 0;
			for (int numb = 0; numb < 10; ++numb) {
				count_temp += numb;
				if (count_temp > 20) {
					count_temp = 1;
				}
			}
		}
	}

	int temp = 1;
	for (int k = 1; k < 30; ++k) {
		if (id == 2) {
			++count;
		}
		else {
			for (int n = 1; n < 11; ++n) {
				n / k;
				++count;
			}
		}
		temp += k;
		if (temp > 99) {
			temp /= 10;
		}
	}

	//cout << "k = " + to_string(temp) + " ----- " + to_string(count) << endl;
	count = 0;
	if (id == 1) {
		Quit();
	}

	Attribute* sr = GetState();

	vector<CObj*> tempcobjv = GetStatePool();
	for (int i = 0; i < tempcobjv.size(); ++i) {
		CObj* cbjm = tempcobjv[i];
		Attribute* attm = cbjm->GetState();
		//		cout << attm->m_id << endl;
	}
}

void test::simulation() {
	Attribute* sr = GetState();
	static int temp = 0;

	temp++;

	if (temp %10000 == 1) {
//		EntityManager::GetEntityManager()->CreateObject("plane");
		std::unordered_map<std::string, ClassInfo*> aaa = reflection::GetInstance().GetClassInfoMap();
		for (auto &c : aaa) {
			cout << c.first << " --- " <<reflection::GetInstance().Get_Member_Count("test") << endl;

			vector<int> vvv = { 1,2,3,4 };

			int age2 = 0;
			int age = 11;
//			cout << age << " " << age2 << endl;
			setRefValue("index", age);
			getRefValue("index", age2);
//			cout << age << " " << age2 << endl;
			setRefValue("v", vvv);
			vector<int> vv;
			getRefValue("v", vv);

			for (int i = 0; i < v.size(); ++i) {
//				cout << v[i] << endl;
			}
		}
		cout << EntityManager::GetEntityManager()->GetALLObjNum() << endl;
	}
}