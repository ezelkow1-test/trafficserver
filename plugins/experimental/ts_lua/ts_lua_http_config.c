/*
  Licensed to the Apache Software Foundation (ASF) under one
  or more contributor license agreements.  See the NOTICE file
  distributed with this work for additional information
  regarding copyright ownership.  The ASF licenses this file
  to you under the Apache License, Version 2.0 (the
  "License"); you may not use this file except in compliance
  with the License.  You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
*/

#include "ts_lua_util.h"

typedef enum {
  TS_LUA_CONFIG_URL_REMAP_PRISTINE_HOST_HDR                   = TS_CONFIG_URL_REMAP_PRISTINE_HOST_HDR,
  TS_LUA_CONFIG_HTTP_CHUNKING_ENABLED                         = TS_CONFIG_HTTP_CHUNKING_ENABLED,
  TS_LUA_CONFIG_HTTP_NEGATIVE_CACHING_ENABLED                 = TS_CONFIG_HTTP_NEGATIVE_CACHING_ENABLED,
  TS_LUA_CONFIG_HTTP_NEGATIVE_CACHING_LIFETIME                = TS_CONFIG_HTTP_NEGATIVE_CACHING_LIFETIME,
  TS_LUA_CONFIG_HTTP_CACHE_WHEN_TO_REVALIDATE                 = TS_CONFIG_HTTP_CACHE_WHEN_TO_REVALIDATE,
  TS_LUA_CONFIG_HTTP_KEEP_ALIVE_ENABLED_IN                    = TS_CONFIG_HTTP_KEEP_ALIVE_ENABLED_IN,
  TS_LUA_CONFIG_HTTP_KEEP_ALIVE_ENABLED_OUT                   = TS_CONFIG_HTTP_KEEP_ALIVE_ENABLED_OUT,
  TS_LUA_CONFIG_HTTP_KEEP_ALIVE_POST_OUT                      = TS_CONFIG_HTTP_KEEP_ALIVE_POST_OUT,
  TS_LUA_CONFIG_HTTP_SERVER_SESSION_SHARING_MATCH             = TS_CONFIG_HTTP_SERVER_SESSION_SHARING_MATCH,
  TS_LUA_CONFIG_NET_SOCK_RECV_BUFFER_SIZE_OUT                 = TS_CONFIG_NET_SOCK_RECV_BUFFER_SIZE_OUT,
  TS_LUA_CONFIG_NET_SOCK_SEND_BUFFER_SIZE_OUT                 = TS_CONFIG_NET_SOCK_SEND_BUFFER_SIZE_OUT,
  TS_LUA_CONFIG_NET_SOCK_OPTION_FLAG_OUT                      = TS_CONFIG_NET_SOCK_OPTION_FLAG_OUT,
  TS_LUA_CONFIG_HTTP_FORWARD_PROXY_AUTH_TO_PARENT             = TS_CONFIG_HTTP_FORWARD_PROXY_AUTH_TO_PARENT,
  TS_LUA_CONFIG_HTTP_ANONYMIZE_REMOVE_FROM                    = TS_CONFIG_HTTP_ANONYMIZE_REMOVE_FROM,
  TS_LUA_CONFIG_HTTP_ANONYMIZE_REMOVE_REFERER                 = TS_CONFIG_HTTP_ANONYMIZE_REMOVE_REFERER,
  TS_LUA_CONFIG_HTTP_ANONYMIZE_REMOVE_USER_AGENT              = TS_CONFIG_HTTP_ANONYMIZE_REMOVE_USER_AGENT,
  TS_LUA_CONFIG_HTTP_ANONYMIZE_REMOVE_COOKIE                  = TS_CONFIG_HTTP_ANONYMIZE_REMOVE_COOKIE,
  TS_LUA_CONFIG_HTTP_ANONYMIZE_REMOVE_CLIENT_IP               = TS_CONFIG_HTTP_ANONYMIZE_REMOVE_CLIENT_IP,
  TS_LUA_CONFIG_HTTP_ANONYMIZE_INSERT_CLIENT_IP               = TS_CONFIG_HTTP_ANONYMIZE_INSERT_CLIENT_IP,
  TS_LUA_CONFIG_HTTP_RESPONSE_SERVER_ENABLED                  = TS_CONFIG_HTTP_RESPONSE_SERVER_ENABLED,
  TS_LUA_CONFIG_HTTP_INSERT_SQUID_X_FORWARDED_FOR             = TS_CONFIG_HTTP_INSERT_SQUID_X_FORWARDED_FOR,
  TS_LUA_CONFIG_HTTP_SERVER_TCP_INIT_CWND                     = TS_CONFIG_HTTP_SERVER_TCP_INIT_CWND,
  TS_LUA_CONFIG_HTTP_SEND_HTTP11_REQUESTS                     = TS_CONFIG_HTTP_SEND_HTTP11_REQUESTS,
  TS_LUA_CONFIG_HTTP_CACHE_HTTP                               = TS_CONFIG_HTTP_CACHE_HTTP,
  TS_LUA_CONFIG_HTTP_CACHE_CLUSTER_CACHE_LOCAL                = TS_CONFIG_HTTP_CACHE_CLUSTER_CACHE_LOCAL,
  TS_LUA_CONFIG_HTTP_CACHE_IGNORE_CLIENT_NO_CACHE             = TS_CONFIG_HTTP_CACHE_IGNORE_CLIENT_NO_CACHE,
  TS_LUA_CONFIG_HTTP_CACHE_IGNORE_CLIENT_CC_MAX_AGE           = TS_CONFIG_HTTP_CACHE_IGNORE_CLIENT_CC_MAX_AGE,
  TS_LUA_CONFIG_HTTP_CACHE_IMS_ON_CLIENT_NO_CACHE             = TS_CONFIG_HTTP_CACHE_IMS_ON_CLIENT_NO_CACHE,
  TS_LUA_CONFIG_HTTP_CACHE_IGNORE_SERVER_NO_CACHE             = TS_CONFIG_HTTP_CACHE_IGNORE_SERVER_NO_CACHE,
  TS_LUA_CONFIG_HTTP_CACHE_CACHE_RESPONSES_TO_COOKIES         = TS_CONFIG_HTTP_CACHE_CACHE_RESPONSES_TO_COOKIES,
  TS_LUA_CONFIG_HTTP_CACHE_IGNORE_AUTHENTICATION              = TS_CONFIG_HTTP_CACHE_IGNORE_AUTHENTICATION,
  TS_LUA_CONFIG_HTTP_CACHE_CACHE_URLS_THAT_LOOK_DYNAMIC       = TS_CONFIG_HTTP_CACHE_CACHE_URLS_THAT_LOOK_DYNAMIC,
  TS_LUA_CONFIG_HTTP_CACHE_REQUIRED_HEADERS                   = TS_CONFIG_HTTP_CACHE_REQUIRED_HEADERS,
  TS_LUA_CONFIG_HTTP_INSERT_REQUEST_VIA_STR                   = TS_CONFIG_HTTP_INSERT_REQUEST_VIA_STR,
  TS_LUA_CONFIG_HTTP_INSERT_RESPONSE_VIA_STR                  = TS_CONFIG_HTTP_INSERT_RESPONSE_VIA_STR,
  TS_LUA_CONFIG_HTTP_CACHE_HEURISTIC_MIN_LIFETIME             = TS_CONFIG_HTTP_CACHE_HEURISTIC_MIN_LIFETIME,
  TS_LUA_CONFIG_HTTP_CACHE_HEURISTIC_MAX_LIFETIME             = TS_CONFIG_HTTP_CACHE_HEURISTIC_MAX_LIFETIME,
  TS_LUA_CONFIG_HTTP_CACHE_GUARANTEED_MIN_LIFETIME            = TS_CONFIG_HTTP_CACHE_GUARANTEED_MIN_LIFETIME,
  TS_LUA_CONFIG_HTTP_CACHE_GUARANTEED_MAX_LIFETIME            = TS_CONFIG_HTTP_CACHE_GUARANTEED_MAX_LIFETIME,
  TS_LUA_CONFIG_HTTP_CACHE_MAX_STALE_AGE                      = TS_CONFIG_HTTP_CACHE_MAX_STALE_AGE,
  TS_LUA_CONFIG_HTTP_KEEP_ALIVE_NO_ACTIVITY_TIMEOUT_IN        = TS_CONFIG_HTTP_KEEP_ALIVE_NO_ACTIVITY_TIMEOUT_IN,
  TS_LUA_CONFIG_HTTP_KEEP_ALIVE_NO_ACTIVITY_TIMEOUT_OUT       = TS_CONFIG_HTTP_KEEP_ALIVE_NO_ACTIVITY_TIMEOUT_OUT,
  TS_LUA_CONFIG_HTTP_TRANSACTION_NO_ACTIVITY_TIMEOUT_IN       = TS_CONFIG_HTTP_TRANSACTION_NO_ACTIVITY_TIMEOUT_IN,
  TS_LUA_CONFIG_HTTP_TRANSACTION_NO_ACTIVITY_TIMEOUT_OUT      = TS_CONFIG_HTTP_TRANSACTION_NO_ACTIVITY_TIMEOUT_OUT,
  TS_LUA_CONFIG_HTTP_TRANSACTION_ACTIVE_TIMEOUT_OUT           = TS_CONFIG_HTTP_TRANSACTION_ACTIVE_TIMEOUT_OUT,
  TS_LUA_CONFIG_HTTP_ORIGIN_MAX_CONNECTIONS                   = TS_CONFIG_HTTP_ORIGIN_MAX_CONNECTIONS,
  TS_LUA_CONFIG_HTTP_CONNECT_ATTEMPTS_MAX_RETRIES             = TS_CONFIG_HTTP_CONNECT_ATTEMPTS_MAX_RETRIES,
  TS_LUA_CONFIG_HTTP_CONNECT_ATTEMPTS_MAX_RETRIES_DEAD_SERVER = TS_CONFIG_HTTP_CONNECT_ATTEMPTS_MAX_RETRIES_DEAD_SERVER,
  TS_LUA_CONFIG_HTTP_CONNECT_ATTEMPTS_RR_RETRIES              = TS_CONFIG_HTTP_CONNECT_ATTEMPTS_RR_RETRIES,
  TS_LUA_CONFIG_HTTP_CONNECT_ATTEMPTS_TIMEOUT                 = TS_CONFIG_HTTP_CONNECT_ATTEMPTS_TIMEOUT,
  TS_LUA_CONFIG_HTTP_POST_CONNECT_ATTEMPTS_TIMEOUT            = TS_CONFIG_HTTP_POST_CONNECT_ATTEMPTS_TIMEOUT,
  TS_LUA_CONFIG_HTTP_DOWN_SERVER_CACHE_TIME                   = TS_CONFIG_HTTP_DOWN_SERVER_CACHE_TIME,
  TS_LUA_CONFIG_HTTP_DOWN_SERVER_ABORT_THRESHOLD              = TS_CONFIG_HTTP_DOWN_SERVER_ABORT_THRESHOLD,
  TS_LUA_CONFIG_HTTP_CACHE_FUZZ_TIME                          = TS_CONFIG_HTTP_CACHE_FUZZ_TIME,
  TS_LUA_CONFIG_HTTP_CACHE_FUZZ_MIN_TIME                      = TS_CONFIG_HTTP_CACHE_FUZZ_MIN_TIME,
  TS_LUA_CONFIG_HTTP_DOC_IN_CACHE_SKIP_DNS                    = TS_CONFIG_HTTP_DOC_IN_CACHE_SKIP_DNS,
  TS_LUA_CONFIG_HTTP_BACKGROUND_FILL_ACTIVE_TIMEOUT           = TS_CONFIG_HTTP_BACKGROUND_FILL_ACTIVE_TIMEOUT,
  TS_LUA_CONFIG_HTTP_RESPONSE_SERVER_STR                      = TS_CONFIG_HTTP_RESPONSE_SERVER_STR,
  TS_LUA_CONFIG_HTTP_CACHE_HEURISTIC_LM_FACTOR                = TS_CONFIG_HTTP_CACHE_HEURISTIC_LM_FACTOR,
  TS_LUA_CONFIG_HTTP_CACHE_FUZZ_PROBABILITY                   = TS_CONFIG_HTTP_CACHE_FUZZ_PROBABILITY,
  TS_LUA_CONFIG_HTTP_BACKGROUND_FILL_COMPLETED_THRESHOLD      = TS_CONFIG_HTTP_BACKGROUND_FILL_COMPLETED_THRESHOLD,
  TS_LUA_CONFIG_NET_SOCK_PACKET_MARK_OUT                      = TS_CONFIG_NET_SOCK_PACKET_MARK_OUT,
  TS_LUA_CONFIG_NET_SOCK_PACKET_TOS_OUT                       = TS_CONFIG_NET_SOCK_PACKET_TOS_OUT,
  TS_LUA_CONFIG_HTTP_INSERT_AGE_IN_RESPONSE                   = TS_CONFIG_HTTP_INSERT_AGE_IN_RESPONSE,
  TS_LUA_CONFIG_HTTP_CHUNKING_SIZE                            = TS_CONFIG_HTTP_CHUNKING_SIZE,
  TS_LUA_CONFIG_HTTP_FLOW_CONTROL_ENABLED                     = TS_CONFIG_HTTP_FLOW_CONTROL_ENABLED,
  TS_LUA_CONFIG_HTTP_FLOW_CONTROL_LOW_WATER_MARK              = TS_CONFIG_HTTP_FLOW_CONTROL_LOW_WATER_MARK,
  TS_LUA_CONFIG_HTTP_FLOW_CONTROL_HIGH_WATER_MARK             = TS_CONFIG_HTTP_FLOW_CONTROL_HIGH_WATER_MARK,
  TS_LUA_CONFIG_HTTP_CACHE_RANGE_LOOKUP                       = TS_CONFIG_HTTP_CACHE_RANGE_LOOKUP,
  TS_LUA_CONFIG_HTTP_NORMALIZE_AE_GZIP                        = TS_CONFIG_HTTP_NORMALIZE_AE_GZIP,
  TS_LUA_CONFIG_HTTP_DEFAULT_BUFFER_SIZE                      = TS_CONFIG_HTTP_DEFAULT_BUFFER_SIZE,
  TS_LUA_CONFIG_HTTP_DEFAULT_BUFFER_WATER_MARK                = TS_CONFIG_HTTP_DEFAULT_BUFFER_WATER_MARK,
  TS_LUA_CONFIG_HTTP_REQUEST_HEADER_MAX_SIZE                  = TS_CONFIG_HTTP_REQUEST_HEADER_MAX_SIZE,
  TS_LUA_CONFIG_HTTP_RESPONSE_HEADER_MAX_SIZE                 = TS_CONFIG_HTTP_RESPONSE_HEADER_MAX_SIZE,
  TS_LUA_CONFIG_HTTP_NEGATIVE_REVALIDATING_ENABLED            = TS_CONFIG_HTTP_NEGATIVE_REVALIDATING_ENABLED,
  TS_LUA_CONFIG_HTTP_NEGATIVE_REVALIDATING_LIFETIME           = TS_CONFIG_HTTP_NEGATIVE_REVALIDATING_LIFETIME,
  TS_LUA_CONFIG_SSL_HSTS_MAX_AGE                              = TS_CONFIG_SSL_HSTS_MAX_AGE,
  TS_LUA_CONFIG_SSL_HSTS_INCLUDE_SUBDOMAINS                   = TS_CONFIG_SSL_HSTS_INCLUDE_SUBDOMAINS,
  TS_LUA_CONFIG_HTTP_CACHE_OPEN_READ_RETRY_TIME               = TS_CONFIG_HTTP_CACHE_OPEN_READ_RETRY_TIME,
  TS_LUA_CONFIG_HTTP_CACHE_MAX_OPEN_READ_RETRIES              = TS_CONFIG_HTTP_CACHE_MAX_OPEN_READ_RETRIES,
  TS_LUA_CONFIG_HTTP_CACHE_RANGE_WRITE                        = TS_CONFIG_HTTP_CACHE_RANGE_WRITE,
  TS_LUA_CONFIG_HTTP_POST_CHECK_CONTENT_LENGTH_ENABLED        = TS_CONFIG_HTTP_POST_CHECK_CONTENT_LENGTH_ENABLED,
  TS_LUA_CONFIG_HTTP_GLOBAL_USER_AGENT_HEADER                 = TS_CONFIG_HTTP_GLOBAL_USER_AGENT_HEADER,
  TS_LUA_CONFIG_HTTP_AUTH_SERVER_SESSION_PRIVATE              = TS_CONFIG_HTTP_AUTH_SERVER_SESSION_PRIVATE,
  TS_LUA_CONFIG_HTTP_SLOW_LOG_THRESHOLD                       = TS_CONFIG_HTTP_SLOW_LOG_THRESHOLD,
  TS_LUA_CONFIG_HTTP_CACHE_GENERATION                         = TS_CONFIG_HTTP_CACHE_GENERATION,
  TS_LUA_CONFIG_BODY_FACTORY_TEMPLATE_BASE                    = TS_CONFIG_BODY_FACTORY_TEMPLATE_BASE,
  TS_LUA_CONFIG_HTTP_CACHE_OPEN_WRITE_FAIL_ACTION             = TS_CONFIG_HTTP_CACHE_OPEN_WRITE_FAIL_ACTION,
  TS_LUA_CONFIG_HTTP_ENABLE_REDIRECTION                       = TS_CONFIG_HTTP_ENABLE_REDIRECTION,
  TS_LUA_CONFIG_HTTP_NUMBER_OF_REDIRECTIONS                   = TS_CONFIG_HTTP_NUMBER_OF_REDIRECTIONS,
  TS_LUA_CONFIG_HTTP_CACHE_MAX_OPEN_WRITE_RETRIES             = TS_CONFIG_HTTP_CACHE_MAX_OPEN_WRITE_RETRIES,
  TS_LUA_CONFIG_HTTP_REDIRECT_USE_ORIG_CACHE_KEY              = TS_CONFIG_HTTP_REDIRECT_USE_ORIG_CACHE_KEY,
  TS_LUA_CONFIG_HTTP_ATTACH_SERVER_SESSION_TO_CLIENT          = TS_CONFIG_HTTP_ATTACH_SERVER_SESSION_TO_CLIENT,
  TS_LUA_CONFIG_HTTP_SAFE_REQUESTS_RETRYABLE                  = TS_CONFIG_HTTP_SAFE_REQUESTS_RETRYABLE,
  TS_LUA_CONFIG_HTTP_ORIGIN_MAX_CONNECTIONS_QUEUE             = TS_CONFIG_HTTP_ORIGIN_MAX_CONNECTIONS_QUEUE,
  TS_LUA_CONFIG_WEBSOCKET_NO_ACTIVITY_TIMEOUT                 = TS_CONFIG_WEBSOCKET_NO_ACTIVITY_TIMEOUT,
  TS_LUA_CONFIG_WEBSOCKET_ACTIVE_TIMEOUT                      = TS_CONFIG_WEBSOCKET_ACTIVE_TIMEOUT,
  TS_LUA_CONFIG_HTTP_UNCACHEABLE_REQUESTS_BYPASS_PARENT       = TS_CONFIG_HTTP_UNCACHEABLE_REQUESTS_BYPASS_PARENT,
  TS_LUA_CONFIG_HTTP_PARENT_PROXY_TOTAL_CONNECT_ATTEMPTS      = TS_CONFIG_HTTP_PARENT_PROXY_TOTAL_CONNECT_ATTEMPTS,
  TS_LUA_CONFIG_HTTP_TRANSACTION_ACTIVE_TIMEOUT_IN            = TS_CONFIG_HTTP_TRANSACTION_ACTIVE_TIMEOUT_IN,
  TS_LUA_CONFIG_SRV_ENABLED                                   = TS_CONFIG_SRV_ENABLED,
  TS_LUA_CONFIG_HTTP_FORWARD_CONNECT_METHOD                   = TS_CONFIG_HTTP_FORWARD_CONNECT_METHOD,
  TS_LUA_CONFIG_SSL_CERT_FILENAME                             = TS_CONFIG_SSL_CERT_FILENAME,
  TS_LUA_CONFIG_SSL_CERT_FILEPATH                             = TS_CONFIG_SSL_CERT_FILEPATH,
  TS_LUA_CONFIG_PARENT_FAILURES_UPDATE_HOSTDB                 = TS_CONFIG_PARENT_FAILURES_UPDATE_HOSTDB,
  TS_LUA_CONFIG_HTTP_PARENT_PROXY_FAIL_THRESHOLD              = TS_CONFIG_HTTP_PARENT_PROXY_FAIL_THRESHOLD,
  TS_LUA_CONFIG_HTTP_PARENT_PROXY_RETRY_TIME                  = TS_CONFIG_HTTP_PARENT_PROXY_RETRY_TIME,
  TS_LUA_CONFIG_HTTP_PER_PARENT_CONNECT_ATTEMPTS              = TS_CONFIG_HTTP_PER_PARENT_CONNECT_ATTEMPTS,
  TS_LUA_CONFIG_HTTP_PARENT_CONNECT_ATTEMPT_TIMEOUT           = TS_CONFIG_HTTP_PARENT_CONNECT_ATTEMPT_TIMEOUT,
  TS_LUA_CONFIG_SSL_CLIENT_VERIFY_SERVER                      = TS_CONFIG_SSL_CLIENT_VERIFY_SERVER,
  TS_LUA_CONFIG_HTTP_ALLOW_MULTI_RANGE                        = TS_CONFIG_HTTP_ALLOW_MULTI_RANGE,
  TS_LUA_CONFIG_HTTP_CACHE_IGNORE_ACCEPT_ENCODING_MISMATCH    = TS_CONFIG_HTTP_CACHE_IGNORE_ACCEPT_ENCODING_MISMATCH,
  TS_LUA_CONFIG_LAST_ENTRY                                    = TS_CONFIG_LAST_ENTRY,
} TSLuaOverridableConfigKey;

