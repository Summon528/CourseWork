/*
    Student No.: 0611262
    Student Name: Chang-Yen Tseng
    Email: vanilla.cv06@nctu.edu.tw
    SE tag: xnxcxtxuxoxsx
    Statement: I am fully aware that this program is not
        supposed to be posted to a public server, such as a
        public GitHub repository or a public web page.
*/

#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

enum Redirection { IN, OUT, NO_REDIRECT };

class Input {
   public:
    std::vector<std::string> args1;
    std::vector<std::string> args2;
    std::string file_name;
    Redirection direction;
    bool detach;
    bool use_pipe;
    explicit Input(const std::string &input);
};

Input::Input(const std::string &input) {
    std::stringstream ss(input, std::ios_base::in);
    std::string token;
    std::vector<std::string> result;
    use_pipe = false;
    direction = NO_REDIRECT;
    detach = false;
    while (ss >> token) {
        if (token == ">") {
            direction = OUT;
            ss >> file_name;
            return;
        }
        if (token == "<") {
            direction = IN;
            ss >> file_name;
            return;
        }
        if (token == "|") {
            use_pipe = true;
            continue;
        }
        if (use_pipe) {
            args2.emplace_back(token);
        } else {
            args1.emplace_back(token);
        }
    }
    if (args1.back() == "&") {
        detach = true;
        args1.pop_back();
    }
}

Input get_input() {
    std::cout << ">" << std::flush;
    std::string input_str;
    std::getline(std::cin, input_str);
    if (std::cin.eof()) {
        exit(0);
    }
    return Input(input_str);
}

void replace_proc(const std::vector<std::string> &args) {
    std::vector<char *> c_args;
    c_args.reserve(args.size());
    for (const auto &s : args) {
        c_args.emplace_back(const_cast<char *>(s.c_str()));
    }
    c_args.emplace_back(nullptr);
    if (const auto r = execvp(c_args.front(), c_args.data()); r == -1) {
        std::cerr << "failed: " << c_args.front() << std::endl;
        _exit(EXIT_FAILURE);
    }
}

void spawn(const Input &input) {
    if (const auto pid(fork()); pid == 0) {
        replace_proc(input.args1);
    } else if (pid > 0) {
        exit(0);
    }
}

void exec(const Input &input) {
    if (input.direction == OUT) {
        const auto fd =
            open(input.file_name.c_str(), O_CREAT | O_WRONLY | O_CLOEXEC,
                 S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
        dup2(fd, STDOUT_FILENO);
        close(fd);
        replace_proc(input.args1);
    } else if (input.direction == IN) {
        const auto fd =
            open(input.file_name.c_str(), O_CREAT | O_RDONLY | O_CLOEXEC);
        dup2(fd, STDIN_FILENO);
        close(fd);
        replace_proc(input.args1);
    } else if (input.use_pipe) {
        int fd[2];
        pipe(static_cast<int *>(fd));
        const auto pid(fork());
        if (pid == 0) {
            dup2(fd[1], STDOUT_FILENO);
            close(fd[0]);
            close(fd[1]);
            replace_proc(input.args1);
        } else if (pid > 0) {
            dup2(fd[0], STDIN_FILENO);
            close(fd[0]);
            close(fd[1]);
            replace_proc(input.args2);
        }
    } else {
        replace_proc(input.args1);
    }
}

int main() {
    while (true) {
        const auto input(get_input());
        if (const auto pid(fork()); pid == 0) {
            if (input.detach) {
                spawn(input);
            } else {
                exec(input);
            }
        } else if (pid > 0) {
            waitpid(pid, nullptr, 0);
        }
    }
}
