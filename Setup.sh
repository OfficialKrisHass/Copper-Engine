ROOT_DIR="$( cd "$( dirname "$0" )" && pwd )"

bash Copper-Engine/lib/PhysX/physx/generate_projects.sh linux-gcc-cpu-only

pushd Copper-Engine/lib/mono > /dev/null
bash autogen.sh --prefix=$ROOT_DIR/Copper-Engine/lib/mono/build
popd > /dev/null

make projects
