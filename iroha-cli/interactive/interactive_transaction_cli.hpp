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

#ifndef IROHA_CLI_INTERACTIVE_TRANSACTION_CLI_HPP
#define IROHA_CLI_INTERACTIVE_TRANSACTION_CLI_HPP

#include <unordered_map>
#include "interactive/interactive_common_cli.hpp"
#include "model/command.hpp"
#include "model/generators/command_generator.hpp"

namespace iroha_cli {
  namespace interactive {
    class InteractiveTransactionCli {
     public:
      /**
       *
       * @param creator_account
       * @param tx_counter
       */
      InteractiveTransactionCli(std::string creator_account,
                                uint64_t tx_counter);
      /**
       * Run interactive query command line
       */
      void run();

     private:
      /**
       * Creator account id
       */
      std::string creator_;
      /**
       *  Transaction counter specific for account creator
       */
      uint64_t tx_counter_;

      iroha::model::generators::CommandGenerator generator_;
      /**
       * Menu context used in cli
       */
      MenuContext current_context_;
      /**
       * Commands menu points
       */
      std::vector<std::string> commands_points_;
      /**
       * Transaction result points
       */
      std::vector<std::string> result_points_;

      // --- Shortcut namings for queries ---
      const std::string ADD_ASSET_QTY = "add_ast_qty";
      const std::string ADD_PEER = "add_peer";
      const std::string ADD_SIGN = "add_sign";
      const std::string ASSIGN_M_KEY = "asn_mk";
      const std::string CREATE_ACC = "crt_acc";
      const std::string CREATE_ASSET = "crt_ast";
      const std::string CREATE_DOMAIN = "crt_dmn";
      const std::string REMOVE_SIGN = "rem_sign";
      const std::string SET_PERM = "set_perm";
      const std::string SET_QUO = "set_qrm";
      const std::string SUB_ASSET_QTY = "sub_ast_qty";
      const std::string TRAN_ASSET = "tran_ast";

      /**
       * Create command menu and assign command handlers for current class
       * object
       */
      void create_command_menu();
      /**
       * Create result menu and assign result handlers for current class object
       */
      void create_result_menu();

      // Commands to be formed
      std::vector<std::shared_ptr<iroha::model::Command>> commands_;

      // ---- Command parsers ----
      using CommandHandler = std::shared_ptr<iroha::model::Command> (
          InteractiveTransactionCli::*)(std::vector<std::string>);

      std::unordered_map<std::string, CommandHandler> command_handlers_;
      std::unordered_map<std::string, std::vector<std::string>> command_params_;

      /**
       * Parse line with iroha Command
       * @param line containg iroha command
       * @return false - if parsing must be stoped, true - if parsing should
       * continue
       */
      bool parseCommand(std::string line);
      // --- Specific model Command parsers -----
      std::shared_ptr<iroha::model::Command> parseAddAssetQuantity(
          std::vector<std::string> line);
      std::shared_ptr<iroha::model::Command> parseAddPeer(
          std::vector<std::string> line);
      std::shared_ptr<iroha::model::Command> parseAddSignatory(
          std::vector<std::string> line);
      std::shared_ptr<iroha::model::Command> parseAssignMasterKey(
          std::vector<std::string> line);
      std::shared_ptr<iroha::model::Command> parseCreateAccount(
          std::vector<std::string> line);
      std::shared_ptr<iroha::model::Command> parseCreateAsset(
          std::vector<std::string> line);
      std::shared_ptr<iroha::model::Command> parseCreateDomain(
          std::vector<std::string> line);
      std::shared_ptr<iroha::model::Command> parseRemoveSignatory(
          std::vector<std::string> line);
      std::shared_ptr<iroha::model::Command> parseSetPermissions(
          std::vector<std::string> line);
      std::shared_ptr<iroha::model::Command> parseSetQuorum(
          std::vector<std::string> line);
      std::shared_ptr<iroha::model::Command> parseSubtractAssetQuantity(
          std::vector<std::string> line);
      std::shared_ptr<iroha::model::Command> parseTransferAsset(
          std::vector<std::string> line);

      // ---- Result parsers ------
      using ResultHandler =
          bool (InteractiveTransactionCli::*)(std::vector<std::string>);
      std::unordered_map<std::string, ResultHandler> result_handlers_;
      /**
       * Parse line for result
       * @param line - cli command
       * @return True - if parsing process must be continued. False if parsing
       * context should be changed
       */
      bool parseResult(std::string line);
      // --- Specific result parsers ---
      bool parseSendToIroha(std::vector<std::string> params);
      bool parseSaveFile(std::vector<std::string> params);
      bool parseGoBack(std::vector<std::string> params);
      bool parseAddCommand(std::vector<std::string> params);
    };
  }  // namespace interactive
}  // namespace iroha_cli

#endif  // IROHA_CLI_INTERACTIVE_TRANSACTION_CLI_HPP
