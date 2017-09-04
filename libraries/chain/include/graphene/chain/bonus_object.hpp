/*
 * Copyright (c) 2017 Cryptonomex, Inc., and contributors.
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

#include <graphene/chain/protocol/asset.hpp>
#include <graphene/db/object.hpp>
#include <graphene/db/generic_index.hpp>


namespace graphene { namespace chain {

   class bonus_object : public abstract_object<bonus_object>
   {
      public:
         static const uint8_t space_id = protocol_ids;
         static const uint8_t type_id  = bonus_object_type;

         asset available(fc::time_point_sec now)const
         {
             return bonus;
         }

         address owner;
         asset   bonus;
         time_point_sec last_claim_date;
         asset_id_type asset_type()const { return bonus.asset_id; }
   };

   struct by_owner;

   /**
    * @ingroup object_index
    */
   using bonus_multi_index_type = multi_index_container<
      bonus_object,
      indexed_by<
         ordered_unique< tag<by_id>, member< object, object_id_type, &object::id > >,
         ordered_non_unique< tag<by_owner>, composite_key<
            bonus_object,
            member<bonus_object, address, &bonus_object::owner>,
            const_mem_fun<bonus_object, asset_id_type, &bonus_object::asset_type>
         > >
      >
   >;

   /**
    * @ingroup object_index
    */
   using bonus_index = generic_index<bonus_object, bonus_multi_index_type>;
} }

FC_REFLECT_DERIVED( graphene::chain::bonus_object, (graphene::db::object),
                    (owner)(bonus)(last_claim_date) )
