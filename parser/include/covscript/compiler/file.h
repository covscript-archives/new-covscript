//
// Created by kiva on 2019-08-13.
//
#pragma once

#include <string>
#include <utility>

namespace cs {
    namespace compiler {
        class SourceFile {
        private:
            std::string sourceName;

        public:
            explicit SourceFile(std::string sourceName) : sourceName(std::move(sourceName)) {}

            const std::string &getSourceName() const {
                return sourceName;
            }
        };
    }
}
