// Win32_MonitorAlign.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <iomanip>
#include <iostream>

using namespace std;

BOOL CALLBACK DisplayMonitors(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{
	int* monitorCount = (int*)dwData;
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

BOOL CALLBACK SnapMonitors(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{
	const unsigned int RESOLUTION_WIDTH = 1920;
	const unsigned int RESOLUTION_HEIGHT = 1080;

	int* monitorCount = (int*)dwData;
	(*monitorCount)++;
	int top = FindNearest(lprcMonitor->top, RESOLUTION_HEIGHT);
	int left = FindNearest(lprcMonitor->left, RESOLUTION_WIDTH);
	int bottom = FindNearest(lprcMonitor->bottom, RESOLUTION_HEIGHT);
	int right = FindNearest(lprcMonitor->right, RESOLUTION_WIDTH);
	int width = abs(lprcMonitor->left - lprcMonitor->right);
	int height = abs(lprcMonitor->top - lprcMonitor->bottom);
	cout << "*Monitor " << *monitorCount;
	cout << "\tTop: " << setw(5) << top;
	cout << "\tLeft: " << setw(5) << left;
	cout << "\tBottom: " << setw(5) << bottom;
	cout << "\tRight: " << setw(5) << right;
	cout << "\tWidth: " << setw(5) << width;
	cout << "\tHeight: " << setw(5) << height;
	cout << endl;
	return TRUE;
}

int main()
{
	cout << "Original Positions:" << endl;
	cout << "------" << endl;
	int monitorCount = 0;
	if (EnumDisplayMonitors(NULL, NULL, DisplayMonitors, (LPARAM)&monitorCount))
	{

	}

	cout << endl;

	cout << "Aligned Positions:" << endl;
	cout << "------" << endl;

	monitorCount = 0;
	if (EnumDisplayMonitors(NULL, NULL, SnapMonitors, (LPARAM)&monitorCount))
	{

	}
}
