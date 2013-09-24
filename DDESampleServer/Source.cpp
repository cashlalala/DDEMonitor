#include <windows.h>
#include <stdio.h>
#include <ddeml.h>


// Globals...
HSZ g_hszAppName;
HSZ g_hszTopicName;
HSZ g_hszItemName;
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

	// Create string handles...
	g_hszAppName = DdeCreateStringHandle(g_idInst, L"DdemlSvr", NULL);
	g_hszTopicName = DdeCreateStringHandle(g_idInst, L"MyTopic", NULL);
	g_hszItemName = DdeCreateStringHandle(g_idInst, L"MyItem", NULL);

	if( (g_hszAppName == 0) || (g_hszTopicName == 0) || (g_hszItemName == 0) ) {
		MessageBox(NULL, L"DdeCreateStringHandle() failed", L"Error", MB_SETFOREGROUND);
		return -2;
	}

	// Register DDE server
	if(!DdeNameService(g_idInst, g_hszAppName, NULL, DNS_REGISTER)) {
		MessageBox(NULL, L"DdeNameService() failed!", L"Error", MB_SETFOREGROUND);
		return -3;
	}

	// Create a timer to simulate changing data...
	SetTimer(0,0,1000,0);

	// Message loop:
	MSG msg;

	while (GetMessage(&msg, NULL, 0, 0))  {
		// On WM_TIMER, change our counter, and update clients...
		if(msg.message == WM_TIMER) {
			g_count = g_count%256;
			g_count++;
			DdePostAdvise(g_idInst, g_hszTopicName, g_hszItemName);
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

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

		if((!DdeCmpStringHandles(hsz1, g_hszTopicName)) &&
			(!DdeCmpStringHandles(hsz2, g_hszItemName)))
			return (HDDEDATA)TRUE;   // SERVER supports Topic|Service
		else
			return FALSE;  // SERVER does not support Topic|Service
		// ----------------------------------------------------------------


	case XTYP_ADVREQ:

		// Client wants our data. Since this is specific to Excel, we'll
		// go ahead and assume they want XlTable-formatted data. For a
		// generic DDE server, you might want to handle various formats
		// specified by the passed in fmt parameter.

		if(!DdeCmpStringHandles(hsz1, g_hszTopicName) &&
			!DdeCmpStringHandles(hsz2, g_hszItemName)) {

				unsigned char xltableData[100];
				memset(xltableData,0x0,100);

				// tdtTable record...
				xltableData[0] = 'a'; // tdtTable
				xltableData[1] = 'b'; // 2 short ints following
				xltableData[2] = 'C'; // # rows
				xltableData[3] = 'd'; // # cols

				// tdtInt record...
				xltableData[4] = 'e';
				xltableData[5] = 'f';
				xltableData[6] = (unsigned char)g_count;

				return DdeCreateDataHandle(g_idInst, (UCHAR*)xltableData, 1*7, 0, g_hszItemName, fmt, 0);
		}    
		// ----------------------------------------------------------------
	default:
		return (HDDEDATA)NULL;
	}
}
