/**
 * Copyright Soramitsu Co., Ltd. 2017 All Rights Reserved.
 * http://soramitsu.co.jp
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "model/generators/block_generator.hpp"
#include <block.pb.h>
#include <chrono>
#include <crypto/hash.hpp>
#include "model/converters/pb_block_factory.hpp"

namespace iroha {
  namespace model {
    namespace generators {
      Block BlockGenerator::generateGenesisBlock(
          std::vector<std::string> peers_address) {
        Block block;
        block.created_ts =
            (ts64_t)std::chrono::system_clock::now().time_since_epoch().count();
        block.height = 1;
        std::fill(block.prev_hash.begin(), block.prev_hash.end(), 0);
        std::fill(block.merkle_root.begin(), block.merkle_root.end(), 0);
        block.txs_number = 1;
        TransactionGenerator tx_generator;
        block.transactions = {tx_generator.generateGenesisTransaction(
            block.created_ts, peers_address)};
        converters::PbBlockFactory blockFactory;
        auto pBlock = blockFactory.serialize(block);
        block.hash = sha3_256(pBlock.payload().SerializeAsString());

        return block;
      }

    }  // namespace generators
  }    // namespace model
}  // namespace iroha
