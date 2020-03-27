#include <arpa/inet.h>
#include <assert.h>
#include <dirent.h>
#include <getopt.h>
#include <libgen.h>
#include <netinet/in.h>
#include <regex.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

string convert_ip(string str, bool ipv6) {
    stringstream ss(str);

    string token;
    uint32_t port;
    vector<char> addr_str(64);

    getline(ss, token, ':');
    if (!ipv6) {
        uint32_t addr_num;
        addr_num = stoul(token, nullptr, 16);
        struct in_addr addr = {addr_num};
        inet_ntop(AF_INET, &addr, addr_str.data(), 64);
    } else {
        uint32_t addr_num[4];
        addr_num[0] = stoul(token.substr(0, 8), nullptr, 16);
        addr_num[1] = stoul(token.substr(8, 8), nullptr, 16);
        addr_num[2] = stoul(token.substr(16, 8), nullptr, 16);
        addr_num[3] = stoul(token.substr(24, 8), nullptr, 16);
        struct in6_addr addr;
        addr.__in6_u.__u6_addr32[0] = addr_num[0];
        addr.__in6_u.__u6_addr32[1] = addr_num[1];
        addr.__in6_u.__u6_addr32[2] = addr_num[2];
        addr.__in6_u.__u6_addr32[3] = addr_num[3];
        inet_ntop(AF_INET6, &addr, addr_str.data(), 64);
    }

    ss >> token;
    port = stoul(token, nullptr, 16);

    stringstream result;
    result << addr_str.data() << ":" << ((port == 0) ? "*" : to_string(port));
    return result.str();
}

int main(int argc, char* argv[]) {
    const struct option longopts[] = {{"udp", 0, NULL, 'u'},
                                      {"tcp", 0, NULL, 't'}};
    const char* opts = "ut";
    char c;
    bool udp = false, tcp = false;
    while ((c = getopt_long(argc, argv, opts, longopts, NULL)) != -1) {
        switch (c) {
            case 'u':
                udp = true;
                break;
            case 't':
                tcp = true;
                break;
            default:
                break;
        }
    }

    string filter = "";
    for (int i = optind; i < argc; i++) {
        filter += argv[i];
        if (i != argc - 1) filter += " ";
    }
    regex_t regex;
    auto reg_status = regcomp(&regex, filter.c_str(), 0);
    if (reg_status != 0) {
        cerr << "Regex Error" << endl;
        exit(reg_status);
    }

    unordered_map<uint32_t, string> inode_map;
    auto dir = opendir("/proc");
    assert(dir);
    struct dirent* dinfo;
    if (dir) {
        while ((dinfo = readdir(dir)) != NULL) {
            if (dinfo->d_type != DT_DIR) continue;
            auto d_name = string(dinfo->d_name);
            if (any_of(d_name.begin(), d_name.end(),
                       [](char c) { return !isdigit(c); })) {
                continue;
            }

            string dirname = "/proc/" + d_name + "/fd";
            auto fddir = opendir(dirname.c_str());
            if (fddir == NULL) continue;

            struct dirent* fddinfo;
            while ((fddinfo = readdir(fddir)) != NULL) {
                if (fddinfo->d_type != DT_LNK) continue;
                string linkname = dirname + "/" + string(fddinfo->d_name);

                struct stat linkstat;
                stat(linkname.c_str(), &linkstat);
                if (S_ISSOCK(linkstat.st_mode)) {
                    fstream cmdlinefs("/proc/" + d_name + "/cmdline",
                                      fstream::in);
                    string token;
                    getline(cmdlinefs, token, '\0');
                    const auto slashpos = token.find_last_of('/');
                    if (slashpos != string::npos) {
                        token = token.substr(slashpos + 1);
                    }

                    stringstream result;
                    result << token;
                    while (getline(cmdlinefs, token, '\0')) {
                        result << ' ' << token;
                    }
                    inode_map[linkstat.st_ino] = d_name + "/" + result.str();
                }
            }
            closedir(fddir);
        }
        closedir(dir);
    }

    vector<string> types;
    if (!(udp ^ tcp)) {
        types = {"tcp", "tcp6", "udp", "udp6"};
    } else if (udp) {
        types = {"udp", "udp6"};
    } else {
        types = {"tcp", "tcp6"};
    }

    bool firsttype = true;
    for (auto type : types) {
        if (type == "tcp") {
            puts("List of TCP connections:");
            cout << left << setw(8) << "Proto" << setw(48) << "Local Address"
                 << setw(48) << "Foreign Address"
                 << "Program name and arguments" << endl;
        } else if (type == "udp") {
            if (!firsttype) puts("");
            puts("List of UDP connections:");
            cout << left << setw(8) << "Proto" << setw(48) << "Local Address"
                 << setw(48) << "Foreign Address"
                 << "Program name and arguments" << endl;
        }

        firsttype = false;
        fstream fs("/proc/net/" + type, fstream::in);
        string line;
        fs.ignore(numeric_limits<streamsize>::max(), '\n');
        while (getline(fs, line)) {
            string token;
            stringstream ss(line);
            int col = 0;
            string local, rem;
            uint64_t inode = 0;

            while (ss >> token) {
                if (col == 1) {
                    local = convert_ip(token, type.back() == '6');
                } else if (col == 2) {
                    rem = convert_ip(token, type.back() == '6');
                } else if (col == 9) {
                    inode = stoull(token);
                    break;
                }
                col++;
            }
            string cmdline = "-";
            const auto target = inode_map.find(inode);
            if (target != inode_map.end()) cmdline = target->second;

            stringstream result_ss;
            result_ss << left << setw(8) << type << setw(48) << local
                      << setw(48) << rem << cmdline << endl;

            regmatch_t pmatch[1];
            if (regexec(&regex, result_ss.str().c_str(), 1, pmatch, 0) == 0) {
                cout << result_ss.str() << endl;
            }
        }
    }
}
