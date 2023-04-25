//
// Created by 86159 on 2023-04-25.
// 初代
//
#include <Arduino.h>

String op;
char w;   // 波的类型 - Waveform
int mv = 0, a = 0, sr = 0;   // 中值 - Median Value、幅值 - Amplitude、采样率 - Sampling Rate
int x = 0;   // 横轴
bool level = true;   // 用于判定方波的高低电平(ture 高; false 低)和三角波的电平上升下降(ture 上升; false 下降)
float tmp = mv - a / 2;

void setup() {
// write your initialization code here
    Serial.begin(9600);
    Serial.println("请输入数据，数据包含以下内容:\n波的类型（方波:0;正弦波:1;三角波:2;锯齿波:4）、中值、幅值、采样率（Hz）\n注意: 数据请用”空格“分隔");
}

void loop() {
// write your code here
    if (Serial.available()) {
        mv = 0, a = 0, sr = 0, x = 0;   // 中值 - Median Value、幅值 - Amplitude、采样率 - Sampling Rate
        int judge = 0;   // 用于判定正在给哪个参数获取值
        bool sign = true;   //中值正负号

        op = Serial.readString();
        Serial.println(op);
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

        tmp = mv - a / 2;
        mv = sign ? mv : -mv;
    }

    if (w == '0') {
        Serial.println("这是方波");
        int high = mv + a / 2, low = mv - a / 2;
        x++;
        if (!(x % sr)) {
            level = !level;
        }
        Serial.println(level ? high : low);
    } else if (w == '1') {
        Serial.println("这是正弦波");
        Serial.println(sin(2 * PI / sr * x) * a / 2 + mv);
        x = (x + 1) % sr;
    } else if (w == '2') {
        Serial.println("这是三角波");
        Serial.println(tmp);
        x++;
        if (!(x % (sr/2))) {
            level = !level;
        }
        tmp = (level ? tmp + (float) 2 * a / sr : tmp - (float) 2 * a / sr);
    } else if (w == '3') {
        Serial.println("这是锯齿波");
        Serial.println(tmp);
        x++;
        if (x % sr == 0) { tmp = mv - (float) a / 2; }
        else { tmp += (float) a / sr; }
    }
}