#include "include/socketipc.h"
#include <stdio.h>

static WSADATA wsaData;
static SOCKET *pSocket;
static SOCKADDR_IN serverAddr;
extern HWND hWnd; // Handle to the window that will receive the messages
static void **classDefs;
static int nDefs;

void CloseSocketIPCServer()
{
    closesocket(*pSocket);
    WSACleanup();
    free(pSocket);
}

bool CreateSocketIPCServer(int nPort, void **classDefsIn, int nDefsIn)
{
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        return FALSE;
    }

    pSocket = (SOCKET *)malloc(sizeof(SOCKET));
    if (pSocket == NULL)
    {
        WSACleanup();
        return FALSE;
    }

    *pSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (*pSocket == INVALID_SOCKET)
    {
        WSACleanup();
        return FALSE;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(nPort);

    if (bind(*pSocket, (SOCKADDR *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        closesocket(*pSocket);
        WSACleanup();
        return FALSE;
    }

    if (listen(*pSocket, 1) == SOCKET_ERROR)
    {
        closesocket(*pSocket);
        WSACleanup();
        return FALSE;
    }

    // Store the class definitions
    classDefs = classDefsIn;
    nDefs = nDefsIn;

    return TRUE;
}

bool TCPServerListenLoopNonBlocking()
{
    // Set the socket to non-blocking mode
    u_long mode = 1;
    ioctlsocket(*pSocket, FIONBIO, &mode);

    // Request asynchronous notification
    if (WSAAsyncSelect(*pSocket, hWnd, WM_USER + 1, FD_ACCEPT) == SOCKET_ERROR)
    {
        return FALSE;
    }

    // The actual accept call will be handled in the window procedure
    return TRUE;
}

void ProcessCommand(SOCKET clientSocket, char *buffer, int bytesRead)
{
    // check if data is a command 001
    if (buffer[0] == 0x30 && buffer[1] == 0x30 && buffer[2] == 0x31)
    {
        // send byteBuffer
        int bytesSent = send(clientSocket, byteBuffer, byteBufferLength, 0);
        if (bytesSent == SOCKET_ERROR)
        {
            printf("Error sending response: %d\n", WSAGetLastError());
        }
        else
        {
            printf("Sent %d bytes\n", bytesSent);
        }

        // Close the client socket
        closesocket(clientSocket);
    }
    else if (buffer[0] == 0x30 && buffer[1] == 0x30 && buffer[2] == 0x32) // check if data is a command 002
    {
        // Send an error response
        char response[] = "Qutting now, Goodbye!";
        int bytesSent = send(clientSocket, response, sizeof(response), 0);
        if (bytesSent == SOCKET_ERROR)
        {
            printf("Error sending response: %d\n", WSAGetLastError());
        }
        else
        {
            printf("Sent %d bytes\n", bytesSent);
        }

        // Post the custom quit message to the window procedure
        PostMessage(hWnd, WM_QUIT, 0, 0);
    }
    else
    {
        // Send an error response
        char response[] = "Unknown command, Goodbye!";
        int bytesSent = send(clientSocket, response, sizeof(response), 0);
        if (bytesSent == SOCKET_ERROR)
        {
            printf("Error sending response: %d\n", WSAGetLastError());
        }
        else
        {
            printf("Sent %d bytes\n", bytesSent);
        }
    }
}

// Window procedure to handle the network events
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_QUIT)
    {
        // Quit the message loop
        PostQuitMessage(0);
        return 0;
    }
    if (uMsg == WM_USER + 1)
    {
        if (WSAGETSELECTEVENT(lParam) == FD_ACCEPT)
        {
            SOCKET clientSocket;
            SOCKADDR_IN clientAddr;
            int clientAddrSize = sizeof(clientAddr);

            clientSocket = accept(*pSocket, (SOCKADDR *)&clientAddr, &clientAddrSize);
            if (clientSocket == INVALID_SOCKET)
            {
                printf("Error accepting connection: %d\n", WSAGetLastError());
                closesocket(*pSocket);
            }
            else
            {
                // Request asynchronous notification for the new socket
                if (WSAAsyncSelect(clientSocket, hwnd, WM_USER + 2, FD_READ | FD_CLOSE) == SOCKET_ERROR)
                {
                    printf("Error requesting asynchronous notification\n");
                    closesocket(clientSocket);
                }

                // Use select to wait for data to become available
                fd_set readfds;
                struct timeval tv;
                int result;
                char buffer[1024];

                // Initialize the file descriptor set
                FD_ZERO(&readfds);
                FD_SET(clientSocket, &readfds);

                // Set timeout to 5 seconds
                tv.tv_sec = 5;
                tv.tv_usec = 0;

                // Wait for data to become available
                result = select(0, &readfds, NULL, NULL, &tv);
                if (result == SOCKET_ERROR)
                {
                    printf("select() failed with error: %d\n", WSAGetLastError());
                }
                else if (result == 0)
                {
                    printf("Timeout occurred! No data available to read.\n");
                }
                else
                {
                    // Data is available, read from the socket
                    int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
                    if (bytesRead == SOCKET_ERROR)
                    {
                        int error = WSAGetLastError();
                        if (error == WSAEWOULDBLOCK)
                        {
                            // Non-blocking socket and no data available
                            printf("No data available to read (WSAEWOULDBLOCK)\n");
                        }
                        else
                        {
                            // Handle other errors
                            printf("Error reading from socket: %d\n", error);
                        }
                    }
                    else if (bytesRead == 0)
                    {
                        // Connection closed by the peer
                        printf("Connection closed by the client\n");
                    }
                    else
                    {
                        // Handle the data
                        printf("Received %d bytes\n", bytesRead);
                        for (int i = 0; i < bytesRead; i++)
                        {
                            printf("%c", buffer[i]);
                        }
                        printf("\n");

                        ProcessCommand(clientSocket, buffer, bytesRead);
                    }
                }

                // Close the client socket
                closesocket(clientSocket);
            }
        }
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}