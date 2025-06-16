# 🧪 Windows Process Monitor

A lightweight C++ console application for listing and managing running processes on Windows. Built using native Windows API calls, it demonstrates how to interact with the operating system at a low level for system monitoring and control.

---

## 🔧 Features

- 🧾 Lists all active processes with PID, executable name, and memory usage
- ❌ Allows user to terminate a process by entering its PID
- 🔁 Refreshes process list interactively
- 🧠 Built entirely in C++ using the Windows API (`tlhelp32.h`, `windows.h`)
- 📝 Logs all listed processes to `process_log.txt`

---

## 🛠 Build Instructions

### ✅ Using Visual Studio
1. Open or create a **Console App** project in Visual Studio
2. Set C++ Language Standard to **C++17**
3. Paste the `ProcessMonitor.cpp` source code
4. Build the solution to generate `ProcessMonitor.exe`

### ✅ Using Command Line (MSVC)
```bash
cl ProcessMonitor.cpp /EHsc
```

### ▶ How to Use
1. Run the compiled ProcessMonitor.exe
2. View a list of currently running processes:
PID       Process Name
------------------------
1234      explorer.exe
5678      chrome.exe
3. Enter the PID of the process you want to terminate, or enter 0 to exit

### 🧠 What I've learned
* Working with the Windows Tool Help API to enumerate processes
* Handling OS-level process access and termination permissions
* Building interactive console-based applications in C++
* Understanding Windows process architecture and user access levels

