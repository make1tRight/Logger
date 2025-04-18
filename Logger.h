#include <string>
#include <sstream>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <fstream>
#include <atomic>
#include <iostream>
#include <thread>
#include <vector>


template <typename T>
std::string to_string_helper(T&& arg) {
    std::stringstream oss;
    oss << std::forward<T>(arg);
    return oss.str();
}

class LogQueue {
public:
    void push(const std::string& msg) {
        std::lock_guard<std::mutex> lock(_mutex);
        _queue.push(msg);
        _cond.notify_one();
    }

    bool pop(std::string& msg) {
        std::unique_lock<std::mutex> lock(_mutex);
        while (_queue.empty() && !_is_shutdown) {
            _cond.wait(lock);
        }
        if (_is_shutdown && _queue.empty()) {
            return false;
        }

        msg = _queue.front();
        _queue.pop();
        return true;
    }

    void shutdown() {
        std::lock_guard<std::mutex> lock(_mutex);
        _is_shutdown = true;
        _cond.notify_all();
    }

private:
    std::mutex _mutex;
    std::queue<std::string> _queue;
    std::condition_variable _cond;
    bool _is_shutdown;
};


class Logger {
public:
    Logger(const std::string& filename)
         : _log_file(filename, std::ios::out|std::ios::app), _exit_flag(false)  {
        if (!_log_file.is_open()) {
            std::cout << "cannot open log file." << std::endl;
        }
        // 后台线程持续从队列中取出消息并写入文件
        _thread = std::thread(&Logger::processQueue, this);
    }

    ~Logger() {
        _log_queue.shutdown();
        if (_thread.joinable()) {
            _thread.join();
        }

        if (_log_file.is_open()) {
            _log_file.close();
        }
    }

    template<typename... Args>
    void log(const std::string& format, Args&&... args) {
        _log_queue.push(formatMessage(format, std::forward<Args>(args)...));
    }

private:
    void processQueue() {
        std::string msg;
        while (_log_queue.pop(msg)) {
            _log_file << msg << std::endl;
        }
    }

    template<typename... Args>
    std::string formatMessage(const std::string& format, Args&&... args) {
        std::vector<std::string> arg_strings = {
            to_string_helper(std::forward<Args>(args))...
        };
        std::stringstream oss;
        std::size_t pos = 0;
        std::size_t arg_index = 0;
        std::size_t placeholder = format.find("{}", pos);

        while (placeholder != std::string::npos) {
            oss << format.substr(pos, placeholder - pos);
            if (arg_index < arg_strings.size()) {
                oss << arg_strings[arg_index++];
            } else {
                oss << "{}";
            }
            pos = placeholder + 2;
            placeholder = format.find("{}", pos);
        }
        oss << format.substr(pos);

        while (arg_index < arg_strings.size()) {
            oss << arg_strings[arg_index++];
        }

        return oss.str();
    }

private:
    std::ofstream _log_file;
    std::atomic<bool> _exit_flag;
    std::thread _thread;
    LogQueue _log_queue;
};