#include "param.h"
#include "notation.h"
#include "logger.h"

int main(int argc, char *argv[])
{
    // 返回值变量
    int ret;
    // 参数结构体
    param_t param;
    // 缓冲区
    uint8_t *buffer;
    // 帧数
    uint32_t num_frames;
    // 每帧的样本数
    uint32_t frame_samples;
    // 总样本数
    uint32_t num_samples;
    // 流式采样音符结构体
    streamed_sampled_notation_t streamed_sampled_notation;
    // WAV文件头结构体
    wav_header_t wav_header;

    // 解析命令行参数
    param = parse_param(argc, argv);

    // 初始化流式采样音符
    streamed_sampled_notation.file = param.input_file;
    streamed_sampled_notation.major = param.major_type;
    streamed_sampled_notation.unit_period_ms = param.unit_period_ms;
    streamed_sampled_notation.sample_rate = param.sample_rate;
    initialize_sample_notation(&streamed_sampled_notation.sampled_notation);

    // 初始化样本数和帧数
    num_samples = 0, num_frames = 0;
    // 计算每帧的样本数
    frame_samples = 1 * param.sample_rate;
    // 分配缓冲区
    buffer = malloc(frame_samples * (16 / 8));

    // 写入WAV文件头
    fwrite(&wav_header, sizeof(wav_header_t), 1, param.output_file);

    // 循环生成样本数据
    while ((ret = fill_with_sampled_notation(&streamed_sampled_notation, buffer, frame_samples * (16 / 8))) > 0)
    {
        // 记录生成的帧数
        LOG_DEBUG("Generated %d frames.", (++num_frames));
        // 累加样本数
        num_samples += ret / (16 / 8);
        // 将缓冲区数据写入输出文件
        fwrite(buffer, 1, ret, param.output_file);
    }
    // 打印总帧数和样本数
    LOG_INFO("Total %d frames, %d samples", num_frames, num_samples);

    // 重新生成WAV文件头
    wav_header = wav_fill_header(1, param.sample_rate, num_samples);
    // 将文件指针移动到文件开头
    fseek(param.output_file, 0, SEEK_SET);
    // 写入新的WAV文件头
    fwrite(&wav_header, sizeof(wav_header_t), 1, param.output_file);

    // 释放缓冲区
    free(buffer);
    // 关闭输入文件
    fclose(param.input_file);
    // 关闭输出文件
    fclose(param.output_file);

    // 返回0表示成功
    return 0;
}
