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

#ifndef IROHA_CLI_INTERACTIVE_COMMON_CLI_HPP
#define IROHA_CLI_INTERACTIVE_COMMON_CLI_HPP

#include <algorithm>
#include <iostream>
#include <nonstd/optional.hpp>
#include <string>
#include <unordered_map>
#include <vector>

namespace iroha_cli {
  namespace interactive {

    /**
     * Parsing menu context, used to identify the context of parsing
     */
    enum MenuContext {
      /**
       * Main menu context, used to print all commands/queries
       */
      MAIN,
      /**
       * Result menu, used for send, save tx/query
       */
      RESULT
    };

    using ParamsDescription = std::vector<std::string>;
    using ParamsMap = std::unordered_map<std::string, ParamsDescription>;

    /**
     * Print help for cli command.
     * @param command - name of the cli command
     * @param parameters needed to run the command
     */
    void printHelp(std::string command, std::vector<std::string> parameters);
    /**
     * Pretty Print of menu
     * @param message - message to print before menu
     * @param menu_points - elements of the menu
     */
    void printMenu(std::string message, std::vector<std::string> menu_points);

    /**
     * Get string input from user
     * @param message Message to ask user
     * @return user's input
     */
    std::string promtString(std::string message);

    /**
     * Parse parameters in interactive and shortcuted mode.
     * Function run interactive mode if in line only the command name is passed.
     * Function will parse all needed parameters from line if the line with
     * commands is passed, it will print help if there are not enough parameters
     * in line.
     * @param line - cli line to parse
     * @param command_name - command name to print
     * @param notes - parameters needed to run the command
     * @return vector with needed parameters
     */
    nonstd::optional<std::vector<std::string>> parseParams(
        std::string line, std::string command_name, ParamsMap params_map);

    /**
     * Add menu point to vector menu
     * @param menu_points to add new point
     * @param description of the command to add
     * @param command_short_name command short name
     */
    void add_menu_point(std::vector<std::string>& menu_points,
                        std::string description,
                        std::string command_short_name);


    /**
     * Put into parser map
     * @tparam V
     * @param command_name
     * @param parser
     * @param parsers_map
     */
    template <typename V>
    void putParserToMap(std::string command_name, V parser,
                        std::unordered_map<std::string, V> parsers_map) {
      // Add parser map
      auto index = std::to_string(parsers_map.size() + 1);
      parsers_map[index] = parser;
      parsers_map[command_name] = parser;
    };

    /**
     * Find in unordered map
     * @tparam K key type
     * @tparam V value type
     * @param command_name - key to find in the map
     * @param params_map - map to find
     * @return nullopt if key not found, value if found
     */
    template <typename K, typename V>
    nonstd::optional<V> findInHandlerMap(K command_name,
                                         std::unordered_map<K, V> params_map) {
      auto it = params_map.find(command_name);
      if (it == params_map.end()) {
        // Command not found
        return nonstd::nullopt;
      }
      return it->second;
    }

  }  // namespace interactive
}  // namespace iroha_cli

#endif  // IROHA_CLI_INTERACTIVE_COMMON_CLI_HPP
