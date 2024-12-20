#ifndef DINGODB_SDK_VECTOR_DISKANN_BUILD_BY_REGION_TASK_H_
#define DINGODB_SDK_VECTOR_DISKANN_BUILD_BY_REGION_TASK_H_

#include <cstdint>
#include <memory>
#include <set>

#include "sdk/client_stub.h"
#include "sdk/rpc/index_service_rpc.h"
#include "sdk/rpc/store_rpc_controller.h"
#include "dingosdk/status.h"
#include "dingosdk/vector.h"
#include "sdk/vector/vector_index.h"
#include "sdk/vector/vector_task.h"

namespace dingodb {
namespace sdk {

class VectorBuildByRegionTask : public VectorTask {
 public:
  VectorBuildByRegionTask(const ClientStub& stub, int64_t index_id, const std::vector<int64_t>& region_ids,
                          ErrStatusResult& result)
      : VectorTask(stub), index_id_(index_id), region_id_(region_ids), result_(result) {}

  ~VectorBuildByRegionTask() override = default;

 private:
  Status Init() override;
  void DoAsync() override;
  bool NeedRetry() override;
  int retry_count_{0};

  std::string Name() const override { return fmt::format("VectorBuildByRegionTask-{}", index_id_); }

  void VectorBuildByRegionRpcCallback(const Status& status, VectorBuildRpc* rpc);

  const int64_t index_id_;
  const std::vector<int64_t>& region_id_;
  ErrStatusResult& result_;

  std::set<int64_t> region_ids_;
  std::shared_ptr<VectorIndex> vector_index_;

  std::vector<StoreRpcController> controllers_;
  std::vector<std::unique_ptr<VectorBuildRpc>> rpcs_;
  std::atomic<int> sub_tasks_count_{0};

  std::shared_mutex rw_lock_;
  Status status_;
};

}  // namespace sdk

}  // namespace dingodb
#endif