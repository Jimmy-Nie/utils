/***********************************************************************
 * Description: spdlog test
 * Autor: Jimmy
 * Email: nieshihua@126.com
 * Create: 2021/8/27.
 * Modify:
 **********************************************************************/
#include "spdlog_impl.h"

using namespace jm;

void test_spdlog() {
    DEFAULT_LOG(info, "log default ... do not have name\n");

    JMLog::init("jm");
    jm_logt("log {} ...", "trace");
    jm_logd("log {} ...", "debug");
    jm_logi("log {} ...", "info");
    jm_logw("log {} ...", "warn");
    jm_loge("log {} ...", "error");
    jm_logc("log {} ...\n", "critical");

    test_loge("Do not init test name ...\n");

    JMLog::init();
    test_logt("log {} ...", "trace");
    test_logd("log {} ...", "debug");
    test_logi("log {} ...", "info");
    test_logw("log {} ...", "warn");
    test_loge("log {} ...", "error");
    test_logc("log {} ...\n", "critical");
}

int main() {
    test_spdlog();

    return 0;
}