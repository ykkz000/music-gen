#include "wav.h"

#include <string.h>

wav_header_t wav_fill_header(uint32_t num_channels, uint32_t sample_rate, uint32_t num_samples)
{
    // 定义WAV文件头结构体变量
    wav_header_t header;
    // 定义字节率和数据大小变量
    uint32_t byte_rate, data_size;
    // 定义数据块对齐变量
    uint16_t block_align;
    
    // 计算字节率、数据块对齐和数据大小
    byte_rate = sample_rate * 1 * (16 / 8);
    block_align = 1 * (16 / 8);
    data_size = num_samples * 1 * (16 / 8);
 
    // 填充WAV文件头
    memcpy(header.riff_id, "RIFF", sizeof(header.riff_id));
    header.riff_size = 36 + data_size; // 36是fmt和data子块之前的字节数
    memcpy(header.wave_id, "WAVE", sizeof(header.wave_id));
    memcpy(header.fmt_id, "fmt ", sizeof(header.fmt_id));
    header.fmt_size = 16; // PCM格式
    header.audio_format = 1; // PCM
    header.num_channels = 1; // 单声道
    header.sample_rate = sample_rate;
    header.byte_rate = byte_rate;
    header.block_align = block_align;
    header.bits_per_sample = 16;
    memcpy(header.data_id, "data", sizeof(header.data_id));
    header.data_size = data_size;

    // 返回填充好的WAV文件头
    return header;
}
