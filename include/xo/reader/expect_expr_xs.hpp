/* file expect_expr_xs.hpp
 *
 * author: Roland Conybeare, Aug 2024
 */

#pragma once

#include "exprstate.hpp"

namespace xo {
    namespace scm {

        /** @class expect_expr_xs
         *  @brief state machine to expect + capture an expression
         **/
        class expect_expr_xs : public exprstate {
        public:
            expect_expr_xs();

            static std::unique_ptr<expect_expr_xs> expect_rhs_expression();

            virtual void on_f64_token(const token_type & tk,
                                      exprstatestack * p_stack,
                                      rp<Expression> * p_emit_expr) override;

        private:
        };

    } /*namespace scm*/
} /*namespace xo*/


/* end expect_expr_xs.hpp */
