// Win32_MonitorAlign.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <string>
#include <iomanip>
#include <iostream>

using namespace std;

BOOL CALLBACK DisplayMonitors(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{
	unsigned int* monitorCount = (unsigned int*)dwData;
	(*monitorCount)++;
	int top = lprcMonitor->top;
	int left = lprcMonitor->left;
	int bottom = lprcMonitor->bottom;
	int right = lprcMonitor->right;
	int width = abs(lprcMonitor->left - lprcMonitor->right);
	int height = abs(lprcMonitor->top - lprcMonitor->bottom);
	cout << "Monitor " << *monitorCount;
	cout << "\tTop: " << setw(5) << top;
	cout << "\tLeft: " << setw(5) << left;
	cout << "\tBottom: " << setw(5) << bottom;
	cout << "\tRight: " << setw(5) << right;
	cout << "\tWidth: " << setw(5) << width;
	cout << "\tHeight: " << setw(5) << height;
	cout << endl;
	return TRUE;
}

int sign(int val)
{
	if (val < 0)
	{
		return -1;
	}
	return 1;
}

int FindNearest(const int val, const unsigned int snap)
{
	if (snap == 0)
	{
		return val;
	}

	int remainder = abs(val) % snap;
	if (remainder < snap/2)
	{
		return val - (remainder * sign(val));
	}

	return val + snap - remainder;
}

const char* GetErrorMessageString(int result)
{
	switch (result)
	{
	case DISP_CHANGE_RESTART:
		return "The computer must be restarted for the graphics mode to work.";
	case DISP_CHANGE_FAILED:
		return "The display driver failed the specified graphics mode.";
	case DISP_CHANGE_BADMODE:
		return "The graphics mode is not supported.";
	case DISP_CHANGE_NOTUPDATED:
		return "Windows NT/2000/XP: Unable to write settings to the registry.";
	case DISP_CHANGE_BADFLAGS:
		return "An invalid set of flags was passed in.";
	case DISP_CHANGE_BADPARAM:
		return "An invalid parameter was passed in. This can include an invalid flag or combination of flags.";
	case DISP_CHANGE_BADDUALVIEW:
		return "Windows XP: The settings change was unsuccessful because the system is DualView capable.";
	}
	return "Unknown Error";
}

void SnapMonitors(const unsigned int monitorCount)
{
	const unsigned int SNAP_WIDTH = 1920;
	const unsigned int SNAP_HEIGHT = 1080;

	for (unsigned int display = 1; display <= monitorCount; ++display)
	{	
		DEVMODE devMode;
		ZeroMemory(&devMode, sizeof(devMode));
		devMode.dmSize = sizeof(devMode);

		devMode.dmFields = DM_POSITION;

		wstring deviceName = L"\\\\.\\DISPLAY";
		deviceName += to_wstring(display);

		if (EnumDisplaySettings(deviceName.c_str(), ENUM_CURRENT_SETTINGS, &devMode))
		{
			int x = FindNearest(devMode.dmPosition.x, SNAP_WIDTH);
			int y = FindNearest(devMode.dmPosition.y, SNAP_HEIGHT);
			if (devMode.dmPosition.x != x || devMode.dmPosition.y != y)
			{
				devMode.dmPosition.x = x;
				devMode.dmPosition.y = y;

				wcout << "Updating " << deviceName << endl;

				LONG res = ChangeDisplaySettingsEx(deviceName.c_str(), &devMode, NULL, CDS_UPDATEREGISTRY | CDS_NORESET, NULL);
				if (res != DISP_CHANGE_SUCCESSFUL)
				{
					const char* error = GetErrorMessageString(res);
					cerr << endl << "Error: " << error << endl;

				}
				ChangeDisplaySettingsEx(NULL, NULL, NULL, 0, NULL); //apply changes

				Sleep(5000);
			}
		}			
	}
}

int main()
{
	cout << "Original Positions:" << endl;
	cout << "------" << endl;
	unsigned int monitorCount = 0;
	if (EnumDisplayMonitors(NULL, NULL, DisplayMonitors, (LPARAM)&monitorCount))
	{

	}

	cout << endl;

	SnapMonitors(monitorCount);

	cout << "Aligned Positions:" << endl;
	cout << "------" << endl;

	monitorCount = 0;
	if (EnumDisplayMonitors(NULL, NULL, DisplayMonitors, (LPARAM)&monitorCount))
	{
	}
}
