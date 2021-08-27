#pragma once

#include "signals/signals.h"

#include <functional>
#include <map>
#include <memory>
#include <mutex>

namespace jm
{

class ITopic {
public:
    using Ptr = std::shared_ptr<ITopic>;

    std::string topic;

    virtual ~ITopic()
    {}
};

template<class T>
class TopicAdapter : public ITopic {
public:
    using Ptr = std::shared_ptr<TopicAdapter<T>>;
    std::shared_ptr<jm::Signal<void(T)>> sig_topic;

    TopicAdapter(const std::string &str)
    {
        sig_topic = std::make_shared<jm::Signal<void(T)>>();
        topic = str;
    }
};

class TopicManager {
    TopicManager() {}
public:
    using Ptr = std::shared_ptr<TopicManager>;

    static TopicManager* instance()
    {
        static TopicManager topic_manager;
        return &topic_manager;
    }

    TopicManager &operator=(TopicManager &) = delete;

    template<class T>
    std::shared_ptr<jm::Signal<void(T)>> get(const std::string &topic)
    {
        std::lock_guard<std::mutex> lg(mut);
        ITopic::Ptr topic_adapter;
        std::string topic_name = topic + "_" + typeid(T).name();
        if (map_topic.find(topic_name) != map_topic.end()) {
            topic_adapter = map_topic[topic_name];
        } else {
            topic_adapter = std::shared_ptr<ITopic>(new TopicAdapter<T>(topic_name));
            map_topic[topic_name] = topic_adapter;
        }
        auto ret = static_cast<TopicAdapter<T> *>(topic_adapter.get());
        return ret->sig_topic;
    }

private:
    std::mutex mut;
    std::map<std::string, ITopic::Ptr> map_topic;
};

class ISubscriber {
public:
    using Ptr = std::shared_ptr<ISubscriber>;

    virtual ~ISubscriber() {}

    virtual void subscribe() = 0;

    virtual void unSubscribe() = 0;

};

template<class T>
class Subscriber : public ISubscriber {
public:
    Subscriber(const std::string &topic, std::function<void(T)> func)
        : func_(std::move(func))
    {
        sig_ = TopicManager::instance()->get<T>(topic);
    }

    void subscribe() override
    {
        unSubscribe();
        con_ = sig_->connect(func_);
    }

    void unSubscribe() override
    {
        con_.disconnect();
    }

public:
    std::shared_ptr<jm::Signal<void(T)>> sig_;
    std::function<void(T)> func_;
    jm::Connection con_;
};

///\brief Publish the topic message
///this publish is single thread, need change to thread pool
template<class T>
void publish(const std::string &topic, T t)
{
    // TODO: use queue
    if (auto sig = TopicManager::instance()->get<T>(topic)) {
        (*sig)(t);  //TODO: change to thread pool
    }
}
}
