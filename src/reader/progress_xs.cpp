/* @file progress_xs.cpp */

#include "progress_xs.hpp"

namespace xo {
    namespace scm {
        std::unique_ptr<progress_xs>
        progress_xs::make(rp<Expression> valex) {
            return std::make_unique<progress_xs>(progress_xs(std::move(valex)));
        }

        progress_xs::progress_xs(rp<Expression> valex)
            : exprstate(exprstatetype::expr_progress),
              gen_expr_{std::move(valex)}
        {}

        bool
        progress_xs::admits_definition() const { return false; }

        bool
        progress_xs::admits_symbol() const { return false; }

        bool
        progress_xs::admits_colon() const { return false; }

        bool
        progress_xs::admits_semicolon() const { return true; }

        bool
        progress_xs::admits_singleassign() const { return false; }

        /* todo: will parse as function call */
        bool
        progress_xs::admits_leftparen() const { return false; }

        bool
        progress_xs::admits_rightparen() const {
            /* satisfies expression form */
            return true;
        }

        bool
        progress_xs::admits_f64() const { return false; }

        void
        progress_xs::on_def_token(const token_type & tk,
                                  exprstatestack * /*p_stack*/)
        {
            constexpr const char * self_name = "progress_xs::on_def";

            /* nothing here - admits_definition unconditionally false */
            this->illegal_input_error(self_name, tk) ;
        }

        void
        progress_xs::on_expr(ref::brw<Expression> /*expr*/,
                             exprstatestack * /*p_stack*/,
                             rp<Expression> * /*p_emit_expr*/)
        {
            /* consecutive expressions isn't legal */
            assert(false);
        }

        void
        progress_xs::on_symbol_token(const token_type & /*tk*/,
                                     exprstatestack * /*p_stack*/,
                                     rp<Expression> * /*p_emit_expr*/)
        {
            /* illegal input, e.g.
             *   foo bar
             */
            assert(false);
        }

        void
        progress_xs::on_typedescr(TypeDescr /*td*/,
                                  exprstatestack * /*p_stack*/,
                                  rp<Expression> * /*p_emit_expr*/)
        {
            /* unreachable */
            assert(false);
        }

        void
        progress_xs::on_colon_token(const token_type & tk,
                                    exprstatestack * /*p_stack*/)
        {
            constexpr const char * self_name = "progress_xs::on_colon";

            this->illegal_input_error(self_name, tk);
        }

        void
        progress_xs::on_semicolon_token(const token_type & tk,
                                        exprstatestack * p_stack,
                                        rp<Expression> * p_emit_expr)
        {
            constexpr bool c_debug_flag = true;
            scope log(XO_DEBUG(c_debug_flag));

            rp<Expression> expr = this->gen_expr_;

            std::unique_ptr<exprstate> self = p_stack->pop_exprstate(); /* NOT KOSHER. invalidates *this */

            p_stack->top_exprstate().on_expr(expr,
                                             p_stack,
                                             p_emit_expr);
            /* control here on input like:
             *   (1.234;
             *
             * a. '(' sets up stack [lparen_0:expect_rhs_expression]
             *     (see exprstate::on_leftparen())
             * b. 1.234 pushes (in case operators) [lparen_0:expect_rhs_expression:expr_progress]
             *     (see exprstate::on_f64())
             * c. semicolon completes expr_progress [lparen_0:expect_rhs_expression]
             *     deliver expresssion to expect_rhs_expression.on_expr()
             *     (see exprstate::on_expr())
             * d. expr_rhs_expression forwards expression to [lparen_0]
             * e. lparen_0 advances to [lparen_1]
             * f. now deliver semicolon;  [lparen_1] rejects
             */

            p_stack->top_exprstate().on_semicolon_token(tk, p_stack, p_emit_expr);
        }

        void
        progress_xs::on_singleassign_token(const token_type & tk,
                                           exprstatestack * /*p_stack*/)
        {
            constexpr const char * self_name = "progress_xs::on_singleassign";

            this->illegal_input_error(self_name, tk);
        }

        void
        progress_xs::on_leftparen_token(const token_type & tk,
                                        exprstatestack * /*p_stack*/,
                                        rp<Expression> * /*p_emit_expr*/)
        {
            constexpr bool c_debug_flag = true;
            scope log(XO_DEBUG(c_debug_flag));

            constexpr const char * self_name = "exprstate::on_leftparen";

            this->illegal_input_error(self_name, tk);
        }

         void
         progress_xs::on_rightparen_token(const token_type & tk,
                                          exprstatestack * p_stack,
                                          rp<Expression> * p_emit_expr)
         {
             constexpr bool c_debug_flag = true;
             scope log(XO_DEBUG(c_debug_flag));

             constexpr const char * self_name = "progress_xs::on_rightparen";

             /* stack may be something like:
              *
              *   lparen_0
              *   expect_rhs_expression
              *   expr_progress
              *                   <-- rightparen
              *
              * 1. rightparen completes expression-in-progress
              * 2. rightparen must then match innermost waiting lparen_0
              */

             /* right paren confirms stack expression */
             rp<Expression> expr = this->gen_expr_;

             std::unique_ptr<exprstate> self = p_stack->pop_exprstate();

             if (p_stack->empty()) {
                 throw std::runtime_error(tostr(self_name,
                                                ": expected non-empty parsing stack"));
             }

             log && log(xtag("stack", p_stack));

             p_stack->top_exprstate().on_expr(expr, p_stack, p_emit_expr);

             /* now deliver rightparen */
             p_stack->top_exprstate().on_rightparen_token(tk, p_stack, p_emit_expr);

         }

        void
        progress_xs::on_f64_token(const token_type & tk,
                                  exprstatestack * /*p_stack*/,
                                  rp<Expression> * /*p_emit_expr*/)
        {
            constexpr bool c_debug_flag = true;
            scope log(XO_DEBUG(c_debug_flag));

            constexpr const char * self_name = "progress_xs::on_f64";

            this->illegal_input_error(self_name, tk);
        }

        void
        progress_xs::print(std::ostream & os) const {
            os << "<progress_xs"
               << xtag("type", exs_type_);
            if (gen_expr_)
                os << xtag("gen_expr", gen_expr_);
            os << ">";
        }


    } /*namespace scm*/
} /*namespace xo*/


/* end progress_xs.cpp */
