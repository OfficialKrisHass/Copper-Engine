import os
import sys

import shutil

# Copies editor files for the system and configuration, system has to be Copper-Engine repository root directory
def editor(dir, system, configuration):
    print(f"\nCopying Editor files ({configuration})\n")

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

        print("Copying Copper-ScriptingAPI")
        shutil.copy2(dir + "/Build/" + system + "-x86_64-" + configuration + "/Copper-ScriptingAPI/Copper-ScriptingAPI.dll", buildDir + "/assets/")
    elif configuration == "Debug":
        print("Copying Copper-ScriptingAPI")
        shutil.copy2(dir + "/Build/" + system + "-x86_64-" + configuration + "/Copper-ScriptingAPI/Copper-ScriptingAPI.dll", editorDir + "/assets/")

    print("Copying binaries")
    copyDir(editorDir + "/bin/mono/" + system + "/" + configuration, buildDir)
    copyDir(editorDir + "/bin/PhysX/" + system + "/" + configuration, buildDir)

# Copies launcher files for the system and configuration, system has to be Copper-Engine repository root directory
def launcher(dir, system, configuration):
    print(f"\nCopying Launcher files ({configuration})\n")

    launcherDir = dir + "/Copper-Launcher"
    buildDir = dir + "/Build/" + system + "-x86_64-" + configuration + "/Copper-Launcher"

    print(f"from: {launcherDir}")
    print(f"to: {buildDir}\n")

    if configuration == "Release":
        print("Copying assets")
        copyDir(launcherDir + "/assets", buildDir + "/assets/")

        print("Copying miscelanous files")
        shutil.copy2(launcherDir + "/imgui.ini", buildDir)


# Copies the entire directory tree from source to dest
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


# Main

if len(sys.argv) < 4:
    print("Too few arguments, at least 3 are required (target, configuration and system)")
    exit(-1)

target = sys.argv[1]
configuration = sys.argv[2]
system = sys.argv[3]

# Check validity of arguments

if target != "editor" and target != "launcher":
    print(f"Invalid target ({target}) provided! Accepted values: editor, launcher")
    exit(-1)

if configuration != "Debug" and configuration != "Release":
    print(f"Invalid configuration ({configuration}) provided! Accepted values: Debug, Release")
    exit(-1)

if system != "linux" and system != "windows":
    print(f"Invalid system ({system}) provided! Accepted values: linux, window")
    exit(-1)

# Run the correct function

dir = os.getcwd()
if not os.path.isfile(dir + "/VERSION"):
    print("You need to run this script from the root directory of the Copper-Engine github repository.")

if target == "editor":
    editor(dir, system, configuration)
elif target == "launcher":
    launcher(dir, system, configuration)
