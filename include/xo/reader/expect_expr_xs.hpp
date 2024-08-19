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

            static void start(parserstatemachine * p_psm);

            virtual void on_lambda_token(const token_type & tk,
                                         parserstatemachine * p_psm) override;

            virtual void on_leftparen_token(const token_type & tk,
                                            parserstatemachine * p_psm) override;

            virtual void on_symbol_token(const token_type & tk,
                                         parserstatemachine * p_psm) override;

            virtual void on_f64_token(const token_type & tk,
                                      parserstatemachine * p_psm) override;

            /** update exprstate in response to a successfully-parsed subexpression **/
            virtual void on_expr(ref::brw<Expression> expr,
                                 parserstatemachine * p_psm) override;

        private:
            static std::unique_ptr<expect_expr_xs> make();
        };

    } /*namespace scm*/
} /*namespace xo*/


/* end expect_expr_xs.hpp */
