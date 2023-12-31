// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: bridge_msgs/basic_msgs/header.proto

#include "bridge_msgs/basic_msgs/header.pb.h"

#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>

PROTOBUF_PRAGMA_INIT_SEG

namespace _pb = ::PROTOBUF_NAMESPACE_ID;
namespace _pbi = _pb::internal;

namespace prescan {
namespace common {
PROTOBUF_CONSTEXPR Header::Header(
    ::_pbi::ConstantInitialized): _impl_{
    /*decltype(_impl_._has_bits_)*/{}
  , /*decltype(_impl_._cached_size_)*/{}
  , /*decltype(_impl_.module_name_)*/{&::_pbi::fixed_address_empty_string, ::_pbi::ConstantInitialized{}}
  , /*decltype(_impl_.frame_id_)*/{&::_pbi::fixed_address_empty_string, ::_pbi::ConstantInitialized{}}
  , /*decltype(_impl_.timestamp_sec_)*/0
  , /*decltype(_impl_.sequence_num_)*/0u
  , /*decltype(_impl_.version_)*/1u} {}
struct HeaderDefaultTypeInternal {
  PROTOBUF_CONSTEXPR HeaderDefaultTypeInternal()
      : _instance(::_pbi::ConstantInitialized{}) {}
  ~HeaderDefaultTypeInternal() {}
  union {
    Header _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 HeaderDefaultTypeInternal _Header_default_instance_;
}  // namespace common
}  // namespace prescan
static ::_pb::Metadata file_level_metadata_bridge_5fmsgs_2fbasic_5fmsgs_2fheader_2eproto[1];
static constexpr ::_pb::EnumDescriptor const** file_level_enum_descriptors_bridge_5fmsgs_2fbasic_5fmsgs_2fheader_2eproto = nullptr;
static constexpr ::_pb::ServiceDescriptor const** file_level_service_descriptors_bridge_5fmsgs_2fbasic_5fmsgs_2fheader_2eproto = nullptr;

const uint32_t TableStruct_bridge_5fmsgs_2fbasic_5fmsgs_2fheader_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  PROTOBUF_FIELD_OFFSET(::prescan::common::Header, _impl_._has_bits_),
  PROTOBUF_FIELD_OFFSET(::prescan::common::Header, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  PROTOBUF_FIELD_OFFSET(::prescan::common::Header, _impl_.timestamp_sec_),
  PROTOBUF_FIELD_OFFSET(::prescan::common::Header, _impl_.module_name_),
  PROTOBUF_FIELD_OFFSET(::prescan::common::Header, _impl_.sequence_num_),
  PROTOBUF_FIELD_OFFSET(::prescan::common::Header, _impl_.version_),
  PROTOBUF_FIELD_OFFSET(::prescan::common::Header, _impl_.frame_id_),
  2,
  0,
  3,
  4,
  1,
};
static const ::_pbi::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, 11, -1, sizeof(::prescan::common::Header)},
};

static const ::_pb::Message* const file_default_instances[] = {
  &::prescan::common::_Header_default_instance_._instance,
};

const char descriptor_table_protodef_bridge_5fmsgs_2fbasic_5fmsgs_2fheader_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n#bridge_msgs/basic_msgs/header.proto\022\016p"
  "rescan.common\"p\n\006Header\022\025\n\rtimestamp_sec"
  "\030\001 \001(\001\022\023\n\013module_name\030\002 \001(\t\022\024\n\014sequence_"
  "num\030\003 \001(\r\022\022\n\007version\030\004 \001(\r:\0011\022\020\n\010frame_i"
  "d\030\005 \001(\t"
  ;
