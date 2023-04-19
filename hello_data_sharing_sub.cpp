#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>
#include <fastdds/dds/subscriber/qos/SubscriberQos.hpp>
#include <fastdds/dds/subscriber/qos/DataReaderQos.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastrtps/subscriber/SampleInfo.h>


#include <thread>
#include <chrono>
#include <string>
#include <cstring>

#include <std_msgs/String.h>

#include <awm/type_support_rosmsg.h>
#include "dds_type_support_adapter.h"

namespace test {

namespace dds = ::eprosima::fastdds::dds;

class HelloWorldSubscriber {
public:
    HelloWorldSubscriber()
    : participant_(nullptr)
    , subscriber_(nullptr)
    , topic_(nullptr)
    , reader_(nullptr) {
        


    }

    ~HelloWorldSubscriber() {
        if (reader_ != nullptr) {
            subscriber_->delete_datareader(reader_);
        }
        if (subscriber_ != nullptr) {
            participant_->delete_subscriber(subscriber_);
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

        dds::SubscriberQos sub_qos = dds::SUBSCRIBER_QOS_DEFAULT;
        subscriber_ = participant_->create_subscriber(sub_qos, nullptr);

        dds::DataReaderQos reader_qos = dds::DATAREADER_QOS_DEFAULT;
        reader_qos.reliability().kind = dds::RELIABLE_RELIABILITY_QOS;
        reader_qos.data_sharing().off();
        reader_ = subscriber_->create_datareader(topic_, reader_qos, &listener_);
    }

private:
    dds::DomainParticipant* participant_;
    dds::Subscriber* subscriber_;
    dds::Topic* topic_;
    dds::DataReader* reader_;


    class SubListener : public dds::DataReaderListener {
        void on_data_available(dds::DataReader* reader) override;
    } listener_;

};

void HelloWorldSubscriber::SubListener::on_data_available(dds::DataReader* reader) {
    dds::SampleInfo info;
    std_msgs::String msg;
    if (reader->take_next_sample(&msg, &info) == ReturnCode_t::RETCODE_OK) {
        std::cout << "Message:" << msg << std::endl;
    }
}


}  // namespace test

int main(int argc, char** argv) {
    test::HelloWorldSubscriber sub;
    sub.init("hello");
    std_msgs::String msg;
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    return 0;
}
