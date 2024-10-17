
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
        if (protocol_name == "JSON") {
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
        nlohmann::json json_data;
        file >> json_data;

        ParseObject(json_data, protocol);
    }

private:
    void ParseObject(const nlohmann::json& json_obj, const Protocol& protocol){
       
    }
    void ParseArray(const nlohmann::json& json_array, const Protocol& protocol){
        
    }

    DataContainer* container;
};

}


#endif // READ_FILES_HPP
