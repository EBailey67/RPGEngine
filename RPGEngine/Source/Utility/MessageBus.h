#pragma once

// Source: https://seanballais.github.io/blog/implementing-a-simple-message-bus-in-cpp/

#include <iostream>
#include <functional>
#include <queue>
#include <vector>

class Message
{
public:
	explicit Message(const std::string_view event)
    {
        message_event = event;
    }

    std::string getEvent() const
    {
        return message_event;
    }
private:
    std::string message_event;
};


class MessageBus
{
public:
    MessageBus() = default;
    ~MessageBus() = default;

    void addReceiver(const std::function<void (Message)>& messageReceiver)
    {
        receivers.push_back(messageReceiver);
    }

    void sendMessage(const Message& message)
    {
        messages.push(message);
    }

    void notify()
    {
        while(!messages.empty()) 
        {
            for (auto iter = receivers.begin(); iter != receivers.end(); ++iter) 
            {
                (*iter)(messages.front());
            }

            messages.pop();
        }
    }

private:
    std::vector<std::function<void (Message)>> receivers;
    std::queue<Message> messages;
};


class BusNode
{
public:
	virtual ~BusNode() = default;

	explicit BusNode(MessageBus *messageBus) :
		message_bus(messageBus)
    {
        message_bus->addReceiver(this->getNotifyFunc());
    }

    virtual void update() {}
protected:
    MessageBus *message_bus;

    std::function<void (Message)> getNotifyFunc()
    {
        auto messageListener = [this](const Message& message) -> void {
            this->onNotify(message);
        };
        return messageListener;
    }

    void send(const Message& message) const
    {
        message_bus->sendMessage(message);
    }

    virtual void onNotify(const Message& message)
    {
    	std::cout << "onNotify function not implemented!\n";
    }
};