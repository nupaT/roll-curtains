/*По нажатию на кнопку включается шаговый двигатель и крутиться заданное 
количество шагов либо останавливается сам (достиг конца) либо жмем 
кнопку он останавливается в произвольном месте жмем еще раз продолжает 
движение в том же направлении до конца. Если остановился сам доядя до 
конца, повторное нажатие кнопки запустит мотор в обрантном направлении*/

#include <AccelStepper.h>
// #include <AnalogKey.h>
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
AccelStepper motor1(4, IN1, IN2, IN3, IN4); //стоял 8 режимю - !!!Попробовать 4

GButton butt(12, LOW_PULL, NORM_OPEN);

bool moveStatus = LOW; //переменная хранит двигается мотор или нет
bool reverseStatus = 1; //переменная для смены направления при достижении конечной точки

long distance1 = 7000;// количество шагов, на которое должен пройти мотор (нижняя точка - шторы закрыты)

void setup() {

//Задаем максимальную скорость вращения двигателя(из даташита)
motor1.setMaxSpeed(800);  
motor1.setAcceleration(400); //ускорение шаг/сек если нужно плавно стартовать
motor1.setSpeed(800); //скорость двигателя в работе
motor1.moveTo(distance1); //на сколько шагов повернуться

//настройка кнопки
butt.setTimeout(300);         //время удержания в мс (по умолчанию 300)
butt.setType(LOW_PULL);       //кнопка подтянута к земле - управляется плюсом
butt.setDirection(NORM_OPEN); //нормально разомкнутая кнопка
butt.setTickMode(AUTO);       //кнопка опрашивается автоматически, если MANUAL то надо в цикле loop самому опрашивать


Serial.begin(9600);

}

void loop() 
{

  // если тапнули меняем статус мотора на противопложный
  if(butt.isClick()){
    //устанавливаем текущее положение мотора как начальное (нужно для корректной работы реверса)
    motor1.setCurrentPosition(motor1.currentPosition());
    //меняем статус мотора движется/не движется
    moveStatus = !moveStatus;
  }

  // если подеражали кнопку >0.3 сек - включили режим реверса  
  if(butt.isHolded() && moveStatus == LOW) {
    motor1.setCurrentPosition(motor1.currentPosition());
    //смена направления движения
    reverseStatus = !reverseStatus;
    moveStatus = HIGH;
  }

  if(moveStatus) {
    moveMotor(distance1, reverseStatus);
  }  else {
    motor1.disableOutputs();
  }

}

void moveMotor(long dist, bool reverse) {
  motor1.moveTo(dist*reverse);      //указываем конечную цель мотору

  if(motor1.distanceToGo() == 0) {    //если расстояние до цели равно 0 (достигли)
    reverseStatus = !reverseStatus;   //вкючили реверс (сменили направление движения)
    moveStatus = LOW;                 //изменяем статус движения мотора на LOW (выкл)
    motor1.disableOutputs();          //отключить питание от пинов мотора (не греется)
  }

  motor1.run();                       //команда пуска мотора
}

