#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <string>
#include <psapi.h>
#include <fstream>
#include <locale>
#include <codecvt>

// Global log file stream, append mode
std::ofstream logFile("process_monitor.log", std::ios::app);

// Function declaration for GetProcessMemoryUsage
bool GetProcessMemoryUsage(DWORD pid, PROCESS_MEMORY_COUNTERS *pmc);

// Convert std::wstring to std::string for logging
std::string WStringToString(const std::wstring& wstr) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.to_bytes(wstr);
}

// List all processes
void ListProcesses() {
    HANDLE hProcessSnap;
    PROCESSENTRY32 pe32;

    // Take a snapshot of all processes
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to take process snapshot.\n";
        return;
    }

    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(hProcessSnap, &pe32)) {
        std::cerr << "Failed to get first process.\n";
        CloseHandle(hProcessSnap);
        return;
    }

    PROCESS_MEMORY_COUNTERS pmc;

    std::cout << "PID\t\tProcess Name\t\tMemory\n";
    std::cout << "-----------------------------\n";

    // Iterate through the processes
    do {
        // Get memory usage for the process
        if (GetProcessMemoryUsage(pe32.th32ProcessID, &pmc)) {
            std::wcout << pe32.th32ProcessID << "\t\t" << pe32.szExeFile << "\t\t" << pmc.WorkingSetSize / 1024 << " KB" << "\n";
            logFile << "Listed PID: " << pe32.th32ProcessID << ", Name: " << WStringToString(pe32.szExeFile) << ", Memory: " << pmc.WorkingSetSize / 1024 << " KB\n";
        } else {
            std::wcout << pe32.th32ProcessID << "\t\t" << pe32.szExeFile << "\t\tN/A\n";
            logFile << "Listed PID: " << pe32.th32ProcessID << ", Name: " << WStringToString(pe32.szExeFile) << ", Memory: N/A\n";
        }
    } while (Process32Next(hProcessSnap, &pe32));

    CloseHandle(hProcessSnap);
}

// Kill a process by PID
bool KillProcess(DWORD pid) {
    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
    if (hProcess == NULL) {
        std::cerr << "Failed to open process with PID " << pid << ".\n";
        return false;
    }

    bool success = TerminateProcess(hProcess, 0);
    CloseHandle(hProcess);
    return success;
}

// Get memory usage of a process by PID
bool GetProcessMemoryUsage(DWORD pid, PROCESS_MEMORY_COUNTERS *pmc) {
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
    if (hProcess == NULL) {
        //std::cerr << "Failed to open process with PID " << pid << ".\n";
        return false;
    }

    if (!GetProcessMemoryInfo(hProcess, pmc, sizeof(PROCESS_MEMORY_COUNTERS))) {
        std::cerr << "Failed to get memory usage for PID " << pid << ".\n";
        CloseHandle(hProcess);
        return false;
	}
  
    CloseHandle(hProcess);
    return true;
}

int main() {
    while (true) {
        ListProcesses();

        std::cout << "\nEnter a PID to kill (or 0 to exit): ";
        DWORD pid;
        std::cin >> pid;

        if (pid == 0) break;

        if (KillProcess(pid)) {
            std::cout << "Process " << pid << " terminated successfully.\n";
            logFile << "Process " << pid << " terminated successfully.\n";
        }
        else {
            std::cerr << "Failed to terminate process " << pid << ".\n";
            logFile << "Failed to terminate process " << pid << ".\n";
        }

        std::cout << "\nPress Enter to refresh process list...\n";
        std::cin.ignore(); std::cin.get();
        system("cls"); // Clear screen on Windows
    }

	logFile.close();
    return 0;
}
