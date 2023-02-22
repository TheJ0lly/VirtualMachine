import os
import sys

if (sys.platform == "linux"):
    os.system("rm -rf bin")
    os.system("mkdir bin")

    FilesToCompilePath = os.getcwd() + "/src"

    os.chdir(FilesToCompilePath)

    FilesInFolder = os.listdir()

    FilesToCompile = []

    for files in FilesInFolder:
        if files.find(".cpp") != -1 and files.find("WINDOWS") == -1:
            FilesToCompile.append(files)


    command = "g++ -Wall -fdiagnostics-color=always -g"

    for files in FilesToCompile:
        command += " "
        command += files

    command += " -o ../bin/VMC"

    os.system(command)

if (sys.platform == "win32"):
    os.system("rmdir /S /Q bin")
    os.system("mkdir bin")

    FilesToCompilePath = os.getcwd() + "\\src"

    os.chdir(FilesToCompilePath)

    FilesInFolder = os.listdir()

    FilesToCompile = []

    for files in FilesInFolder:
        if files.find(".cpp") != -1 and files.find("LINUX") == -1:
            FilesToCompile.append(files)


    command = "g++ -Wall -fdiagnostics-color=always -g"

    for files in FilesToCompile:
        command += " "
        command += files

    command += " -o ..\\bin\\VMC"

    os.system(command)
