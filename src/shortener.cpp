#include "../include/shortener.h"

Shortener::Shortener()
{

}

bool Shortener::ServiceExist(const std::string &service)
{
    bool status = false;
    for(int srv=0; srv < this->Services->size(); srv++){
        if(this->Services[srv] == service){
            status = true;
            break;
        }
    }
    return status;
}

bool Shortener::FormatExist(const std::string &format)
{
    bool status = false;
    for(int frm=0; frm < this->Services->size(); frm++){
        if(this->Formats[frm] == format){
            status = true;
            break;
        }
    }
    return status;
}

bool Shortener::LinkValidate(const std::string &uri)
{
    if(this->StartWith("https://", uri) || this->StartWith("http://", uri) || this->StartWith("www.", uri)){
        return true;
    }else{
        return false;
    }
}

std::string Shortener::ListServices()
{
    std::string Servs = "(";
    int o(this->Services->size()-1);
    for(int i(0); i < o; i++){
        if(i == o-1){
            Servs += this->Services[i];
        }else{
            Servs += this->Services[i] + ", ";
        }
    }
    Servs += ")";
    return Servs;
}

std::string Shortener::ListFormats()
{
    std::string Formts = "(";
    for(int i(0); i < this->Formats->size(); i++){
        if(i  == this->Formats->size()-1){
            Formts += this->Formats[i];
        }else{
            Formts += this->Formats[i] + ", ";
        }
    }
    Formts += ")";
    return Formts;
}

Json Shortener::Short(const std::string &uri, const std::string &alias, const std::string &service)
{
    Json result;
    if(service == "is.gd")          result = this->isgd(uri, alias);
    else if (service == "0i.is")    result = this->Oiis(uri, alias);
    else if (service == "bit.ly")   result = this->bitly(uri, alias);
    else if (service == "git.io")   result = this->git(uri);
    return result;
}

bool Shortener::StartWith(const std::string &string, const std::string &var)
{
    std::string value="";
    for (int i=0;i<string.size();i++){
        value += var[i];
    }
    return (value == string);
}

Json Shortener::isgd(const std::string &uri, const std::string &alias)
{
    std::string URL = "https://is.gd/create.php?format=json&url=";
    URL += uri;
    if(alias != ""){
        URL += "&shorturl=";
        URL += alias;
    }
    cpr::Response result = cpr::Get(cpr::Url{URL});
    if(result.status_code == 200){
        auto Js3 = Json::parse(result.text);
        if(Js3["shorturl"].is_string()){
            Json success = {
                {"err", false},
                {"code", 0},
                {"result", Js3["shorturl"]}
            };
            return success;
        }else if(Js3["errorcode"].is_number()){
            Json error = {
                {"error", true},
                {"code", 1},
                {"result", Js3["errormessage"]}
            };
            return error;
        }else{
            Json unknown_error = {
                {"error", true},
                {"code", -1},
                {"result", "UNKNOWN_ERROR"}
            };
            return unknown_error;
        }
    }else{
        Json error = {
            {"error", true},
            {"code", -2},
            {"result", "error while connecting to the server / check you're network connection"}
        };
        return error;
    }
}

Json Shortener::bitly(const std::string &uri, const std::string &alias)
{
    (void)alias;
    Json Body = {
        {"long_url", uri}
    };
    cpr::Response result = cpr::Post(
                cpr::Url{"https://api-ssl.bitly.com/v4/shorten"},
                cpr::Header{
                    {"Content-Type", "application/json"},
                    {"Authorization", "Bearer 554c038521a07cec6f0dcc205a8983f686690772"}
                },
                cpr::Body{
                    Body.dump()
                }
    );
    if(result.status_code == 201 || result.status_code == 200){
        auto Js3 = Json::parse(result.text);
        if(Js3["link"].is_string()){
            Json success = {
                {"err", false},
                {"code", 0},
                {"result", Js3["link"]}
            };
            return success;
        }else if(Js3["errors"].is_array()){
            Json error = {
                {"error", true},
                {"code", 1},
                {"result", Js3["description"]}
            };
            return error;
        }else{
            Json unknown_error = {
                {"error", true},
                {"code", -1},
                {"result", "UNKNOWN_ERROR"}
            };
            return unknown_error;
        }
    }else{
        Json error = {
            {"error", true},
            {"code", -2},
            {"result", "error while connecting to the server / check you're network connection"}
        };
        return error;
    }
}

Json Shortener::Oiis(const std::string &uri, const std::string &alias)
{
    std::string URL = "https://0i.is/api/?key=ncwZVsy2Yvyf&url=";
    URL += uri;
    if(alias != ""){
        URL += "&custom=";
        URL += alias;
    }
    cpr::Response result = cpr::Get(
                cpr::Url{URL},
                cpr::Header{{"User-Agent", "Mozilla/5.0 (X11; Linux x86_64; rv:78.0) Gecko/20100101 Firefox/78.0"}}
                );
    if(result.status_code == 200){
        auto Js3 = Json::parse(result.text);
        if(Js3["short"].is_string() && Js3["error"] == 0){
            Json success = {
                {"err", false},
                {"code", 0},
                {"result", Js3["short"]}
            };
            return success;
        }else if(Js3["error"] != 0){
            Json error = {
                {"error", true},
                {"code", 1},
                {"result", Js3["msg"]}
            };
            return error;
        }else{
            Json unknown_error = {
                {"error", true},
                {"code", -1},
                {"result", "UNKNOWN_ERROR"}
            };
            return unknown_error;
        }
    }else{
        Json error = {
            {"error", true},
            {"code", -2},
            {"result", "error while connecting to the server / check you're network connection"}
        };
        return error;
    }
}

Json Shortener::git(const std::string &uri)
{
    cpr::Response result = cpr::Post(
                cpr::Url{"https://git.io/create"},
                cpr::Body{"url="+uri}
    );
    if(result.status_code == 200){
        Json success = {
            {"err", false},
            {"code", 0},
            {"result", "https://git.io/" + result.text}
        };
        return success;
    }else{
        Json error = {
            {"error", true},
            {"code", -2},
            {"result", result.text}
        };
        return error;
    }
}
