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
#include <graphene/chain/bonus_evaluator.hpp>
#include <graphene/chain/protocol/transaction.hpp>
#include <graphene/chain/exceptions.hpp>

namespace graphene { namespace chain {

void_result bonus_claim_evaluator::do_evaluate(const bonus_claim_operation& op)
{
   database& d = db();
   bonus = &op.bonus_to_claim(d);

   GRAPHENE_ASSERT(
             op.bonus_owner_key == bonus->owner ||
             pts_address(op.bonus_owner_key, false, 56) == bonus->owner ||
             pts_address(op.bonus_owner_key, true, 56) == bonus->owner ||
             pts_address(op.bonus_owner_key, false, 0) == bonus->owner ||
             pts_address(op.bonus_owner_key, true, 0) == bonus->owner,
             bonus_claim_owner_mismatch,
             "Balance owner key was specified as '${op}' but bonus's actual owner is '${bal}'",
             ("op", op.bonus_owner_key)
             ("bal", bonus->owner)
             );
   if( !(d.get_node_properties().skip_flags & (database::skip_authority_check |
                                               database::skip_transaction_signatures)) )

   FC_ASSERT(op.total_claimed.asset_id == bonus->asset_type());

   // if( bonus->is_vesting_bonus() )
   // {
   //    GRAPHENE_ASSERT(
   //       bonus->vesting_policy->is_withdraw_allowed(
   //          { bonus->bonus,
   //            d.head_block_time(),
   //            op.total_claimed } ),
   //       bonus_claim_invalid_claim_amount,
   //       "Attempted to claim ${c} from a vesting bonus with ${a} available",
   //       ("c", op.total_claimed)("a", bonus->available(d.head_block_time()))
   //       );
   //    GRAPHENE_ASSERT(
   //       d.head_block_time() - bonus->last_claim_date >= fc::days(1),
   //       bonus_claim_claimed_too_often,
   //       "Genesis vesting bonuss may not be claimed more than once per day."
   //       );
   //    return {};
   // }

   FC_ASSERT(op.total_claimed == bonus->bonus);
   return {};
}

/**
 * @note the fee is always 0 for this particular operation because once the
 * bonus is claimed it frees up memory and it cannot be used to spam the network
 */
void_result bonus_claim_evaluator::do_apply(const bonus_claim_operation& op)
{
   database& d = db();

   if( op.total_claimed < bonus->bonus )
      d.modify(*bonus, [&](bonus_object& b) {
         b.bonus -= op.total_claimed;
         b.last_claim_date = d.head_block_time();
      });
   else
      d.remove(*bonus);

   d.adjust_bonus(op.deposit_to_account, op.total_claimed);
   return {};
}
} } // namespace graphene::chain