typedef enum {
  TS_LUA_TIMEOUT_ACTIVE      = 0,
  TS_LUA_TIMEOUT_CONNECT     = 1,
  TS_LUA_TIMEOUT_DNS         = 2,
  TS_LUA_TIMEOUT_NO_ACTIVITY = 3
} TSLuaTimeoutKey;

ts_lua_var_item ts_lua_http_config_vars[] = {
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_URL_REMAP_PRISTINE_HOST_HDR),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_CHUNKING_ENABLED),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_NEGATIVE_CACHING_ENABLED),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_NEGATIVE_CACHING_LIFETIME),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_CACHE_WHEN_TO_REVALIDATE),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_KEEP_ALIVE_ENABLED_IN),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_KEEP_ALIVE_ENABLED_OUT),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_KEEP_ALIVE_POST_OUT),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_SERVER_SESSION_SHARING_MATCH),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_NET_SOCK_RECV_BUFFER_SIZE_OUT),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_NET_SOCK_SEND_BUFFER_SIZE_OUT),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_NET_SOCK_OPTION_FLAG_OUT),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_FORWARD_PROXY_AUTH_TO_PARENT),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_ANONYMIZE_REMOVE_FROM),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_ANONYMIZE_REMOVE_REFERER),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_ANONYMIZE_REMOVE_USER_AGENT),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_ANONYMIZE_REMOVE_COOKIE),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_ANONYMIZE_REMOVE_CLIENT_IP),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_ANONYMIZE_INSERT_CLIENT_IP),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_RESPONSE_SERVER_ENABLED),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_INSERT_SQUID_X_FORWARDED_FOR),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_SERVER_TCP_INIT_CWND),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_SEND_HTTP11_REQUESTS),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_CACHE_HTTP),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_CACHE_CLUSTER_CACHE_LOCAL),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_CACHE_IGNORE_CLIENT_NO_CACHE),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_CACHE_IGNORE_CLIENT_CC_MAX_AGE),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_CACHE_IMS_ON_CLIENT_NO_CACHE),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_CACHE_IGNORE_SERVER_NO_CACHE),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_CACHE_CACHE_RESPONSES_TO_COOKIES),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_CACHE_IGNORE_AUTHENTICATION),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_CACHE_CACHE_URLS_THAT_LOOK_DYNAMIC),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_CACHE_REQUIRED_HEADERS),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_INSERT_REQUEST_VIA_STR),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_INSERT_RESPONSE_VIA_STR),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_CACHE_HEURISTIC_MIN_LIFETIME),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_CACHE_HEURISTIC_MAX_LIFETIME),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_CACHE_GUARANTEED_MIN_LIFETIME),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_CACHE_GUARANTEED_MAX_LIFETIME),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_CACHE_MAX_STALE_AGE),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_KEEP_ALIVE_NO_ACTIVITY_TIMEOUT_IN),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_KEEP_ALIVE_NO_ACTIVITY_TIMEOUT_OUT),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_TRANSACTION_NO_ACTIVITY_TIMEOUT_IN),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_TRANSACTION_NO_ACTIVITY_TIMEOUT_OUT),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_TRANSACTION_ACTIVE_TIMEOUT_OUT),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_ORIGIN_MAX_CONNECTIONS),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_CONNECT_ATTEMPTS_MAX_RETRIES),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_CONNECT_ATTEMPTS_MAX_RETRIES_DEAD_SERVER),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_CONNECT_ATTEMPTS_RR_RETRIES),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_CONNECT_ATTEMPTS_TIMEOUT),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_POST_CONNECT_ATTEMPTS_TIMEOUT),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_DOWN_SERVER_CACHE_TIME),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_DOWN_SERVER_ABORT_THRESHOLD),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_CACHE_FUZZ_TIME),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_CACHE_FUZZ_MIN_TIME),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_DOC_IN_CACHE_SKIP_DNS),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_BACKGROUND_FILL_ACTIVE_TIMEOUT),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_RESPONSE_SERVER_STR),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_CACHE_HEURISTIC_LM_FACTOR),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_CACHE_FUZZ_PROBABILITY),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_BACKGROUND_FILL_COMPLETED_THRESHOLD),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_NET_SOCK_PACKET_MARK_OUT),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_NET_SOCK_PACKET_TOS_OUT),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_INSERT_AGE_IN_RESPONSE),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_CHUNKING_SIZE),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_FLOW_CONTROL_ENABLED),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_FLOW_CONTROL_LOW_WATER_MARK),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_FLOW_CONTROL_HIGH_WATER_MARK),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_CACHE_RANGE_LOOKUP),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_NORMALIZE_AE_GZIP),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_DEFAULT_BUFFER_SIZE),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_DEFAULT_BUFFER_WATER_MARK),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_REQUEST_HEADER_MAX_SIZE),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_RESPONSE_HEADER_MAX_SIZE),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_NEGATIVE_REVALIDATING_ENABLED),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_NEGATIVE_REVALIDATING_LIFETIME),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_SSL_HSTS_MAX_AGE),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_SSL_HSTS_INCLUDE_SUBDOMAINS),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_CACHE_OPEN_READ_RETRY_TIME),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_CACHE_MAX_OPEN_READ_RETRIES),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_CACHE_RANGE_WRITE),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_POST_CHECK_CONTENT_LENGTH_ENABLED),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_GLOBAL_USER_AGENT_HEADER),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_AUTH_SERVER_SESSION_PRIVATE),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_SLOW_LOG_THRESHOLD),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_CACHE_GENERATION),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_BODY_FACTORY_TEMPLATE_BASE),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_CACHE_OPEN_WRITE_FAIL_ACTION),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_ENABLE_REDIRECTION),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_NUMBER_OF_REDIRECTIONS),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_CACHE_MAX_OPEN_WRITE_RETRIES),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_REDIRECT_USE_ORIG_CACHE_KEY),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_ATTACH_SERVER_SESSION_TO_CLIENT),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_SAFE_REQUESTS_RETRYABLE),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_ORIGIN_MAX_CONNECTIONS_QUEUE),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_WEBSOCKET_NO_ACTIVITY_TIMEOUT),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_WEBSOCKET_ACTIVE_TIMEOUT),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_UNCACHEABLE_REQUESTS_BYPASS_PARENT),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_PARENT_PROXY_TOTAL_CONNECT_ATTEMPTS),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_TRANSACTION_ACTIVE_TIMEOUT_IN),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_SRV_ENABLED),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_FORWARD_CONNECT_METHOD),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_SSL_CERT_FILENAME),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_SSL_CERT_FILEPATH),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_PARENT_FAILURES_UPDATE_HOSTDB),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_PARENT_PROXY_FAIL_THRESHOLD),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_PARENT_PROXY_RETRY_TIME),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_PER_PARENT_CONNECT_ATTEMPTS),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_PARENT_CONNECT_ATTEMPT_TIMEOUT),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_SSL_CLIENT_VERIFY_SERVER),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_HTTP_ALLOW_MULTI_RANGE),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_CONFIG_LAST_ENTRY),
};

