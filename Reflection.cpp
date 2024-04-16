#include"Reflection.h"

reflection::~reflection()
{
	auto mapit = class_member_map.begin();
	for (; mapit != class_member_map.end(); ++mapit) {
		for (auto it = (*mapit).second.begin(); it != (*mapit).second.end(); ++it) {
			delete  (*it);
			*it = nullptr;
		}
	}	
//	cout<<  " ========= ======== reflection release ======== ======== "  <<endl;
}

// map µÄ×¢²á½Ó¿Ú
bool reflection::Register_Class(ClassInfo* ci) {

	if (ci) {
		if (class_info_map.find(ci->class_name_) == class_info_map.end()) {
			class_info_map.insert(
				std::unordered_map<std::string, ClassInfo*>::value_type(ci->class_name_, ci));
		}
	}
	return true;
}

bool reflection::Register_Member(const string& ClassName, const string& memberName, const string& type, size_t offset)
{
	if (class_member_map.find(ClassName) != class_member_map.end()) {
		auto memv = class_member_map[ClassName];
		for (int i = 0; i < memv.size(); ++i) {
			if (memv[i]->name() == memberName && memv[i]->type() == type) {
				return false;
			}
		}
	}
	class_member_map[ClassName].push_back(new member(memberName, type, offset));
	return true;
}

int reflection::Get_Member_Count(const string& ClassName)
{
	return class_member_map[ClassName].size();
}

member* reflection::Get_Member(const string& ClassName, int pos)
{
	int size = class_member_map[ClassName].size();
	if (pos < 0 || pos >= size) {
		return nullptr;
	}
	return class_member_map[ClassName][pos];
}

member* reflection::Get_Member(const string& ClassName, const string& memberName)
{
	if (class_member_map.find(ClassName) == class_member_map.end()) {
		return nullptr;
	}
	auto mem = class_member_map[ClassName];
	for (auto it = mem.begin(); it != mem.end(); ++it) {
		if ((*it)->name() == memberName) {
			return (*it);
		}
	}
	return nullptr;
}