static ::_pbi::once_flag descriptor_table_bridge_5fmsgs_2fbasic_5fmsgs_2fheader_2eproto_once;
const ::_pbi::DescriptorTable descriptor_table_bridge_5fmsgs_2fbasic_5fmsgs_2fheader_2eproto = {
    false, false, 167, descriptor_table_protodef_bridge_5fmsgs_2fbasic_5fmsgs_2fheader_2eproto,
    "bridge_msgs/basic_msgs/header.proto",
    &descriptor_table_bridge_5fmsgs_2fbasic_5fmsgs_2fheader_2eproto_once, nullptr, 0, 1,
    schemas, file_default_instances, TableStruct_bridge_5fmsgs_2fbasic_5fmsgs_2fheader_2eproto::offsets,
    file_level_metadata_bridge_5fmsgs_2fbasic_5fmsgs_2fheader_2eproto, file_level_enum_descriptors_bridge_5fmsgs_2fbasic_5fmsgs_2fheader_2eproto,
    file_level_service_descriptors_bridge_5fmsgs_2fbasic_5fmsgs_2fheader_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK const ::_pbi::DescriptorTable* descriptor_table_bridge_5fmsgs_2fbasic_5fmsgs_2fheader_2eproto_getter() {
  return &descriptor_table_bridge_5fmsgs_2fbasic_5fmsgs_2fheader_2eproto;
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY2 static ::_pbi::AddDescriptorsRunner dynamic_init_dummy_bridge_5fmsgs_2fbasic_5fmsgs_2fheader_2eproto(&descriptor_table_bridge_5fmsgs_2fbasic_5fmsgs_2fheader_2eproto);
namespace prescan {
namespace common {

// ===================================================================

class Header::_Internal {
 public:
  using HasBits = decltype(std::declval<Header>()._impl_._has_bits_);
  static void set_has_timestamp_sec(HasBits* has_bits) {
    (*has_bits)[0] |= 4u;
  }
  static void set_has_module_name(HasBits* has_bits) {
    (*has_bits)[0] |= 1u;
  }
  static void set_has_sequence_num(HasBits* has_bits) {
    (*has_bits)[0] |= 8u;
  }
  static void set_has_version(HasBits* has_bits) {
    (*has_bits)[0] |= 16u;
  }
  static void set_has_frame_id(HasBits* has_bits) {
    (*has_bits)[0] |= 2u;
  }
};

Header::Header(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor(arena, is_message_owned);
  // @@protoc_insertion_point(arena_constructor:prescan.common.Header)
}
Header::Header(const Header& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  Header* const _this = this; (void)_this;
  new (&_impl_) Impl_{
      decltype(_impl_._has_bits_){from._impl_._has_bits_}
    , /*decltype(_impl_._cached_size_)*/{}
    , decltype(_impl_.module_name_){}
    , decltype(_impl_.frame_id_){}
    , decltype(_impl_.timestamp_sec_){}
    , decltype(_impl_.sequence_num_){}
    , decltype(_impl_.version_){}};

  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  _impl_.module_name_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.module_name_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (from._internal_has_module_name()) {
    _this->_impl_.module_name_.Set(from._internal_module_name(), 
      _this->GetArenaForAllocation());
  }
  _impl_.frame_id_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.frame_id_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (from._internal_has_frame_id()) {
    _this->_impl_.frame_id_.Set(from._internal_frame_id(), 
      _this->GetArenaForAllocation());
  }
  ::memcpy(&_impl_.timestamp_sec_, &from._impl_.timestamp_sec_,
    static_cast<size_t>(reinterpret_cast<char*>(&_impl_.version_) -
    reinterpret_cast<char*>(&_impl_.timestamp_sec_)) + sizeof(_impl_.version_));
  // @@protoc_insertion_point(copy_constructor:prescan.common.Header)
}

inline void Header::SharedCtor(
    ::_pb::Arena* arena, bool is_message_owned) {
  (void)arena;
  (void)is_message_owned;
  new (&_impl_) Impl_{
      decltype(_impl_._has_bits_){}
    , /*decltype(_impl_._cached_size_)*/{}
    , decltype(_impl_.module_name_){}
    , decltype(_impl_.frame_id_){}
    , decltype(_impl_.timestamp_sec_){0}
    , decltype(_impl_.sequence_num_){0u}
    , decltype(_impl_.version_){1u}
  };
  _impl_.module_name_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.module_name_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  _impl_.frame_id_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.frame_id_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
}

Header::~Header() {
  // @@protoc_insertion_point(destructor:prescan.common.Header)
  if (auto *arena = _internal_metadata_.DeleteReturnArena<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>()) {
  (void)arena;
    return;
  }
  SharedDtor();
}

inline void Header::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
  _impl_.module_name_.Destroy();
  _impl_.frame_id_.Destroy();
}

void Header::SetCachedSize(int size) const {
  _impl_._cached_size_.Set(size);
}

void Header::Clear() {
// @@protoc_insertion_point(message_clear_start:prescan.common.Header)
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  cached_has_bits = _impl_._has_bits_[0];
  if (cached_has_bits & 0x00000003u) {
    if (cached_has_bits & 0x00000001u) {
      _impl_.module_name_.ClearNonDefaultToEmpty();
    }
    if (cached_has_bits & 0x00000002u) {
      _impl_.frame_id_.ClearNonDefaultToEmpty();
    }
  }
  if (cached_has_bits & 0x0000001cu) {
    ::memset(&_impl_.timestamp_sec_, 0, static_cast<size_t>(
        reinterpret_cast<char*>(&_impl_.sequence_num_) -
        reinterpret_cast<char*>(&_impl_.timestamp_sec_)) + sizeof(_impl_.sequence_num_));
    _impl_.version_ = 1u;
  }
  _impl_._has_bits_.Clear();
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* Header::_InternalParse(const char* ptr, ::_pbi::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  _Internal::HasBits has_bits{};
  while (!ctx->Done(&ptr)) {
    uint32_t tag;
    ptr = ::_pbi::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // optional double timestamp_sec = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 9)) {
          _Internal::set_has_timestamp_sec(&has_bits);
          _impl_.timestamp_sec_ = ::PROTOBUF_NAMESPACE_ID::internal::UnalignedLoad<double>(ptr);
          ptr += sizeof(double);
        } else
          goto handle_unusual;
        continue;
      // optional string module_name = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 18)) {
          auto str = _internal_mutable_module_name();
          ptr = ::_pbi::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(ptr);
          #ifndef NDEBUG
          ::_pbi::VerifyUTF8(str, "prescan.common.Header.module_name");
          #endif  // !NDEBUG
        } else
          goto handle_unusual;
        continue;
      // optional uint32 sequence_num = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 24)) {
          _Internal::set_has_sequence_num(&has_bits);
          _impl_.sequence_num_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint32(&ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // optional uint32 version = 4 [default = 1];
      case 4:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 32)) {
          _Internal::set_has_version(&has_bits);
          _impl_.version_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint32(&ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // optional string frame_id = 5;
      case 5:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 42)) {
          auto str = _internal_mutable_frame_id();
          ptr = ::_pbi::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(ptr);
          #ifndef NDEBUG
          ::_pbi::VerifyUTF8(str, "prescan.common.Header.frame_id");
          #endif  // !NDEBUG
        } else
          goto handle_unusual;
        continue;
      default:
        goto handle_unusual;
    }  // switch
  handle_unusual:
    if ((tag == 0) || ((tag & 7) == 4)) {
      CHK_(ptr);
      ctx->SetLastTag(tag);
      goto message_done;
    }
    ptr = UnknownFieldParse(
        tag,
        _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
        ptr, ctx);
    CHK_(ptr != nullptr);
  }  // while
message_done:
  _impl_._has_bits_.Or(has_bits);
  return ptr;
failure:
  ptr = nullptr;
  goto message_done;
#undef CHK_
}