// Needed to make sure we have the latest list of overridable http config vars when compiling
#define NUM_HTTP_CONFIG_VARS (sizeof(ts_lua_http_config_vars) / sizeof(ts_lua_http_config_vars[0]))
extern char __ts_lua_http_config_vars_static_assert[NUM_HTTP_CONFIG_VARS == TS_CONFIG_LAST_ENTRY + 1 ? 0 : -1];

ts_lua_var_item ts_lua_http_timeout_vars[] = {
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_TIMEOUT_ACTIVE), TS_LUA_MAKE_VAR_ITEM(TS_LUA_TIMEOUT_CONNECT),
  TS_LUA_MAKE_VAR_ITEM(TS_LUA_TIMEOUT_DNS), TS_LUA_MAKE_VAR_ITEM(TS_LUA_TIMEOUT_NO_ACTIVITY),
};

static void ts_lua_inject_http_config_variables(lua_State *L);

static int ts_lua_http_config_int_set(lua_State *L);
static int ts_lua_http_config_int_get(lua_State *L);
static int ts_lua_http_config_float_set(lua_State *L);
static int ts_lua_http_config_float_get(lua_State *L);
static int ts_lua_http_config_string_set(lua_State *L);
static int ts_lua_http_config_string_get(lua_State *L);
static int ts_lua_http_timeout_set(lua_State *L);
static int ts_lua_http_client_packet_mark_set(lua_State *L);
static int ts_lua_http_server_packet_mark_set(lua_State *L);
static int ts_lua_http_client_packet_tos_set(lua_State *L);
static int ts_lua_http_server_packet_tos_set(lua_State *L);
static int ts_lua_http_client_packet_dscp_set(lua_State *L);
static int ts_lua_http_server_packet_dscp_set(lua_State *L);
static int ts_lua_http_enable_redirect(lua_State *L);
static int ts_lua_http_set_debug(lua_State *L);

