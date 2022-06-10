//--------------------------------------------------
// Atta IO System
// httpCurlpp.cpp
// Date: 2022-06-08
// By Breno Cunha Queiroz
//--------------------------------------------------
// The .cpp file with the http implementation is chosen at compile time

#include <atta/ioSystem/http/http.h>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Exception.hpp>
#include <curlpp/Infos.hpp>

namespace atta::io
{
    const std::string Http::implType = "curlpp";
    const bool Http::supported = true;

    Http::Response Http::request(const Http::Request& req, Http::RequestType reqType)
    {
        Http::Response res {};
        try
        {
            //---------- Build request ----------//
            curlpp::Cleanup myCleanup;
            // Populate params
            std::string urlWithParams = _url;
            for(const auto& [key, value] : req.params)
                urlWithParams += "&" + key + "=" + value;

            // Create request
            curlpp::Easy creq;
            creq.setOpt(curlpp::options::Url(urlWithParams));

            // Setup request type
            if(reqType == GET)
            {}
            else if(reqType == PUT)
                creq.setOpt(curlpp::options::CustomRequest{"PUT"});
            else if(reqType == POST)
                creq.setOpt(curlpp::options::CustomRequest{"POST"});

            // Populate headers
            std::list<std::string> headers;
            for(const auto& [key, value] : req.headers)
                headers.push_back(key + ": " + value);
            if(headers.size() > 0)
                creq.setOpt(curlpp::options::HttpHeader(headers));

            // Set body helper
            std::ostringstream os;
            curlpp::options::WriteStream ws(&os);
		    creq.setOpt(ws);

            // Set header helper
            std::string headersBuffer;
            creq.setOpt(curlpp::options::HeaderFunction(
                [&headersBuffer](char* ptr, size_t size, size_t nbItems)
                {
                    const auto incomingSize = size * nbItems;
                    headersBuffer.append(ptr, incomingSize);
                    return incomingSize;
                }));

            //---------- Do request ----------//
            creq.perform();

            //---------- Get response ----------//
            // Get body
            res.body = os.str();

            // Get status
            res.statusCode = curlpp::infos::ResponseCode::get(creq);

            // Get headers
            res.body += headersBuffer;
            unsigned curr = 0;
            while(curr < headersBuffer.size())
            {
                size_t nlPos = headersBuffer.find('\n', curr);
                size_t dotPos = headersBuffer.find(':', curr);

                if(dotPos != std::string::npos && nlPos != std::string::npos)
                {
                    std::string key = headersBuffer.substr(curr, dotPos-curr);
                    std::string value = headersBuffer.substr(dotPos+2, nlPos-dotPos-2);
                    res.headers[key] = value;
                }

                if(nlPos != std::string::npos)
                    curr = nlPos+1;
                else
                    curr = headersBuffer.size();
            }
        }
        catch(curlpp::RuntimeError &e)
        {
            LOG_WARN("io::Http", "Could not perform request to [w]$0[]. Runtime error: [w]$1", _url, e.what());
        }
        catch(curlpp::LogicError &e)
        {
            LOG_WARN("io::Http", "Could not perform request to [w]$0[]. Logic error: [w]$1", _url, e.what());
        }
        return res;
    }
}
