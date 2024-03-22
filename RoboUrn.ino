  /*
  Created 2024
  by RodX
  RodX Labs Inc.
  
  RoboUrn
  Роботизированная корзина для мусора. 
  Открывает и плавно закрывает крышку при поднесении на 10-30 см. двух ладоней, мерцает светодиодом при обнаружении рук на расстоянии 30-50см. 
  Оригинальнвый механизм поднимания крышки - только саморезы сервопривода и термоклей.
  Arduino NANO, HC-SR04, SG-90 2шт.
  */

#include <Servo.h>   //используем библиотеку для работы с сервоприводом
Servo servoL, servoR;     //объявляем переменную servo типа Servo. Если скленить донья серв, то получится левая и правая рука  дном
int iTop = 90; //верхня точка
int iBottom = 40; //нижняя точка
int trigPin = 2;    //Триггер
int echoPin = 4;    //Эхо
long duration, dist;   //переменные
long aver[3];   //массив для расчёта среднего арифметического

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  servoR.attach(8);  //серво на 8 порту
  servoL.attach(6);  //серво на 6 порту     
  pinMode(trigPin, OUTPUT);  //пин триггера в режиме выхода
  pinMode(echoPin, INPUT);  //пин эхо в режиме входа
  servoR.write(iTop);  //(0-низ), при подключении питания открыть_
  servoL.write(180 - iTop);  //(180-низ)при подключении питания открыть_  
  delay(500);
  slowly();  //_закрыть крышку
}

void measure() {  //блок, измеряющий расстояние
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(15);
  digitalWrite(trigPin, LOW);
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
  dist = (duration / 2) / 29.1;  //получаем расстояние с датчика
}

void slowly() {    //функция плавного закрывания
  digitalWrite(LED_BUILTIN, HIGH); //подсветим закрытие крышки
  for (int i = iTop - 1; i > iBottom; i--) {      
      servoR.write(i);     // опускаем правым серво крышку на один шаг
      servoL.write(180 -i);   //одновременно левым серво опускаем на один шаг      
      delay(20);              //рекомендуемая задержка между шагами
    }
  digitalWrite(LED_BUILTIN, LOW); 
}

void loop() { 
  
    //блок более точного(тройного) измерения расстояния до руки с мусором
    for (int i = 0; i <= 2; i++) { //заполняем массив тремя измерениями
      measure();               //измерить расстояние, получаем dist
      aver[i] = dist;          //присваиваем значение dist элементу массива с номером i
      delay(50);              //рекомендуемая задержка между измерениями
    }
    dist = (aver[0] + aver[1] + aver[2]) / 3; //расчёт среднего арифметического с 3-х измерений
 
    if (dist > 10 && dist < 30){ //Если рука с мусором от 10 до 30 см, то поднимем крышку и медленно опустим
      servoR.write(iTop);  //тянем вверх крышку правым серво
      servoL.write(180-iTop);  //одновременно тянем вверх левым серво 
      delay(2000);       //подождать 2 секунды
      slowly();    //медленно закрыть крышку
      delay(500);      //подождать
    }

    //Мерцание индикатором на расстоянии 30-50 см.
    if (dist > 30 && dist < 50){ //Если   рука с мусором от 10 до 30 см, то поднимем крышку и медленно опустим    
      digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
      delay(50);                      // wait for a second
      digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
      delay(50);                      // wait for a second      delay(500);      //подождать
    }

}
