#include "param.h"
#include "logger.h"

#include <stdlib.h>
#include <string.h>
#include <getopt.h>

/**
 * 定义一个函数，根据字符串返回调代码
 * @param str 字符串
 * @return 调代码，未知或不受支持调返回-1
 */
static inline int get_major(const char *str)
{
    // 如果字符串是"C"
    if (strcmp(str, "C") == 0)
    {
        // 返回0表示C调
        return 0;
    }
    // 如果字符串是"bE"
    if (strcmp(str, "bE") == 0)
    {
        // 返回1表示bE调
        return 1;
    }
    // 如果字符串不符合上述条件，返回-1表示未知调
    return -1;
}

static inline void print_help(int exit_code)
{
    printf("Usage: mgen -i <input_file> -o <output_url> [-t <major_type>] [-u <unit_period_ms>] [-r <sample_rate>]\n");
    printf("Options:\n");
    printf("  -h, --help                   Print this help message\n");
    printf("  -i, --input-file <file>      Input file\n");
    printf("  -o, --output-file <file>     Output file\n");
    printf("  -t, --major-type <major>     Major type, C or bE\n");
    printf("  -u, --unit-period-ms <ms>    Unit period in milliseconds\n");
    printf("  -r, --sample-rate <rate>     Sample rate\n");
    exit(exit_code);
}

param_t parse_param(int argc, char *argv[])
{
    param_t param = {NULL, NULL, 0, 1000, 44100};
    int opt;

    while ((opt = getopt(argc, argv, "hi:o:t:u:r:")) != -1)
    {
        switch (opt)
        {
        case 'h':
            print_help(0);
        case 'i':
            param.input_file = fopen(optarg, "r");
            break;
        case 'o':
            param.output_file = fopen(optarg, "w");
            break;
        case 't':
            param.major_type = get_major(optarg);
            break;
        case 'u':
            param.unit_period_ms = atof(optarg);
            break;
        case 'r':
            param.sample_rate = atoi(optarg);
            break;
        default:
            LOG_ERROR("Unrecognized option: %c", opt);
            print_help(-1);
        }
    }
    if (param.input_file == NULL)
    {
        LOG_ERROR("Input file is not specified.");
        print_help(-1);
    }
    if (param.output_file == NULL)
    {
        LOG_ERROR("Output file is not specified.");
        print_help(-1);
    }
    if (param.major_type == -1)
    {
        LOG_ERROR("Illegal major type.");
        print_help(-1);
    }
    if (param.unit_period_ms <= 0)
    {
        LOG_ERROR("Illegal unit period.");
        print_help(-1);
    }
    if (param.sample_rate <= 0)
    {
        LOG_ERROR("Illegal sample rate.");
        print_help(-1);
    }
    return param;
}
