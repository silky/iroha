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

#include <gflags/gflags.h>
#include <responses.pb.h>
#include <fstream>
#include <iostream>
#include <model/converters/json_query_factory.hpp>
#include "bootstrap_network.hpp"
#include "common/assert_config.hpp"
#include "genesis_block_client_impl.hpp"
#include "model/converters/json_block_factory.hpp"
#include "model/converters/json_common.hpp"
#include "model/generators/block_generator.hpp"
#include "model/generators/signature_generator.hpp"
#include "validators.hpp"

#include "client.hpp"
#include "grpc_response_handler.hpp"
#include "impl/keys_manager_impl.hpp"
#include "logger/logger.hpp"

#include "interactive/interactive_cli.hpp"

// ** Genesis Block and Provisioning ** //
// Reference is here (TODO: move to doc):
// https://hackmd.io/GwRmwQ2BmCFoCsAGARtOAWBIBMcAcS0GcAZjhNNPvpAKZIDGQA==

DEFINE_string(config, "", "Trusted peer's ip addresses");
// DEFINE_validator(config, &iroha_cli::validate_config);

// DEFINE_string(genesis_block, "", "Genesis block for sending network");
// DEFINE_validator(genesis_block, &iroha_cli::validate_genesis_block);

DEFINE_bool(new_account, false, "Choose if account does not exist");
DEFINE_string(name, "", "Name of the account");
DEFINE_string(pass_phrase, "", "Name of the account");

// Sending transaction to Iroha
DEFINE_bool(grpc, false, "Send sample transaction to IrohaNetwork");
DEFINE_string(address, "0.0.0.0", "Address of the Iroha node");
DEFINE_int32(torii_port, 50051, "Port of iroha's Torii");
// DEFINE_validator(torii_port, &iroha_cli::validate_port);
DEFINE_string(json_transaction, "", "Transaction in json format");
DEFINE_string(json_query, "", "Query in json format");

// Genesis block generator:
DEFINE_bool(genesis_block, false,
            "Generate genesis block for new Iroha network");
DEFINE_string(peers_address, "", "File with peers address");

// Interactive
DEFINE_bool(interactive, false, "Interactive cli");

using namespace iroha::protocol;
using namespace iroha::model::generators;
using namespace iroha::model::converters;
using namespace iroha_cli::interactive;

int main(int argc, char* argv[]) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  gflags::ShutDownCommandLineFlags();
  auto logger = logger::log("CLI-MAIN");
  if (FLAGS_new_account) {
    // Create new pub/priv key
    auto keysManager = iroha_cli::KeysManagerImpl(FLAGS_name);
    if (not keysManager.createKeys(FLAGS_pass_phrase)) {
      logger->error("Keys already exist");
    } else {
      logger->info(
          "Public and private key has been generated in current directory");
    };
  } else if (FLAGS_grpc) {
    iroha_cli::CliClient client(FLAGS_address, FLAGS_torii_port);
    iroha_cli::GrpcResponseHandler response_handler;
    if (not FLAGS_json_transaction.empty()) {
      logger->info("Send transaction to {}:{} ", FLAGS_address,
                   FLAGS_torii_port);
      // Read from file
      std::ifstream file(FLAGS_json_transaction);
      std::string str((std::istreambuf_iterator<char>(file)),
                      std::istreambuf_iterator<char>());
      iroha::model::converters::JsonTransactionFactory serializer;
      auto doc = iroha::model::converters::stringToJson(str);
      if (not doc.has_value()) {
        logger->error("Json has wrong format.");
      }
      auto tx_opt = serializer.deserialize(doc.value());
      if (not tx_opt.has_value()) {
        logger->error("Json transaction has wrong format.");
      } else {
        response_handler.handle(client.sendTx(tx_opt.value()));
      }
    }
    if (not FLAGS_json_query.empty()) {
      logger->info("Send query to {}:{}", FLAGS_address, FLAGS_torii_port);
      std::ifstream file(FLAGS_json_query);
      std::string str((std::istreambuf_iterator<char>(file)),
                      std::istreambuf_iterator<char>());
      iroha::model::converters::JsonQueryFactory serializer;
      auto query_opt = serializer.deserialize(std::move(str));
      if (not query_opt.has_value()) {
        logger->error("Json has wrong format.");
      } else {
        response_handler.handle(client.sendQuery(query_opt.value()));
      }
    }

  } else if (FLAGS_genesis_block) {
    BlockGenerator generator;
    std::ifstream file(FLAGS_peers_address);
    std::vector<std::string> peers_address;
    std::copy(std::istream_iterator<std::string>(file),
              std::istream_iterator<std::string>(),
              std::back_inserter(peers_address));
    // Generate genesis block
    auto block = generator.generateGenesisBlock(peers_address);
    // Sign block with fake signature from known seed
    // TODO: generate signature depending on specific Iroha-net
    block.sigs = {generateSignature(42)};
    block.transactions.at(0).signatures = {generateSignature(42)};
    // Convert to json
    JsonBlockFactory json_factory;
    auto doc = json_factory.serialize(block);
    std::ofstream output_file("genesis.block");
    output_file << jsonToString(doc);
    logger->info("File saved to genesis.block");
  } else if (FLAGS_interactive) {
    // TODO: add login logic (e.g. password check)
    if (FLAGS_name.empty()) {
      logger->error("Specify account name");
      return -1;
    }
    // TODO: Init counters from Iroha, or read from disk ?
    InteractiveCli interactiveCli(FLAGS_name, 0, 0);
    interactiveCli.run();
  } else {
    assert_config::assert_fatal(false, "Invalid flags");
  }
  return 0;
}
