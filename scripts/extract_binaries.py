import os
import zipfile


def extract(dir):
    print("Extracting editor binaries.")

    with zipfile.ZipFile(dir + "/Copper-Editor/bin/EditorBinaries.zip", 'r') as editorBinaries:
        editorBinaries.extractall(dir + "/Copper-Editor/bin")


dir = os.getcwd()
if os.path.isfile(dir + "/VERSION"):
    extract(dir)
else:
    print("You need to run this script from the root directory of the Copper-Engine github repository.")
