/* file expect_symbol_xs.cpp
 *
 * author: Roland Conybeare
 */

#include "expect_symbol_xs.hpp"
#include "parserstatemachine.hpp"

namespace xo {
    namespace scm {
        std::unique_ptr<expect_symbol_xs>
        expect_symbol_xs::make() {
            return std::make_unique<expect_symbol_xs>(expect_symbol_xs());
        }

        void
        expect_symbol_xs::start(exprstatestack * p_stack)
        {
            p_stack->push_exprstate(expect_symbol_xs::make());
        }

        expect_symbol_xs::expect_symbol_xs()
            : exprstate(exprstatetype::expect_symbol)
        {}

        void
        expect_symbol_xs::on_symbol_token(const token_type & tk,
                                          parserstatemachine * p_psm)
        {
            auto p_stack = p_psm->p_stack_;
            auto p_emit_expr = p_psm->p_emit_expr_;

            /* have to do pop first, before sending symbol to
             * the o.g. symbol-requester
             */
            std::unique_ptr<exprstate> self = p_stack->pop_exprstate();


            p_stack->top_exprstate().on_symbol(tk.text(),
                                               p_stack, p_emit_expr);
            return;
        }
    } /*namespace scm*/
} /*namespace xo*/


/* end expect_symbol_xs.cpp */
