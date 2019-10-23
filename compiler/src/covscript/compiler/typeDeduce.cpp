//
// Created by kiva on 2019/9/27.
//
#include <covscript/compiler/typeDeduce.hpp>

namespace cs {
    namespace compiler {
        constexpr Type anyType() {
            return Type(TypeFlags::TYPE_ANY);
        }

        constexpr Type unknownType() {
            return Type(TypeFlags::TYPE_UNKNOWN);
        }

        /**
         * Type promotion state machine.
         * Char, Bool -> Int -> Float -> String (maybe)
         * @param heading current inferred type
         * @param tailing candidate type
         * @param includingString Whether String type should be included
         */
        static void typeCoercionTrans(Type &heading, Type &tailing, bool includingString) {
            switch (heading.getTypeFlag()) {
                case TypeFlags::CHAR:
                case TypeFlags::BOOL:
                    if (tailing.isNumericType()) {
                        heading = tailing;
                    }
                    // no break
                case TypeFlags::INT:
                    if (tailing.isFloatType()) {
                        heading = tailing;
                    }
                    // no break
                case TypeFlags::FLOAT:
                    if (includingString && tailing.isStringType()) {
                        heading = tailing;
                    }
                    break;
            }
        }

        Type TypeDeduce::deduceExpr(Parser::ExpressionContext *expr) {
            return expr == nullptr
                   ? unknownType()
                   : deduceConditionalExpr(expr->conditionalExpression());
        }

        Type TypeDeduce::deduceConditionalExpr(Parser::ConditionalExpressionContext *expr) {
            if (expr == nullptr) {
                return unknownType();
            }

            Type condType = deduceConditionalLogicExpr(expr->conditionalLogicExpression());

            if (expr->COLON() == nullptr && expr->QUESTION() == nullptr) {
                return condType;

            } else {
                if (!condType.isBoolType()) {
                    // TODO: report semantic error
                    return unknownType();
                }
                // TODO: check condition result and deduce result type
                return anyType();
            }
        }

        Type TypeDeduce::deduceConditionalLogicExpr(Parser::ConditionalLogicExpressionContext *expr) {
            if (expr == nullptr) {
                return unknownType();
            }

            if (!expr->conditionalLogicExpressionRest().empty()) {
                return BoolType();
            }

            return deduceBitOperationExpr(expr->bitOperationExpression());
        }

        Type TypeDeduce::deduceBitOperationExpr(Parser::BitOperationExpressionContext *expr) {
            if (expr == nullptr) {
                return unknownType();
            }

            if (!expr->bitOperationExpressionRest().empty()) {
                // Only bit operations on int are allowed.
                return IntType();
            }

            return deduceRelationalExpr(expr->relationalExpression());
        }

        Type TypeDeduce::deduceRelationalExpr(Parser::RelationalExpressionContext *expr) {
            if (expr == nullptr) {
                return unknownType();
            }

            if (expr->additiveExpression().size() > 1) {
                return BoolType();
            }

            return deduceAdditiveExpr(expr->additiveExpression(0));
        }

        Type TypeDeduce::deduceAdditiveExpr(Parser::AdditiveExpressionContext *expr) {
            if (expr == nullptr) {
                return unknownType();
            }

            Type heading = deduceMultiplicativeExpr(expr->multiplicativeExpression());

            if (heading.isPrimitiveType()) {
                auto &&restExpr = expr->additiveExpressionRest();
                if (!restExpr.empty()) {
                    // Check whether type promotion on primitive types is needed
                    // eg. 1        -> IntType
                    // eg. 1 + 1    -> IntType
                    // eg. 1 + 1.0  -> FloatType
                    // eg. 1 + "s"  -> StringType
                    for (auto &rest : restExpr) {
                        // only `+` and `-` need promotion
                        if (rest->ADD() == nullptr && rest->SUB() == nullptr) {
                            continue;
                        }

                        Type tailing = deduceMultiplicativeExpr(rest->multiplicativeExpression());

                        // State transfer
                        // Bool, Char -> Int -> Float -> String
                        typeCoercionTrans(heading, tailing, true);

                        // When a type is promoted to String
                        // There's no need to check again.
                        // String is the final result.
                        if (heading.isStringType()) {
                            break;
                        }
                    }
                }
            }

            return heading;
        }

        Type TypeDeduce::deduceMultiplicativeExpr(Parser::MultiplicativeExpressionContext *expr) {
            if (expr == nullptr) {
                return unknownType();
            }

            Type heading = deduceUnaryExpr(expr->unaryExpression());

            if (heading.isPrimitiveType()) {
                auto &&restExpr = expr->multiplicativeExpressionRest();
                if (!restExpr.empty()) {
                    // Check whether type promotion on primitive types is needed
                    // eg. 1        -> IntType
                    // eg. 1 + 1    -> IntType
                    // eg. 1 + 1.0  -> FloatType
                    // eg. 1 + "s"  -> StringType
                    for (auto &rest : restExpr) {
                        Type tailing = deduceUnaryExpr(rest->unaryExpression());

                        // State transfer
                        // Bool, Char -> Int -> Float
                        typeCoercionTrans(heading, tailing, false);

                        // When a type is promoted to Float
                        // There's no need to check again.
                        // Float is the final result.
                        if (heading.isFloatType()) {
                            break;
                        }
                    }
                }
            }

            return heading;
        }

