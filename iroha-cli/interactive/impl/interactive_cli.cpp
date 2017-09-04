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

#include "interactive/interactive_cli.hpp"
#include "interactive/interactive_transaction_cli.hpp"
#include "parser/parser.hpp"

namespace iroha_cli {
  namespace interactive {

    void InteractiveCli::assign_main_handlers() {
      // Add transaction menu and parser
      add_menu_point(menu_points_, "New transaction", TX_CODE);
      putParserToMap(TX_CODE, &InteractiveCli::startTx, main_handler_map_);

      // Add query menu and parser
      add_menu_point(menu_points_, "New query", QRY_CODE);
      putParserToMap(QRY_CODE,  &InteractiveCli::startQuery, main_handler_map_);
    }


    InteractiveCli::InteractiveCli(std::string account_name)
    // TODO : assign counters from Iroha Network
        : creator_(account_name), tx_cli_(creator_, 0), query_cli_(creator_, 0) {
      assign_main_handlers();
    }

    void InteractiveCli::parseMain(std::string line) {
      std::transform(line.begin(), line.end(), line.begin(), ::tolower);
      // Find in main handler map
      auto command = parser::split(line)[0];

      auto it = main_handler_map_.find(command);
      if (it != main_handler_map_.end()) {
        (this->*it->second)();
      } else {
        std::cout << "Command not found " << command << std::endl;
      }
    }

    void InteractiveCli::startQuery() {
      query_cli_.run();
    }

    void InteractiveCli::startTx() {
      tx_cli_.run();
    }

    void InteractiveCli::run() {
      std::cout << "Welcome to Iroha-Cli. " << std::endl;
      // Parsing cycle
      while (true) {
        printMenu("Choose what to do:", menu_points_);
        auto line = promtString("> ");
        parseMain(line);
      }
    }

  }  // namespace interactive
}  // namespace iroha_cli
