//
// Created by 86159 on 2023-05-05.
//
#include "../lib/arduinoFFT/src/arduinoFFT.h"
#include "../lib/Keypad/src/Keypad.h"
#include "../lib/LiquidCrystal_I2C/LiquidCrystal_I2C.h"

// 定义波形默认参数
int defaultWaveType = 0;  // 波形类型，默认为正弦波
float defaultWaveFreq = 1000.0;  // 波形频率，默认为1kHz
float defaultSamplingFreq = 10000.0; // 采样频率，默认为10kHz
float defaultWaveDc = 0.0;   // 波形直流分量，默认为0
float defaultWaveAmpl = 1.0; // 波形幅值，默认为1
int defaultWaveSamples = 128; // 波形数据采样率，默认为100个样本点
int defaultPhoto = 0;   // 输出图像，默认原图

// 定义波形变量
int waveType = defaultWaveType;
float waveFreq = defaultWaveFreq;
float samplingFreq = defaultSamplingFreq;
float waveDc = defaultWaveDc;
float waveAmpl = defaultWaveAmpl;
int waveSamples = defaultWaveSamples;
int photo = defaultPhoto;
double wave[128];
int idx = 0;

// 设置 1602 液晶显示屏的 I2C 地址，宽度和高度
LiquidCrystal_I2C lcd(0x27, 16, 2);

const byte ROWS = 4; // 4行
const byte COLS = 4; // 4列

// 定义键盘上的按键字符
char keys[ROWS][COLS] = {
        {'1', '2', '3', 'A'},
        {'4', '5', '6', 'B'},
        {'7', '8', '9', 'C'},
        {'*', '0', '#', 'D'}
};

/*
 * 字符含义定义
 * 数字 - 数字
 * A - 加/上（加减/上下）
 * B - 右（左右）
 * C - 清空/归零/恢复默认值
 * D - 退格/删除（backspace/delete)
 * * - 小数点/正负号
 * # - 确认
 */

// 将行引脚连接到 Arduino Nano 的数字引脚
byte rowPins[ROWS] = {2, 3, 4, 5};

// 将列引脚连接到 Arduino Nano 的数字引脚
byte colPins[COLS] = {6, 7, 8, 9};

// 初始化键盘
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void fft() {   // 结果有误
    // 初始化 FFT 对象
    double vImag[waveSamples];
    memset(vImag, 0, waveSamples * sizeof(double));
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
    for (int i = 0; i < (waveSamples >> 1); i++) {
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
        if (waveType == 0) {   // 正弦波
            v += sin(2 * PI * t * waveFreq) * waveAmpl;
        } else if (waveType == 1) {   // 方波
            v += waveAmpl * (fmod(t * waveFreq, 1) < 0.5 ? -1 : 1);
        } else if (waveType == 2) {   // 三角波
            double trianglePhase = fmod(t * waveFreq, 1);
            v += waveAmpl * (2 * fabs(2 * trianglePhase - 1) - 1);
//            v += waveAmpl * (trianglePhase < 0.5 ? 4 * trianglePhase - 1 : 3 - 4 * trianglePhase);
        } else if (waveType == 3) {   // 锯齿波
            double sawtoothPhase = fmod(t * waveFreq, 1);
            v += waveAmpl * (2 * sawtoothPhase - 1);
        }
        wave[idx++] = v;
//        Serial.println(v);
    }
}

void draw() {
    if (photo == 0) {
        digitalWrite(LED_BUILTIN, LOW);
        for (int i = 0; i < idx; ++i) {
            Serial.println(wave[i]);
        }
    } else {
        digitalWrite(LED_BUILTIN, HIGH);
        fft();
    }
}

