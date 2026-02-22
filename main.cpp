#include <windows.h>
#include <string>
#include <sstream>

// ---------------- ATM CLASS ----------------
class ATM {
private:
    double balance;

public:
    ATM(double initialBalance = 1000) {
        balance = initialBalance;
    }

    void deposit(double amount) {
        if (amount > 0)
            balance += amount;
    }

    bool withdraw(double amount) {
        if (amount > 0 && amount <= balance) {
            balance -= amount;
            return true;
        }
        return false;
    }

    double getBalance() {
        return balance;
    }
};

// ---------------- GLOBALS ----------------
ATM userATM(1000);
HWND hEdit;

// ---------------- WINDOW PROCEDURE ----------------
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {

    switch (msg) {

    case WM_CREATE:
        CreateWindow("STATIC", "ATM SYSTEM",
                     WS_VISIBLE | WS_CHILD,
                     140, 20, 200, 30,
                     hwnd, NULL, NULL, NULL);

        CreateWindow("STATIC", "Enter Amount:",
                     WS_VISIBLE | WS_CHILD,
                     50, 70, 120, 25,
                     hwnd, NULL, NULL, NULL);

        hEdit = CreateWindow("EDIT", "",
                             WS_VISIBLE | WS_CHILD | WS_BORDER,
                             170, 70, 150, 25,
                             hwnd, NULL, NULL, NULL);

        CreateWindow("BUTTON", "Deposit",
                     WS_VISIBLE | WS_CHILD,
                     50, 120, 100, 35,
                     hwnd, (HMENU)1, NULL, NULL);

        CreateWindow("BUTTON", "Withdraw",
                     WS_VISIBLE | WS_CHILD,
                     170, 120, 100, 35,
                     hwnd, (HMENU)2, NULL, NULL);

        CreateWindow("BUTTON", "Check Balance",
                     WS_VISIBLE | WS_CHILD,
                     110, 180, 120, 35,
                     hwnd, (HMENU)3, NULL, NULL);
        break;

    case WM_COMMAND:
        if (LOWORD(wp) == 1) { // Deposit
            char buffer[100];
            GetWindowText(hEdit, buffer, 100);
            double amount = atof(buffer);

            userATM.deposit(amount);
            MessageBox(hwnd, "Deposit Successful!", "Success", MB_OK);
        }

        if (LOWORD(wp) == 2) { // Withdraw
            char buffer[100];
            GetWindowText(hEdit, buffer, 100);
            double amount = atof(buffer);

            if (userATM.withdraw(amount))
                MessageBox(hwnd, "Withdrawal Successful!", "Success", MB_OK);
            else
                MessageBox(hwnd, "Insufficient Balance!", "Error", MB_OK | MB_ICONERROR);
        }

        if (LOWORD(wp) == 3) { // Check Balance
            std::stringstream ss;
            ss << "Current Balance: " << userATM.getBalance();
            std::string msg = ss.str();

            MessageBox(hwnd, msg.c_str(), "Balance", MB_OK);
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, msg, wp, lp);
    }

    return 0;
}

// ---------------- WINMAIN ----------------
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow) {

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProcedure;
    wc.hInstance = hInst;
    wc.lpszClassName = "ATMWindow";

    RegisterClass(&wc);

    HWND hwnd = CreateWindow(
        "ATMWindow",
        "ATM Simulation System",
        WS_OVERLAPPEDWINDOW,
        300, 200, 400, 300,
        NULL, NULL, hInst, NULL
    );

    ShowWindow(hwnd, nCmdShow);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}