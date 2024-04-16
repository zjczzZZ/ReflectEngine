#include "ForcePlane.h"
//#include "../GeneralUtilities/GeneralUtilities.h"

IMPLEMENT_CLASS(ForcePlane, classInit)

ForcePlane::ForcePlane(void)
{
	initAirCraft = false;
	isFollow = false;
	leaderID = 0;
	isGo = false;
	planesCount = 0;
	isFormation = false;
	isFireMissile = false;
}

ForcePlane::~ForcePlane(void)
{
}

void ForcePlane::classInit()
{
	//QString entry=property("paraEntry").toString();
	//QByteArray str;
	//str.append(entry);	
}

void ForcePlane::init()
{
	srAircraft = GetState();
	srAircraft->m_id = id;
	srAircraft->category = 1;
	srAircraft->country = 2;
	srAircraft->subcategory = 3;

	srAircraft->m_orientation[0] = 0;
	srAircraft->m_orientation[1] = 20;
	srAircraft->m_orientation[2] = 0;
	srAircraft->m_velocity[0] = 0;
	srAircraft->m_velocity[1] = 100;
	srAircraft->m_velocity[2] = 0;
	srAircraft->m_worldlocation[0] = 120;
	srAircraft->m_worldlocation[1] = 20;
	srAircraft->m_worldlocation[2] = 1200;

	char tempr[20] = "planePara.txt";
	aircraft.read_performance(tempr);

	Attribute* srObj=GetState();

	InitPara para;
	para.airCraftType = 1;//0 直升机  1 固定翼
	para.m_dPitch_deg = -0.1;
	para.m_dRWAlti_m = 1200;
	para.m_dRWHead_deg = 20;
	para.m_dRWSpeed_mps = 100;
	para.m_dRWLati_deg = srObj->m_worldlocation[1];
	para.m_dRWLongi_deg = srObj->m_worldlocation[0];

	strcpy_s(para.airCraftTypeFileName,"");
	/*QString entry=property("paraEntry").toString();
	QByteArray str;
	str.append(entry);
	para.airCraftTypeFileName=str.data();*/
	aircraft.initCraft(para);

	double ttt = aircraft.GetLongitude();
	double tttt = aircraft.GetLatitude();

	for (int i = 1; i < 5; ++i) {
		RoutePoint temprp;
		temprp.lon = 120.0 + i / 50.0;
		temprp.lat = 20.0 + i / 50.0;
		temprp.alt = 1000 + i * 200;
		temprp.v = 100;
		temprp.stayT = 0;
		temprp.flag = true;
		routepoints.push_back(temprp);
	}

	aircraft.SetPoints(routepoints.size(), &routepoints[0]);
	aircraft.Go();
	isGo = true;

}


void ForcePlane::tick()
{
	srAircraft = GetState();
	double step=0.01;
	aircraft.Tick(step);
	double ttttttt = aircraft.GetVelocity();
	if(ttttttt){
		
		double x = srAircraft->m_orientation[2];
		double z = srAircraft->m_velocity[2];
		int i=0;
	}
	
	srAircraft->m_velocity[0] = aircraft.GetVelocity()*sin(srAircraft->m_orientation[1])*3.6;
	srAircraft->m_velocity[1] = aircraft.GetVelocity()*cos(srAircraft->m_orientation[1])*3.6;
	srAircraft->m_velocity[2] =0;
	srAircraft->m_worldlocation[0] = aircraft.GetLongitude();
	srAircraft->m_worldlocation[1] = aircraft.GetLatitude();
	srAircraft->m_worldlocation[2] = aircraft.GetHeight();
	srAircraft->m_orientation[1] = aircraft.GetHeading();
	srAircraft->m_orientation[2] =aircraft.GetRoll();
	srAircraft->m_orientation[0] = aircraft.GetPitch();//弧度*/
//	cout << "=-=-=-= " << srAircraft->m_worldlocation[0] << " =-=-=-= " << srAircraft->m_worldlocation[1] << " =-=-=-= " << srAircraft->m_worldlocation[2] << endl;
}