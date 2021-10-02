#include <iostream>
#include <cxxopts.hpp>
#include "include/shortener.h"

void print(const std::string& string){
    std::cout << string << std::endl;
}
void printxml(Json Response){
    std::string code = std::to_string(int(Response["code"]));
    std::string result = Response["result"];
    std::string err = Response["err"]?"true":"fasle";
    std::string xml = fmt::format("<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n<root>\n\t<err>{0}</err>\n\t<code>{1}</code>\n\t<result>{2}</result>\n</root>", err, code, result);
    std::cout << xml << std::endl;
}

int main(int argc, char** argv)
{
    std::string logo = R"(
   ______       _      __
  / __/ /  ____(_)__  / /__
 _\ \/ _ \/ __/ / _ \/  '_/
/___/_//_/_/ /_/_//_/_/\_\
)";
    Shortener Sh;
    cxxopts::Options Options("shrink", "short links using command line");
    Options.add_options("Required", {{"u,uri", "long url", cxxopts::value<std::string>()}});
    Options.add_options("Optinal", {
                            {"a,alias", "short alias (if service support)", cxxopts::value<std::string>()},
                            {"s,service", "shortening service, " + Sh.ListServices(), cxxopts::value<std::string>()},
                            {"f,format", "output format, " + Sh.ListFormats(), cxxopts::value<std::string>()},
                            {"h,help", "this help message"},
                            {"v,version", "print shrink version"}});
    try {
        auto result = Options.parse(argc, argv);
        if (result.count("version")){
            print(R"(
   ______       _      __
  / __/ /  ____(_)__  / /__
 _\ \/ _ \/ __/ / _ \/  '_/
/___/_//_/_/ /_/_//_/_/\_\
)");
            print(Sh.vrac());
            print("about: command line tool to short long links directly from your cmd.");
            print("auther: Anas Yousef <anasybal@protonmail.com>.");
            exit(0);
        }
        if (result.count("help") || !result.count("uri")){
            print(logo);
            std::cout << Options.help() << std::endl;
            exit(0);
        }
        if(result.count("uri")){
            std::string format, alias="", service;
            if(result.count("alias")){
                alias = result["alias"].as<std::string>();
            }
            if(!Sh.LinkValidate(result["uri"].as<std::string>())){
                std::cout << "this link isn't Valid, please try again with valid one." << std::endl;
                exit(1);
            }
            if(result.count("format")){
                if(!Sh.FormatExist(result["format"].as<std::string>())){
                    std::cout << "the format `" << result["format"].as<std::string>() << "` isn't accepted." << std::endl;
                    exit(1);
                }else{
                    format = result["format"].as<std::string>();
                }
            }else{
                format = "cli";
            }
            if(result.count("service")){
                if(!Sh.ServiceExist(result["service"].as<std::string>())){
                    std::cout << "the service `" << result["service"].as<std::string>() << "` isn't accepted." << std::endl;
                    exit(1);
                }else{
                    service = result["service"].as<std::string>();
                }
            }else{
                service = "is.gd";
            }
            Json Response = Sh.Short(result["uri"].as<std::string>(), alias, service);
            if(format == "text"){
                if(Response["err"]){
                    std::cerr << "Error: " << Response["result"] << std::endl;
                    exit(Response["code"]);
                }else{
                    std::cout << std::string(Response["result"]) << std::endl;
                }
            }else if(format == "cli"){
                if(Response["err"]){
                    std::cerr << "Error: " << Response["result"] << std::endl;
                    exit(Response["code"]);
                }else{
                    std::string resp = R"(
    Url Shortened Successfully.
    ---------------------------
    short : )";
                    resp += Response["result"];
                    print(logo);
                    std::cout << resp << std::endl << std::endl;
                }
            }else if(format == "json"){
                if(Response["err"]){
                    std::cerr << Response["result"].dump(4) << std::endl;
                    exit(Response["code"]);
                }else{
                    print(Response.dump(4));
                }
            }else if(format == "xml"){
                if(Response["err"]){
                    printxml(Response);
                    exit(Response["code"]);
                }else{
                    printxml(Response);
                }
            }
        }
    }  catch (cxxopts::option_not_exists_exception) {
        print(logo);
        std::cout << Options.help() << std::endl;
        exit(1);
    }
}
