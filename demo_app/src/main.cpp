#include <iostream>

#include "Engine.h"
#include <Logger.h>

int main() {
    Engine engine;

    LOG_INFO("Demo app started");

	engine.run();    
}