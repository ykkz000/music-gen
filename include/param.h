#ifndef __PARAM_H__
#define __PARAM_H__

#include <stdio.h>
#include <stdint.h>

typedef struct param
{
    FILE *input_file;
    FILE *output_file;
    int major_type;
    double unit_period_ms;
    uint32_t sample_rate;
} param_t;

/**
 * 解析命令行参数
 * @param argc 命令行参数个数
 * @param argv 命令行参数数组
 * @return 解析后的参数结构体
 */
param_t parse_param(int argc, char *argv[]);

#endif
