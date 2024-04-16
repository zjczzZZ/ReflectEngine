#pragma once
#include<stdlib.h>
#include<string>
#include<unordered_map>
#include<vector>
#include<iostream>

using namespace std;

class CObj;
class ClassInfo;
class member;
typedef CObj* (*ObjectConstructorFn)(void);
typedef void (*ClassInitFunc)();


class reflection {
private:
    reflection() = default;
    reflection(const reflection& r) = delete;
    reflection& operator=(const reflection& r) = delete;

public:
    ~reflection();

    static reflection& GetInstance() {
        static reflection m_reflection;
        return m_reflection;
    }
    std::unordered_map<std::string, ClassInfo*>& GetClassInfoMap() {
        return class_info_map;
    }

    bool Register_Class(ClassInfo* ci);
    bool Register_Member(const string& ClassName, const string& memberName, const string& type, size_t offset);


    int Get_Member_Count(const string& ClassName);
    member* Get_Member(const string& ClassName, int pos);
    member* Get_Member(const string& ClassName, const string& memberName);

private:
    std::unordered_map<std::string, ClassInfo*> class_info_map;
    std::unordered_map<std::string, std::vector<member*>> class_member_map;
};


class ClassInfo {
public:
    ClassInfo(){}

    //�෴�书�ܵ�ע��
    ClassInfo(const std::string class_name, ObjectConstructorFn object_constructor, ClassInitFunc class_init)
        : class_name_(class_name)
        , object_constructor_(object_constructor) {
        reflection::GetInstance().Register_Class(this);  // �����ʱ��ͽ���ע��
        class_init();
    }

    ~ClassInfo() {}

    //��Ա���������ע��
    void RegisterMember(const std::string& class_name, const std::string& memberName, const std::string& type, size_t offset) {
        std::cout << "className: " << class_name << " memberName: " << memberName << " type: " << type << " size: " << offset << std::endl;
        reflection::GetInstance().Register_Member(class_name, memberName, type, offset);
    }

    // ���ݱ��������ĺ���ָ�빹�����
    CObj* CreateObject() const {
        return object_constructor_ ? (*object_constructor_)() : nullptr;
    }
    
public:
    std::string class_name_;
    ObjectConstructorFn object_constructor_;
};


class member {
public:
    member():m_name(""),m_type(""),m_offset(0){}
    member(const string& name,const string & type,size_t offset)
        : m_name(name)
        , m_type(type)
        , m_offset(offset) {}
    ~member(){}

    const string& name() { return m_name; }
    const string& type() { return m_type; }
    rsize_t offset() { return m_offset; }

private:
    std::string m_name;
    std::string m_type;
    size_t m_offset;
};


// ÿ��������Ķ���Ҫһ�� ClassInfo �� CreateObject
#define DECLARE_CLASS \
protected: \
    static ClassInfo class_info_; \
public: \
    static CObj* CreateObject(); \
	virtual std::string GetClassName();


// class_info ����������� CreateObject ������ʼ��
// ��ÿ��������� cpp �ļ���ʹ�øú�
#define IMPLEMENT_CLASS(name,classInit) \
ClassInfo name::class_info_(#name, (ObjectConstructorFn)name::CreateObject,name::classInit);\
CObj* name::CreateObject() { \
    return new name; \
} \
std::string name::GetClassName(){ \
    return class_info_.class_name_; \
}


//ע�������������ʱ��ȡ����ĳ�Ա����
#define REGISTER_CLASS_MEMBER(className,memberName,type) \
    class_info_.RegisterMember(#className,#memberName,#type,offsetof(className,memberName));
