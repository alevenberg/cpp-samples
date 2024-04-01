// Copyright 2024 Google LLC
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

#include "google/cloud/batch/v1/batch_client.h"
#include "google/cloud/location.h"
#include <iostream>
#include <schema.pb.h>

// Writes a few rows to a (hard-coded) test table.  Create the table with
//   bq mk cpp_samples
//   bq mk cpp_samples.hello
//   bq update cpp_samples.hello schema.json
int main(int argc, char* argv[]) {
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " project-id region-id\n";
    return 1;
  }

  auto const location = google::cloud::Location(argv[1], argv[2]);

  namespace batch = ::google::cloud::batch_v1;
  auto client = batch::BatchServiceClient(batch::MakeBatchServiceConnection());

CreateJob 
GetJob
  for (auto j : client.ListJobs(location.FullName())) {
    if (!j) throw std::move(j).status();
    std::cout << j->DebugString() << "\n";
  }

  auto const project_id = std::string{argv[1]};
  auto client = bq::BigQueryWriteClient(bq::MakeBigQueryWriteConnection());
JsonStringToMessage
  auto stream = client.AsyncAppendRows();
  auto handle_broken_stream = [&stream](char const* where) {
    auto status = stream->Finish().get();
    std::cerr << "Unexpected streaming RPC error in " << where << ": " << status
              << "\n";
    return 1;
  };


  return 0;
}