uint8_t* Header::_InternalSerialize(
    uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:prescan.common.Header)
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = _impl_._has_bits_[0];
  // optional double timestamp_sec = 1;
  if (cached_has_bits & 0x00000004u) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteDoubleToArray(1, this->_internal_timestamp_sec(), target);
  }

  // optional string module_name = 2;
  if (cached_has_bits & 0x00000001u) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::VerifyUTF8StringNamedField(
      this->_internal_module_name().data(), static_cast<int>(this->_internal_module_name().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::SERIALIZE,
      "prescan.common.Header.module_name");
    target = stream->WriteStringMaybeAliased(
        2, this->_internal_module_name(), target);
  }

  // optional uint32 sequence_num = 3;
  if (cached_has_bits & 0x00000008u) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteUInt32ToArray(3, this->_internal_sequence_num(), target);
  }

  // optional uint32 version = 4 [default = 1];
  if (cached_has_bits & 0x00000010u) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteUInt32ToArray(4, this->_internal_version(), target);
  }

  // optional string frame_id = 5;
  if (cached_has_bits & 0x00000002u) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::VerifyUTF8StringNamedField(
      this->_internal_frame_id().data(), static_cast<int>(this->_internal_frame_id().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::SERIALIZE,
      "prescan.common.Header.frame_id");
    target = stream->WriteStringMaybeAliased(
        5, this->_internal_frame_id(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:prescan.common.Header)
  return target;
}

size_t Header::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:prescan.common.Header)
  size_t total_size = 0;

  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  cached_has_bits = _impl_._has_bits_[0];
  if (cached_has_bits & 0x0000001fu) {
    // optional string module_name = 2;
    if (cached_has_bits & 0x00000001u) {
      total_size += 1 +
        ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
          this->_internal_module_name());
    }

    // optional string frame_id = 5;
    if (cached_has_bits & 0x00000002u) {
      total_size += 1 +
        ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
          this->_internal_frame_id());
    }

    // optional double timestamp_sec = 1;
    if (cached_has_bits & 0x00000004u) {
      total_size += 1 + 8;
    }

    // optional uint32 sequence_num = 3;
    if (cached_has_bits & 0x00000008u) {
      total_size += ::_pbi::WireFormatLite::UInt32SizePlusOne(this->_internal_sequence_num());
    }

    // optional uint32 version = 4 [default = 1];
    if (cached_has_bits & 0x00000010u) {
      total_size += ::_pbi::WireFormatLite::UInt32SizePlusOne(this->_internal_version());
    }

  }
  return MaybeComputeUnknownFieldsSize(total_size, &_impl_._cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData Header::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSourceCheck,
    Header::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*Header::GetClassData() const { return &_class_data_; }


void Header::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg) {
  auto* const _this = static_cast<Header*>(&to_msg);
  auto& from = static_cast<const Header&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:prescan.common.Header)
  GOOGLE_DCHECK_NE(&from, _this);
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = from._impl_._has_bits_[0];
  if (cached_has_bits & 0x0000001fu) {
    if (cached_has_bits & 0x00000001u) {
      _this->_internal_set_module_name(from._internal_module_name());
    }
    if (cached_has_bits & 0x00000002u) {
      _this->_internal_set_frame_id(from._internal_frame_id());
    }
    if (cached_has_bits & 0x00000004u) {
      _this->_impl_.timestamp_sec_ = from._impl_.timestamp_sec_;
    }
    if (cached_has_bits & 0x00000008u) {
      _this->_impl_.sequence_num_ = from._impl_.sequence_num_;
    }
    if (cached_has_bits & 0x00000010u) {
      _this->_impl_.version_ = from._impl_.version_;
    }
    _this->_impl_._has_bits_[0] |= cached_has_bits;
  }
  _this->_internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void Header::CopyFrom(const Header& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:prescan.common.Header)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Header::IsInitialized() const {
  return true;
}

