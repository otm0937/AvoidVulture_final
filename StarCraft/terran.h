#pragma once

#define SCV				1
#define Marine			2
#define Firebat			3
#define Ghost			4
#define Medic			5
#define Vulture			6

#define SiegeTank		7
#define SiegeTankS		8
#define Goliath			9

#define Wraith			10
#define Dropship		11
#define BattleCruiser	12
#define Valkyrie		13

#define ScienceVessel	14


#define SCVdir				L"resource/units/Terran/ground/NT/SCV/scv"

#define Marinedir			L"resource/units/Terran/ground/NT/Marine/marine"

#define Firebatdir			L"resource/units/Terran/ground/NT/Firebat/firebat"

#define Ghostdir			L"resource/units/Terran/ground/NT/Ghost/ghost"

#define Medicdir			L"resource/units/Terran/ground/NT/Medic/medic"

#define Vulturedir			L"resource/units/Terran/ground/NT/Vulture/vulture"
	#define VultureMinedir		L"resource/units/Terran/ground/NT/Vulture/SpiderMine/spider"


#define SiegeTankdir		L"resource/units/Terran/ground/Turret/SiegeTank/tank"
	#define SiegeTankTdir		L"resource/units/Terran/ground/Turret/SiegeTank/Turret/tankt"
#define SiegeTankSdir		L"resource/units/Terran/ground/Turret/SiegeTank/Siege/stank"
	#define SiegeTankSTdir		L"resource/units/Terran/ground/Turret/SiegeTank/Siege/Turret/stankt"

#define Goliathdir			L"resource/units/Terran/ground/Turret/Goliath/goliath"
	#define GoliathTdir			L"resource/units/Terran/ground/Turret/Goliath/Turret/goliatht"



#define Wraithdir			L"resource/units/Terran/air/NT/Wraith/phoenix"

#define Dropshipdir			L"resource/units/Terran/air/NT/Dropship/dropship"

#define BattleCruiserdir	L"resource/units/Terran/air/NT/BattleCruiser/battlecr"

#define Valkyriedir			L"resource/units/Terran/air/NT/Valkyrie/bomber"


#define ScienceVesseldir	L"resource/units/Terran/air/Turret/ScienceVessel/wessel"
	#define ScienceVesselTdir	L"resource/units/Terran/air/Turret/ScienceVessel/Turret/wesselt"






#define SCVImgCount				51

#define MarineImgCount			229

#define FirebatImgCount			170

#define GhostImgCount			229

#define MedicImgCount			230

#define VultureImgCount			17
#define VultureMineImgCount		12


#define SiegeTankImgCount		51
#define SiegeTankTImgCount		17
#define SiegeTankSImgCount		6
#define SiegeTankSTImgCount		86

#define GoliathImgCount			170
#define GoliathTImgCount		187



#define WraithImgCount			17

#define DropshipImgCount		17

#define BattleCruiserImgCount	17

#define ValkyrieImgCount		17


#define ScienceVesselImgCount	1
#define ScienceVesselTImgCount	17





unsigned int SCVimg[SCVImgCount][bmpSize][bmpSize];

unsigned int Marineimg[MarineImgCount][bmpSize][bmpSize];

unsigned int Firebatimg[FirebatImgCount][bmpSize][bmpSize];

unsigned int Ghostimg[GhostImgCount][bmpSize][bmpSize];

unsigned int Medicimg[MedicImgCount][bmpSize][bmpSize];

unsigned int Vultureimg[VultureImgCount][bmpSize][bmpSize];
	unsigned int  VultureMineimg[VultureMineImgCount][bmpSize][bmpSize];


unsigned int SiegeTankimg[SiegeTankImgCount][bmpSize][bmpSize];
	unsigned int SiegeTankTimg[SiegeTankTImgCount][bmpSize][bmpSize];
unsigned int SiegeTankSimg[SiegeTankSImgCount][bmpSize][bmpSize];
		unsigned int SiegeTankSTimg[SiegeTankSImgCount][bmpSize][bmpSize];

unsigned int Goliathimg[GoliathImgCount][bmpSize][bmpSize];
	unsigned int GoliathTimg[GoliathTImgCount][bmpSize][bmpSize];


unsigned int Wraithimg[WraithImgCount][bmpSize][bmpSize];

unsigned int Dropshipimg[DropshipImgCount][bmpSize][bmpSize];

unsigned int BattleCruiserimg[BattleCruiserImgCount][bmpSize][bmpSize];

unsigned int Valkyrieimg[ValkyrieImgCount][bmpSize][bmpSize];

unsigned int ScienceVesselimg[ScienceVesselImgCount][bmpSize][bmpSize];
	unsigned int ScienceVesselTimg[ScienceVesselTImgCount][bmpSize][bmpSize];

typedef struct { 
	int name;

	int maxHP, HP;
	int maxShield, Shield;
	int maxMana, Mana;
	
	int damageGround, damageAir;
	
	int Armor;
	
	int costMinerals;
	int costGas;
	int costPSI;

	int makeTime;
	
	int sight;
	int rangeGround;
	int rangeAir;

	int sizeX, sizeY;
	
	float speed;
	int attackCycle;

	int attackTypeGround,attackTypeAir;

	int transVolume;

	int x, y;
	int destX, destY;
	int attackX, attackY;
	int moveStep;

	int animation;
	int animationTurret;

	int route[8000][2];
	unsigned char wait;
	unsigned char Death;
	unsigned char isUsed;
	unsigned char inMap;
	unsigned char selected;
	unsigned char moving;
	unsigned char temp;

	unsigned char direction;
	unsigned char state;

	unsigned char directionTurret;
	unsigned char stateTurret;

} Unit;

#define Unitcnt 200


#define normal	1
#define vibrate	2
#define explode	3


Unit Units[Unitcnt];
Unit Enemy[Unitcnt];
Unit SCVUnit;
Unit MarineUnit;
Unit FirebatUnit;
Unit GhostUnit;
Unit MedicUnit;
Unit VultureUnit;
Unit SiegeTankUnit;
Unit SiegeTankSUnit;
Unit GoliathUnit;
Unit WraithUnit;
Unit DropshipUnit;
Unit BattleCruiserUnit;
Unit ValkyrieUnit;
Unit ScienceVesselUnit;