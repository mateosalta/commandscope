#ifndef CLIENT_H_
#define CLIENT_H_

#include <atomic>
#include <deque>
#include <map>
#include <memory>
#include <string>
#include <core/net/http/request.h>
#include <core/net/uri.h>

#include <QJsonDocument>

class Client {
public:
    ///
    /// \brief The Config struct
    ///
    struct Config {
        typedef std::shared_ptr<Config> Ptr;

        // The root of all API request URLs
        std::string apiroot { "http://www.commandlinefu.com" };

        // The custom HTTP user agent string for this library
        std::string user_agent { "example-network-scope 0.1; (foo)" };
    };
    ///
    /// \brief The Weather struct
    ///
    struct Weather {
        std::string id;
        std::string command;
        std::string summary;
        std::string votes;
        std::string url;

    };
    ///
    /// \brief WeatherList
    ///
    typedef std::deque<Weather> WeatherList;
    ///
    /// \brief The Current struct
    ///
    struct Current {
        Weather weather;
    };
    ///
    /// \brief The Forecast struct
    ///
    struct Command {
        WeatherList weather;
    };
    ///
    /// \brief Client
    /// \param config
    ///
    Client(Config::Ptr config);

    virtual ~Client() = default;
    ///
    /// \brief forecast_daily
    /// \param query
    /// \param new_sys
    /// \param sort_sys
    /// \return
    ///
    virtual Command forecast_daily(const std::string &query, const std::string &new_sys, const std::string &sort_sys);
    ///
    /// \brief cancel
    ///
    virtual void cancel();
    ///
    /// \brief config
    /// \return
    ///
    virtual Config::Ptr config();
    ///
protected:
    void get(const core::net::Uri::Path &path,
             const core::net::Uri::QueryParameters &parameters,
             QJsonDocument &root);
    ///
    /// \brief progress_report
    /// \param progress
    /// \return
    ///
    core::net::http::Request::Progress::Next progress_report(
            const core::net::http::Request::Progress& progress);
    ///
    /// \brief config_
    ///
    Config::Ptr config_;
    ///
    /// \brief cancelled_
    ///
    std::atomic<bool> cancelled_;
};

#endif // CLIENT_H_

