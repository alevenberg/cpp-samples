// Copyright 2024 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "google/cloud/bigquery/storage/v1/bigquery_read_client.h"
#include "google/cloud/project.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <avro/Compiler.hh>
#include <avro/DataFile.hh>
#include <avro/Decoder.hh>
#include <avro/Generic.hh>
#include <avro/GenericDatum.hh>
#include <avro/Stream.hh>
#include <avro/ValidSchema.hh>

namespace {

avro::ValidSchema GetAvroSchema(
    ::google::cloud::bigquery::storage::v1::AvroSchema const& schema) {
  // Create a valid reader schema.
  std::istringstream schema_bytes(schema.schema(), std::ios::binary);
  avro::ValidSchema valid_schema;
  avro::compileJsonSchema(schema_bytes, valid_schema);

  // [optional] Write the schema to a file. This could be useful if you want to
  // re-use the schema elsewhere.
  std::ofstream output("schema.avsc");
  if (output.is_open()) {
    valid_schema.toJson(output);
    output.close();
  } else {
    std::cerr << "Error opening the file!" << std::endl;
  }
  return valid_schema;
}

void ProcessRowsInAvroFormat(
    ::google::cloud::bigquery::storage::v1::AvroSchema const& schema,
    ::google::cloud::bigquery::storage::v1::AvroRows const& rows,
    std::int64_t row_count) {
  avro::ValidSchema valid_schema = GetAvroSchema(schema);

  // Get an avro reader.
  // std::istringstream iss(rows.serialized_binary_rows(), std::ios::binary);
  // std::unique_ptr<avro::InputStream> in = avro::istreamInputStream(iss);
  // avro::DecoderPtr d = avro::validatingDecoder(valid_schema,
  // avro::binaryDecoder()); avro::GenericReader gr(valid_schema, d);
  // d->init(*in);

  avro::DecoderPtr decoder =
      avro::validatingDecoder(valid_schema, avro::binaryDecoder());
  std::istringstream row_bytes(rows.serialized_binary_rows(), std::ios::binary);
  std::unique_ptr<avro::InputStream> in = avro::istreamInputStream(row_bytes);
  avro::GenericReader reader(valid_schema, decoder);
  decoder->init(*in);
  avro::GenericDatum datum(valid_schema);

  // Read rows using into a generic datum.
  // avro::GenericDatum datum(valid_schema);

  // for (auto i = 0; i < row_count; ++i) {
  //   reader.read(*decoder, datum, valid_schema);
  // if (datum.type() == avro::AVRO_RECORD) {
  // const avro::GenericRecord &r = datum.value<avro::GenericRecord>();
  // std::cout << "Field-count: " << r.fieldCount() << std::endl;
  //     for (auto i = 0; i < r.fieldCount(); i++)
  //     {
  //         const avro::GenericDatum &f0 = r.fieldAt(i);

  //         if (f0.type() == avro::AVRO_STRING)
  //         {
  //             std::cout << "string: " << f0.value<std::string>() <<
  //             std::endl;
  //         }
  //         else if (f0.type() == avro::AVRO_INT)
  //         {
  //             std::cout << "int: " << f0.value<int>() << std::endl;
  //         }
  //         else if (f0.type() == avro::AVRO_LONG)
  //         {
  //             std::cout << "long: " << f0.value<long>() << std::endl;
  //         }
  //         else
  //         {
  //             std::cout << f0.type() << std::endl;
  //         }
  //     }
  // }
  // }

  // reader.drain();
  // }
}

}  // namespace

int main(int argc, char* argv[]) try {
  if (argc != 4) {
    std::cerr << "Usage: " << argv[0]
              << " <project-id> <dataset-name> <table-name>\n";
    return 1;
  }

  std::string const project_id = argv[1];
  std::string const dataset_name = argv[2];
  std::string const table_name = argv[3];

  std::string const table_id = "projects/" + project_id + "/datasets/" +
                               dataset_name + "/tables/" + table_name;

  // Create a namespace alias to make the code easier to read.
  namespace bigquery_storage = ::google::cloud::bigquery_storage_v1;
  constexpr int kMaxReadStreams = 1;
  // Create the ReadSession.
  auto client = bigquery_storage::BigQueryReadClient(
      bigquery_storage::MakeBigQueryReadConnection());
  ::google::cloud::bigquery::storage::v1::ReadSession read_session;
  read_session.set_data_format(
      google::cloud::bigquery::storage::v1::DataFormat::AVRO);
  read_session.set_table(table_id);
  auto session =
      client.CreateReadSession(google::cloud::Project(project_id).FullName(),
                               read_session, kMaxReadStreams);
  if (!session) throw std::move(session).status();

  // Read rows from the ReadSession.
  constexpr int kRowOffset = 0;
  auto read_rows = client.ReadRows(session->streams(0).name(), kRowOffset);

  std::int64_t num_rows = 0;
  for (auto const& row : read_rows) {
    if (row.ok()) {
      num_rows += row->row_count();
      ProcessRowsInAvroFormat(session->avro_schema(), row->avro_rows(),
                              num_rows);
    }
  }

  std::cout << num_rows << " rows read from table: " << table_name << "\n";
  return 0;
} catch (google::cloud::Status const& status) {
  std::cerr << "google::cloud::Status thrown: " << status << "\n";
  return 1;
}
