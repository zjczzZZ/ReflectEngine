#pragma once
#include "..\engine\HLAengine.h"
#include "..\Model_Plane\PlaneModel_dll.h"
#include "..\Model_Plane\PlaneModel.h"
#include "..\Model_Plane\FormationServer.h"

#pragma comment(lib,"Model_Plane.lib")

class ForcePlane : public Component
{
	DECLARE_CLASS
public:
	 ForcePlane(void);
	~ForcePlane(void);

	static void classInit();
	virtual	void tick();
	virtual	void init();
protected:
	PlaneModel	aircraft;

private:
	
	Attribute* srAircraft;//·É»ú×´Ì¬³Ø
	std::vector<RoutePoint> routepoints;
	bool initAirCraft;
	bool isFollow;
	PlaneModel* planes[20];
	int planesCount;
	int leaderID;
	bool isFormation;
	std::vector<int> followIdVec;
	FormationServer formationServer;
//	std::vector<Attribute*> tgtSr;
	bool isFireMissile;
	RoutePoint airport;

public:
	bool isGo;
	bool oncef;
};