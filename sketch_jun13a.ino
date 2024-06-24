#include <TaskScheduler.h>
#include <GyverEncoder.h>
#include <Bounce2.h>
#include <avr/eeprom.h>

#define CLK 12
#define DT 11
#define SW 10
Encoder enc(CLK, DT, SW);

void printOneSEG();
void printTwoSEG();
void printTreSEG();
void inCrementTime();
void deCrementTime();
void changeMode();
void toggleTimer();
void chekPushButton();

#define TIMETASK 1

Scheduler runner;
Task task1(TIMETASK, TASK_FOREVER , &printOneSEG);
Task task2(TIMETASK, TASK_FOREVER , &printTwoSEG);
Task task3(TIMETASK, TASK_FOREVER , &printTreSEG);
Task task4(TIMETASK, TASK_FOREVER , &inCrementTime);
Task task5(TIMETASK, TASK_FOREVER , &deCrementTime);
Task task6(500, TASK_FOREVER , &changeMode);

#define A 5
#define B 9 
#define C A2
#define D A4
#define E A5
#define F 6
#define G A1
#define DP A3
#define ONE 4
#define TWO 7
#define TRE 8

#define StartStop 2
#define INPUTFINISH 3

#define TIME 5

volatile uint16_t timer = 100;
volatile uint16_t tempTime;
volatile bool timerRunning = false, chekStop = false;
volatile bool mode = 0, exitFlag = false;

bool setingsForPrintNUM[10][8] = {
  {0,0,0,0,0,0,1,1}, //0
  {1,0,0,1,1,1,1,1}, //1
  {0,0,1,0,0,1,0,1}, //2
  {0,0,0,0,1,1,0,1}, //3
  {1,0,0,1,1,0,0,1}, //4
  {0,1,0,0,1,0,0,1}, //5
  {0,1,0,0,0,0,0,1}, //6
  {0,0,0,1,1,1,1,1}, //7
  {0,0,0,0,0,0,0,1}, //8
  {0,0,0,0,1,0,0,1}  //9
};

