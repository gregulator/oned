#include "oned/config_reader.hpp"

#include <iostream>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

int main() {
    // Create a ConfigReader object
    oned::ConfigReader configReader;
    
    if (!configReader.loadXML("./oned/configs/jsonConfig.xml")) {
        std::cerr << "Failed to load XML configuration!" << std::endl;
        return -1;
    }

    // Retrieve the protocol by name (e.g., "JSON")
    const oned::Protocol* jsonProtocol = configReader.getProtocol("JSON");
    if (jsonProtocol) {
        std::cout << "Protocol: " << jsonProtocol->name << std::endl;

        // Iterate over the fields of the protocol and print them
        for (const auto& field : jsonProtocol->fields) {
            std::cout << "Field: " << field.name << " (Type: " << field.type << ")" << std::endl;

            // If the field is an object, print its subfields
            if (field.type == "object") {
                std::cout << "  Subfields: " << std::endl;
                for (const auto& subField : field.subFields) {
                    std::cout << "    SubField: " << subField.name << " (Type: " << subField.type << ")" << std::endl;
                }
            }

            // If the field is an array, indicate it's an array
            if (field.isArray) {
                std::cout << "  This field is an array" << std::endl;
            }
        }
    } else {
        std::cerr << "Protocol not found!" << std::endl;
    }

    return 0;
}
