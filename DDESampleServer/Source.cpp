#include <windows.h>
#include <stdio.h>
#include <ddeml.h>

#include <time.h>
// Globals...
HSZ g_hszAppName;
HSZ g_hszTopicName;
HSZ g_hszItemVolume;
HSZ g_hszItemStrikePC;
HSZ g_hszItemRiseFall;
HSZ g_hszItemNameTime;
HSZ g_hszItemNameBuyPC;
HSZ g_hszItemNameSellPC;
HSZ g_hszItemName;
HSZ g_hszItemCode;
int g_count = 0;
DWORD g_idInst = 0;

// Declarations:
HDDEDATA EXPENTRY DdeCallback(UINT type, UINT fmt, HCONV hConv, HSZ hsz1, HSZ hsz2, HDDEDATA hData, DWORD dwData1, DWORD dwData2);

// WinMain()..
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	// Initialize DDEML...
	if(DdeInitialize(&g_idInst, DdeCallback, APPCLASS_STANDARD, 0)) {
		MessageBox(NULL, L"DdeInitialize() failed", L"Error", MB_SETFOREGROUND);
		return -1;
	}

	srand(time(NULL));

	// Create string handles...
	g_hszAppName = DdeCreateStringHandle(g_idInst, L"MMSDDE", NULL);
	g_hszTopicName = DdeCreateStringHandle(g_idInst, L"FUSA", NULL);
	g_hszItemVolume = DdeCreateStringHandle(g_idInst, L"M1JY&.404", NULL);
	g_hszItemStrikePC = DdeCreateStringHandle(g_idInst, L"M1JY&.125", NULL);
	g_hszItemRiseFall = DdeCreateStringHandle(g_idInst, L"M1JY&.184", NULL);
	g_hszItemNameTime = DdeCreateStringHandle(g_idInst, L"M1JY&.143", NULL);
	g_hszItemNameBuyPC = DdeCreateStringHandle(g_idInst, L"M1JY&.101", NULL);
	g_hszItemNameSellPC = DdeCreateStringHandle(g_idInst, L"M1JY&.102", NULL);
	g_hszItemName = DdeCreateStringHandle(g_idInst, L"M1JY&.2020", NULL);
	g_hszItemCode = DdeCreateStringHandle(g_idInst, L"M1JY&.1018", NULL);

	if( (g_hszAppName == 0) || (g_hszTopicName == 0) || (g_hszItemVolume == 0)
		|| (g_hszItemStrikePC == 0) || (g_hszItemRiseFall == 0)
		|| (g_hszItemNameTime == 0) || (g_hszItemNameBuyPC == 0)
		|| (g_hszItemNameSellPC == 0) || (g_hszItemName == 0)
		|| (g_hszItemCode == 0)) {
		MessageBox(NULL, L"DdeCreateStringHandle() failed", L"Error", MB_SETFOREGROUND);
		return -2;
	}

	// Register DDE server
	if(!DdeNameService(g_idInst, g_hszAppName, NULL, DNS_REGISTER)) {
		MessageBox(NULL, L"DdeNameService() failed!", L"Error", MB_SETFOREGROUND);
		return -3;
	}

	// Create a timer to simulate changing data...
	SetTimer(0,0,1500,0);

	// Message loop:
	MSG msg;

	while (GetMessage(&msg, NULL, 0, 0))  {
		// On WM_TIMER, change our counter, and update clients...
		if(msg.message == WM_TIMER) {
			switch (g_count%6)
			{
			case 0:
				DdePostAdvise(g_idInst, g_hszTopicName, g_hszItemVolume);
				break;
			case 1:
				DdePostAdvise(g_idInst, g_hszTopicName, g_hszItemStrikePC);
				break;
			case 2:
				DdePostAdvise(g_idInst, g_hszTopicName, g_hszItemRiseFall);
				break;
			case 3:
				DdePostAdvise(g_idInst, g_hszTopicName, g_hszItemNameTime);
				break;
			case 4:
				DdePostAdvise(g_idInst, g_hszTopicName, g_hszItemNameBuyPC);
				break;
			case 5:
				DdePostAdvise(g_idInst, g_hszTopicName, g_hszItemNameSellPC);
			default:
				break;
			}
			g_count = rand()%10 + 48; //always show chars
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if(!DdeNameService(g_idInst, g_hszAppName, NULL, DNS_UNREGISTER)) {
		MessageBox(NULL, L"DdeNameService() failed!", L"Error", MB_SETFOREGROUND);
		return -3;
	}

	DdeFreeStringHandle(g_idInst,g_hszAppName);
	DdeFreeStringHandle(g_idInst,g_hszTopicName);
	DdeFreeStringHandle(g_idInst,g_hszItemVolume);
	DdeFreeStringHandle(g_idInst,g_hszItemStrikePC);
	DdeFreeStringHandle(g_idInst,g_hszItemRiseFall);
	DdeFreeStringHandle(g_idInst,g_hszItemNameTime);
	DdeFreeStringHandle(g_idInst,g_hszItemNameBuyPC);
	DdeFreeStringHandle(g_idInst,g_hszItemNameSellPC);
	DdeFreeStringHandle(g_idInst,g_hszItemName);
	DdeFreeStringHandle(g_idInst,g_hszItemCode);

	DdeUninitialize(g_idInst);

	return msg.wParam;
}


// Our DDE Callback function...
HDDEDATA EXPENTRY DdeCallback(UINT wType, UINT fmt, HCONV hConv, HSZ hsz1, HSZ hsz2, HDDEDATA hData, DWORD dwData1, DWORD dwData2) {

	switch (wType) {

		// ----------------------------------------------------------------
	case XTYP_CONNECT:
		// Client is trying to connect. Respond TRUE if we have what they want...
		if ((!DdeCmpStringHandles(hsz1, g_hszTopicName)) &&
			(!DdeCmpStringHandles(hsz2, g_hszAppName)))
			return (HDDEDATA)TRUE;   // SERVER supports Topic|Service
		else
			return FALSE;  // SERVER does not support Topic|Service

		// ----------------------------------------------------------------
	case XTYP_ADVSTART:
		// Client starting advisory loop.
		// Say "ok" if we have what they are asking for...   

		if((!DdeCmpStringHandles(hsz1, g_hszTopicName)) /*&&
			(!DdeCmpStringHandles(hsz2, g_hszItemVolume) ||
			 !DdeCmpStringHandles(hsz2, g_hszItemStrikePC) ||
			 !DdeCmpStringHandles(hsz2, g_hszItemRiseFall) )*/
			)
			return (HDDEDATA)TRUE;   // SERVER supports Topic|Service
		else
			return FALSE;  // SERVER does not support Topic|Service
		// ----------------------------------------------------------------
	case XTYP_REQUEST:
	case XTYP_ADVREQ:

		// Client wants our data. Since this is specific to Excel, we'll
		// go ahead and assume they want XlTable-formatted data. For a
		// generic DDE server, you might want to handle various formats
		// specified by the passed in fmt parameter.
		{
			unsigned char xltableData[100];
			memset(xltableData,0x0,100);
			int nSize = 0;

			if(!DdeCmpStringHandles(hsz1, g_hszTopicName) &&
				!DdeCmpStringHandles(hsz2, g_hszItemVolume)) 
			{
				// tdtTable record...
				xltableData[0] = '3'; // tdtTable
				xltableData[1] = '2'; // 2 short ints following
				xltableData[2] = '7'; // # rows
				xltableData[3] = '6'; // # cols

				// tdtInt record...
				xltableData[4] = '4';
				xltableData[5] = '5';
				xltableData[6] = (unsigned char)g_count;					

				nSize = sizeof(char) * 7;
				return DdeCreateDataHandle(g_idInst, (UCHAR*)xltableData, nSize, 0, g_hszItemVolume, fmt, 0);
			}
			else if (!DdeCmpStringHandles(hsz1, g_hszTopicName) &&
				!DdeCmpStringHandles(hsz2, g_hszItemNameSellPC))
			{
				// tdtTable record...
				xltableData[0] = '4'; // tdtTable
				xltableData[1] = '1'; // 2 short ints following
				xltableData[2] = '2'; // # cols
				xltableData[3] = '0';
				xltableData[4] = (unsigned char)g_count;

				nSize = sizeof(char) * 5;
				return DdeCreateDataHandle(g_idInst, (UCHAR*)xltableData, nSize, 0, g_hszItemNameSellPC, fmt, 0);
			}
			else if (!DdeCmpStringHandles(hsz1, g_hszTopicName) &&
				!DdeCmpStringHandles(hsz2, g_hszItemNameBuyPC))
			{
				// tdtTable record...
				xltableData[0] = '3'; // tdtTable
				xltableData[1] = '7'; // 2 short ints following
				xltableData[2] = '9'; // # cols
				xltableData[3] = (unsigned char)g_count;
				xltableData[4] = '6';

				nSize = sizeof(char) * 5;
				return DdeCreateDataHandle(g_idInst, (UCHAR*)xltableData, nSize, 0, g_hszItemNameBuyPC, fmt, 0);
			}
			else if (!DdeCmpStringHandles(hsz1, g_hszTopicName) &&
				!DdeCmpStringHandles(hsz2, g_hszItemStrikePC))
			{
				// tdtTable record...
				xltableData[0] = '7'; // tdtTable
				xltableData[1] = '8'; // 2 short ints following
				xltableData[2] = '9';
				xltableData[3] = '5'; // # cols
				xltableData[4] = '1';
				xltableData[5] = (unsigned char)g_count;

				nSize = sizeof(char) * 6;
				return DdeCreateDataHandle(g_idInst, (UCHAR*)xltableData, nSize, 0, g_hszItemStrikePC, fmt, 0);
			}
			else if (!DdeCmpStringHandles(hsz1, g_hszTopicName) &&
				!DdeCmpStringHandles(hsz2, g_hszItemNameTime))
			{
				// tdtTable record...
				xltableData[0] = '3'; // tdtTable
				xltableData[1] = '1'; // 2 short ints following
				xltableData[2] = (unsigned char)g_count;
				xltableData[3] = '4'; // # cols
				xltableData[4] = '0';

				nSize = sizeof(char) * 5;
				return DdeCreateDataHandle(g_idInst, (UCHAR*)xltableData, nSize, 0, g_hszItemNameTime, fmt, 0);
			}
			else if (!DdeCmpStringHandles(hsz1, g_hszTopicName) &&
				!DdeCmpStringHandles(hsz2, g_hszItemRiseFall))
			{
				// tdtTable record...
				xltableData[0] = '5'; // tdtTable
				xltableData[1] = '7'; // 2 short ints following
				xltableData[2] = '9'; // # rows
				xltableData[3] = (unsigned char)g_count; // # cols
				xltableData[4] = '1';

				nSize = sizeof(char) * 5;
				return DdeCreateDataHandle(g_idInst, (UCHAR*)xltableData, nSize, 0, g_hszItemRiseFall, fmt, 0);
			}
			else if (!DdeCmpStringHandles(hsz1, g_hszTopicName) &&
				!DdeCmpStringHandles(hsz2, g_hszItemName))
			{
				if (XTYP_REQUEST == wType)
				{
					// tdtTable record...
					xltableData[0] = 'n'; // tdtTable
					xltableData[1] = 'a'; // 2 short ints following
					xltableData[2] = 'm'; // # rows
					xltableData[3] = 'e';

					nSize = sizeof(char) * 4;
					return DdeCreateDataHandle(g_idInst, (UCHAR*)xltableData, nSize, 0, g_hszItemName, fmt, 0);
				}
			}
			else if (!DdeCmpStringHandles(hsz1, g_hszTopicName) &&
				!DdeCmpStringHandles(hsz2, g_hszItemCode))
			{
				if (XTYP_REQUEST == wType)
				{
					// tdtTable record...
					xltableData[0] = 'c'; // tdtTable
					xltableData[1] = 'o'; // 2 short ints following
					xltableData[2] = 'd'; // # rows
					xltableData[3] = 'e';

					nSize = sizeof(char) * 4;
					return DdeCreateDataHandle(g_idInst, (UCHAR*)xltableData, nSize, 0, g_hszItemCode, fmt, 0);
				}
			}
			return (HDDEDATA)NULL;
		}

		// ----------------------------------------------------------------
	default:
		return (HDDEDATA)NULL;
	}
}
