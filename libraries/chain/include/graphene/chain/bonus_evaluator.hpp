/*
 * Copyright (c) 2015 Cryptonomex, Inc., and contributors.
 *
 * The MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#pragma once

#include <graphene/chain/database.hpp>
#include <graphene/chain/bonus_object.hpp>
#include <graphene/chain/evaluator.hpp>

namespace graphene { namespace chain {

class bonus_claim_evaluator : public evaluator<bonus_claim_evaluator>
{
public:
   typedef bonus_claim_operation operation_type;

   const bonus_object* bonus = nullptr;

   void_result do_evaluate(const bonus_claim_operation& op);

   /**
    * @note the fee is always 0 for this particular operation because once the
    * bonus is claimed it frees up memory and it cannot be used to spam the network
    */
   void_result do_apply(const bonus_claim_operation& op);
};

} } // graphene::chain
