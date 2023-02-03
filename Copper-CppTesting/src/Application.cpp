#include <Copper.h>

#include <Engine/Core/Entry.h>
#include <Engine/Renderer/Primitives.h>

using namespace Copper;

Scene scene;

void Init() {

	scene = Scene();
	scene.Deserialize("assets/EmptyTemplate.copper");

	LoadScene(&scene);

}
void Update() {

	

}

void AppEntryPoint() {

	AddPostInitEventFunc(Init);
	AddUpdateEventFunc(Update);

}