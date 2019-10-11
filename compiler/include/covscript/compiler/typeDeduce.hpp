//
// Created by kiva on 2019/9/27.
//
#pragma once

#include <covscript/compiler/variable.hpp>
#include <covscript/compiler/parser.hpp>

namespace cs {
    namespace compiler {
        /**
         * The TypeDeduce class deduces a supposed type form an expression.
         */
        class TypeDeduce {
        public:
            static Type deduceConditionalExpr(Parser::ConditionalExpressionContext *expr);

            static Type deduceConditionalLogicExpr(Parser::ConditionalLogicExpressionContext *expr);

            static Type deduceBitOperationExpr(Parser::BitOperationExpressionContext *expr);

            static Type deduceRelationalExpr(Parser::RelationalExpressionContext *expr);

            static Type deduceAdditiveExpr(Parser::AdditiveExpressionContext *expr);

            static Type deduceMultiplicativeExpr(Parser::MultiplicativeExpressionContext *expr);

            static Type deduceUnaryExpr(Parser::UnaryExpressionContext *expr);

            static Type deduceCastExpr(Parser::CastExpressionContext *expr);

            static Type deducePrimaryExpr(Parser::PrimaryExpressionContext *expr);

            static Type deducePrimaryPrefix(Parser::PrimaryPrefixContext *expr);

            static Type deducePrimaryPrefixAsStmtExpr(Parser::PrimaryPrefixAsStatementContext *expr);

            static Type deduceLiteralExpr(Parser::LiteralExpressionContext *expr);

            static Type deduceIdentifier(antlr4::tree::TerminalNode *ID);

            static Type deduceQuotedExpr(Parser::QuotedExpressionContext *expr);

            static Type deduceAllocationExpr(Parser::AllocationExpressionContext *expr);

            static Type deduceExpr(Parser::ExpressionContext *expr);
        };
    }
}
