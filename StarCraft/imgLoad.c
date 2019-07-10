#include "header.h"
#include "terran.h"
#include "UI.h"
#include "tile.h"

#define loadPrint printf("   ."); printf("."); printf(".");



int loadBitmap(unsigned int img[bmpSize][bmpSize], wchar_t *filename) {
	if (_waccess_s(filename, 00) != 0)
		return 0;
	HBITMAP hbitmap = (HBITMAP)LoadImage(NULL, filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	BITMAP bitmap;
	GetObject(hbitmap, sizeof(BITMAP), &bitmap);

	unsigned int(*src)[bmpSize] = bitmap.bmBits;

	const int h = abs(bitmap.bmHeight);
	if (bitmap.bmHeight >= 0)
		for (int i = 0; i < h; i++)
			for (int j = 0; j < bitmap.bmWidth; j++)
				img[h - i - 1][j] = src[i][j];

	DeleteObject(hbitmap);
	return 1;
}


void Load(int count, wchar_t*directory, unsigned int imgarr[][bmpSize][bmpSize]) {

	wchar_t filecnt[100];
	for (int i = 0; i < count; i++)
	{
		swprintf(filecnt, 100, L"%s%03d.bmp", directory, i);
		loadBitmap(imgarr[i], filecnt);
		printf("Unit Files Loading ... ");
		wprintf_s(L"%s", filecnt);
		loadPrint
		if (loadBitmap(imgarr[i], filecnt))
			printf("DONE\n");
		else
			printf("CannotFIndImage\n");
	}
}



int loadBackground(unsigned int img[bmpSize][bmpSize], wchar_t *filename) {
	if (_waccess_s(filename, 00) != 0)
		return 0;
	HBITMAP hbitmap = (HBITMAP)LoadImage(NULL, filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	BITMAP bitmap;
	GetObject(hbitmap, sizeof(BITMAP), &bitmap);

	unsigned int(*src)[bmpSize] = bitmap.bmBits;

	const int h = abs(bitmap.bmHeight);
	if (bitmap.bmHeight >= 0)
		for (int i = 0; i < h; i++)
			for (int j = 0; j < bitmap.bmWidth; j++)
				img[h - i - 1][j] = src[i][j];

	DeleteObject(hbitmap);
	return 1;
}

void imgLoad() {

	Load(SCVImgCount, SCVdir, SCVimg);
	Load(MarineImgCount, Marinedir, Marineimg);
	Load(FirebatImgCount, Firebatdir, Firebatimg);
	Load(GhostImgCount, Ghostdir, Ghostimg);
	Load(MedicImgCount, Medicdir, Medicimg);
	Load(VultureImgCount, Vulturedir, Vultureimg);
	Load(VultureMineImgCount, VultureMinedir, VultureMineimg);
	Load(SiegeTankImgCount, SiegeTankdir, SiegeTankimg);
	Load(SiegeTankTImgCount, SiegeTankTdir, SiegeTankTimg);
	Load(SiegeTankSImgCount, SiegeTankSdir, SiegeTankSimg);
	Load(SiegeTankSTImgCount, SiegeTankSTdir, SiegeTankSTimg);
	Load(GoliathImgCount, Goliathdir, Goliathimg);
	Load(GoliathTImgCount, GoliathTdir, GoliathTimg);
	Load(WraithImgCount, Wraithdir, Wraithimg);
	Load(DropshipImgCount, Dropshipdir, Dropshipimg);
	Load(BattleCruiserImgCount, BattleCruiserdir, BattleCruiserimg);
	Load(ValkyrieImgCount, Valkyriedir, Valkyrieimg);
	Load(ScienceVesselImgCount, ScienceVesseldir, ScienceVesselimg);
	Load(ScienceVesselTImgCount, ScienceVesselTdir, ScienceVesselTimg);

	Load(arrowImgCount, arrowdir, arrowimg);			//getchar();
	Load(dragImgCount, dragdir, dragimg);				//getchar();
	Load(maggImgCount, maggdir, maggimg);				//getchar();
	Load(magrImgCount, magrdir, magrimg);				//getchar();
	Load(magyImgCount, magydir, magyimg);				//getchar();
	Load(scrolldImgCount, scrollddir, scrolldimg);		//getchar();
	Load(scrolldlImgCount, scrolldldir, scrolldlimg);	//getchar();
	Load(scrolldrImgCount, scrolldrdir, scrolldrimg);	//getchar();
	Load(scrolllImgCount, scrollldir, scrolllimg);		//getchar();
	Load(scrollrImgCount, scrollrdir, scrollrimg);		//getchar();
	Load(scrolluImgCount, scrolludir, scrolluimg);		//getchar();
	Load(scrollulImgCount, scrolluldir, scrollulimg);	//getchar();
	Load(scrollurImgCount, scrollurdir, scrollurimg);	//getchar();
	Load(targgImgCount, targgdir, targgimg);			//getchar();
	Load(targrImgCount, targrdir, targrimg);			//getchar();
	Load(targyImgCount, targydir, targyimg);			//getchar();
	Load(targnImgCount, targndir, targnimg);			//getchar();
	loadBackground(mapBlackimg, mapBlackdir);
	loadBackground(mapWhiteimg, mapWhitedir);
	loadBackground(mapBlackWhiteimg, mapBlackWhitedir);

	//getchar();
	system("cls");
}