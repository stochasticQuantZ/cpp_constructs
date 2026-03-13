set(PROTO_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/permutation.proto
)

# Generated files go into build/protobuffs_gen
set(PROTO_GEN_DIR ${CMAKE_BINARY_DIR}/protobuffs_gen)
file(MAKE_DIRECTORY ${PROTO_GEN_DIR})

# Prefer imported protoc target if available
if(TARGET protobuf::protoc)
  set(_PROTOC_EXECUTABLE $<TARGET_FILE:protobuf::protoc>)
else()
  find_program(_PROTOC_EXECUTABLE protoc REQUIRED)
endif()

find_program(_GRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin REQUIRED)

set(GENERATED_PROTO_SRCS)
set(GENERATED_PROTO_HDRS)
set(GENERATED_GRPC_SRCS)
set(GENERATED_GRPC_HDRS)

foreach(PROTO_FILE ${PROTO_FILES})
  get_filename_component(PROTO_NAME ${PROTO_FILE} NAME_WE)

  set(PROTO_SRC  ${PROTO_GEN_DIR}/${PROTO_NAME}.pb.cc)
  set(PROTO_HDR  ${PROTO_GEN_DIR}/${PROTO_NAME}.pb.h)
  set(GRPC_SRC   ${PROTO_GEN_DIR}/${PROTO_NAME}.grpc.pb.cc)
  set(GRPC_HDR   ${PROTO_GEN_DIR}/${PROTO_NAME}.grpc.pb.h)

  add_custom_command(
    OUTPUT ${PROTO_SRC} ${PROTO_HDR} ${GRPC_SRC} ${GRPC_HDR}
    COMMAND ${_PROTOC_EXECUTABLE}
    ARGS
      --proto_path=${CMAKE_CURRENT_SOURCE_DIR}
      --cpp_out=${PROTO_GEN_DIR}
      --grpc_out=${PROTO_GEN_DIR}
      --plugin=protoc-gen-grpc=${_GRPC_CPP_PLUGIN_EXECUTABLE}
      ${PROTO_FILE}
    DEPENDS ${PROTO_FILE}
    COMMENT "Generating gRPC sources for ${PROTO_NAME}.proto"
    VERBATIM
  )

  list(APPEND GENERATED_PROTO_SRCS ${PROTO_SRC})
  list(APPEND GENERATED_PROTO_HDRS ${PROTO_HDR})
  list(APPEND GENERATED_GRPC_SRCS ${GRPC_SRC})
  list(APPEND GENERATED_GRPC_HDRS ${GRPC_HDR})
endforeach()

add_library(permutation_proto
  ${GENERATED_PROTO_SRCS}
  ${GENERATED_GRPC_SRCS}
)

target_include_directories(permutation_proto
  PUBLIC
    ${PROTO_GEN_DIR}
)

target_link_libraries(permutation_proto
  PUBLIC
    protobuf::libprotobuf
    gRPC::grpc++
)

# Optional alias for nicer linking
add_library(cpp_constructs::permutation_proto ALIAS permutation_proto)