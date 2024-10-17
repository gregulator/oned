#ifndef CONFIG_READER_HPP
#define CONFIG_READER_HPP

#include <string>
#include <fstream> 
#include <vector>
#include <unordered_map>
#include "tinyxml2.h"
#include <iostream>
#include "oned/data_container.hpp" 
#include <memory>
#include <nlohmann/json.hpp>

namespace oned
{
    struct Field
    {
        std::string name;
        std::string type;
        bool isArray;
        std::vector<Field> subFields; // To store nested fields (for objects)

        Field(const std::string &n, const std::string &t, bool arr = false)
            : name(n), type(t), isArray(arr) {}
    };

    class Protocol
    {
    public:
        std::string name;
        std::vector<Field> fields;

        Protocol(const std::string &protocolName) : name(protocolName) {}

        void addField(const Field &field)
        {
            fields.push_back(field);
        }
    };

    class ConfigReader
    {
    private:
        std::unordered_map<std::string, Protocol> protocols;

        void parseProtocol(tinyxml2::XMLElement* protocolElement) {
            const char* protocolName = protocolElement->Attribute("Name");
            if (!protocolName) {
                std::cerr << "Protocol name not found!" << std::endl;
                return;
            }

            Protocol protocol(protocolName);
            tinyxml2::XMLElement* fieldElement = protocolElement->FirstChildElement("Field");

            // Parse all fields within the protocol
            while (fieldElement) {
                parseField(fieldElement, protocol);
                fieldElement = fieldElement->NextSiblingElement("Field");
            }

            // Store protocol in map
            protocols.insert({protocol.name, protocol});

        }
        void parseField(tinyxml2::XMLElement *fieldElement, Protocol &protocol, bool isArray = false){
            const char* fieldName = fieldElement->Attribute("Name");
            const char* fieldType = fieldElement->Attribute("Type");

            if (!fieldName || !fieldType) {
                std::cerr << "Field attributes are missing!" << std::endl;
                return;
            }

            // Check if this field is an object
            if (strcmp(fieldType, "object") == 0) {
                Field objectField(fieldName, "object");

                // Parse subfields inside the object
                tinyxml2::XMLElement* subFieldElement = fieldElement->FirstChildElement("SubField");
                while (subFieldElement) {
                    parseSubField(subFieldElement, objectField);
                    subFieldElement = subFieldElement->NextSiblingElement("SubField");
                }

                protocol.addField(objectField);
            }
            // Check if this field is an array
            else if (isArray) {
                protocol.addField(Field(fieldName, fieldType, true));
            } 
            else if (strcmp(fieldType, "array") == 0) {
                // If the field is an array, parse the nested ArrayField elements
                tinyxml2::XMLElement* arrayFieldElement = fieldElement->FirstChildElement("ArrayField");
                while (arrayFieldElement) {
                    parseField(arrayFieldElement, protocol, true);
                    arrayFieldElement = arrayFieldElement->NextSiblingElement("ArrayField");
                }
            } 
            else {
                // Regular field
                protocol.addField(Field(fieldName, fieldType));
            }
        }
        void parseSubField(tinyxml2::XMLElement *subFieldElement, Field &parentField){
            const char* subFieldName = subFieldElement->Attribute("Name");
            const char* subFieldType = subFieldElement->Attribute("Type");

            if (!subFieldName || !subFieldType) {
                std::cerr << "SubField attributes are missing!" << std::endl;
                return;
            }

            // Add the subfield to the parent object field
            parentField.subFields.push_back(Field(subFieldName, subFieldType));
        }

    public:
        ConfigReader() = default;
        bool loadXML(const std::string &fileName){
            tinyxml2::XMLDocument doc;
            if (doc.LoadFile(fileName.c_str()) != tinyxml2::XML_SUCCESS) {
                std::cerr << "Failed to load XML file!" << std::endl;
                return false;
            }

            tinyxml2::XMLElement* root = doc.RootElement();
            if (!root) {
                std::cerr << "Invalid XML file!" << std::endl;
                return false;
            }

            // Parse each protocol element
            std::cout << "Root element name: " << root->Name() << std::endl;
            parseProtocol(root);
            tinyxml2::XMLElement* protocolElement = root->NextSiblingElement("Protocol");
            while (protocolElement) {
                parseProtocol(protocolElement);
                protocolElement = protocolElement->NextSiblingElement("Protocol");
            }

            return true;
        }
        const Protocol *getProtocol(const std::string &protocolName) const{
            auto it = protocols.find(protocolName);
            if (it != protocols.end()) {
                return &it->second;
            }
            return nullptr;
        }
    };
    

}
#endif // CONFIG_READER_HPP
