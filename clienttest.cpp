/*=========================================================
 //
 // File: ClientTest.cpp
 //
 // Created by Ned Phipps, Oct-2004
 //
 =============================================================================*/

#include <string.h>
#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <vector>

#include "cortex.h"

using namespace std;

sFrameOfData *FrameofData = NULL;

void MyErrorMsgHandler(int iLevel, const char *szMsg)
{
  const char *szLevel = NULL;

  if (iLevel == VL_Debug)
  {
    szLevel = "Debug";
  }
  else if (iLevel == VL_Info)
  {
    szLevel = "Info";
  }
  else if (iLevel == VL_Warning)
  {
    szLevel = "Warning";
  }
  else if (iLevel == VL_Error)
  {
    szLevel = "Error";
  }
  printf("  %s: %s\n", szLevel, szMsg);
}

void DataHandler(sFrameOfData *pFrameOfData) {
  FrameofData = pFrameOfData;
  printf("Received frame of data with %d bodies\n", FrameofData->nBodies);
}

int main()
{

  sBodyDefs *pBodyDefs = NULL;
  sBodyDef *pBody{NULL};

  std::vector<int> bodyMarkers;
  int nBytes;
  void *pResponse;

  int retval = RC_Okay;

  Cortex_SetVerbosityLevel(VL_Info);
  Cortex_SetDataHandlerFunc(DataHandler);
  Cortex_SetErrorMsgHandlerFunc(MyErrorMsgHandler);

  retval = Cortex_Initialize("127.0.0.1", nullptr, 1510, 1511);
  if (retval != RC_Okay)
  {
    printf("Error: Unable to initialize ethernet communication\n");
    retval = Cortex_Exit();
    return 1;
  }

  // cortex frame rate //
  printf("\n****** Cortex_FrameRate ******\n");
  retval = Cortex_Request("GetContextFrameRate", &pResponse, &nBytes);
  if (retval != RC_Okay)
    printf("ERROR, GetContextFrameRate\n");
  float *contextFrameRate = (float *)pResponse;
  printf("ContextFrameRate = %3.1f Hz\n", *contextFrameRate);

  printf("\n****** Cortex_GetBodyDefs ******\n");
  pBodyDefs = Cortex_GetBodyDefs();
  if (pBodyDefs == NULL)
  {
    printf("Failed to get body defs\n");
  }
  else
  {
    int totalBodies = pBodyDefs->nBodyDefs;
    cout << "total no of bodies tracked " << totalBodies << endl;
    for (int iBody = 0; iBody < totalBodies; iBody++)
    {
      bodyMarkers.push_back(pBodyDefs->BodyDefs[iBody].nMarkers);
      pBody = &pBodyDefs->BodyDefs[iBody];
      cout << "number of markers defined in body " << iBody + 1 << " (\"" << pBody->szName << "\") : " << bodyMarkers.at(iBody) << endl;

      for (int iMarker = 0; iMarker < pBody->nMarkers; iMarker++)
      {
        cout << iMarker << " " << pBody->szMarkerNames[iMarker] << endl;
      }
    }
  }

  printf("\n*** start live mode ***\n");
  Cortex_Request("LiveMode", &pResponse, &nBytes);

  while (1)
  {
    usleep(1000000);
  }

  return 0;
}
