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

/**
 * Provide a nice way to access the HTTP API.
 *
 * We don't want our scope's code to be mixed together with HTTP and JSON handling.
 */
class Client {
public:

    /**
     * Client configuration
     */
    struct Config {
        typedef std::shared_ptr<Config> Ptr;

        // The root of all API request URLs
        std::string apiroot { "http://www.commandlinefu.com" };

        // The custom HTTP user agent string for this library
        std::string user_agent { "example-network-scope 0.1; (foo)" };
    };



    /**
     * Weather information for a day.
     */
    struct Weather {
        std::string id;
        std::string command;
        std::string summary;
        std::string votes;
        std::string url;

    };

    /**
     * A list of weather information
     */
    typedef std::deque<Weather> WeatherList;

    /**
     * Weather information about the current day
     */
    struct Current {
        Weather weather;
    };

    /**
     * Forecast information about a city
     */
    struct Forecast {
        WeatherList weather;
    };

    Client(Config::Ptr config);

    virtual ~Client() = default;

    /**
     * Get the weather forecast for the specified location and duration
     */
    virtual Forecast forecast_daily(const std::string &query);

    /**
     * Cancel any pending queries (this method can be called from a different thread)
     */
    virtual void cancel();

    virtual Config::Ptr config();

protected:
    void get(const core::net::Uri::Path &path,
             const core::net::Uri::QueryParameters &parameters,
             QJsonDocument &root);
    /**
     * Progress callback that allows the query to cancel pending HTTP requests.
     */
    core::net::http::Request::Progress::Next progress_report(
            const core::net::http::Request::Progress& progress);

    /**
     * Hang onto the configuration information
     */
    Config::Ptr config_;

    /**
     * Thread-safe cancelled flag
     */
    std::atomic<bool> cancelled_;
};

#endif // CLIENT_H_

