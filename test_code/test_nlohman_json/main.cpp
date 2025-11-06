#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

int main() {
    json j = { {"name","Quad"}, {"components", json::array()} };
    j["components"].push_back({ {"type", "Transform"}, {"position", {0,1,0}} });
    std::cout << j.dump(2) << std::endl;

    auto cbor = json::to_cbor(j);
    std::string cborStr(cbor.begin(), cbor.end());
    std::cout << "CBOR data: " << cborStr << std::endl;
    auto j2 = json::from_cbor(cbor);
    std::cout << "Loaded from CBOR: " << j2.dump(2) << std::endl;
    return 0;
}