void
ts_lua_inject_http_config_api(lua_State *L)
{
  ts_lua_inject_http_config_variables(L);

  lua_pushcfunction(L, ts_lua_http_config_int_set);
  lua_setfield(L, -2, "config_int_set");

  lua_pushcfunction(L, ts_lua_http_config_int_get);
  lua_setfield(L, -2, "config_int_get");

  lua_pushcfunction(L, ts_lua_http_config_float_set);
  lua_setfield(L, -2, "config_float_set");

  lua_pushcfunction(L, ts_lua_http_config_float_get);
  lua_setfield(L, -2, "config_float_get");

  lua_pushcfunction(L, ts_lua_http_config_string_set);
  lua_setfield(L, -2, "config_string_set");

  lua_pushcfunction(L, ts_lua_http_config_string_get);
  lua_setfield(L, -2, "config_string_get");

  lua_pushcfunction(L, ts_lua_http_timeout_set);
  lua_setfield(L, -2, "timeout_set");

  lua_pushcfunction(L, ts_lua_http_client_packet_mark_set);
  lua_setfield(L, -2, "client_packet_mark_set");

  lua_pushcfunction(L, ts_lua_http_server_packet_mark_set);
  lua_setfield(L, -2, "server_packet_mark_set");

  lua_pushcfunction(L, ts_lua_http_client_packet_tos_set);
  lua_setfield(L, -2, "client_packet_tos_set");

  lua_pushcfunction(L, ts_lua_http_server_packet_tos_set);
  lua_setfield(L, -2, "server_packet_tos_set");

  lua_pushcfunction(L, ts_lua_http_client_packet_dscp_set);
  lua_setfield(L, -2, "client_packet_dscp_set");

  lua_pushcfunction(L, ts_lua_http_server_packet_dscp_set);
  lua_setfield(L, -2, "server_packet_dscp_set");

  lua_pushcfunction(L, ts_lua_http_enable_redirect);
  lua_setfield(L, -2, "enable_redirect");

  lua_pushcfunction(L, ts_lua_http_set_debug);
  lua_setfield(L, -2, "set_debug");
}

