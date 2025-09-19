#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <mutex>
#include <regex>
#include <ctime>
#include "httplib.h"
#include "json.hpp"

using json = nlohmann::json;

struct Offer {
    std::string user_id, title, desc, want, img;
};

static std::mutex g_mutex;
static std::vector<Offer> g_offers;

// Validate simple text fields to avoid injection/garbage
bool safe_text(const std::string& s, size_t max=64) {
    static const std::regex re("^[a-zA-Z0-9 _\\-.,]{1,64}$");
    return !s.empty() && s.size() <= max && std::regex_match(s, re);
}

// Very basic match finder (2-cycle trades)
json find_matches() {
    std::unordered_map<std::string, std::vector<int>> idx_by_have;
    for (int i=0; i<(int)g_offers.size(); ++i) {
        idx_by_have[g_offers[i].title].push_back(i);
    }

    json results = json::array();
    std::unordered_set<std::string> seen;

    for (int i=0; i<(int)g_offers.size(); ++i) {
        for (int j : idx_by_have[g_offers[i].want]) {
            if (i == j) continue;
            if (g_offers[j].want == g_offers[i].title && g_offers[i].user_id != g_offers[j].user_id) {
                std::string key = std::to_string(std::min(i,j)) + "-" + std::to_string(std::max(i,j));
                if (!seen.count(key)) {
                    seen.insert(key);
                    results.push_back({
                        {"type","2-cycle"},
                        {"offers",{ 
                            {{"user_id",g_offers[i].user_id},{"give",g_offers[i].title},{"get",g_offers[i].want}},
                            {{"user_id",g_offers[j].user_id},{"give",g_offers[j].title},{"get",g_offers[j].want}}
                        }}
                    });
                }
            }
        }
    }
    return results;
}

int main() {
    httplib::Server svr;

    // -------------------------------------------------------------------------
    // Upload handler WITHOUT get_file_value
    // -------------------------------------------------------------------------
    // We manually parse the multipart form-data.
    // This is minimal parsing just for MVP demo, not production hardened.
    svr.Post("/api/upload", [](const httplib::Request& req, httplib::Response& res) {
        // Content-Type header will look like: multipart/form-data; boundary=----WebKitFormBoundaryabc123
        auto content_type = req.get_header_value("Content-Type");
        std::string boundary_key = "boundary=";
        auto pos = content_type.find(boundary_key);
        if (pos == std::string::npos) {
            res.status = 400;
            res.set_content("{\"error\":\"Missing boundary\"}", "application/json");
            return;
        }
        std::string boundary = "--" + content_type.substr(pos + boundary_key.size());

        // Find start of file content
        auto body = req.body;
        auto start = body.find("\r\n\r\n"); // end of headers inside multipart section
        if (start == std::string::npos) {
            res.status = 400;
            res.set_content("{\"error\":\"Invalid multipart data\"}", "application/json");
            return;
        }
        start += 4; // skip the header separator

        // Find the boundary marking the end of the file
        auto end = body.find(boundary, start);
        if (end == std::string::npos) {
            end = body.size();
        }

        // Extract the raw file content
        std::string file_content = body.substr(start, end - start);
        
        // Generate a filename (timestamp based)
        std::string fname = std::to_string(std::time(nullptr)) + ".bin";
        std::string path = "./uploads/" + fname;

        // Save the file
        std::ofstream ofs(path, std::ios::binary);
        ofs.write(file_content.data(), file_content.size());
        ofs.close();

        // Respond with the URL to access it
        json resp = {{"url", "/uploads/" + fname}};
        res.set_content(resp.dump(), "application/json");
    });

    // -------------------------------------------------------------------------
    // Add new offer
    // -------------------------------------------------------------------------
    svr.Post("/api/offer",[](const httplib::Request& req, httplib::Response& res){
        auto j = json::parse(req.body);
        Offer o{j["user_id"], j["title"], j["desc"], j["want"], j["img"]};
        if (!safe_text(o.user_id,32) || !safe_text(o.title) || !safe_text(o.want)) {
            res.status = 400; 
            res.set_content("{\"error\":\"Invalid input\"}", "application/json");
            return;
        }
        std::lock_guard<std::mutex> lk(g_mutex);
        g_offers.push_back(o);
        res.set_content("{\"ok\":true}", "application/json");
    });

    // -------------------------------------------------------------------------
    // Get all offers
    // -------------------------------------------------------------------------
    svr.Get("/api/offers",[](const httplib::Request&, httplib::Response& res){
        std::lock_guard<std::mutex> lk(g_mutex);
        json arr = json::array();
        for (auto& o : g_offers) arr.push_back({
            {"user_id",o.user_id},{"title",o.title},{"desc",o.desc},{"want",o.want},{"img",o.img}
        });
        res.set_content(arr.dump(), "application/json");
    });

    // -------------------------------------------------------------------------
    // Find matches
    // -------------------------------------------------------------------------
    svr.Post("/api/match",[](const httplib::Request&, httplib::Response& res){
        auto matches = find_matches();
        res.set_content(matches.dump(), "application/json");
    });

    // Serve uploaded files
    svr.set_mount_point("/uploads", "./uploads");

    svr.listen("0.0.0.0",8081);
}
