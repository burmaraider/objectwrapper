#pragma once

#include <winsock2.h>
#include <stdbool.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#define WM_CUSTOM_QUIT (WM_USER + 100)

extern char* byteBuffer;
extern int byteBufferLength;

bool CreateSocketIPCServer(int nPort, void **classDefs, int nDefs);
bool TCPServerListenLoopNonBlocking();
void CloseSocketIPCServer();