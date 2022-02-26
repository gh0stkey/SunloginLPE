#include "stdafx.h"
#include<regex>
using namespace std;

/**
*	��ȡCMDִ�н��
*/
int getCmdResult(char* cmd, char* result)
{
	char buffer[1024];
	FILE * pipe = _popen(cmd, "r");
	if (!pipe)
	{
		return 0;
	}
	while (!feof(pipe))
	{
		if (fgets(buffer, sizeof(buffer), pipe))
		{
			// ׷��
			strcat_s(result, sizeof(buffer), buffer);
		}
	}
	_pclose(pipe);

	return 1;
}


/**
*	��ȡ���տ�PID
*/
int getSunloginPID(char* oPID)
{
	// �����л�ȡtasklist /svc���
	char cCmdBuffer[4096] = { 0 };
	int retCode = getCmdResult("tasklist /svc | findstr SunloginService", cCmdBuffer);
	if (retCode)
	{
		string sCmdStr = cCmdBuffer;
		if (sCmdStr == "") {
			return 0;
		}
		regex rPID(" ([0-9].*?) ");
		smatch mResult;
		regex_search(sCmdStr, mResult, rPID);
		strcpy_s(oPID, 10, mResult.str(1).c_str());
	}
	else
	{
		printf("Func: getCmdResult, Failed!");
		return 0;
	}
	return 1;

}

/**
*	��ȡ���տ�PORT
*/
int getSunloginPort(char* iPID, char* oPort)
{
	char cmd[80];
	char* cCmdTemplate = "netstat -ano | findstr \" %s \" | findstr LISTENING | findstr /v 127.0.0.1";
	sprintf_s(cmd, cCmdTemplate, iPID);
	// �����л�ȡnetstat -ano���
	char cCmdBuffer[4096] = { 0 };
	int retCode = getCmdResult(cmd, cCmdBuffer);
	if (retCode)
	{
		string sCmdStr = cCmdBuffer;
		if (sCmdStr == "") {
			return 0;
		}
		regex rPort(" 0\.0\.0\.0:([0-9]{5}) ");
		smatch mResult;
		regex_search(sCmdStr, mResult, rPort);
		strcpy_s(oPort, 10, mResult.str(1).c_str());
	}
	else
	{
		printf("Func: getCmdResult, Failed!");
		return 0;
	}
	// printf("%s \n", cCmdBuffer);
	return 1;
}