void parameterRead() {   // 参数输入
    digitalWrite(LED_BUILTIN, HIGH);
    // 解析输入命令
    int parameter = 0;   //正在输入的参数 0~6 分别对应变量定义里的七个参数
    String tmp = "";
    int idx = 0;   // tmp 的索引
    while (parameter <= 6) {
        lcd.setCursor(idx, 1);
        lcd.blink();
        lcd.cursor();
        // lcd 显示当前正在读取的参数
        switch (parameter) {
            case 0:
                lcd.setCursor(0, 0);
                lcd.print("Wave Type:");
                break;
            case 1:
                lcd.setCursor(0, 0);
                lcd.print("Wave Freq:");
                break;
            case 2:
                lcd.setCursor(0, 0);
                lcd.print("Samp Freq:");
                break;
            case 3:
                lcd.setCursor(0, 0);
                lcd.print("Wave Dc:  ");
                break;
            case 4:
                lcd.setCursor(0, 0);
                lcd.print("Wave Ampl:");
                break;
            case 5:
                lcd.setCursor(0, 0);
                lcd.print("Wave Samp:");
                break;
            case 6:
                lcd.setCursor(0, 0);
                lcd.print("Img Mode: ");
                break;
            default:
                break;
        }
        // 读取按键输入，赋值的主体部分
        char key = keypad.getKey();
        // 判定输入进行操作
        if (isdigit(key)) {
            if (idx < tmp.length()) {
                tmp[idx] = key;
            } else {
                tmp += key;
            }
            lcd.setCursor(idx, 1);
            lcd.print(key);
            idx++;
        } else {
            switch (key) {
                case 'A':
                    tmp[idx]+=1;
                    char i;
                    if (parameter!=0 && parameter!=6){
                        i = (tmp[idx]-'0')%10+'0';
                    } else if (parameter==0){
                        i  = (tmp[idx]-'0')%4+'0';
                    } else if (parameter==6){
                        i  = (tmp[idx]-'0')%2+'0';
                    }
                    tmp[idx] = i;
                    lcd.setCursor(idx, 1);
                    lcd.print(tmp[idx]);
                    break;
                case 'B':
                    idx = (idx + 1) % (tmp.length()+1);
                    break;
                case 'C':
                    tmp = "";
                    idx = 0;
                    lcd.clear();
                    break;
                case 'D':
                    idx--;
                    for (int j = idx; j < tmp.length(); ++j) {
                        lcd.setCursor(j,1);
                        lcd.print(" ");
                    }
                    tmp = tmp.substring(0,idx);
                    break;
                case '*':
                    if (idx==0){
                        if (idx < tmp.length()) {
                            tmp[idx] = '-';
                        } else {
                            tmp += '-';
                        }
                        lcd.setCursor(idx, 1);
                        lcd.print('-');
                        idx++;
                    } else{
                        if (idx < tmp.length()) {
                            tmp[idx] = '.';
                        } else {
                            tmp += '.';
                        }
                        lcd.setCursor(idx, 1);
                        lcd.print('.');
                        idx++;
                    }

                    break;
                case '#':
                    switch (parameter) {
                        case 0:
                            waveType = tmp[tmp.length() - 1]-'0';
//                            Serial.println(waveType);
                            break;
                        case 1:
                            waveFreq = tmp.toFloat();
//                            Serial.println(waveFreq);
                            break;
                        case 2:
                            samplingFreq = tmp.toFloat();
//                            Serial.println(samplingFreq);
                            break;
                        case 3:
                            waveDc = tmp.toFloat();
//                            Serial.println(waveDc);
                            break;
                        case 4:
                            waveAmpl = tmp.toFloat();
//                            Serial.println(waveAmpl);
                            break;
                        case 5:
                            waveSamples = (int) tmp.toFloat();
//                            Serial.println(waveSamples);
                            break;
                        case 6:
                            photo = (int) tmp.toFloat();
//                            Serial.println(photo);
                            break;
                        default:
                            break;
                    }
                    tmp = "";
                    idx = 0;
                    parameter++;
                    lcd.clear();
                    break;
                default:
                    break;
            }
        }
//        Serial.println(tmp);
    }
    delay(10000);
    digitalWrite(LED_BUILTIN, LOW);
}

void setup() {
    // 初始化串行通信
    Serial.begin(9600);
    // 初始化 LCD
    lcd.init();
    lcd.backlight();
    // 在 LCD 上显示按键字符
    lcd.setCursor(0, 0);   // 设置光标位置
    lcd.print("Key Press:");
    // 初始化 LED 灯
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
    // 任意按键输入
    if (keypad.getKey()) {
        parameterRead();
    }
    getWave();
    draw();
}