void setup() {
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(D, OUTPUT);
  pinMode(E, OUTPUT);
  pinMode(F, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(DP, OUTPUT);
  pinMode(ONE, OUTPUT);
  pinMode(TWO, OUTPUT);
  pinMode(TRE, OUTPUT);
  pinMode(StartStop, INPUT);
  pinMode(INPUTFINISH, OUTPUT);
  
  //digitalWrite(ONE, HIGH); // угадал
  //digitalWrite(TWO, HIGH); //угадал
  //digitalWrite(TRE, HIGH); //угадал
  //digitalWrite(A, LOW); // угадал
  //digitalWrite(B, LOW);
  //digitalWrite(C, LOW); // угадал
  //digitalWrite(D, LOW); // угадал
  //digitalWrite(E, LOW); 
  //digitalWrite(F, LOW); // угадал
  //digitalWrite(G, LOW); 
  //digitalWrite(DP, LOW); // угадал

  enc.setType(TYPE2);

  runner.init();

  // Добавляем задачи в планировщик
  runner.addTask(task1);
  runner.addTask(task2);
  runner.addTask(task3);
  runner.addTask(task4);
  runner.addTask(task5);
  runner.addTask(task6);

  // Запускаем задачи
  task1.enable();
  task2.enable();
  task3.enable();
  task4.enable();
  task5.enable();
  task6.enable();

  attachInterrupt(digitalPinToInterrupt(StartStop), toggleTimer, FALLING);
  
  eeprom_read_block((void*)&timer, 0, sizeof(timer));

  if(timer > 999 || timer < 1){
    timer = 60;
  }

  for (int i = 0; i < 100; i++) {
    printSNP();
  }
}

void loop() {
  runner.execute();
  eeprom_read_block((void*)&timer, 0, sizeof(timer));
  if (timerRunning) {
    exitFlag = false;
    startTimer();
  }
  else{
    digitalWrite(INPUTFINISH, LOW); 
    timerRunning = false;
    exitFlag = true;
  }
}

void toggleTimer() {
  timerRunning = !timerRunning;
}

void startTimer() {
  digitalWrite(INPUTFINISH, HIGH);
  if (!mode) {
    while (timerRunning && timer > 0) {
      if (exitFlag) { 
        digitalWrite(INPUTFINISH, LOW); // Проверяем флаг выхода
        return;
      }
      uint8_t temp = 40;
      while(temp > 0)
      {
        temp--;
        printTreSEG();
        delay(5);
        printOneSEG();
        delay(5);
        printTwoSEG();
        delay(5);
        printTreSEG();
        delay(5);
        printOneSEG();
        delay(5);
        printTwoSEG();
      }
      timer--;
    }
  } 
  else {
    tempTime = timer; 
    timer = 0;
    while (timerRunning && (timer < tempTime)) {
      if (exitFlag) { 
        digitalWrite(INPUTFINISH, LOW); // Проверяем флаг выхода
        return;
      }
      uint8_t temp = 40;
      while(temp > 0)
      {
        temp--;
        printTreSEG();
        delay(5);
        printOneSEG();
        delay(5);
        printTwoSEG();
        delay(5);
        printTreSEG();
        delay(5);
        printOneSEG();
        delay(5);
        printTwoSEG();
      }
      timer++;
    }
  }
  if (timer == 0 || timer == tempTime) {
    timerRunning = false;
    eeprom_read_block((void*)&timer, 0, sizeof(timer));
    digitalWrite(INPUTFINISH, LOW);
  }
}

void printSNP() {
  //----------------------------------------------------------C
  digitalWrite(ONE, HIGH);
  digitalWrite(TRE, LOW);
  digitalWrite(TWO, LOW);
  //----------------------------------------------------------
  digitalWrite(A, LOW);
  digitalWrite(B, HIGH);
  digitalWrite(C, HIGH);
  digitalWrite(D, LOW);
  digitalWrite(E, LOW);
  digitalWrite(F, LOW);
  digitalWrite(G, HIGH);
  //----------------------------------------------------------
  delay(TIME);//-----------------------------------------------
  //----------------------------------------------------------
  digitalWrite(TWO, HIGH);
  digitalWrite(ONE, LOW);
  digitalWrite(TRE, LOW);
  //----------------------------------------------------------
  digitalWrite(A, HIGH);
  digitalWrite(B, LOW);
  digitalWrite(C, LOW);
  digitalWrite(D, HIGH);
  digitalWrite(E, LOW);
  digitalWrite(F, LOW);
  digitalWrite(G, LOW);
  //----------------------------------------------------------
  delay(TIME);//-----------------------------------------------
  //----------------------------------------------------------
  digitalWrite(TRE, HIGH);
  digitalWrite(ONE, LOW);
  digitalWrite(TWO, LOW);
  //----------------------------------------------------------
  digitalWrite(A, LOW);
  digitalWrite(B, LOW);
  digitalWrite(C, LOW);
  digitalWrite(D, HIGH);
  digitalWrite(E, LOW);
  digitalWrite(F, LOW);
  digitalWrite(G, HIGH);
  //----------------------------------------------------------
  delay(TIME);//-----------------------------------------------
  digitalWrite(A, HIGH);
  digitalWrite(B, HIGH);
  digitalWrite(C, HIGH);
  digitalWrite(D, HIGH);
  digitalWrite(E, HIGH);
  digitalWrite(F, HIGH);
  digitalWrite(G, HIGH);
  digitalWrite(DP, HIGH);
}

void printOneSEG() {
  uint8_t Num1 = timer / 100;
  if(Num1 == 0)
  {
    return;
  }
  //---------------СЕГМЕНТ 1-------------------------------------------
  digitalWrite(TRE, LOW);
  digitalWrite(TWO, LOW);
  digitalWrite(A, setingsForPrintNUM[Num1][0]);
  digitalWrite(B, setingsForPrintNUM[Num1][1]);
  digitalWrite(C, setingsForPrintNUM[Num1][2]);
  digitalWrite(D, setingsForPrintNUM[Num1][3]);
  digitalWrite(E, setingsForPrintNUM[Num1][4]);
  digitalWrite(F, setingsForPrintNUM[Num1][5]);
  digitalWrite(G, setingsForPrintNUM[Num1][6]);
  digitalWrite(DP, setingsForPrintNUM[Num1][7]);  
  digitalWrite(ONE, HIGH);
}

void printTwoSEG() {
  uint8_t Num2 = (timer / 10) % 10;
  uint8_t Num1 = timer / 100;
  if(Num2 == 0 && Num1 == 0)
  {
    return;
  }
  //---------------СЕГМЕНТ 2-------------------------------------------
  digitalWrite(ONE, LOW);
  digitalWrite(TRE, LOW);
  digitalWrite(A, setingsForPrintNUM[Num2][0]);
  digitalWrite(B, setingsForPrintNUM[Num2][1]);
  digitalWrite(C, setingsForPrintNUM[Num2][2]);
  digitalWrite(D, setingsForPrintNUM[Num2][3]);
  digitalWrite(E, setingsForPrintNUM[Num2][4]);
  digitalWrite(F, setingsForPrintNUM[Num2][5]);
  digitalWrite(G, setingsForPrintNUM[Num2][6]);
  digitalWrite(DP, setingsForPrintNUM[Num2][7]);
  digitalWrite(TWO, HIGH);
}

void printTreSEG() {
  uint8_t Num3 = timer % 10;
  //---------------СЕГМЕНТ 3-------------------------------------------
  digitalWrite(ONE, LOW);
  digitalWrite(TWO, LOW);
  digitalWrite(A, setingsForPrintNUM[Num3][0]);
  digitalWrite(B, setingsForPrintNUM[Num3][1]);
  digitalWrite(C, setingsForPrintNUM[Num3][2]);
  digitalWrite(D, setingsForPrintNUM[Num3][3]);
  digitalWrite(E, setingsForPrintNUM[Num3][4]);
  digitalWrite(F, setingsForPrintNUM[Num3][5]);
  digitalWrite(G, setingsForPrintNUM[Num3][6]);
  digitalWrite(DP, setingsForPrintNUM[Num3][7]);
  digitalWrite(TRE, HIGH);
}

void inCrementTime() {
  enc.tick();
  if (enc.isRight()) {
    timer++;
    if (timer > 999) {
      timer = 1;
    }
    eeprom_update_block((void*)&timer, 0, sizeof(timer));
  }
}

void deCrementTime() {
  enc.tick();
  if (enc.isLeft()) {
    timer--;
    if (timer < 1) {
      timer = 999;
    }
    eeprom_update_block((void*)&timer, 0, sizeof(timer));
  }
}

void changeMode() {
  enc.tick();
  if (enc.isHold()) {
    digitalWrite(TRE, HIGH);
    digitalWrite(ONE, LOW);
    digitalWrite(TWO, LOW);
    if (mode) {
      //----------------------------------------------------------
      digitalWrite(G, HIGH);
      digitalWrite(D, HIGH);
      digitalWrite(A, LOW);
      digitalWrite(F, LOW);
      digitalWrite(E, LOW);
      digitalWrite(B, LOW);
      digitalWrite(C, LOW);
    } else {
      digitalWrite(G, LOW);
      digitalWrite(D, HIGH);
      digitalWrite(A, HIGH);
      digitalWrite(F, LOW);
      digitalWrite(E, LOW);
      digitalWrite(B, LOW);
      digitalWrite(C, LOW);
    }
    delay(3000);
    enc.tick();
    if (enc.isHolded()) {
      mode = !mode;
      if (mode) {
        digitalWrite(G, HIGH);
        digitalWrite(D, HIGH);
        digitalWrite(A, LOW);
        digitalWrite(F, LOW);
        digitalWrite(E, LOW);
        digitalWrite(B, LOW);
        digitalWrite(C, LOW);
      } else {
        digitalWrite(G, LOW);
        digitalWrite(D, HIGH);
        digitalWrite(A, HIGH);
        digitalWrite(F, LOW);
        digitalWrite(E, LOW);
        digitalWrite(B, LOW);
        digitalWrite(C, LOW);
      }
      delay(2000);
    }
  }
}