static void
ts_lua_inject_http_config_variables(lua_State *L)
{
  size_t i;

  for (i = 0; i < sizeof(ts_lua_http_config_vars) / sizeof(ts_lua_var_item); i++) {
    lua_pushinteger(L, ts_lua_http_config_vars[i].nvar);
    lua_setglobal(L, ts_lua_http_config_vars[i].svar);
  }

  for (i = 0; i < sizeof(ts_lua_http_timeout_vars) / sizeof(ts_lua_var_item); i++) {
    lua_pushinteger(L, ts_lua_http_timeout_vars[i].nvar);
    lua_setglobal(L, ts_lua_http_timeout_vars[i].svar);
  }
}

static int
ts_lua_http_config_int_set(lua_State *L)
{
  int conf;
  int value;
  ts_lua_http_ctx *http_ctx;

  GET_HTTP_CONTEXT(http_ctx, L);

  conf  = luaL_checkinteger(L, 1);
  value = luaL_checkinteger(L, 2);

  TSHttpTxnConfigIntSet(http_ctx->txnp, conf, value);

  return 0;
}

static int
ts_lua_http_config_int_get(lua_State *L)
{
  int conf;
  int64_t value;
  ts_lua_http_ctx *http_ctx;

  GET_HTTP_CONTEXT(http_ctx, L);

  conf = luaL_checkinteger(L, 1);

  TSHttpTxnConfigIntGet(http_ctx->txnp, conf, &value);

  lua_pushnumber(L, value);

  return 1;
}

