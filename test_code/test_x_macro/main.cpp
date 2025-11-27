#include <iostream>
#include <string>
#include <string_view>

#pragma region Old Enum Way
enum class StateOld {
    Connected,
    Disconnected,
    Error,
};

std::string_view getStateStringOld(const StateOld& state) {
    switch (state) {
        using enum StateOld;
        case Connected:
            return "Device Connected";
        case Disconnected:
            return "Device Disconnected";
        case Error:
            return "Connection Error";
    }
    return "Invalid State";
}

void main_old() {
    StateOld state = StateOld::Connected;
    std::cout << "State: " << getStateStringOld(state) << "\n";
}
#pragma endregion

#pragma region X-Macro Way
#define STATE_LIST  \
    X(Connected, "Device Connected")  \
    X(Disconnected, "Device Disconnected")  \
    X(Error, "Connection Error")

enum class State {
    // Step 1: Describe list above as (name, desc) pairs
    // Step 2: For each pair, get the name and replace the text here
    #define X(name, desc) name,
    STATE_LIST
    #undef X
};

std::string_view getStateString(const State& state) {
    switch (state) {
        using enum State;
        // Step 1: Describe list above as (name, desc) pairs
        // Step 2: For each pair, get the name, desc and replace the text here
        #define X(name, desc) case name: return desc;
        STATE_LIST
        #undef X
    }
    return "Invalid State";
}

void main_x_macro() {
    State state = State::Connected;
    std::cout << "State: " << getStateString(state) << "\n";
}
#pragma endregion

int main() {
    std::cout << "Test X-Macro\n\n";

    std::cout << "Running old enum way:\n";
    main_old();

    std::cout << "\nRunning X-Macro way:\n";
    main_x_macro();

    return 0;
}