        Type TypeDeduce::deduceUnaryExpr(Parser::UnaryExpressionContext *expr) {
            if (expr == nullptr) {
                return unknownType();
            }

            if (expr->ADD() != nullptr || expr->SUB() != nullptr) {
                return NumericType(TypeFlags::NUMERIC);
            }

            if (expr->BITNOT() != nullptr
                || expr->preIncrementExpression() != nullptr
                || expr->preDecrementExpression() != nullptr
                || expr->INC() != nullptr
                || expr->DEC() != nullptr) {
                return IntType();
            }

            if (expr->BANG() != nullptr || expr->KEYWORD_NOT() != nullptr) {
                return BoolType();
            }

            auto castExpr = expr->castExpression();
            if (castExpr != nullptr) {
                return deduceCastExpr(castExpr);
            }

            auto primaryExpr = expr->primaryExpression();
            if (primaryExpr != nullptr) {
                return deducePrimaryExpr(primaryExpr);
            }

            return Type(TypeFlags::TYPE_UNKNOWN);
        }

        Type TypeDeduce::deduceCastExpr(Parser::CastExpressionContext *expr) {
            if (expr == nullptr) {
                return unknownType();
            }

            // TODO: discover user-defined types
            return anyType();
        }

        Type TypeDeduce::deducePrimaryExpr(Parser::PrimaryExpressionContext *expr) {
            if (expr == nullptr) {
                return unknownType();
            }

            Type heading = deducePrimaryPrefix(expr->primaryPrefix());

            if (!heading.isAnyType()) {
                auto &&restExpr = expr->primarySuffix();
                if (!restExpr.empty()) {
                    for (auto &suffix : restExpr) {
                        if (heading.isAnyType()) {
                            break;
                        }

                        // State transfer
                        // * -> Any
                        auto invokeExpr = suffix->invocationExpression();
                        if (invokeExpr != nullptr) {
                            // Current type is a function type
                            // TODO: obtain return type from functions
                            heading = anyType();
                            continue;
                        } else {
                            // array flatten
                            // array visit
                            // member visit
                            heading = anyType();
                            continue;
                        }
                    }
                }
            }

            return heading;
        }

        Type TypeDeduce::deducePrimaryPrefix(Parser::PrimaryPrefixContext *expr) {
            if (expr == nullptr) {
                return unknownType();
            }

            auto literalExpr = expr->literalExpression();
            if (literalExpr != nullptr) {
                return deduceLiteralExpr(literalExpr);
            }

            auto stmtExpr = expr->primaryPrefixAsStatement();
            if (stmtExpr != nullptr) {
                return deducePrimaryPrefixAsStmtExpr(stmtExpr);
            }

            auto typeIdExpr = expr->typeidExpression();
            if (typeIdExpr != nullptr) {
                return ObjectType();
            }

            return unknownType();
        }

        Type TypeDeduce::deducePrimaryPrefixAsStmtExpr(Parser::PrimaryPrefixAsStatementContext *expr) {
            if (expr == nullptr) {
                return unknownType();
            }

            auto id = expr->IDENTIFIER();
            if (id != nullptr) {
                return deduceIdentifier(id);
            }

            auto quotedExpr = expr->quotedExpression();
            if (quotedExpr != nullptr) {
                return deduceQuotedExpr(quotedExpr);
            }

            auto allocExpr = expr->allocationExpression();
            if (allocExpr != nullptr) {
                return deduceAllocationExpr(allocExpr);
            }

            auto lambdaExpr = expr->lambdaExpression();
            if (lambdaExpr != nullptr) {
                // TODO: fixme
                return LambdaType();
            }

            return unknownType();
        }

        Type TypeDeduce::deduceLiteralExpr(Parser::LiteralExpressionContext *expr) {
            if (expr == nullptr) {
                return unknownType();
            }

            if (expr->IntegerLiteral() != nullptr) {
                return IntType();
            } else if (expr->FloatingPointLiteral() != nullptr) {
                return FloatType();
            } else if (expr->StringLiteral() != nullptr) {
                return StringType();
            } else if (expr->CharacterLiteral() != nullptr) {
                return CharType();
            } else if (expr->BooleanLiteral() != nullptr) {
                return BoolType();
            } else if (expr->arrayLiteral() != nullptr) {
                return ObjectType();
            } else if (expr->mapLiteral() != nullptr) {
                return ObjectType();
            }

            // we regard null as any in case they are assigned later
            // TODO: replace with a nullType() ?
            return anyType();
        }

        Type TypeDeduce::deduceIdentifier(antlr4::tree::TerminalNode *ID) {
            if (ID == nullptr) {
                return unknownType();
            }
            // TODO: discover user-defined variables
            return anyType();
        }

        Type TypeDeduce::deduceQuotedExpr(Parser::QuotedExpressionContext *expr) {
            if (expr == nullptr) {
                return unknownType();
            }

            // Grammar definition guarantees that
            // the expr->conditionalLogicExpression() is not empty.
            // So there is no need to check again.
            return deduceConditionalLogicExpr(expr->conditionalLogicExpression().back());
        }

        Type TypeDeduce::deduceAllocationExpr(Parser::AllocationExpressionContext *expr) {
            if (expr == nullptr) {
                return unknownType();
            }
            // TODO: discover user-defined types
            return anyType();
        }
    }
}