static int
ts_lua_http_config_float_set(lua_State *L)
{
  int conf;
  float value;
  ts_lua_http_ctx *http_ctx;

  GET_HTTP_CONTEXT(http_ctx, L);

  conf  = luaL_checkinteger(L, 1);
  value = luaL_checknumber(L, 2);

  TSHttpTxnConfigFloatSet(http_ctx->txnp, conf, value);

  return 0;
}

static int
ts_lua_http_config_float_get(lua_State *L)
{
  int conf;
  float value;
  ts_lua_http_ctx *http_ctx;

  GET_HTTP_CONTEXT(http_ctx, L);

  conf = luaL_checkinteger(L, 1);

  TSHttpTxnConfigFloatGet(http_ctx->txnp, conf, &value);

  lua_pushnumber(L, value);

  return 1;
}

static int
ts_lua_http_config_string_set(lua_State *L)
{
  int conf;
  const char *value;
  size_t value_len;
  ts_lua_http_ctx *http_ctx;

  GET_HTTP_CONTEXT(http_ctx, L);

  conf  = luaL_checkinteger(L, 1);
  value = luaL_checklstring(L, 2, &value_len);

  TSHttpTxnConfigStringSet(http_ctx->txnp, conf, value, value_len);

  return 0;
}

static int
ts_lua_http_config_string_get(lua_State *L)
{
  int conf;
  const char *value;
  int value_len;
  ts_lua_http_ctx *http_ctx;

  GET_HTTP_CONTEXT(http_ctx, L);

  conf = luaL_checkinteger(L, 1);

  TSHttpTxnConfigStringGet(http_ctx->txnp, conf, &value, &value_len);

  lua_pushlstring(L, value, value_len);

  return 1;
}

