#include <iostream>
#include <set>
#include <unordered_map>
#include <string>
#include <vector>
#include <tuple>

std::unordered_map<std::string, int> roomsTaken;
std::vector<std::tuple<int, std::string, int, int>> brones;
std::unordered_map<std::string, std::set<std::pair<int, int> > > clientsInHotel;

int main() {
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
            }
            clientsInHotel[hotel_name].erase(it);
            clientsInHotel[hotel_name].insert({client_id, count + 1});
            while (std::get<0>(brones[p]) >= time) {
                int oldClient = std::get<2>(brones[p]);
                int oldRoomCount = std::get<3>(brones[p]);
                std::string oldHotel = std::get<1>(brones[p]);
                roomsTaken[oldHotel] -= oldRoomCount;
                auto oldIt = clientsInHotel[oldHotel].upper_bound({oldClient, -1});
                int count = oldIt->second;
                if (oldIt == clientsInHotel[oldHotel].end() || oldIt->first != oldClient) {
                    oldIt = clientsInHotel[oldHotel].insert({oldClient, 0}).first;
                }
                clientsInHotel[hotel_name].erase(oldIt);
                clientsInHotel[hotel_name].insert({client_id, count + 1});
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
}