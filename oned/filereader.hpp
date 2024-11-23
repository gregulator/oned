
#ifndef READ_FILES_HPP
#define READ_FILES_HPP

#include <string>
#include <memory>
#include "oned/config_reader.hpp"  // Class to parse the XML config
#include <nlohmann/json.hpp>
#include "oned/data_container.hpp"  // Data container for storage


namespace oned
{

class FileReaderFactory {
public:
    static std::unique_ptr<FileReader> CreateFileReader(const std::string& protocol_name){
        if (protocol_name == oned::ProtocolType::JSON) {
            return std::make_unique<JSONFileReader>();
        } 
        // Add cases for other file formats like ROS
        else {
            std::cerr << "Unsupported protocol: " << protocol_name << std::endl;
            return nullptr;
        }
    }
};



class FileReader {
public:
    virtual ~FileReader() = default;
    virtual void ReadFile(const std::string& file_path, const Protocol& protocol) = 0;
};

class JSONFileReader : public FileReader {
public:
    void ReadFile(const std::string& file_path , const Protocol& protocol) override{
        std::ifstream file(file_path);
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << file_path << std::endl;
            return;
        }
        nlohmann::json json_data;
        file >> json_data;
        file.close();

        // Create a DataContainer to store the data from JSON
        container = new DataContainer();

        // Parse the JSON object based on the protocol
        ParseObject(json_data, protocol);

        
    }

private:
    void ParseObject(const nlohmann::json& json_obj, const Protocol& protocol) {
        for (const auto& field : protocol.fields) {
            if (json_obj.contains(field.name)) {
                const auto& value = json_obj[field.name];
                if (field.type == "object") {
                    // Recursively parse subfields of the object
                    ParseObject(value, field);
                } else if (field.isArray) {
                    // Handle array fields
                    ParseArray(value, field);
                } else {
                    // Handle regular fields
                    AddFieldToChannel(field, value);
                }
            }
        }
    }
    void ParseObject(const nlohmann::json& json_obj, const Field& field) {
        if (json_obj.contains(field.name)) {
            const auto& value = json_obj[field.name];
            if (field.type == "object") {
                // Recursively parse subfields of the object
                ParseObject(value, field);
            } else if (field.isArray) {
                // Handle array fields
                ParseArray(value, field);
            } else {
                // Handle regular fields
                AddFieldToChannel(field, value);
            }
        }

    }
    
    void ParseArray(const nlohmann::json& json_array, const Field& field) {
        for (const auto& item : json_array) {
            ParseObject(item, field);
        }
    }

    void AddFieldToChannel(const Field& field, const nlohmann::json& value) {
        DataContainer::Channel* channel = nullptr;

        // Add a channel based on the field type if it doesn't exist
        if (!container->GetChannel(field.name)) {
            if (field.type == "double") {
                channel = container->AddChannel<double>(field.name);
            } else if (field.type == "int") {
                channel = container->AddChannel<int>(field.name);
            } else if (field.type == "string") {
                channel = container->AddChannel<std::string>(field.name);
            }
            // Add cases for other types if needed
        } else {
            channel = container->GetChannel(field.name);
        }

        // Add the value to the channel (assuming a single chunk for simplicity)
        if (channel != nullptr) {
            if (field.type == "double") {
                channel->MutableData<double>(-1)->push_back(value.get<double>());
                //*channel->MutableData<double>(0) = value.get<double>();
            } else if (field.type == "int") {
                channel->MutableData<int>(-1)->push_back(value.get<int>());
                //*channel->MutableData<int>(0) = std::vector<int>{value.get<int>()};
            } else if (field.type == "string") {
                channel->MutableData<std::string>(-1)->push_back(value.get<std::string>());
                //*channel->MutableData<std::string>(0) =  std::vector<std::string>{value.get<std::string>()};
            }
        }
    }


    DataContainer* container;
};

}


#endif // READ_FILES_HPP