static int
ts_lua_http_timeout_set(lua_State *L)
{
  int conf;
  int value;
  ts_lua_http_ctx *http_ctx;

  GET_HTTP_CONTEXT(http_ctx, L);

  conf  = luaL_checkinteger(L, 1);
  value = luaL_checkinteger(L, 2);

  switch (conf) {
  case TS_LUA_TIMEOUT_ACTIVE:
    TSDebug(TS_LUA_DEBUG_TAG, "setting active timeout");
    TSHttpTxnActiveTimeoutSet(http_ctx->txnp, value);
    break;

  case TS_LUA_TIMEOUT_CONNECT:
    TSDebug(TS_LUA_DEBUG_TAG, "setting connect timeout");
    TSHttpTxnConnectTimeoutSet(http_ctx->txnp, value);
    break;

  case TS_LUA_TIMEOUT_DNS:
    TSDebug(TS_LUA_DEBUG_TAG, "setting dns timeout");
    TSHttpTxnDNSTimeoutSet(http_ctx->txnp, value);
    break;

  case TS_LUA_TIMEOUT_NO_ACTIVITY:
    TSDebug(TS_LUA_DEBUG_TAG, "setting no activity timeout");
    TSHttpTxnNoActivityTimeoutSet(http_ctx->txnp, value);
    break;

  default:
    TSError("[ts_lua] Unsupported timeout config option for lua plugin");
    break;
  }

  return 0;
}

