#include <ComponentFactory.h>

namespace {

} // anonymous namespace

std::unique_ptr<Component> createCompFromJson(std::string_view typeName, const nlohmann::json& jData) {
    return nullptr;
}
