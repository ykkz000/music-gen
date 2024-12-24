#ifndef __WAV_H__
#define __WAV_H__

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef enum sample_format
{
    SAMPLE_FMT_U8 = 8,
    SAMPLE_FMT_S16 = 16,
    SAMPLE_FMT_S32 = 32,
    SAMPLE_FMT_NONE
} sample_format_t;

typedef struct __attribute__((packed)) wav_header
{
    char riff_id[4];
    uint32_t riff_size;
    char wave_id[4];
    char fmt_id[4];
    uint32_t fmt_size;
    uint16_t audio_format;
    uint16_t num_channels;
    uint32_t sample_rate;
    uint32_t byte_rate;
    uint16_t block_align;
    uint16_t bits_per_sample;
    char data_id[4];
    uint32_t data_size;
} wav_header_t;

/**
 * 填充WAV文件头
 * @param num_channels 通道数
 * @param sample_rate 采样率
 * @param num_samples 采样数
 */
wav_header_t wav_fill_header(uint32_t num_channels, uint32_t sample_rate, uint32_t num_samples);

#endif
