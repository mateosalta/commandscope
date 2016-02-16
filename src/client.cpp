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
///
/// \brief Client::get
/// \param path
/// \param parameters
/// \param root
///
void Client::get(const net::Uri::Path &path,
                 const net::Uri::QueryParameters &parameters, QJsonDocument &root) {
    auto client = http::make_client();
    http::Request::Configuration configuration;
    net::Uri uri = net::make_uri(config_->apiroot, path, parameters);
    configuration.uri = client->uri_to_string(uri);
    configuration.header.add("User-Agent", config_->user_agent);
    auto request = client->head(configuration);
    try {
        auto response = request->execute(
                    bind(&Client::progress_report, this, placeholders::_1));

        if (response.status != http::Status::ok) {
            throw domain_error(response.body);
        }
        root = QJsonDocument::fromJson(response.body.c_str());
    } catch (net::Error &) {
    }
}
///
/// \brief Client::command_list
/// \param query
/// \param new_sys
/// \param sort_sys
/// \return
///
Client::Command Client::command_list(const string& query, const string& new_sys, const string& sort_sys) {
    QJsonDocument root;
    ///// SPECIAL CODE! THANK YOU NATHAN OSMAN!!!!!!!!!!!!!!
    auto text = QByteArray::fromStdString(query).toBase64();
    ////////////////////////////////////////////////////////
    get( { "commands", new_sys, query, text.toStdString(), sort_sys, "json"},
    {}, root);
    // http://www.commandlinefu.com/commands/browse/json
    Command result;
    QVariantList variant;
        variant = root.toVariant().toList();
    for (const QVariant &i : variant) {
        QVariantMap item = i.toMap();
        result.commands.emplace_back(
                    Commands { item["id"].toString().toStdString(),
                              item["command"].toString().toStdString(),
                              item["summary"].toString().toStdString(),
                              item["votes"].toString().toStdString(),
                              item["url"].toString().toStdString(),
                                });
    }
    return result;
}
///
/// \brief Client::progress_report
/// \return
///
http::Request::Progress::Next Client::progress_report(
        const http::Request::Progress&) {
    return cancelled_ ?
                http::Request::Progress::Next::abort_operation :
                http::Request::Progress::Next::continue_operation;
}
///
/// \brief Client::cancel
///
void Client::cancel() {
    cancelled_ = true;
}
///
/// \brief Client::config
/// \return
///
Client::Config::Ptr Client::config() {
    return config_;
}
