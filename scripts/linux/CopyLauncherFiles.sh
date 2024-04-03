#!/bin/bash

echo
echo ---- COPYING LAUNCHER FILES ----
echo

build_dir=
launcher_dir=

configuration=
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
  if [[ -f "$PWD/Copper-Launcher/CMakeLists.txt" ]]; then
    return
  elif [[ -f "$PWD/../../Copper-Launcher/CMakeLists.txt" ]]; then
    build_dir="../../"
    launcher_dir="../../"
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

build_dir="${build_dir}Build/linux-x86_64-$configuration/Copper-Launcher"
launcher_dir="${launcher_dir}Copper-Launcher"

if [[ ! -d "$build_dir" ]]; then
  mkdir -p $build_dir
fi

echo Copying Launcher assets
cp -r $launcher_dir/assets $build_dir

echo
echo Copying imgui.ini
cp $launcher_dir/imgui.ini $build_dir
