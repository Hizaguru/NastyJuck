# NastyJuck

## Prerequisites

You will need two virtual machines:

1. **Linux-based virtual machine** for the server.
2. **Windows-based virtual machine** for running the malware (the older, the better, as this malware is already detected by the latest antivirus scanners).

## Setup Instructions

### Linux Server Setup

1. Navigate to the `server` directory.
2. Compile and run the server using GCC:
   ```sh
   gcc -o server server.c
   ./server
   ```
3. Make sure to replace the IP address in the code with the correct IP address of the Linux machine.

### Windows Client Setup

1. On the Linux machine, compile the Windows executable using the following command:
   ```sh
   i686-w64-mingw32-gcc -o malware.exe main.c -lwsock32 -lwininet
   ```
2. Transfer the compiled `malware.exe` file to the Windows virtual machine.
3. Run the `malware.exe` on the Windows machine. This should open a backdoor to the system.

## Important Notes

- Ensure network connectivity between the Linux and Windows machines.
- Use this malware only for educational purposes and within a controlled environment.
- Running this malware on unauthorized systems is illegal and unethical.

## Disclaimer

This guide is for **educational purposes only**. The author is not responsible for any misuse of this information.

---

**Note:** This project is created solely for experimental purposes and out of an interest in cybersecurity. Do not use this malware for any malicious activities.
