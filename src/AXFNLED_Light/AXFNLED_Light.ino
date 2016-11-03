/*By Novang*/
#include <Keyboard.h>

#define KEYA_CHAR   'a' // 키보드 a의 입력값을 저장하는 변수.
#define KEYX_CHAR   'x' // 키보드 x의 입력값을 저장하는 변수
#define KEYFN_CHAR  KEY_ESC // ESC 의 입력값을 저장하는 변수
#define BAUDRATE    250000  // baudrate for serial communication 1초동안 전송되는 신호의 수
#define KP_A        A0      // a key pin number  키패드 디지털 핀 지정
#define KP_X        A1      // x key pin number
#define KP_FN       A2      // fn key pin number

#define LP_A        9       // a led pin number led 디지털 핀 지정
#define LP_X        10      // x led pin number
#define LP_FN       11      // fn led pin number

#define PROC_SPD    100       // processing speed (us)  반복 속도
#define LD          255     // led power (0-255) 밝기
#define LD_TM       1     // led minus count (0-255) 밝기를 줄여주기 위한 변수

unsigned short V_A = 0, V_X = 0, V_FN = 0;           // for multi-key input processing (동시 입력값)
unsigned short _ON_A = 0, _ON_X = 0, _ON_FN = 0;     // for prevent unnecessory processing(예방)
unsigned short TM_A = LD, TM_X = LD, TM_FN = LD;   // fading tmp value (각 키패드를 눌렀을 때 led값을 저장)

/* 
 * Init
*/
void setup() {
  Serial.begin(BAUDRATE); //
  Serial.print("Arduino based osu keyboard. BY NOVANG! Welcome!\n");
  pinMode(KP_A,INPUT_PULLUP);     // define pinmode 핀모드를 정의한다.
  pinMode(KP_X,INPUT_PULLUP);   // 아두이노 내부에 있는 풀업 저항을 이용하는 함수
  pinMode(KP_FN,INPUT_PULLUP);
  pinMode(LP_A,OUTPUT);     //각각 LP 변수에 저장되어 있는 값 출력 
  pinMode(LP_X,OUTPUT);
  pinMode(LP_FN,OUTPUT);
  digitalWrite(LP_A,LD);    // indicate start-up
  digitalWrite(LP_X,LD);    //led의 값을 LP_ 값에 디지털 신호로 입력한다.
  digitalWrite(LP_FN,LD);
  delay(1000); 
}

/* 
 * Debug
*/
void debug() {    //디버그를 할 때 사용하는 함수, 각 변수값을 확인 할 수 있도록 출력한다.
  Serial.print("KEY: ");  
  Serial.print(digitalRead(KP_A));
  Serial.print(" ");
  Serial.print(digitalRead(KP_X));
  Serial.print(" ");
  Serial.print(digitalRead(KP_FN));
  Serial.print("   ");
  Serial.print("LED: ");
  Serial.print(digitalRead(LP_A));
  Serial.print(" ");
  Serial.print(digitalRead(LP_X));
  Serial.print(" ");
  Serial.print(digitalRead(LP_FN));
  Serial.print("\n");
}

/*
 * Read pin connection value
*/
void READPIN() { //동시 입력을 할 때 사용되는 함수로서, 각 변수의 값을 읽어들인다.
  V_FN = digitalRead(KP_FN);
  V_A = digitalRead(KP_A);
  V_X = digitalRead(KP_X);
}

/*
 * Loop
*/
void loop() { //본격적인 메인함수.
  delayMicroseconds(PROC_SPD); //너무 빨리 값을 읽는 것을 방지하기 위한 딜레이.
  READPIN(); //동시 입력 함수
  //debug(); //디버그 함수
  
  if(V_FN == 0) { //exc 값을 검사하는 조건 검사문
    if(_ON_FN == 0) { //입력 신호가 0이면 exc값을 입력하도록 한다.
      Keyboard.press(KEYFN_CHAR);    // fn(`) key (replay)  release 함수가 호출되기 전까지 계속 누르고 있는 동작
      _ON_FN = 1;  //신호를 바꾼다.
      analogWrite(LP_FN,LD); //아날로그 신호를 출력하기 위한 함수, (신호를 출력할 핀, 출력할 신호)
      TM_FN = LD; // TM_FN 변수에 LD 값을 저장한다.(LED가 밝기 값만큼 켜짐)
    }
  }else if(V_FN == 1) { //동시 입력을 멈출 것을 검사
    if(_ON_FN == 1) { // 입력을 멈추는 것인지 검사.
      Keyboard.release(KEYFN_CHAR); //멈추면 입력을 멈추는 신호를 보낸다.
      _ON_FN = 0; //값 저장
    }
    if(TM_FN > 0) { // 한번에 LED가 꺼지지 않고, 서서히 꺼지도록 검사하는 조건문
      TM_FN = TM_FN - LD_TM; //255 였던 값이 1씩 빠르게 줄어든다.
      analogWrite(LP_FN, TM_FN); //led의 밝기를 조절할 수 있기 때문에 아날로그 신호를 쓴다.
    }
  }
  
  if(V_A == 0) {  //exc 와 똑같은 기능을 한다.
    if(_ON_A == 0) {
      Keyboard.press(KEYA_CHAR);    // a key (a)
      _ON_A = 1;
      analogWrite(LP_A,LD);
      TM_A = LD;
    }
  }else if(V_A == 1) {
    if(_ON_A == 1) {
      Keyboard.release(KEYA_CHAR);
      _ON_A = 0;
    }
    if(TM_A > 0) {
      TM_A = TM_A - LD_TM;
      analogWrite(LP_A, TM_A); 
    }
  }

  if(V_X == 0) { //마찬가지.
    if(_ON_X == 0) {
      Keyboard.press(KEYX_CHAR);    // x key (x)
      _ON_X = 1;
      analogWrite(LP_X,LD);
      TM_X = LD;
    }
  }else if(V_X == 1) {
    if(_ON_X == 1) {
      Keyboard.release(KEYX_CHAR);
      _ON_X = 0;
    }
    if(TM_X > 0) {
      TM_X = TM_X - LD_TM;
      analogWrite(LP_X, TM_X);
    }
  }
}
