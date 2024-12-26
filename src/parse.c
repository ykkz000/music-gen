#define _GNU_SOURCE 1

#include "notation.h"

#include <string.h>
#include <math.h>

static const double NUMBERED_MUSICAL_NOTATION[][3][8] = {
    /* C */
    {
        {0, 261.63, 293.66, 329.63, 349.23, 391.99, 440, 493.88},
        {0, 532.25, 587.33, 659.25, 698.46, 783.99, 880, 987.76},
        {0, 1046.50, 1174.66, 1318.51, 1396.62, 1567.98, 1760, 1975.52}},
    /* bE */
    {
        {0, 155.56, 174.61, 196.00, 207.65, 233.08, 261.63, 293.66},
        {0, 311.13, 349.23, 392.00, 415.30, 466.16, 523.25, 587.33},
        {0, 622.25, 698.46, 783.99, 830.61, 932.33, 1046.50, 1174.66}}};

/**
 * 定义钢琴音色生成函数
 * @param frequency 频率
 * @param amplitude 幅度
 * @param phase 相位
 * @return 生成的音色值
 */
static double piano_timbre(double frequency, double amplitude, double phase)
{
    // 定义钢琴的谐波幅度数组
    const static double harmonic_amplitude[] = {1, 0.340, 0.102, 0.085, 0.070, 0.065, 0.028, 0.085, 0.011, 0.030, 0.010, 0.014, 0.012, 0.013, 0.004, 0.004, 0.004, 0.004, 0.004, 0.004};

    // 初始化音色值为0
    double value = 0;
    // 遍历谐波幅度数组
    for (int i = 0; i < sizeof(harmonic_amplitude) / sizeof(double); i++)
    {
        // 计算当前谐波的音色值并累加
        value += sin(2 * M_PI * frequency * (i + 1) * phase / 1000) * amplitude * harmonic_amplitude[i] / 1.889;
    }
    // 定义衰减系数
    double attenuation;
    // 如果相位小于25
    if (phase < 25)
    {
        // 衰减系数线性增加
        attenuation = phase / 25 * 1.0;
    }
    // 如果相位在25到100之间
    else if (phase < 100)
    {
        // 衰减系数线性减少
        attenuation = 1.0 - (phase - 25) / 75 * 0.6;
    }
    // 如果相位在100到500之间
    else if (phase < 500)
    {
        // 衰减系数继续线性减少
        attenuation = 0.4 - (phase - 100) / 400 * 0.2;
    }
    // 如果相位大于500
    else
    {
        // 衰减系数逐渐减少到0
        attenuation = fmax(0.2 - (phase - 500) / 500 * 0.1, 0);
    }
    // 返回最终的音色值，乘以衰减系数
    return value * attenuation;
}

int parse_notation(notation_t *notation, FILE *file, int major, double unit_period_ms)
{
    // 定义音符的x和y坐标
    int notation_x, notation_y;
    // 定义振幅和周期
    double amplitude, period;
    // 定义缓冲区指针
    char *buffer;
    // 定义缓冲区大小
    size_t buffer_size;
    // 定义返回值
    int ret;

    // 初始化缓冲区
    buffer = NULL;
    // 读取文件直到文件结束
    while (!feof(file) && getline(&buffer, &buffer_size, file))
    {
        // 如果行首字符不是#且不是空行
        if (buffer[0] != '#' && buffer[0] != 0 && buffer[0] != '\n' && buffer[0] != '\r')
        {
            // 退出循环
            break;
        }
    }
    // 如果文件结束
    if (feof(file))
    {
        // 设置返回值为0
        ret = 0;
        // 跳转到finalize标签
        goto finalize;
    }
    // 从缓冲区解析音符的x坐标、y坐标、振幅和周期
    sscanf(buffer, "%d %d %lf %lf", &notation_x, &notation_y, &amplitude, &period);
    // 设置音符的频率
    notation->frequency = NUMBERED_MUSICAL_NOTATION[major][notation_x][notation_y];
    // 设置音符的振幅
    notation->amplitude = amplitude;
    // 设置音符的周期（毫秒）
    notation->period_ms = period * unit_period_ms;
    // 设置音符的音色为钢琴音色
    notation->timbre = piano_timbre;
    // 设置返回值为1
    ret = 1;

finalize:
    // 如果缓冲区指针不为空
    if (buffer != NULL)
    {
        // 释放缓冲区内存
        free(buffer);
    }
    // 返回解析结果
    return ret;
}
