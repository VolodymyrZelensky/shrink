#ifndef SHORTENER_H
#define SHORTENER_H

#include <iostream>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <fmt/core.h>

using Json = nlohmann::json;

class Shortener
{
public:
    Shortener();
    std::string version = "1.0.0";
    std::string version_code = "beta";
    std::string vrac(){return fmt::format("version: {0}, {1}", this->version, this->version_code);}
    bool ServiceExist(const std::string& service);
    bool FormatExist(const std::string& format);
    bool LinkValidate(const std::string& uri);
    std::string ListServices();
    std::string ListFormats();
    Json Short(const std::string& uri, const std::string& alias, const std::string& service);

private:
    bool StartWith(const std::string& string, const std::string &var);
    std::string Services[4] = {
        "is.gd",
        "bit.ly",
        "git.io",
        "0i.is"
    };
    std::string Formats[4] = {
        "text",
        "cli",
        "json",
        "xml"
    };
    Json isgd(const std::string& uri, const std::string& alias);
    Json bitly(const std::string& uri, const std::string& alias);
    Json Oiis(const std::string& uri, const std::string& alias);
    Json git(const std::string& uri);
};

#endif // SHORTENER_H
