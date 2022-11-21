#include <math.h>
#include "gpschsk.h"

float altToTilt(float distance, float alt)
{
  float y = distance, x = alt;
  if (x <= 0) x = (x * -1);
  if (y == 0) y = 1;
  float z = asin(x / ( sqrt( (x * x) + (y * y) ) ) ) * 180 / 3.1415;
  // return z;
  float step = ((float)maxTiltValue - (float)minTiltValue) / 90;
  z = step * z + minTiltValue;
  if (z >= maxTiltValue) z = maxTiltValue;
  if (z <= minTiltValue) z = minTiltValue;
  return (z);
}


float coordToPan()
{
  /*
  Будем использовать формулу угла между векторами
  https://ru.onlinemschool.com/math/library/vector/angl/#:~:text=%D0%A3%D0%B3%D0%BB%D0%BE%D0%BC%20%D0%BC%D0%B5%D0%B6%D0%B4%D1%83%20%D0%B4%D0%B2%D1%83%D0%BC%D1%8F%20%D0%B2%D0%B5%D0%BA%D1%82%D0%BE%D1%80%D0%B0%D0%BC%D0%B8%2C%20%D0%BE%D1%82%D0%BB%D0%BE%D0%B6%D0%B5%D0%BD%D0%BD%D1%8B%D0%BC%D0%B8,%D0%BF%D0%BE%D0%BB%D0%BE%D0%B6%D0%B5%D0%BD%D0%B8%D1%8F%20%D1%81%D0%BE%D0%BD%D0%B0%D0%BF%D1%80%D0%B0%D0%B2%D0%BB%D0%B5%D0%BD%D0%BD%D0%BE%D1%81%D1%82%D0%B8%20%D1%81%20%D0%B4%D1%80%D1%83%D0%B3%D0%B8%D0%BC%20%D0%B2%D0%B5%D0%BA%D1%82%D0%BE%D1%80%D0%BE%D0%BC.&text=%D0%9A%D0%BE%D1%81%D0%B8%D0%BD%D1%83%D1%81%20%D1%83%D0%B3%D0%BB%D0%B0%20%D0%BC%D0%B5%D0%B6%D0%B4%D1%83%20%D0%B2%D0%B5%D0%BA%D1%82%D0%BE%D1%80%D0%B0%D0%BC%D0%B8%20%D1%80%D0%B0%D0%B2%D0%B5%D0%BD,%D0%BF%D0%BE%D0%B4%D0%B5%D0%BB%D0%B5%D0%BD%D0%BD%D0%BE%D0%BC%D1%83%20%D0%BD%D0%B0%20%D0%BF%D1%80%D0%BE%D0%B8%D0%B7%D0%B2%D0%B5%D0%B4%D0%B5%D0%BD%D0%B8%D0%B5%20%D0%BC%D0%BE%D0%B4%D1%83%D0%BB%D0%B5%D0%B9%20%D0%B2%D0%B5%D0%BA%D1%82%D0%BE%D1%80%D0%BE%D0%B2.
 Сначала находим 2 вектора: первый вектор стартовой позиции дрона, второй - текущей
 Для этого вычитаем из точек точку антены
 127 и 199 - "волшебные константы" для приведения широты/долготы в одни единицы измерения
 можно заменить на один коэфициет
*/

float lat1 = antPoz.lat / DIVIDER;   // ant
float long1 = antPoz.lon / DIVIDER;  // ant
float lat2 = homePoz.lat / DIVIDER;     // dron home
float long2 = homePoz.lon / DIVIDER;    // dron home
float lat3 = curPoz.lat / DIVIDER;
float long3 = curPoz.lon / DIVIDER;

 float vektor1[2], vektor2[2];
 vektor1[0] = (long2 - long1) * 127;
 vektor1[1] = (lat2 - lat1) * 199;
 vektor2[0] = (long3 - long1) * 127;
 vektor2[1] = (lat3 - lat1) * 199;

 float scalar = vektor1[0] * vektor2[0] + vektor1[1] * vektor2[1];

 float modul1 = sqrt(vektor1[0]*vektor1[0] + vektor1[1]*vektor1[1]);
 float modul2 = sqrt(vektor2[0]*vektor2[0] + vektor2[1]*vektor2[1]);

 // Отдельно находим знак угла (против часовой или по часовой)
 // https://www.cyberforum.ru/algebra/thread1503757.html
 float sign = vektor1[0]*vektor2[1] - vektor1[1]*vektor2[0];

 float angle = acos(scalar/(modul1*modul2)) * 180 / M_PI;

 float angleAnt = (sign > 0) ? angle : (angle * -1);

 //angle = (sign) ? angle : (angle * -1);

 // ставим знак + перевод угла из радиан в градусы
 // angleSpan.innerHTML = sign > 0 ? "- " + angle * 180 / Math.PI : angle * 180 / Math.PI;
 float delta = ((float)maxPanValue - (float)minPanValue);
 float step = 9.7; //delta / 360.0;
 printf ("delta : %f, Step : %f, Sign : %f, %f\n", delta, step, sign, angleAnt);
 angleAnt = angleAnt * step + delta / 2;
 if (angleAnt >= maxPanValue) angleAnt = maxPanValue;
 if (angleAnt <= minPanValue) angleAnt = minPanValue;
    return (angleAnt);
};
