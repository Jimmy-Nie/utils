/***********************************************************************
 * Description: spdlog init and realization
 * Autor: Jimmy
 * Email: nieshihua@126.com
 * Create: 2021/8/27.
 * Modify:
 **********************************************************************/
#pragma once

#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

#ifndef  SPDLOG_ACTIVE_LEVEL
    #define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#else
    #undef SPDLOG_ACTIVE_LEVEL
    #define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#endif

namespace jm {
/* Level
{
    trace,
    debug,
    info,
    warn,
    err,
    critical,
    off,
}*/

#if SPDLOG_ACTIVE_LEVEL < SPDLOG_LEVEL_OFF
#define DEFAULT_LOG(Level, format, ...) spdlog::log(spdlog::level::Level, format, ##__VA_ARGS__)

#ifndef USE_DEFAULT_SPDLOG_MACRO
#define LOG_SPD(name, Level, format, ...)                                                                                  \
    {                                                                                                                  \
        if (auto logger_in = spdlog::get(#name)) {                                                                     \
            SPDLOG_LOGGER_CALL(logger_in, spdlog::level::Level, format, ##__VA_ARGS__);                                \
        } else {                                                                                                       \
            DEFAULT_LOG(Level, #name " is not defined! " + std::string(format), ##__VA_ARGS__);                        \
        }                                                                                                              \
    }                                                                                                                  \
    void(0)
#else
#define LOG_SPD(name, Level, format, ...)                                                                                  \
    SPDLOG_LOGGER_CALL(spdlog::get(#name), spdlog::level::Level, format, ##__VA_ARGS__);
#endif

#if SPDLOG_ACTIVE_LEVEL <= SPDLOG_LEVEL_TRACE
#define LOG_TRACE(name, format, ...) LOG_SPD(name, trace, format, ##__VA_ARGS__)
#else
#define LOG_TRACE(...) (void)0
#endif

#if SPDLOG_ACTIVE_LEVEL <= SPDLOG_LEVEL_DEBUG
#define LOG_DEBUG(name, format, ...) LOG_SPD(name, debug, format, ##__VA_ARGS__)
#else
#define LOG_DEBUG(...) (void)0
#endif

#if SPDLOG_ACTIVE_LEVEL <= SPDLOG_LEVEL_INFO
#define LOG_INFO(name, format, ...) LOG_SPD(name, info, format, ##__VA_ARGS__)
#else
#define LOG_INFO(...) (void)0
#endif

#if SPDLOG_ACTIVE_LEVEL <= SPDLOG_LEVEL_WARN
#define LOG_WARN(name, format, ...) LOG_SPD(name, warn, format, ##__VA_ARGS__)
#else
#define LOG_WARN(...) (void)0
#endif

#if SPDLOG_ACTIVE_LEVEL <= SPDLOG_LEVEL_ERROR
#define LOG_ERR(name, format, ...) LOG_SPD(name, err, format, ##__VA_ARGS__)
#else
#define LOG_ERR(...) (void)0
#endif

#if SPDLOG_ACTIVE_LEVEL <= SPDLOG_LEVEL_CRITICAL
#define LOG_CRITICAL(name, format, ...) LOG_SPD(name, critical, format, ##__VA_ARGS__)
#else
#define LOG_CRITICAL(...) (void)0
#endif
#endif


/********************************************宏定义****************************************/
#define jm_logt(fmt, ...) LOG_TRACE(jm, fmt, ##__VA_ARGS__)
#define jm_logd(fmt, ...) LOG_DEBUG(jm, fmt, ##__VA_ARGS__)
#define jm_logi(fmt, ...) LOG_INFO(jm, fmt, ##__VA_ARGS__)
#define jm_logw(fmt, ...) LOG_WARN(jm, fmt, ##__VA_ARGS__)
#define jm_loge(fmt, ...) LOG_ERR(jm, fmt, ##__VA_ARGS__)
#define jm_logc(fmt, ...) LOG_CRITICAL(jm, fmt, ##__VA_ARGS__)

#define test_logt(fmt, ...) LOG_TRACE(test, fmt, ##__VA_ARGS__)
#define test_logd(fmt, ...) LOG_DEBUG(test, fmt, ##__VA_ARGS__)
#define test_logi(fmt, ...) LOG_INFO(test, fmt, ##__VA_ARGS__)
#define test_logw(fmt, ...) LOG_WARN(test, fmt, ##__VA_ARGS__)
#define test_loge(fmt, ...) LOG_ERR(test, fmt, ##__VA_ARGS__)
#define test_logc(fmt, ...) LOG_CRITICAL(test, fmt, ##__VA_ARGS__)

class JMLog {
public:
    static void init() {
        init("test", "./",
                spdlog::level::trace,
                spdlog::level::trace,
                spdlog::level::trace);
    }

    static void init(const std::string& log_class,      ///The log type
                     const std::string& output_dir  = "./",
                     spdlog::level::level_enum file_log_level   = spdlog::level::trace,
                     spdlog::level::level_enum console_log_level  = spdlog::level::trace,
                     spdlog::level::level_enum class_log_level  = spdlog::level::trace
    );

private:
    JMLog() = delete;
    ~JMLog() = delete;
};

inline void JMLog::init(const std::string& log_class,
                        const std::string& output_dir,
                        spdlog::level::level_enum file_log_level,
                        spdlog::level::level_enum console_log_level,
                        spdlog::level::level_enum class_log_level
)
{
    auto file_path = output_dir + "/" + log_class + ".log";

    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_level(console_log_level);

    auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(file_path, 1024*1024*5, 3);
    file_sink->set_level(file_log_level);

    spdlog::sinks_init_list         sinks{console_sink, file_sink};
    std::shared_ptr<spdlog::logger> logger = std::make_shared<spdlog::logger>(log_class, sinks);
    logger->set_level(class_log_level);
    logger->flush_on(spdlog::level::trace);

    logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%^%l%$] [%s:%#:%!]: %v");
    spdlog::register_logger(logger);
}

}   //end namespace jm