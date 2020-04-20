#include "model.hpp"
#include <cstdint>

void initModelsConstructors() {
    //Project constructor  std::type_index&& eventType
    LQAppModel::createModel("project", typeid(Project),
        [](LQRawData& data, const bool* attr) -> LQitem* {
            auto p_project = new Project{
                attr[0] ? data.parse<int32_t>() : -1,  // nom
                attr[3] ? data.parse<char*>() : nullptr,  // nom
                attr[4] ? data.parse<char*>() : nullptr,  // tag
                attr[5] ? data.parse<char*>() : nullptr,  // descr
                attr[6] ? data.parse<LQImageData>() : LQImageData{0, 0, 0, nullptr},  // img
                attr[7] ? data.parse<char*>() : nullptr};  // auteur}

            return p_project;
        });
}