static int
ts_lua_http_client_packet_mark_set(lua_State *L)
{
  int value;
  ts_lua_http_ctx *http_ctx;

  GET_HTTP_CONTEXT(http_ctx, L);

  value = luaL_checkinteger(L, 1);

  TSDebug(TS_LUA_DEBUG_TAG, "client packet mark set");
  TSHttpTxnClientPacketMarkSet(http_ctx->txnp, value);

  return 0;
}

static int
ts_lua_http_server_packet_mark_set(lua_State *L)
{
  int value;
  ts_lua_http_ctx *http_ctx;

  GET_HTTP_CONTEXT(http_ctx, L);

  value = luaL_checkinteger(L, 1);

  TSDebug(TS_LUA_DEBUG_TAG, "server packet mark set");
  TSHttpTxnServerPacketMarkSet(http_ctx->txnp, value);

  return 0;
}

static int
ts_lua_http_client_packet_tos_set(lua_State *L)
{
  int value;
  ts_lua_http_ctx *http_ctx;

  GET_HTTP_CONTEXT(http_ctx, L);

  value = luaL_checkinteger(L, 1);

  TSDebug(TS_LUA_DEBUG_TAG, "client packet tos set");
  TSHttpTxnClientPacketTosSet(http_ctx->txnp, value);

  return 0;
}

static int
ts_lua_http_enable_redirect(lua_State *L)
{
  int value;
  ts_lua_http_ctx *http_ctx;

  GET_HTTP_CONTEXT(http_ctx, L);

  value = luaL_checkinteger(L, 1);

  TSDebug(TS_LUA_DEBUG_TAG, "enable redirect");
  TSHttpTxnFollowRedirect(http_ctx->txnp, value);

  return 0;
}

static int
ts_lua_http_set_debug(lua_State *L)
{
  int value;
  ts_lua_http_ctx *http_ctx;

  GET_HTTP_CONTEXT(http_ctx, L);

  value = luaL_checkinteger(L, 1);

  TSDebug(TS_LUA_DEBUG_TAG, "set debug");
  TSHttpTxnDebugSet(http_ctx->txnp, value);

  return 0;
}

static int
ts_lua_http_server_packet_tos_set(lua_State *L)
{
  int value;
  ts_lua_http_ctx *http_ctx;

  GET_HTTP_CONTEXT(http_ctx, L);

  value = luaL_checkinteger(L, 1);

  TSDebug(TS_LUA_DEBUG_TAG, "server packet tos set");
  TSHttpTxnServerPacketTosSet(http_ctx->txnp, value);

  return 0;
}

static int
ts_lua_http_client_packet_dscp_set(lua_State *L)
{
  int value;
  ts_lua_http_ctx *http_ctx;

  GET_HTTP_CONTEXT(http_ctx, L);

  value = luaL_checkinteger(L, 1);

  TSDebug(TS_LUA_DEBUG_TAG, "client packet dscp set");
  TSHttpTxnClientPacketDscpSet(http_ctx->txnp, value);

  return 0;
}

static int
ts_lua_http_server_packet_dscp_set(lua_State *L)
{
  int value;
  ts_lua_http_ctx *http_ctx;

  GET_HTTP_CONTEXT(http_ctx, L);

  value = luaL_checkinteger(L, 1);

  TSDebug(TS_LUA_DEBUG_TAG, "server packet dscp set");
  TSHttpTxnServerPacketDscpSet(http_ctx->txnp, value);

  return 0;
}
