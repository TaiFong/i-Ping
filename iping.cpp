#include <windows.h>
#include <shlobj.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BUTTON1_ID 101
#define BUTTON2_ID 102
#define BUTTON3_ID 103
#define BUTTON4_ID 104
#define BUTTON5_ID 105
#define BUTTON6_ID 106

// Function declarations
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void OnButton1Click(HWND hwnd);
void OnButton2Click(HWND hwnd);
void OnButton3Click(HWND hwnd);
void OnButton4Click(HWND hwnd);
void OnButton5Click(HWND hwnd);
void OnButton6Click(HWND hwnd);

// Helper functions for CSV handling and Ping
int PingHost(const char *hostname);
void UpdateCSV(const char *filename, const char *hostname, const char *status);

// Main
int main() {
    // Initialize window class
    HINSTANCE hInstance = GetModuleHandle(NULL);
    const char *className = "SimpleAppClass";
    HICON hIcon = (HICON) LoadImage(NULL, "C:\\Users\\UserInfo", IMAGE_ICON, 0, 0, LR_LOADFROMFILE);

    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc; // Window procedure
    wc.hInstance = hInstance;
    wc.lpszClassName = className;
    wc.hIcon = hIcon;  // Set the icon for the window class
    RegisterClass(&wc);

    // Create window
    HWND hwnd = CreateWindowEx(
        0, className, "iPing App", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 300,
        NULL, NULL, hInstance, NULL
    );

    // Create buttons
    CreateWindow("BUTTON", "Row 1", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                 45, 50, 150, 30, hwnd, (HMENU)BUTTON1_ID, hInstance, NULL);
    CreateWindow("BUTTON", "Row 2", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                 45, 100, 150, 30, hwnd, (HMENU)BUTTON2_ID, hInstance, NULL);
    CreateWindow("BUTTON", "Row 3", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                 45, 150, 150, 30, hwnd, (HMENU)BUTTON3_ID, hInstance, NULL);
    CreateWindow("BUTTON", "Row 4", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                 205, 50, 150, 30, hwnd, (HMENU)BUTTON4_ID, hInstance, NULL);
    CreateWindow("BUTTON", "Row 5", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                 205, 100, 150, 30, hwnd, (HMENU)BUTTON5_ID, hInstance, NULL);
    CreateWindow("BUTTON", "Row 6", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                 205, 150, 150, 30, hwnd, (HMENU)BUTTON6_ID, hInstance, NULL);

    // Show window
    ShowWindow(hwnd, SW_SHOWNORMAL);
    UpdateWindow(hwnd);

    // Message loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

// Window procedure for handling messages
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case BUTTON1_ID:
                    OnButton1Click(hwnd);  // Call the button 1 function
                    break;
                case BUTTON2_ID:
                    OnButton2Click(hwnd);
                    break;
                case BUTTON3_ID:
                    OnButton3Click(hwnd);
                    break;
                case BUTTON4_ID:
                    OnButton4Click(hwnd);
                    break;
                case BUTTON5_ID:
                    OnButton5Click(hwnd);
                    break;
                case BUTTON6_ID:
                    OnButton6Click(hwnd);
                    break;
            }
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// Button 1 functionality (Ping Hosts from data1.csv and Update temp.csv immediately)
void OnButton1Click(HWND hwnd) {
    const char *filename = "../data/row1.csv";  // Change the filename to data1.csv
    char hostname[256];
    char status[256];

    // Open the file for reading
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        MessageBox(hwnd, "Unable to open row1.csv file.", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    // Open a temporary file for writing the updated data
    char tempFilename[] = "../log/row1log.csv";
    FILE *tempFile = fopen(tempFilename, "w");

    if (tempFile == NULL) {
        fclose(file);
        MessageBox(hwnd, "Unable to create row1log.csv file.", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    // Read each line from data1.csv and ping the host
    while (fscanf(file, "%255[^\n]\n", hostname) != EOF) {
        // Ping the hostname
        int result = PingHost(hostname);
        strcpy(status, (result == 0) ? "Up" : "Down");

        // Write the hostname and st+atus to temp.csv
        fprintf(tempFile, "%s,%s\n", hostname, status);
    }

    fclose(file);
    fclose(tempFile);

    MessageBox(hwnd, "Host statuses have been updated in /log/row1log.csv.", "Info", MB_OK | MB_ICONINFORMATION);
}


// Empty button handler functions
void OnButton2Click(HWND hwnd) {}
void OnButton3Click(HWND hwnd) {}
void OnButton4Click(HWND hwnd) {}
void OnButton5Click(HWND hwnd) {}
void OnButton6Click(HWND hwnd) {}

// Function to ping the host and return 0 if successful, non-zero otherwise
int PingHost(const char *hostname) {
    char command[512];
    snprintf(command, sizeof(command), "ping -n 1 %s", hostname);
    int result = system(command);
    return result;
}
