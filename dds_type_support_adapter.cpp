#include "dds_type_support_adapter.h"

#include <cstring>
#include <fastdds/rtps/common/Types.h>

namespace awm {

namespace dds = eprosima::fastdds::dds;
namespace rtps = eprosima::fastrtps::rtps;

Buffer Payload2Buffer(const rtps::SerializedPayload_t& payload) {
    Buffer buffer{0};
    buffer.data = reinterpret_cast<char*>(payload.data);
    buffer.length = payload.length;
    buffer.max_size = payload.max_size;
    return buffer;
}

DDSTypeSupportAdapter::DDSTypeSupportAdapter(TypeSupportBase* impl) : impl_(impl) {
    setName(impl_->GetTypeName());
    m_typeSize = 128;
    m_isGetKeyDefined = impl_->IsGetKeyDefined();
}

DDSTypeSupportAdapter::~DDSTypeSupportAdapter() {}

bool DDSTypeSupportAdapter::serialize(void* data, rtps::SerializedPayload_t* payload) {
    Buffer buffer = Payload2Buffer(*payload);
    bool ret = false;
    try {
        ret = impl_->Serialize(data, &buffer);
        payload->length = buffer.length;
    } catch (...) {
        return false;
    }
    return ret;
}

bool DDSTypeSupportAdapter::deserialize(rtps::SerializedPayload_t* payload, void* data) {
    Buffer buffer = Payload2Buffer(*payload);
    bool ret = false;
    try {
        ret = impl_->Deserialize(&buffer, data);
    } catch (...) {
        return false;
    }
    return ret;
}

std::function<uint32_t()> DDSTypeSupportAdapter::getSerializedSizeProvider(void* data) {
    return [this, data]() -> uint32_t { return impl_->GetSerializedSize(data); };
}

bool DDSTypeSupportAdapter::getKey(void* data, rtps::InstanceHandle_t* ihandle, bool force_md5) {
    const char* key = impl_->GetKey(data);
    const int len = std::min(strlen(key), size_t(16));
    std::memcpy(ihandle->value, key, len);
    return true;
}

void* DDSTypeSupportAdapter::createData() { return impl_->CreateData(); }

void DDSTypeSupportAdapter::deleteData(void* data) { impl_->DeleteData(data); }

inline bool DDSTypeSupportAdapter::is_bounded() const { return true; }

inline bool DDSTypeSupportAdapter::is_plain() const { return false; }

}  // namespace awm
