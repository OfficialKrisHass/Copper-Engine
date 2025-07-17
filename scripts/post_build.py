import os
import sys

import shutil

physxConfig = {
    "Debug": "checked",
    "Release": "release"
}

# Copies library binaries for the system and configuration, dir has to be Copper-Engine repository root directory
def libraries(dir, system, configuration):
    print(f"\nCopying library binaries ({configuration})\n")

    libDir = dir + "/Copper-Engine/lib"
    destDir = dir + "/Copper-Editor/bin"

    print(f"from: {libDir}")
    print(f"to: {destDir}\n")

    print("Preparing directories")
    if not os.path.exists(dir + "/Copper-Editor/bin/PhysX/" + system + "/" + configuration):
        os.makedirs(dir + "/Copper-Editor/bin/PhysX/" + system + "/" + configuration)
    if not os.path.exists(dir + "/Copper-Editor/bin/mono/" + system + "/" + configuration):
        os.makedirs(dir + "/Copper-Editor/bin/mono/" + system + "/" + configuration)
    if not os.path.exists(dir + "/Copper-Editor/lib/mono/" + system + "/mono/4.5"):
        os.makedirs(dir + "/Copper-Editor/lib/mono/" + system + "/mono/4.5")

    # PhysX
    print("Copying PhysX binaries")

    physxBinDir = None
    if system == "linux":
        physxBinDir = libDir + "/PhysX/physx/bin/linux.x86_64/" + physxConfig[configuration]

    physxDestDir = destDir + "/PhysX/" + system + "/" + configuration

    shutil.copy2(physxBinDir + "/libPhysX_static_64.a", physxDestDir)
    shutil.copy2(physxBinDir + "/libPhysXCommon_static_64.a", physxDestDir)
    shutil.copy2(physxBinDir + "/libPhysXExtensions_static_64.a", physxDestDir)
    shutil.copy2(physxBinDir + "/libPhysXFoundation_static_64.a", physxDestDir)
    shutil.copy2(physxBinDir + "/libPhysXPvdSDK_static_64.a", physxDestDir)

    print("Copying mono binaries")

    monoBinDir = libDir + "/mono/build/lib"
    monoDestDir = destDir + "/mono/" + system + "/" + configuration

    shutil.copy2(monoBinDir + "/libmonosgen-2.0.a", monoDestDir)

    print("Copying mono libraries")
    
    monoLibDir = libDir + "/mono/build/lib/mono/4.5"
    copyDir(monoLibDir, dir + "/Copper-Editor/lib/mono/" + system + "/mono/4.5/")


# Copies editor files for the system and configuration, dir has to be Copper-Engine repository root directory
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

# Copies launcher files for the system and configuration, dir has to be Copper-Engine repository root directory
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

if target != "libraries" and target != "editor" and target != "launcher":
    print(f"Invalid target ({target}) provided! Accepted values: libraries, editor, launcher")
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

if target == "libraries":
    libraries(dir, system, configuration)
elif target == "editor":
    editor(dir, system, configuration)
elif target == "launcher":
    launcher(dir, system, configuration)
