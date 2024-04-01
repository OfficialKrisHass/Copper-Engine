#!/bin/bash

build_dir=Build/linux-x86_64-$1/Copper-Editor
editor_dir=Copper-Editor

configuration=

# If first argument is a config use it, if not, prompt the user to choose one

set_configuration() {
  
  if [[ $1 != Debug && $1 != Release ]]; then
    options=("Debug" "Release")
    
    select opt in "${options[@]}"
    do
      case $opt in
        "1) Debug")
          break
          configuration=Debug
          ;;
        "2) Release")
          break
          configuration=Release
          ;;
        *)
          echo "invalid option $REPLY"
          exit 1
          ;;
      esac
    done
  else
    configuration=$1
  fi

}
check_directory() {

  if [[ -f "$PWD/Copper-Editor/CMakeLists.txt" ]]; then
    return
  elif [[ -f "$PWD/../../Copper-Editor/CMakeLists.txt" ]]; then
    editor_dir=${editor_dir/#/"../../"}
    build_dir=${build_dir/#/"../../"}
    return
  else
    echo You are running this code from an invalid directory!
    echo You must run it from the project root directory "(contains Copper-Engine, Copper-Editor, etc source files)"
    echo Or from the script directory "(scripts/linus/)"

    exit 1
  fi

  echo $editor_dir
  echo

}

set_configuration $1
check_directory

if [[ ! -d "$build_dir" ]]; then
  mkdir -p $build_dir
fi

echo Copying Editor assets
cp -r $editor_dir/assets $build_dir

echo
echo Copying Editor utils
cp -r $editor_dir/util $build_dir

echo
echo Copying mono files
mkdir -p $build_dir/lib/mono
cp -r $editor_dir/lib/mono/lib/linux/mono $build_dir/lib
cp $editor_dir/lib/mono/bin/linux/$1/libmonosgen-2.0.a $build_dir

echo
echo Copying PhysX files
cp -r $editor_dir/lib/PhysX/linux/$1/. $build_dir

echo
echo Copying etc
cp $editor_dir/imgui.ini $build_dir
