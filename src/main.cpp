#include <string>

#include "Window.h"

int main(int argc, char **argv)
{
    std::string title = "Physically-Based Renderer";
    int width = 640;
    int height = 480;

    PBR::Window window(title, width, height);
    window.loop();

    return 0;
}
