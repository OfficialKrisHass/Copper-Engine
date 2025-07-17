python scripts/extract_binaries.py
echo ""

bash Copper-Engine/lib/PhysX/physx/generate_projects.sh linux-gcc

make projects
