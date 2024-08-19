/* file parserstatemachine.hpp
 *
 * author: Roland Conybeare, Aug 2024
 */

#pragma once

#include "exprstate.hpp"
#include "envframestack.hpp"

namespace xo {
    namespace scm {
        /** @class parserstatemachine
         *  @brief public parser state.
         *
         *  Schematica parser state; sent to subsidiary single-feature state machines.
         *  For example entry points for the lambda feature (@ref lambda_xs)
         *  will accept a non-const parserstatemachine pointer argument
         **/
        class parserstatemachine {
        public:
            using Expression = xo::ast::Expression;

        public:
            parserstatemachine(exprstatestack * p_stack,
                               envframestack * p_env_stack,
                               rp<Expression> * p_emit_expr)
                : p_stack_{p_stack},
                  p_env_stack_{p_env_stack},
                  p_emit_expr_{p_emit_expr} {}

            std::unique_ptr<exprstate> pop_exprstate();
            exprstate & top_exprstate();
            void push_exprstate(std::unique_ptr<exprstate> x);

        public:
            /** stack of incomplete parser work.
             *  generally speaking, push when to start new work for nested content;
             *  pop when work complete
             **/
            exprstatestack * p_stack_;
            /** stack of environment frames, one for each enclosing lambda **/
            envframestack * p_env_stack_;
            rp<Expression> * p_emit_expr_;
        };
    } /*namespace scm*/
} /*namespace xo*/


/* end parserstatemachine.hpp */
