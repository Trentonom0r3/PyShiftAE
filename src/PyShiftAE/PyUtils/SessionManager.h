#pragma once
#include <boost/interprocess/ipc/message_queue.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/export.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/variant/variant.hpp>
#include <boost/variant/get.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/variant.hpp>
#include <iostream>
#include <thread>
#include <sstream>
#include <string>
#include "rpc.h"
#include "objbase.h"

std::string createUUID();

enum class Commands {
    reportInfo,
    beginUndoGroup,
    endUndoGroup,
    getProject

};

typedef boost::variant<int, float, std::string, bool, std::vector<std::string>> CommandArg;
typedef std::vector<CommandArg> CommandArgs;

struct Command {
    std::string sessionID = "0";
    std::string name = "name";
    CommandArgs args; // Vector of arguments

    template <class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar& sessionID;
        ar& name;
        ar& args;
    }
};

struct Response {
    std::string sessionID = "0";
    CommandArgs args; // Vector of arguments
    //serialize method
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar& sessionID;
        ar& args;
    }
};      // Response to be sent to the client


class MessageQueueManager {
public:
    // Deletes copy constructor and assignment operator
    MessageQueueManager(const MessageQueueManager&) = delete;
    MessageQueueManager& operator=(const MessageQueueManager&) = delete;

    // Static method to get the instance of the class
    static MessageQueueManager& getInstance() {
        static MessageQueueManager instance; // Guaranteed to be destroyed and instantiated on first use
        return instance;
    }
 

    void clearQueues() {
		clearQueue("PyC1");
		clearQueue("PyR1");
	}

    void sendCommand(Command command) {
        std::stringstream ss;
        boost::archive::text_oarchive oa(ss);
        oa << command;
        std::string serializedCommand = ss.str();
        commandQueue->send(serializedCommand.c_str(), serializedCommand.size(), 0);
    }

    Command receiveCommand() {
        boost::interprocess::message_queue::size_type recvd_size;
        unsigned int priority;
        std::size_t max_msg_size = commandQueue->get_max_msg_size();
        std::vector<char> buffer(max_msg_size);
        commandQueue->receive(buffer.data(), buffer.size(), recvd_size, priority);
        std::string serializedCommand(buffer.begin(), buffer.begin() + recvd_size);
        std::stringstream ss(serializedCommand);
        boost::archive::text_iarchive ia(ss);
        Command command;
        ia >> command;
        return command;
    }


    Response receiveResponse() {
        boost::interprocess::message_queue::size_type recvd_size;
        unsigned int priority;
        responseQueue->get_max_msg_size();
        std::vector<char> buffer(responseQueue->get_max_msg_size());
        responseQueue->receive(buffer.data(), buffer.size(), recvd_size, priority);
        std::string serializedResponse(buffer.begin(), buffer.begin() + recvd_size);
        std::stringstream ss(serializedResponse);
        boost::archive::text_iarchive ia(ss);
        Response response;
        ia >> response;
        return response;
    }


    void sendResponse(Response response) {
        std::stringstream ss;
        boost::archive::text_oarchive oa(ss);
        oa << response;
        std::string serializedResponse = ss.str();
        responseQueue->send(serializedResponse.c_str(), serializedResponse.size(), 0);
    }

    void sendSuccessResponse(const std::string& sessionID) {
        Response resp;
        resp.sessionID = sessionID;
        std::string success = "Success";
        resp.args.push_back(success);
        this->sendResponse(resp);
    }

private:
    MessageQueueManager() {
        // Using smart pointers for automatic resource management
        commandQueue = std::make_unique<boost::interprocess::message_queue>(
            boost::interprocess::open_or_create, "PyC1", 100, 1024);
        responseQueue = std::make_unique<boost::interprocess::message_queue>(
            boost::interprocess::open_or_create, "PyR1", 100, 1024);
    }
    ~MessageQueueManager() {
		boost::interprocess::message_queue::remove("PyC1");
		boost::interprocess::message_queue::remove("PyR1");
	}
    std::unique_ptr<boost::interprocess::message_queue> commandQueue;
    std::unique_ptr<boost::interprocess::message_queue> responseQueue;
    void clearQueue(const char* queueName) {
        if (boost::interprocess::message_queue::remove(queueName)) {
            std::cout << "Cleared existing queue: " << queueName << std::endl;
        }
    }
};