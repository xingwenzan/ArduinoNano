//
// Created by 86159 on 2023-04-30.
//
#include <Arduino.h>
#include "..\lib\arduinoFFT\src\arduinoFFT.h"

// 定义默认参数
char defaultWaveType = '0';  // 波形类型，默认为正弦波
float defaultWaveFreq = 1000.0;  // 波形频率，默认为1kHz
float defaultSamplingFreq = 10000.0; // 采样频率，默认为10kHz
float defaultWaveDc = 0.0;   // 波形直流分量，默认为0
float defaultWaveAmpl = 1.0; // 波形幅值，默认为1
int defaultWaveSamples = 128; // 波形数据采样率，默认为100个样本点
int defaultPhoto = 0;   // 输出图像，默认原图

// 定义变量
char waveType = defaultWaveType;
float waveFreq = defaultWaveFreq;
float samplingFreq = defaultSamplingFreq;
float waveDc = defaultWaveDc;
float waveAmpl = defaultWaveAmpl;
int waveSamples = defaultWaveSamples;
int photo = defaultPhoto;
double wave[128];
int idx = 0;

int freeMemory() {   // 查看剩余内存,最好永远不要用上
    extern int __heap_start, *__brkval;
    int v;
    return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

void fft() {   // 结果有误
    // 初始化 FFT 对象
    double vImag[waveSamples];
    memset(vImag,0,waveSamples * sizeof(double));
    arduinoFFT FFT = arduinoFFT(wave, vImag, waveSamples, samplingFreq);
//    int availableMemory = freeMemory();
//    Serial.print("Free memory: ");
//    Serial.print(availableMemory);
//    Serial.println(" bytes");
    /* 对数据执行 FFT */
    FFT.Windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.Compute(FFT_FORWARD);
    FFT.ComplexToMagnitude();

//    Serial.print("Free memory: ");
//    Serial.print(availableMemory);
//    Serial.println(" bytes");
    /* 输出频谱图数据到串口绘图器 */
    for (int i = 0; i < (waveSamples>>1); i++) {
//        double frequency = (i * 1.0 * samplingFreq) / waveSamples;
//        Serial.print(frequency);
//        Serial.print(",");
        Serial.println(wave[i]);
    }
//    Serial.println(FFT.MajorPeak());
//    while (1);
}

void getWave() {
    idx = 0;
    for (int i = 0; i < waveSamples; i++) {
        float t = (float) i / samplingFreq;  // 计算当前时间
        double v = waveDc;  // 加上直流分量
        if (waveType == '0') {   // 正弦波
            v += sin(2 * PI * t * waveFreq) * waveAmpl;
        } else if (waveType == '1') {   // 方波
            v += waveAmpl * (fmod(t * waveFreq, 1) < 0.5 ? -1 : 1);
        }else if (waveType == '2') {   // 三角波
            double trianglePhase = fmod(t * waveFreq, 1);
            v += waveAmpl * (2 * fabs(2 * trianglePhase - 1) - 1);
//            v += waveAmpl * (trianglePhase < 0.5 ? 4 * trianglePhase - 1 : 3 - 4 * trianglePhase);
        } else if (waveType == '3') {   // 锯齿波
            double sawtoothPhase = fmod(t * waveFreq, 1);
            v += waveAmpl * (2 * sawtoothPhase - 1);
        }
        wave[idx++] = v;
//        Serial.println(v);
    }
}

void draw() {
    if (photo==0) {
        digitalWrite(LED_BUILTIN, LOW);
        for (int i = 0; i < idx; ++i) {
            Serial.println(wave[i]);
        }
    } else {
        digitalWrite(LED_BUILTIN, HIGH);
        fft();
    }
}

void parameterRead(const String &string) {   // 参数输入
    digitalWrite(LED_BUILTIN, HIGH);
    // 初始化参数
    waveType = defaultWaveType;
    waveFreq = defaultWaveFreq;
    samplingFreq = defaultSamplingFreq;
    waveDc = defaultWaveDc;
    waveAmpl = defaultWaveAmpl;
    waveSamples = defaultWaveSamples;
    photo = defaultPhoto;
    // 解析输入命令
    int parameter = 0;   //正在输入的参数 0~5 分别对应变量定义里的六个参数
    String tmp = "";
    for (auto item: string) {
        if (item == ' ' || item=='\r') {
            switch (parameter) {
                case 0:
                    waveType = tmp[tmp.length() - 1];
//                    Serial.println(tmp);
                    break;
                case 1:
                    waveFreq = tmp.toFloat();
//                    Serial.println(tmp);
                    break;
                case 2:
                    samplingFreq = tmp.toFloat();
                    break;
                case 3:
                    waveDc = tmp.toFloat();
//                    Serial.println(tmp);
                    break;
                case 4:
                    waveAmpl = tmp.toFloat();
//                    Serial.println(tmp);
                    break;
                case 5:
                    waveSamples = (int) tmp.toFloat();
//                    Serial.println(tmp);
                    break;
                case 6:
                    photo = (int) tmp.toFloat();
//                    Serial.println(tmp);
                    break;
                default:
                    break;
            }
            parameter++;
            tmp = "";
        } else if (isdigit(item) || item == '.' || item == '-') {
            tmp += item;
        }
    }
    delay(10000);
    digitalWrite(LED_BUILTIN, LOW);
}

void setup() {
    Serial.begin(9600); // 初始化串口
    Serial.println(
            "请输入数据:\n波的类型（正弦波:0;方波:1;三角波:2;锯齿波:4）、频率、中值、幅值、采样率（Hz）\n注意: 数据请用”空格“分隔");
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
    // 数据读入
    if (Serial.available()) { // 如果有可用数据
        String cmd = Serial.readString();
        parameterRead(cmd);
    }
    // 获取波
    getWave();
    draw();
}