/*По нажатию на кнопку включается шаговый двигатель и крутиться заданное 
количество шагов либо останавливается сам (достиг конца) либо жмем 
кнопку он останавливается в произвольном месте жмем еще раз продолжает 
движение в том же направлении до конца. Если остановился сам доядя до 
конца, повторное нажатие кнопки запустит мотор в обрантном направлении*/

#include <AccelStepper.h>
#include <AnalogKey.h>
#include <GyverButton.h>
//с Гайверовской библиотекой для шаговых двигателей что-то работает не так.
//довести до ума с AccleStepper - потом разбираться с Гайверовской

//пины к которым подключено контроллер двигателя
#define IN1 11
#define IN2 9
#define IN3 10
#define IN4 8

//задаем имя мотору 8 или 4 (полушаговый - 8, шаговый - 4 режим работы)
// полушаговый мощнее но медленнее
AccelStepper motor1(4, IN1, IN2, IN3, IN4);

GButton butt(12, LOW_PULL, NORM_OPEN);

bool moveStatus = LOW; //переменная хранит двигается мотор или нет
int setStatus = 1;
int distance1 = 2048; // коичество шагов, на которое должен пройти мотор

void setup() {

//Задаем максимальную скорость вращения двигателя(из даташита)
motor1.setMaxSpeed(600);  
motor1.setAcceleration(200); //ускорение шаг/сек если нужно плавно стартовать
motor1.setSpeed(200); //скорость двигателя в работе
motor1.moveTo(distance1); //на сколько шагов повернуться

//настройка кнопки
butt.setTimeout(500);         //время удержания в мс (по умолчанию 300)
butt.setType(LOW_PULL);       //кнопка подтянута к земле - управляется плюсом
butt.setDirection(NORM_OPEN); //нормально разомкнутая кнопка
butt.setTickMode(AUTO);       //кнопка опрашивается автоматически, если MANUAL то надо в цикле loop самому опрашивать


Serial.begin(9600);

}

void loop() 
{

  if(butt.isClick()){
    moveStatus = !moveStatus;
  }

  if(moveStatus) {
    moveMotor(distance1);
  }
}

void moveMotor(long dist) {
  motor1.moveTo(dist*setStatus);      //указываем конечную цель мотору
  if(motor1.distanceToGo() == 0) {    //если расстояние до цели равно 0 (достигли)
    setStatus = setStatus*(-1);       //меняем статус мотору на противоположный (крутим в обратную сторону)
    motor1.setCurrentPosition(0);     //определяем текущее положение как начальное (мотор в нулевой точке)
    moveStatus = LOW;                 //изменяем статус движения мотора на LOW (выкл)
    motor1.disableOutputs();          //отключить питание от пинов мотора (не греется)
  }
  motor1.run();                       //команда пуска мотора
}