void Header::InternalSwap(Header* other) {
  using std::swap;
  auto* lhs_arena = GetArenaForAllocation();
  auto* rhs_arena = other->GetArenaForAllocation();
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  swap(_impl_._has_bits_[0], other->_impl_._has_bits_[0]);
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::InternalSwap(
      &_impl_.module_name_, lhs_arena,
      &other->_impl_.module_name_, rhs_arena
  );
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::InternalSwap(
      &_impl_.frame_id_, lhs_arena,
      &other->_impl_.frame_id_, rhs_arena
  );
  ::PROTOBUF_NAMESPACE_ID::internal::memswap<
      PROTOBUF_FIELD_OFFSET(Header, _impl_.sequence_num_)
      + sizeof(Header::_impl_.sequence_num_)
      - PROTOBUF_FIELD_OFFSET(Header, _impl_.timestamp_sec_)>(
          reinterpret_cast<char*>(&_impl_.timestamp_sec_),
          reinterpret_cast<char*>(&other->_impl_.timestamp_sec_));
  swap(_impl_.version_, other->_impl_.version_);
}

::PROTOBUF_NAMESPACE_ID::Metadata Header::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_bridge_5fmsgs_2fbasic_5fmsgs_2fheader_2eproto_getter, &descriptor_table_bridge_5fmsgs_2fbasic_5fmsgs_2fheader_2eproto_once,
      file_level_metadata_bridge_5fmsgs_2fbasic_5fmsgs_2fheader_2eproto[0]);
}

// @@protoc_insertion_point(namespace_scope)
}  // namespace common
}  // namespace prescan
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::prescan::common::Header*
Arena::CreateMaybeMessage< ::prescan::common::Header >(Arena* arena) {
  return Arena::CreateMessageInternal< ::prescan::common::Header >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
