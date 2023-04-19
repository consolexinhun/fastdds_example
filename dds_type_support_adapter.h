#pragma once

#include <memory>
#include <fastdds/dds/topic/TopicDataType.hpp>
#include <awm/type_support.h>

namespace awm {

class DDSTypeSupportAdapter : public eprosima::fastdds::dds::TopicDataType {
public:
    explicit DDSTypeSupportAdapter(TypeSupportBase* impl);
    ~DDSTypeSupportAdapter() override;
    bool serialize(void* data, eprosima::fastrtps::rtps::SerializedPayload_t* payload) override;
    bool deserialize(eprosima::fastrtps::rtps::SerializedPayload_t* payload, void* data) override;
    std::function<uint32_t()> getSerializedSizeProvider(void* data) override;
    bool getKey(void* data, eprosima::fastrtps::rtps::InstanceHandle_t* ihandle, bool force_md5 = false) override;
    void* createData() override;
    void deleteData(void* data) override;
    inline bool is_bounded() const override;
    inline bool is_plain() const override;

private:
    TypeSupportBase* impl_;
};

}  // namespace awm
