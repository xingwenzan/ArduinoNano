//
// Created by 86159 on 2023-04-30.
//
#include <Arduino.h>
#include "..\lib\arduinoFFT\src\arduinoFFT.h"

// 定义默认参数
char defaultWaveType = '0';  // 波形类型，默认为正弦波
float defaultWaveFreq = 1000.0;  // 波形频率，默认为1kHz
float defaultWaveDc = 0.0;   // 波形直流分量，默认为0
float defaultWaveAmpl = 1.0; // 波形幅值，默认为1
int defaultWaveSamples = 128; // 波形数据采样率，默认为100个样本点
char defaultPhoto = '0';   // 输出图像，默认原图

// 定义变量
char waveType = defaultWaveType;
float waveFreq = defaultWaveFreq;
float waveDc = defaultWaveDc;
float waveAmpl = defaultWaveAmpl;
int waveSamples = defaultWaveSamples;
char photo = defaultPhoto;
double wave[128];
int idx = 0;


void parameterRead(const String &string) {   // 参数输入
    digitalWrite(LED_BUILTIN, HIGH);
    // 初始化参数
    waveType = defaultWaveType;
    waveFreq = defaultWaveFreq;
    waveDc = defaultWaveDc;
    waveAmpl = defaultWaveAmpl;
    waveSamples = defaultWaveSamples;
    photo = defaultPhoto;
    // 解析输入命令
    int parameter = 0;   //正在输入的参数 0~5 分别对应变量定义里的六个参数
    String tmp = "";
    for (auto item: string) {
        if (item == ' ') {
            switch (parameter) {
                case 0:
                    waveType = tmp[tmp.length() - 1];
                    break;
                case 1:
                    waveFreq = tmp.toFloat();
                    break;
                case 2:
                    waveDc = tmp.toFloat();
                    break;
                case 3:
                    waveAmpl = tmp.toFloat();
                    break;
                case 4:
                    waveSamples = (int) tmp.toFloat();
                    break;
                case 5:
                    photo = tmp[tmp.length() - 1];
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
    Serial.println(photo);
    delay(10000);
    digitalWrite(LED_BUILTIN, LOW);
}

void fft() {
    double vReal[idx], vImag[idx];
    for (int i = 0; i < idx; i++) {
        vReal[i] = wave[i];
        vImag[i] = 0;
    }
    arduinoFFT FFT = arduinoFFT();
    // 加窗，使用汉明窗
    FFT.Windowing(vReal, waveSamples, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    // 执行FFT
    FFT.Compute(vReal, vImag, idx, FFT_FORWARD);
    // 获取幅度
    FFT.ComplexToMagnitude(vReal, vImag, idx);
    // 输出频谱图
    for (int i = 0; i < (idx / 2); i++) {
        Serial.println((i * waveFreq) / idx);
        Serial.println(vReal[i] * 2 / idx);
    }
}

void getWave() {
    idx = 0;
    for (int i = 0; i < waveSamples; i++) {
        float t = (float) i / waveFreq / waveSamples;  // 计算当前时间
        double v = waveDc;  // 加上直流分量
        if (waveType == '0') {   // 正弦波
            v += sin(2 * PI * t * waveFreq) * waveAmpl;
        } else if (waveType == '1') {   // 方波
            v += waveAmpl * (t * waveFreq < 0.5 ? -1 : 1);
        }
        wave[idx++] = v;
//        Serial.println(v);
    }
}

void draw() {
    Serial.println(photo);
    if (photo == '0') {
        digitalWrite(LED_BUILTIN, LOW);
        for (int i = 0; i < idx; ++i) {
            Serial.println(wave[i]);
        }
    } else {
        digitalWrite(LED_BUILTIN, HIGH);
        fft();
    }
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
    // 在这里将波形数据发送回计算机端，并输出到串口
//    for (int i = 0; i < idx; i++) {
//        Serial.println(wave[i]);
//    }
}