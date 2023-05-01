//
// Created by 86159 on 2023-04-25.
// 初代
//
#include <Arduino.h>

// 数据读取与波形绘制部分
String op;
char w;   // 波的类型 - Waveform
int mv, a, sr;   // 中值 - Median Value、幅值 - Amplitude、采样率 - Sampling Rate
int x;   // 横轴
bool level;   // 用于判定方波的高低电平(ture 高; false 低)和三角波的电平上升下降(ture 上升; false 下降)
float tmp;
int judge;   // 用于判定正在给哪个参数获取值
bool sign;   //中值正负号

void myInit() {
    mv = 0, a = 0, sr = 0;   // 中值 - Median Value、幅值 - Amplitude、采样率 - Sampling Rate
    x = 0;   // 横轴
    level = true;   // 用于判定方波的高低电平(ture 高; false 低)和三角波的电平上升下降(ture 上升; false 下降)
    tmp = mv - (float) a / 2;
    judge = 0;   // 用于判定正在给哪个参数获取值
    sign = true;   //中值正负号
}

void squareWave(){   // 方波
    int high = mv + a / 2, low = mv - a / 2;
    x++;
    if (!(x % sr)) {
        level = !level;
    }
    int out = level ? high : low;
    Serial.println(out);
}

void sineWave(){   // 正弦波
    double out = sin(2 * PI / sr * x) * a / 2 + mv;
    Serial.println(out);
    x = (x + 1) % sr;
}

void triangleWave(){   // 三角波
    int high = mv + a / 2, low = mv - a / 2;
    float k = (float) 2 * a / sr;
    Serial.println(tmp);
    x++;
    if (tmp+k>high) {
        level = false;
    }
    if (tmp-k<low){
        level = true;
    }
    tmp = (level ? tmp + k : tmp - k);
}

void sawtoothWave(){   // 锯齿波
    Serial.println(tmp);
    x++;
    if (x % sr == 0) { tmp = mv - (float) a / 2; }
    else { tmp += (float) a / sr; }
}


void setup() {
// write your initialization code here
    Serial.begin(9600);
    Serial.println(
            "请输入数据，数据包含以下内容:\n波的类型（方波:0;正弦波:1;三角波:2;锯齿波:4）、中值、幅值、采样率（Hz）\n注意: 数据请用”空格“分隔");
    myInit();
}

void loop() {
// write your code here
    if (Serial.available()) {
        // 初始化
        myInit();

        op = Serial.readString();
        // Serial.println(op);
        for (char i: op) {
            if (i == ' ') {
                judge++;
            } else if (i == '-') {
                sign = false;
            } else {
                if (judge == 0) {
                    w = i;
                } else if (judge == 1) {
                    mv = mv * 10 + (i - '0');
                } else if (judge == 2) {
                    a = a * 10 + (i - '0');
                } else if (judge == 3) {
                    sr = sr * 10 + (i - '0');
                }
            }
        }

        mv = sign ? mv : -mv;
        tmp = mv - (float) a / 2;
    }

    if (w == '0') {
        Serial.println("这是方波");
        squareWave();
    } else if (w == '1') {
        Serial.println("这是正弦波");
        sineWave();
    } else if (w == '2') {
        Serial.println("这是三角波");
        triangleWave();
    } else if (w == '3') {
        Serial.println("这是锯齿波");
        sawtoothWave();
    }
}