
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
        for (const auto& entry : json_data) {
            ParseObject(entry, protocol);
        }
        

        
    }

private:
    void ParseObject(const nlohmann::json& json_obj, const Protocol& protocol) {
        std::unordered_map<std::string, int> arrayCounts;

        for (const auto& field : protocol.fields) {
            if (json_obj.contains(field.name)) {
                const auto& value = json_obj[field.name];
                if (field.type == "object") {
                    // Recursively parse subfields of the object
                    ParseNestedObject(value, field.name, field.subFields);
                } else if (field.isArray) {
                    // Handle array fields
                    ParseArray(value, field, arrayCounts);
                } else {
                    // Handle regular fields
                    AddFieldToChannel(field.name, value, field.type);
                }
            }
        }
    }
    void ParseNestedObject(const nlohmann::json &json_obj, const std::string &prefix, const std::vector<Field> &subFields) {
    for (const auto &subField : subFields) {
        if (json_obj.contains(subField.name)) {
            const auto &value = json_obj[subField.name];
            const std::string fullName = prefix + "." + subField.name;

            if (subField.type == "object") {
                // Recursively parse nested objects
                ParseNestedObject(value, fullName, subField.subFields);
            } else if (subField.isArray) {
                // Handle arrays in nested objects
                std::unordered_map<std::string, int> nestedArrayCounts;
                ParseArray(value, subField, nestedArrayCounts);
            } else {
                // Add simple fields
                AddFieldToChannel(fullName, value, subField.type);
            }
        }
    }
}
    
    void ParseArray(const nlohmann::json &json_array, const Field &field, std::unordered_map<std::string, int> &arrayCounts) {
        const std::string arrayName = field.name + ".array";
        const std::string arraySupportName = "arraysupport." + field.name;

        int initialCount = arrayCounts[arrayName];

        // Append array elements to the array container
        for (const auto &item : json_array) {
            AddFieldToChannel(arrayName, item, field.type);
            initialCount++;
        }

        // Update arraysupport container with the cumulative count
        arrayCounts[arrayName] = initialCount;
        AddFieldToChannel(arraySupportName, initialCount, "integer");
    }

    void AddFieldToChannel(const std::string &field_name, const nlohmann::json &value, const std::string &field_type) {
        DataContainer::Channel *channel = nullptr;

        // Create or retrieve the channel
        if (!container->GetChannel(field_name)) {
            if (field_type == "double") {
                channel = container->AddChannel<double>(field_name);
            } else if (field_type == "integer") {
                channel = container->AddChannel<int>(field_name);
            } else if (field_type == "string") {
                channel = container->AddChannel<std::string>(field_name);
            }
        } else {
            channel = container->GetChannel(field_name);
        }

        // Add the value to the channel
        if (channel != nullptr) {
            if (field_type == "double") {
                channel->MutableData<double>(-1)->push_back(value.get<double>());
            } else if (field_type == "integer") {
                channel->MutableData<int>(-1)->push_back(value.get<int>());
            } else if (field_type == "string") {
                channel->MutableData<std::string>(-1)->push_back(value.get<std::string>());
            }
        }
    }


    DataContainer* container;
};

}


#endif // READ_FILES_HPP
