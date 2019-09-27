//
// Created by kiva on 2019-08-13.
//
#pragma once

#include <cstdio>

namespace cs {
    namespace compiler {
        struct CharacterData {
            virtual int getProperties(int ch) = 0;

            virtual int getType(int ch) = 0;

            virtual bool isWhitespace(int ch) = 0;

            virtual bool isMirrored(int ch) = 0;

            virtual bool isJavaIdentifierStart(int ch) = 0;

            virtual bool isJavaIdentifierPart(int ch) = 0;

            virtual bool isUnicodeIdentifierStart(int ch) = 0;

            virtual bool isUnicodeIdentifierPart(int ch) = 0;

            virtual bool isIdentifierIgnorable(int ch) = 0;

            virtual int toLowerCase(int ch) = 0;

            virtual int toUpperCase(int ch) = 0;

            virtual int toTitleCase(int ch) = 0;

            virtual int digit(int ch, int radix) = 0;

            virtual int getNumericValue(int ch) = 0;

            virtual unsigned char getDirectionality(int ch) = 0;

            virtual int toUpperCaseEx(int ch) {
                return toUpperCase(ch);
            }

            virtual bool isOtherLowercase(int ch) {
                return false;
            }

            virtual bool isOtherUppercase(int ch) {
                return false;
            }

            virtual bool isOtherAlphabetic(int ch) {
                return false;
            }

            virtual bool isIdeographic(int ch) {
                return false;
            }

            static CharacterData &of(int ch);
        };

        struct Character {
            static constexpr int ERROR = -1;
            static constexpr int MIN_RADIX = 2;
            static constexpr int MAX_RADIX = 36;
            static constexpr int DECIMAL_DIGIT_NUMBER = 9;
            static constexpr int UNASSIGNED = 0;
            static constexpr int PRIVATE_USE = 18;

            static constexpr int DIRECTIONALITY_LEFT_TO_RIGHT = 0;
            static constexpr int DIRECTIONALITY_LEFT_TO_RIGHT_EMBEDDING = 14;
            static constexpr int DIRECTIONALITY_RIGHT_TO_LEFT_EMBEDDING = 16;
            static constexpr int DIRECTIONALITY_POP_DIRECTIONAL_FORMAT = 18;
            static constexpr int DIRECTIONALITY_LEFT_TO_RIGHT_OVERRIDE = 15;
            static constexpr int DIRECTIONALITY_RIGHT_TO_LEFT_OVERRIDE = 17;
            static constexpr int DIRECTIONALITY_UNDEFINED = -1;

            static constexpr int MIN_SUPPLEMENTARY_CODE_POINT = 0x010000;
            static constexpr int MIN_HIGH_SURROGATE = 55296; // int representation of '\uD800'
            static constexpr int MIN_LOW_SURROGATE = 56320; // int representation of '\uDC00'

            static bool isJavaIdentifierPart(int codePoint) {
                return CharacterData::of(codePoint).isJavaIdentifierPart(codePoint);
            }

            static bool isJavaIdentifierStart(int codePoint) {
                return CharacterData::of(codePoint).isJavaIdentifierStart(codePoint);
            }

            static bool isUnicodeIdentifierPart(int codePoint) {
                return CharacterData::of(codePoint).isUnicodeIdentifierPart(codePoint);
            }

            static bool isUnicodeIdentifierStart(int codePoint) {
                return CharacterData::of(codePoint).isUnicodeIdentifierStart(codePoint);
            }

            static int toCodePoint(wchar_t high, wchar_t low) {
                // Optimized form of:
                // return ((high - MIN_HIGH_SURROGATE) << 10)
                //         + (low - MIN_LOW_SURROGATE)
                //         + MIN_SUPPLEMENTARY_CODE_POINT;
                return ((high << 10) + low) + (MIN_SUPPLEMENTARY_CODE_POINT
                                               - (MIN_HIGH_SURROGATE << 10)
                                               - MIN_LOW_SURROGATE);
            }
        };
    }
}
