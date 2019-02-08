#pragma once

#include "json.hpp"
#include "fifo_map.hpp"

/*
 * To solve issue: keep order of json objects
 * https://github.com/nlohmann/json/issues/485
 */
template<class K, class V, class dummy_compare, class A>
using my_workaround_fifo_map = nlohmann::fifo_map<K, V, nlohmann::fifo_map_compare<K>, A>;
using order_json = nlohmann::basic_json<my_workaround_fifo_map>;