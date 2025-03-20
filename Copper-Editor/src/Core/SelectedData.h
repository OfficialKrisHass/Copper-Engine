#pragma once

#include "Copper.h"

namespace Editor {

    using namespace Copper;

    struct SelectedData {

        enum class Type : uint8 {

            Entity = 0,
            File,

            None,

        };

        Type type = Type::None;
        union {

            Entity entity;
            fs::path file;

        };

        SelectedData() : entity(nullptr) {}
        SelectedData(const fs::path& file) : type(Type::File) {

            CUP_FUNCTION();

            this->file = file;

        }
        ~SelectedData() {

            switch (type) {

                case Type::Entity: break;
                case Type::File: file.~path(); break;
                default: break;

            }

        }

    };

}
