#!/bin/bash

echo
echo ---- COPYING EDITOR FILES ----
echo

build_dir=
editor_dir=

configuration=

# If first argument is a config use it, if not, prompt the user to choose one
set_configuration() {
  if [[ $1 != Debug && $1 != Release ]]; then
    configurations=("Debug" "Release")
      select opt in "${configurations[@]}" Exit; do
        if [[ $REPLY < 1 || $REPLY > 3 ]]; then
        echo Invalid option: $REPLY
        exit 1
      elif [[ $REPLY == 3 ]]; then
        exit 1
      fi

      configuration=$opt
      break
    done
  else
    configuration=$1
  fi
}
check_directory() {
  if [[ -f "$PWD/Copper-Editor/CMakeLists.txt" ]]; then
    return
  elif [[ -f "$PWD/../../Copper-Editor/CMakeLists.txt" ]]; then
    editor_dir="../../"
    build_dir="../../" 
    return
  else
    echo You are running this code from an invalid directory!
    echo You must run it from the project root directory "(contains Copper-Engine, Copper-Editor folders)"
    echo Or from the script directory "(scripts/linux/)"

    exit 1
  fi
}

set_configuration $1
check_directory

build_dir="${build_dir}Build/linux-x86_64-$configuration/Copper-Editor"
editor_dir="${editor_dir}Copper-Editor"

if [[ ! -d "$build_dir" ]]; then
  mkdir -p $build_dir
fi

if [ $configuration == Release ]; then
    echo Copying Editor assets
    cp -r $editor_dir/assets $build_dir

    echo
    echo Copying Editor utils
    cp -r $editor_dir/util $build_dir

    echo
    echo Copying mono files
    mkdir -p $build_dir/lib/mono
    cp -r $editor_dir/lib/mono/lib/linux/mono $build_dir/lib

    echo
    echo Copying misc
    cp $editor_dir/imgui.ini $build_dir
    echo
fi

echo Copying editor binaries
cp $editor_dir/lib/mono/bin/linux/$configuration/libmonosgen-2.0.a $build_dir
cp -r $editor_dir/lib/PhysX/linux/$configuration/. $build_dir
