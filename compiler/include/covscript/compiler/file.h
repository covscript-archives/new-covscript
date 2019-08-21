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
            bool isFileSource;
            std::string source;

        public:
            explicit SourceFile(std::string sourceName, bool isFileSource = true)
                : source(std::move(sourceName)), isFileSource(isFileSource) {
            }

            const std::string &getSource() const {
                return source;
            }

            bool isFile() const {
                return isFileSource;
            }
        };
    }
}
