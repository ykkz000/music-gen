#ifndef __BEEP_H__
#define __BEEP_H__

#include "wav.h"

typedef double (*timbre_t)(double, double, double);
typedef struct notation
{
    double frequency;
    double amplitude;
    double period_ms;
    timbre_t timbre;
} notation_t;
typedef struct sampled_notation
{
    uint8_t *data;
    uint8_t *p;
    size_t size;
    size_t max_size;
} sampled_notation_t;
typedef struct streamed_sampled_notation
{
    FILE *file;
    int major;
    double unit_period_ms;
    sampled_notation_t sampled_notation;
    uint32_t sample_rate;
} streamed_sampled_notation_t;

/**
 * 解析音符的函数
 * @param notation 音符结构体指针
 * @param file 文件指针
 * @param major 主音调
 * @param unit_period_ms 单位周期时间（毫秒）
 * @return 解析成功返回1，失败返回0
 */
int parse_notation(notation_t *notation, FILE *file, int major, double unit_period_ms);
/**
 * 初始化采样音符结构体
 * @param sampled_notation 采样后的音符
 */
void initialize_sample_notation(sampled_notation_t *sampled_notation);
/**
 * 采样音符函数
 * @param sampled_notation 采样后的音符
 * @param notation 原始音符
 * @param sample_rate 采样率
 * @return 采样点数，失败返回-1
 */
int sample_notation(sampled_notation_t *sampled_notation, const notation_t *notation, uint32_t sample_rate);
/**
 * 释放采样音符结构体及其数据
 * @param sampled_notation 采样后的音符
 */
void finalize_sampled_notation(sampled_notation_t *sampled_notation);
/**
 *
 * 填充缓冲区，使用采样后的音符数据
 * @param streamed_sampled_notation 采样后的音符流结构体指针
 * @param buffer 缓冲区指针
 * @param buffer_size 缓冲区大小
 * @return 实际填充的缓冲区大小
 */
int fill_with_sampled_notation(streamed_sampled_notation_t *streamed_sampled_notation, uint8_t *buffer, size_t buffer_size);

#endif
