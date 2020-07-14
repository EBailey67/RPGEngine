#include "SDL.h"
#include "Managers/SystemManager.h"

int main(int argc, char* argv[])
{
	SystemManager Instance;
	Instance.Activate();

	return 0;
}
