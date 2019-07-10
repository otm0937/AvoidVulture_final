#include "header.h"
#include "terran.h"

// 유닛 설정


/*							이름			,만피	,피		,만쉴드	,쉴드	,만마나	,마나	,땅뎀	,공뎀	,방어력	,미네랄	,가스	,인구	,시간	,시야	,땅사	,공사	,가로	,세로	,이속	,공주기	,땅공		,공공		,공간*/
Unit SCVUnit =				{ SCV			,60		,60		,0		,0		,0		,0		,5		,0		,0		,50		,0		,1		,13		,7		,1		,0		,23		,23		,2.344	,15		,normal		,0			,1 };
Unit MarineUnit =			{ Marine		,40		,40		,0		,0		,0		,0		,6		,6		,0		,50		,0		,1		,15		,7		,4		,4		,17		,20		,1.875	,15		,normal		,normal		,1 };
Unit FirebatUnit =			{ Firebat		,50		,50		,0		,0		,0		,0		,16		,0		,1		,50		,25		,1		,15		,7		,1		,0		,23		,22		,1.875	,22		,vibrate	,0			,1 };
Unit GhostUnit =			{ Ghost			,45		,45		,0		,0		,200	,50		,10		,10		,0		,25		,75		,1		,32		,9		,7		,7		,15		,22		,1.875	,22		,vibrate	,vibrate	,1 };
Unit MedicUnit =			{ Medic			,60		,60		,0		,0		,200	,50		,0		,0		,1		,50		,25		,1		,19		,9		,0		,0		,14		,18		,1.875	,0		,0			,0			,1 };
Unit VultureUnit =			{ Vulture		,80		,80		,0		,0		,0		,0		,20		,0		,0		,75		,0		,2		,19		,8		,5		,0		,16		,50		,3.126	,30		,vibrate	,0			,1 };
Unit SiegeTankUnit =		{ SiegeTank		,150	,150	,0		,0		,0		,0		,30		,0		,1		,150	,100	,2		,32		,10		,7		,0		,32		,32		,1.875	,75		,explode	,0			,4 };
Unit SiegeTankSUnit =		{ SiegeTankS	,150	,150	,0		,0		,0		,0		,70		,0		,1		,150	,100	,2		,32		,10		,12		,0		,32		,10		,0		,75		,explode	,0			,4 };
Unit GoliathUnit =			{ Goliath		,125	,125	,0		,0		,0		,0		,12		,20		,1		,100	,50		,2		,25		,8		,6		,5		,32		,32		,2,2	,22		,normal		,explode	,2 };
Unit WraithUnit =			{ Wraith		,120	,120	,0		,0		,200	,50		,8		,20		,0		,150	,100	,2		,38		,7		,5		,5		,38		,30		,3.126	,22		,normal		,explode	,0 };
Unit DropshipUnit =			{ Dropship		,150	,150	,0		,0		,0		,0		,0		,0		,1		,100	,100	,2		,31		,8		,0		,0		,49		,37		,2.563	,0		,0			,0			,0 };
Unit BattleCruiserUnit =	{ BattleCruiser	,500	,500	,0		,0		,200	,50		,25		,25		,3		,400	,300	,6		,84		,11		,6		,6		,75		,59		,1.172	,30		,normal		,normal		,0 };
Unit ValkyrieUnit =			{ Valkyrie		,200	,200	,0		,0		,0		,0		,0		,6		,2		,250	,125	,3		,31		,8		,0		,6		,49		,37		,3.094	,64		,0			,explode	,0 };
Unit ScienceVesselUnit =	{ ScienceVessel	,200	,200	,0		,0		,200	,0		,0		,0		,1		,100	,225	,2		,50		,10		,0		,0		,40		,40		,2.344	,0		,0			,0			,0 };