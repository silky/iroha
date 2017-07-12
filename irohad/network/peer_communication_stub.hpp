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
#ifndef IROHA_PEER_COMMUNICATION_STUB_HPP
#define IROHA_PEER_COMMUNICATION_STUB_HPP

#include <network/network_api.h>
#include <ametsuchi/storage.hpp>
#include <consensus/consensus_service_stub.hpp>
#include <ordering/ordering_service.hpp>
#include <validation/chain/validator.hpp>
#include <validation/stateful/validator.hpp>
#include <model/model_crypto_provider.hpp>
#include <model/model_hash_provider.hpp>
#include <ordering/ordering_service_stub.hpp>

namespace iroha {
  namespace network {
    class PeerCommunicationServiceStub : public PeerCommunicationService {
     public:
      PeerCommunicationServiceStub(
          ordering::OrderingServiceStub &orderer,
          consensus::ConsensusServiceStub &consensus
      );

      rxcpp::observable<model::Proposal> on_proposal() override;

      rxcpp::observable<rxcpp::observable<model::Block>> on_commit() override;

     private:
       ordering::OrderingServiceStub orderer_;
       consensus::ConsensusServiceStub consensus_;
    };
  }
}

#endif  // IROHA_PEER_COMMUNICATION_STUB_HPP