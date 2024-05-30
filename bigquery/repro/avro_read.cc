
#include <fstream>
#include <avro/GenericDatum.hh>
#include <avro/ValidSchema.hh>
#include <avro/Compiler.hh>

int main(int argc, char* argv[]) {
  // Read in and compile schema.
  std::ifstream in("schema.avsc");
  avro::ValidSchema valid_schema;
  avro::compileJsonSchema(in, valid_schema);

  // Create a generic datum (segfaults).
  avro::GenericDatum datum(valid_schema); 
  return 0;
}
