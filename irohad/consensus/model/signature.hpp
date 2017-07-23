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

#ifndef IROHA_CONSENSUS_SIGNATURE_HPP
#define IROHA_CONSENSUS_SIGNATURE_HPP

#include <consensus.pb.h>
#include <model/message.hpp>

namespace consensus {
  namespace model {

    class Signature final : public ::model::Message<const proto::Signature> {
     public:
      Signature(const proto::Signature *ptr);

      bool is_schema_valid() const noexcept override;

      /**
       * Returns parsed public key.
       * @return
       */
      const pubkey_t pubkey() const;

      /**
       * Returns parsed signature.
       * @return
       */
      const signature_t signature() const;

      /**
       * Returns
       * @return
       */
      const ts64_t timestamp() const noexcept;

      /**
       * Verify ed25519 signature.
       * @param msg
       * @return
       */
      bool verify(const std::vector<uint8_t> &msg) const noexcept;
      bool verify(const std::string &msg) const noexcept;
    };
  }
}
#endif  // IROHA_CONSENSUS_SIGNATURE_HPP