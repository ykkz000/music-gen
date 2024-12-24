#include "notation.h"

#include <string.h>

void initialize_sample_notation(sampled_notation_t *sampled_notation)
{
    // 初始化最大大小为0
    sampled_notation->max_size = 0;
    // 初始化数据大小为0
    sampled_notation->size = 0;
    // 初始化数据指针为NULL
    sampled_notation->data = NULL;
}

int sample_notation(sampled_notation_t *sampled_notation, const notation_t *notation, uint32_t sample_rate)
{
    // 定义循环变量
    int i;
    // 定义采样值
    double value;

    // 计算采样后的数据大小
    sampled_notation->size = (int)(notation->period_ms * sample_rate / 1000) * (16 / 8);
    // 检查是否需要重新分配内存
    if (sampled_notation->size > sampled_notation->max_size)
    {
        // 重新分配内存
        sampled_notation->data = (uint8_t *)realloc(sampled_notation->data, sampled_notation->size);
        // 更新最大大小
        sampled_notation->max_size = sampled_notation->size;
    }
    // 检查内存分配是否成功
    if (sampled_notation->data == NULL)
    {
        // 如果内存分配失败，返回-1表示失败
        return -1;
    }
    // 初始化数据指针
    sampled_notation->p = sampled_notation->data;
    // 初始化数据为0
    memset(sampled_notation->data, 0, sampled_notation->size);
    // 循环采样音符
    for (i = 0; i * 1000.0 / sample_rate < notation->period_ms; i++)
    {
        // 计算当前采样点的值
        value = notation->timbre(notation->frequency, notation->amplitude, i * 1000.0 / sample_rate);
        ((int16_t *)sampled_notation->data)[i] = (int16_t)(value * 0x7FFF);
    }
    // 返回采样点数
    return i;
}

void finalize_sampled_notation(sampled_notation_t *sampled_notation)
{
    // 检查数据是否为空
    if (sampled_notation->data != NULL)
    {
        // 释放数据内存
        free(sampled_notation->data);
    }
    // 重置数据大小为0
    sampled_notation->size = 0;
    // 重置数据最大大小为0
    sampled_notation->max_size = 0;
}

int fill_with_sampled_notation(streamed_sampled_notation_t *streamed_sampled_notation, uint8_t *buffer, size_t buffer_size)
{
    // 定义符号结构体
    notation_t notation;
    // 定义填充大小和已填充大小
    size_t filling_size, filled_size;

    // 初始化已填充大小为0
    filled_size = 0;
    // 初始化缓冲区为0
    memset(buffer, 0, buffer_size * sizeof(uint8_t));
    // 当已填充大小小于缓冲区大小时，继续循环
    while (filled_size < buffer_size)
    {
        // 如果采样后的音符流中没有数据
        if (streamed_sampled_notation->sampled_notation.size == 0)
        {
            // 从文件中解析音符数据
            if (parse_notation(&notation, streamed_sampled_notation->file, streamed_sampled_notation->major, streamed_sampled_notation->unit_period_ms) == 0)
            {
                // 解析失败，退出循环
                break;
            }
            // 采样音符数据
            sample_notation(&streamed_sampled_notation->sampled_notation, &notation, streamed_sampled_notation->sample_rate);
        }
        // 获取当前采样后的音符流中的数据大小
        filling_size = streamed_sampled_notation->sampled_notation.size;
        // 如果已填充大小加上当前数据大小超过缓冲区大小
        if (filled_size + filling_size > buffer_size)
        {
            // 重新计算填充大小，使其不超过缓冲区大小
            filling_size = buffer_size - filled_size;
        }
        // 将采样后的音符数据复制到缓冲区
        memcpy(buffer + filled_size, streamed_sampled_notation->sampled_notation.p, filling_size);
        // 更新已填充大小
        filled_size += filling_size;
        // 更新采样后的音符流中的数据指针
        streamed_sampled_notation->sampled_notation.p += filling_size;
        // 更新采样后的音符流中的数据大小
        streamed_sampled_notation->sampled_notation.size -= filling_size;
    }
    // 返回实际填充的缓冲区大小
    return filled_size;
}
