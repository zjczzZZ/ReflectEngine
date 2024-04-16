#pragma once
#include<cstdlib>
using namespace std;

struct Attribute
{
	short m_id;
	short category;
	short subcategory;
	short country;
	double m_worldlocation[3];
	double m_velocity[3];
	double m_orientation[3];
	char* m_report;
	void* m_blackboard;
};
