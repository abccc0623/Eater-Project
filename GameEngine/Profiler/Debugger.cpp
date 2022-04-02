#pragma warning(disable: 4996)

#include "ProfilerDLL.h"
#include "Debugger.h"
#include "Timer.h"
#include "Logger.h"

#include <iostream>
#include <stdarg.h>
#include <tchar.h>
#include <iomanip>
#include <sstream>

Debugger::Debugger()
{
	AllocConsole();
	system("cls");
	m_Console = GetStdHandle(STD_OUTPUT_HANDLE);
	m_LogCount = 0;

	if ((_waccess(_T("./Log"), 0)) == -1) //여기에 LOG폴더가 없으면...
		CreateDirectory(_T("./Log"), NULL);

	m_Log.LogName = "./Log/";
	time_t currentSec = time(NULL);
	tm* t = localtime(&currentSec);

	std::ostringstream oss;

	oss.clear();
	int year = t->tm_year + 1900;
	int mon = t->tm_mon + 1;
	int day = t->tm_mday;

	oss << std::setfill('0') << std::setw(2) << year << "." << mon / 10 << mon % 10 << "." << day / 10 << day % 10 << ".log";
	
	m_Log.LogName += oss.str();

	m_Log.LogFile = fopen(m_Log.LogName.c_str(), "a+");
}

void Debugger::TimerStart(PROFILE_OUTPUT outputType, const char* func, int& line, const char* key, int& totalFrame)
{
	// 해당 문자열 Hash 추출..
	Hash hashKey = std::hash<const char*>()(key);

	// Hash Key 검색..
	m_NowKey = m_Timer.find(hashKey);

	// 만약 등록된 Key가 아니라면 등록..
	if (m_NowKey == m_Timer.end())
	{
		TIME_DESC timer;
		timer.Location = std::string(func) + " - " + std::to_string(line);
		timer.TotalTime = std::chrono::seconds::zero();
		timer.OutputType = outputType;
		timer.NowFrame = totalFrame;
		timer.TotalFrame = totalFrame;

		m_NowKey = m_Timer.insert(std::pair<Hash, TIME_DESC>(hashKey, timer)).first;
	}

	// 현재 시작 시간 설정..
	m_NowKey->second.Start = std::chrono::system_clock::now();

	// 측정 프레임 카운트 감소..
	m_NowKey->second.NowFrame--;
}

void Debugger::TimerEnd(const char* key)
{
	// Hash Key 검색..
	m_NowKey = m_Timer.find(std::hash<const char*>()(key));

	// 등록된 Hash Key가 아니라면 처리하지 않음.. 사용자의 실수
	if (m_NowKey == m_Timer.end()) return;

	// 검색된 Timer 추출..
	TIME_DESC& nowTimer = m_NowKey->second;

	// 출력 전까지 시간 누적..
	nowTimer.TotalTime += std::chrono::system_clock::now() - nowTimer.Start;

	if (nowTimer.NowFrame <= 0)
	{
		// 해당 측정 결과 출력..
		Log(nowTimer.OutputType, 
			"-----------------------[Timer]-----------------------\n %s Timer\n [Locate] \t: %s\n [TotalFrame] \t: %d\n [TotalTime] \t: %.6f sec\n [AverageTime] \t: %.6f sec\n-----------------------------------------------------\n\n",
			key,
			nowTimer.Location.c_str(),
			nowTimer.TotalFrame,
			nowTimer.TotalTime.count(),
			nowTimer.TotalTime.count() / (double)nowTimer.TotalFrame);

		// 측정이 끝난 Timer 제거..
		m_Timer.erase(m_NowKey);
	}
}

void Debugger::Log(PROFILE_OUTPUT& outputType, const char* message, ...)
{
	int length;
	va_list args;

	va_start(args, message);

	// 가변인자 문자열 길이 측정..
	length = _vscprintf(message, args) + 1;

	// 가변인자 문자열 변환..
	char* cBuf = new char[length];
	memset(cBuf, NULL, length);

	vsprintf(cBuf, message, args);

	va_end(args);

	// 출력 타입에 따른 처리..
	switch (outputType)
	{
	case PROFILE_OUTPUT::CONSOLE:
	{
		WriteFile(m_Console, cBuf, (DWORD)(length), NULL, NULL);

		m_LogCount++;
		if (m_LogCount > m_Timer.size() * 10)
		{
			system("cls");
			m_LogCount = 0;
		}
	}
		break;
	case PROFILE_OUTPUT::VS_CODE:
	{
		wchar_t* wBuf = new wchar_t[length];
		memset(wBuf, NULL, length);
		MultiByteToWideChar(CP_UTF8, 0, cBuf, -1, wBuf, length);

		OutputDebugStringW(wBuf);
		OutputDebugStringA("\n");

		delete[] wBuf;
	}
		break;
	default:
		break;
	}

	delete[] cBuf;
}

void Debugger::Log(PROFILE_OUTPUT& outputType, const char* fileInfo, char* message, int length)
{
	// 출력 타입에 따른 처리..
	switch (outputType)
	{
	case PROFILE_OUTPUT::LOG_FILE:
	{
		std::string output = GetTime();
		output += fileInfo;
		output += message;
		output += "\n\n";

		fputs(output.c_str(), m_Log.LogFile);
	}
		break;
	case PROFILE_OUTPUT::CONSOLE:
	{
		WriteFile(m_Console, message, (DWORD)(length), NULL, NULL);

		m_LogCount++;
		if (m_LogCount > 25)
		{
			system("cls");
			m_LogCount = 0;
		}
	}
	break;
	case PROFILE_OUTPUT::VS_CODE:
	{
		wchar_t* wBuf = new wchar_t[length];
		memset(wBuf, NULL, length);
		MultiByteToWideChar(CP_UTF8, 0, message, -1, wBuf, length);

		OutputDebugStringW(wBuf);
		OutputDebugStringA("\n");

		delete[] wBuf;
	}
	break;
	default:
		break;
	}
}

std::string Debugger::GetFileInfo(const char* file, const char* func, int& line)
{
	std::string outinfo;
	std::string fileinfo = file;

	size_t fileStart = fileinfo.rfind('\\') + 1;
	size_t fileEnd = fileinfo.size();
	fileinfo = fileinfo.substr(fileStart, fileEnd);

	outinfo += "[FILE] ";
	outinfo += fileinfo;
	outinfo += " [FUNC] ";
	outinfo += func;
	outinfo += " [LINE] ";
	outinfo += std::to_string(line);
	outinfo += "\n";

	return outinfo;
}

std::string Debugger::GetTime()
{
	time_t currentSec = time(NULL);
	tm* t = localtime(&currentSec);

	std::ostringstream oss;

	oss.clear();
	int hour = t->tm_hour;
	int min = t->tm_min;
	int sec = t->tm_sec;

	oss << "[TIME] " << hour / 10 << hour % 10;
	oss << ":" << min / 10 << min % 10;
	oss << ":" << sec / 10 << sec % 10 << " ";

	return oss.str();
}
