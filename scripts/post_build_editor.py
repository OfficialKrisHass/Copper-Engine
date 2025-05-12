import os
import sys

import shutil


def copyFiles(dir, system, configuration):
    print(f"Copying Editor files ({configuration})")

    editorDir = dir + "/Copper-Editor"
    buildDir = dir + "/Build/" + system + "-x86_64-" + configuration + "/Copper-Editor"

    print(f"from: {editorDir}")
    print(f"to: {buildDir}\n")

    if configuration == "Release":
        print("Copying assets")
        copyDir(editorDir + "/assets", buildDir + "/assets/")

        print("Copying utils")
        copyDir(editorDir + "/util", buildDir + "/util/")

        print("Copying mono libraries")
        if not os.path.exists(buildDir + "/lib/mono"):
            os.mkdir(buildDir + "/lib/mono")
        os.system("cp -r " + editorDir + "/lib/mono/" + system + "/mono " + buildDir + "/lib")

        print("Copying miscelanous files")
        shutil.copy2(editorDir + "/imgui.ini", buildDir)

    print("Copying binaries")
    copyDir(editorDir + "/bin/mono/" + system + "/" + configuration, buildDir)
    copyDir(editorDir + "/bin/PhysX/" + system + "/" + configuration, buildDir)


def copyDir(source, dest):
    if not os.path.exists(dest):
        os.mkdir(dest)

    files = os.listdir(source)
    for file in files:
        path = os.path.join(source, file)

        if (os.path.isdir(path)):
            copyDir(source + "/" + file, dest + "/" + file)
        else:
            shutil.copy2(path, dest, follow_symlinks=True)


if len(sys.argv) < 3:
    print("Too few arguments, at least 2 are required (configuration and system)")
    exit(-1)

configuration = sys.argv[1]
system = sys.argv[2]

if configuration != "Debug" and configuration != "Release":
    print(f"Invalid configuration ({configuration}) provided! Accepted values: Debug, Release")
    exit(-1)

if system != "linux" and system != "windows":
    print(f"Invalid system ({system}) provided! Accepted values: linux, window")
    exit(-1)

dir = os.getcwd()
if os.path.isfile(dir + "/VERSION"):
    copyFiles(dir, system, configuration)
else:
    print("You need to run this script from the root directory of the Copper-Engine github repository.")
