#include <fastdds/dds/publisher/Publisher.hpp>
#include <fastdds/dds/publisher/qos/PublisherQos.hpp>
#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/dds/publisher/qos/DataWriterQos.hpp>
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/domain/DomainParticipant.hpp>


#include <thread>
#include <chrono>
#include <string>
#include <cstring>

#include <std_msgs/String.h>

#include <awm/type_support_rosmsg.h>
#include "dds_type_support_adapter.h"

namespace test {

namespace dds = ::eprosima::fastdds::dds;

class HelloWorldPublisher {
public:
    HelloWorldPublisher()
    : participant_(nullptr)
    , publisher_(nullptr)
    , topic_(nullptr)
    , writer_(nullptr) {
        
    }

    ~HelloWorldPublisher() {
        if (writer_ != nullptr) {
            publisher_->delete_datawriter(writer_);
        }
        if (publisher_ != nullptr) {
            participant_->delete_publisher(publisher_);
        }
        if (topic_ != nullptr) {
            participant_->delete_topic(topic_);
        }
        dds::DomainParticipantFactory::get_instance()->delete_participant(participant_);
    }

    void init(const char* topic) {
        dds::DomainParticipantQos participantQos = dds::PARTICIPANT_QOS_DEFAULT;
        participant_ = dds::DomainParticipantFactory::get_instance()->create_participant(0, participantQos);

        awm::TypeSupportBase* ts = awm::TypeSupportCreator<std_msgs::String>::GetInstance();
        dds::TypeSupport type_support(new awm::DDSTypeSupportAdapter(ts));
        type_support.register_type(participant_);

        dds::TopicQos topic_qos = dds::TOPIC_QOS_DEFAULT;
        topic_ = participant_->create_topic(topic, type_support.get_type_name(), topic_qos);
        
        dds::PublisherQos pub_qos = dds::PUBLISHER_QOS_DEFAULT;
        publisher_ = participant_->create_publisher(pub_qos, nullptr);

        dds::DataWriterQos write_qos = dds::DATAWRITER_QOS_DEFAULT;
        write_qos.data_sharing().off();
        writer_ = publisher_->create_datawriter(topic_, write_qos);
    }

    bool Write(void* sample) {
        return writer_->write(sample);
    }

private:
    dds::DomainParticipant* participant_;
    dds::Publisher* publisher_;
    dds::Topic* topic_;
    dds::DataWriter* writer_;
};

}  // namespace test

int main(int argc, char** argv) {
    test::HelloWorldPublisher pub;
    pub.init("hello");
    std_msgs::String msg;
    int cnt = 0;
    while (true) {
        msg.data = std::to_string(++cnt);
        pub.Write(static_cast<void*>(&msg));
        std::cout << msg << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    return 0;
}
