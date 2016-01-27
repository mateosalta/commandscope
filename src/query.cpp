#include <query.h>
#include <localization.h>

#include <unity/scopes/Annotation.h>
#include <unity/scopes/CategorisedResult.h>
#include <unity/scopes/CategoryRenderer.h>
#include <unity/scopes/QueryBase.h>
#include <unity/scopes/SearchReply.h>

#include <iomanip>
#include <iostream>
#include <sstream>

namespace sc = unity::scopes;

using namespace std;




const static string FORECAST_TEMPLATE =
        R"(
{
        "schema-version": 1,
        "template": {
        "category-layout": "vertical-journal",
        "card-layout": "horizontal",
        "card-size": "small"
        },
        "components": {
        "title": "title",
        "emblem" : {
        "field": "mascot"
        },
        "subtitle": "subtitle",
        "summary": "summary"
        }
        }
        )";

Query::Query(const sc::CannedQuery &query, const sc::SearchMetadata &metadata,
             Client::Config::Ptr config) :
    sc::SearchQueryBase(query, metadata), client_(config) {
}

void Query::cancelled() {
    client_.cancel();
}


void Query::run(sc::SearchReplyProxy const& reply) {
    try {
        // Start by getting information about the query
        const sc::CannedQuery &query(sc::SearchQueryBase::query());

        // Get the query string
        string query_string = query.query_string();

        /// Populate current weather category

        // the Client is the helper class that provides the results
        // without mixing APIs and scopes code.
        // Add your code to retreive xml, json, or any other kind of result
        // in the client.

        Client::Forecast forecast;
        if (query_string.empty()) {
            // If there is no search string, get the forecast for London
            forecast = client_.forecast_daily("ssh");
        } else {
            // otherwise, get the forecast for the search string
            forecast = client_.forecast_daily(query_string);
        }

        // Register a category for the forecast
        auto forecast_cat = reply->register_category("forecast", _("Commands"), "",
                                                     sc::CategoryRenderer(FORECAST_TEMPLATE));

        // For each of the forecast days
        for (const auto &weather : forecast.weather) {
            // Create a result
            sc::CategorisedResult res(forecast_cat);

            // We must have a URI
            res.set_uri(weather.url);

            res["title"] = weather.summary;

            // Set the rest of the attributes
            res["mascot"] = "http://www.commandlinefu.com/images/favicon.ico";
            res["subtitle"] = "⇳" + weather.votes;
            res["summary"] = weather.command;

            // Push the result
            if (!reply->push(res)) {
                // If we fail to push, it means the query has been cancelled.
                // So don't continue;
                return;
            }
        }

    } catch (domain_error &e) {
        // Handle exceptions being thrown by the client API
        //cerr << e.what() << endl;
        reply->error(current_exception());
    }
}

