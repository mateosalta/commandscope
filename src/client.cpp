#include <client.h>

#include <core/net/error.h>
#include <core/net/http/client.h>
#include <core/net/http/content_type.h>
#include <core/net/http/response.h>
#include <QVariantMap>
#include <QByteArray>

namespace http = core::net::http;
namespace net = core::net;

using namespace std;

Client::Client(Config::Ptr config) :
    config_(config), cancelled_(false) {
}


void Client::get(const net::Uri::Path &path,
                 const net::Uri::QueryParameters &parameters, QJsonDocument &root) {
    // Create a new HTTP client
    auto client = http::make_client();

    // Start building the request configuration
    http::Request::Configuration configuration;

    // Build the URI from its components
    net::Uri uri = net::make_uri(config_->apiroot, path, parameters);
    configuration.uri = client->uri_to_string(uri);

    // Give out a user agent string
    configuration.header.add("User-Agent", config_->user_agent);

    // Build a HTTP request object from our configuration
    auto request = client->head(configuration);

    try {
        // Synchronously make the HTTP request
        // We bind the cancellable callback to #progress_report
        auto response = request->execute(
                    bind(&Client::progress_report, this, placeholders::_1));

        // Check that we got a sensible HTTP status code
        if (response.status != http::Status::ok) {
            throw domain_error(response.body);
        }
        // Parse the JSON from the response
        root = QJsonDocument::fromJson(response.body.c_str());


    } catch (net::Error &) {
    }
}



Client::Forecast Client::forecast_daily(const string& query) {
    QJsonDocument root;

    ///// SPECIAL CODE! NON WORKING YET
    QByteArray text(query);
      text.toBase64();
    //////////////////////////////

    // Build a URI and get the contents
    // The fist parameter forms the path part of the URI.
    // The second parameter forms the CGI parameters.
    get( { "commands", "browse", "json", query, text },
    {}, root);
    // e.g. http://api.openweathermap.org/data/2.5/forecast/daily/?q=QUERY&units=metric&cnt=7
    // http://www.commandlinefu.com/commands/browse/json

    Forecast result;

    QVariantList variant;

        variant = root.toVariant().toList();

    for (const QVariant &i : variant) {
        QVariantMap item = i.toMap();

        // Extract the temperature data

        // Add a result to the weather list
        result.weather.emplace_back(
                    Weather { item["id"].toString().toStdString(),
                              item["command"].toString().toStdString(),
                              item["summary"].toString().toStdString(),
                              item["votes"].toString().toStdString(),
                              item["url"].toString().toStdString(),
                                });
    }

    return result;
}

http::Request::Progress::Next Client::progress_report(
        const http::Request::Progress&) {

    return cancelled_ ?
                http::Request::Progress::Next::abort_operation :
                http::Request::Progress::Next::continue_operation;
}

void Client::cancel() {
    cancelled_ = true;
}

Client::Config::Ptr Client::config() {
    return config_;
}

