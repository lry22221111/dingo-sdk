// Copyright (c) 2023 dingodb.com, Inc. All Rights Reserved
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef DINGODB_SDK_RAW_KV_BATCH_COMPARE_AND_SET_TASK_H_
#define DINGODB_SDK_RAW_KV_BATCH_COMPARE_AND_SET_TASK_H_

#include <map>
#include <string_view>
#include <vector>

#include "sdk/client_stub.h"
#include "sdk/rawkv/raw_kv_task.h"
#include "dingosdk/status.h"
#include "sdk/rpc/store_rpc.h"
#include "sdk/rpc/store_rpc_controller.h"
namespace dingodb {
namespace sdk {
class RawKvBatchCompareAndSetTask : public RawKvTask {
 public:
  RawKvBatchCompareAndSetTask(const ClientStub& stub, const std::vector<KVPair>& kvs,
                              const std::vector<std::string>& expected_values, std::vector<KeyOpState>& out_states);

  ~RawKvBatchCompareAndSetTask() override = default;

 private:
  Status Init() override;
  void DoAsync() override;
  void PostProcess() override;

  std::string Name() const override { return "RawKvBatchCompareAndSetTask"; }

  void KvBatchCompareAndSetRpcCallback(const Status& status, KvBatchCompareAndSetRpc* rpc);

  struct CompareAndSetContext {
    const KVPair& kv_pair;
    const std::string& expected_value;
  };

  const std::vector<KVPair>& kvs_;
  const std::vector<std::string>& expected_values_;
  std::vector<KeyOpState>& out_states_;
  std::vector<KeyOpState> tmp_out_states_;

  // should not change
  std::map<std::string_view, CompareAndSetContext> compare_and_set_contexts_;

  std::vector<StoreRpcController> controllers_;
  std::vector<std::unique_ptr<KvBatchCompareAndSetRpc>> rpcs_;

  std::shared_mutex rw_lock_;
  std::set<std::string_view> next_keys_;
  Status status_;

  std::atomic<int> sub_tasks_count_;
};
}  // namespace sdk

}  // namespace dingodb
#endif  // DINGODB_SDK_RAW_KV_BATCH_COMPARE_AND_SET_TASK_H_