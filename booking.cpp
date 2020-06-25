#include <iostream>
#include <set>
#include <unordered_map>
#include <string>
#include <vector>
#include <ios>
#include <tuple>

typedef long long ll;
#define int ll
std::unordered_map<std::string, int> roomsTaken;
std::vector<std::tuple<int, std::string, int, int>> brones;
std::unordered_map<std::string, std::set<std::pair<int, int> > > clientsInHotel;

signed main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int q;
    std::cin >> q;

    int p = 0;

    while (q--) {
        std::string type;
        std::cin >> type;

        if (type == "BOOK") {
            int time, client_id, room_count;
            std::string hotel_name;
            std::cin >> time >> hotel_name >> client_id >> room_count;

            auto t = std::make_tuple(time, hotel_name, client_id, room_count);
            brones.push_back(t);
            roomsTaken[hotel_name] += room_count;

            auto it = clientsInHotel[hotel_name].upper_bound({client_id, -1});
            int count = it->second;
            if (it == clientsInHotel[hotel_name].end() || it->first != client_id) {
                it = clientsInHotel[hotel_name].insert({client_id, 0}).first;
                count = 0;
            }
            clientsInHotel[hotel_name].erase(it);
            clientsInHotel[hotel_name].insert({client_id, count + 1});

            while (time - 86400ll >= std::get<0>(brones[p]) && p != (int) brones.size() - 1) {
                int oldClient = std::get<2>(brones[p]);
                int oldRoomCount = std::get<3>(brones[p]);
                std::string oldHotel = std::get<1>(brones[p]);
                roomsTaken[oldHotel] -= oldRoomCount;

                auto oldIt = clientsInHotel[oldHotel].upper_bound({oldClient, -1});
                int count = oldIt->second;
                clientsInHotel[oldHotel].erase(oldIt);
                if (count - 1 > 0) {
                    clientsInHotel[oldHotel].insert({oldClient, count - 1});
                }
                p++;
            }
        } else if (type == "CLIENTS") {
            std::string hotel;
            std::cin >> hotel;
            std::cout << clientsInHotel[hotel].size() << "\n";
        } else if (type == "ROOMS") {
            std::string hotel;
            std::cin >> hotel;
            std::cout << roomsTaken[hotel] << "\n";
        }
    }
    return 0;
}