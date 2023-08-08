#include <stdint.h>
#include <stdio.h>
#include "filter.h"

/*!
  \brief Функция простейшей сортировки массива.
  Пожалуй самая неэффективная, но железобетонно работающая.
  \return Не возвращает ничего
  \param[in,out] p - указатель на массив, который необходимо отсортировать
  \param[in] n - размер массива в элементах
 */
void bubble_sort (uint16_t *p, size_t n)
{
    uint16_t temp;

    if (n < 2) return;

    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 1; j < (n-i); j++)
        {
            if (p[j-1] > p[j])
            {
                temp = p[j-1];
                p[j-1] = p[j];
                p[j] = temp;
            }
        }
    }
}

/*!
  \brief Функция сортировки Шелла
  \return Не возвращает ничего
  \param[in,out] p - указатель на массив, который необходимо отсортировать
  \param[in] n - размер массива в элементах
*/
void shall_sort (uint16_t *p, size_t n)
{
    uint16_t  temp;

    if (n < 2) return;

    int d = n / 2;
    while (d > 0)
    {
        for (int i = 0; i < n - d; i++)
        {
            int j = i;
            while (j >= 0 && p[j] > p[j+d])
            {
                temp = p[j];
                p[j] = p[j+d];
                p[j+d] = temp;
                j--;
            }
        }
        d /= 2;
    }
}

/*!
  \brief Функция медианного фильтра.
  Значения отсчётов внутри окна фильтра сортируются в порядке возрастания (убывания);
  и значение, находящееся в середине упорядоченного списка, поступает на выход фильтра
  \warning Исходный массив после вызова медианного фильтра будет отсортирован!
  \return выходное значение фильтра
  \param[in] p - указатель на массив с выборкой
  \param[in] n - размер выборки (в элементах массива)
*/
uint16_t mediane_filter (uint16_t *p, size_t n)
{
    if (!n) return 0;

    shall_sort(p, n);

    size_t i = n/2;
    if (n & 1)  return (p[i]);
    else        return (((uint32_t)p[i-1]+p[i])/2);
}

/*!
  \brief Функция взятия медианы по трем значениям       [uint16_t]
  (для цифровых сигналов это вроде называется признаком мажоритарности)
  \return Значение медианы
  \param[in] a - первое значение
  \param[in] b - второе значение
  \param[in] c - третье значение
*/
uint16_t mediane_3p_u16 (uint16_t a, uint16_t b, uint16_t c)
{
    if (a > b)
    {
        // ba ? c
        if (c > a)
        {
            // bac
            return a;
        }
        else
        {
            return (b > c) ? b : c;
        }
    }
    // ab ? c
    if (c > b)
    {
        // abc
        return b;
    }
    return (a > c) ? a : c;
}

/*!
  \brief Функция взятия медианы по трем значениям       [uint16_t]
  (для цифровых сигналов это вроде называется признаком мажоритарности)
  \return Значение медианы
  \param[in] p указатель на массив из минимум 3 элементов
*/
uint16_t mediane_3p_pointer_u16 (uint16_t *p)
{
    return mediane_3p_u16(p[0], p[1], p[2]);
}

/*!
  \brief Функция взятия медианы по трем значениям       [int]
  (для цифровых сигналов это вроде называется признаком мажоритарности)
  \return Значение медианы
  \param[in] a - первое значение
  \param[in] b - второе значение
  \param[in] c - третье значение
*/
int mediane_3p_int (int a, int b, int c)
{
    if (a > b)
    {
        // ba ? c
        if (c > a)
        {
            // bac
            return a;
        }
        else
        {
            return (b > c) ? b : c;
        }
    }
    // ab ? c
    if (c > b)
    {
        // abc
        return b;
    }
    return (a > c) ? a : c;
}

/*!
  \brief Функция взятия медианы по трем значениям       [int]
  (для цифровых сигналов это вроде называется признаком мажоритарности)
  \return Значение медианы
  \param[in] p указатель на массив из минимум 3 элементов
*/
int mediane_3p_pointer_int (int *p)
{
    return mediane_3p_int(p[0], p[1], p[2]);
}

/*!
  \brief Функция взятия медианы по трем значениям       [float]
  (для цифровых сигналов это вроде называется признаком мажоритарности)
  \return Значение медианы
  \param[in] a - первое значение
  \param[in] b - второе значение
  \param[in] c - третье значение
*/
float mediane_3p_float (float a, float b, float c)
{
    if (a > b)
    {
        // ba ? c
        if (c > a)
        {
            // bac
            return a;
        }
        else
        {
            return (b > c) ? b : c;
        }
    }
    // ab ? c
    if (c > b)
    {
        // abc
        return b;
    }
    return (a > c) ? a : c;
}

/*!
*  \brief Функция взятия медианы по трем значениям       [float]
  (для цифровых сигналов это вроде называется признаком мажоритарности)
  \return Значение медианы
  \param[in] p указатель на массив из минимум 3 элементов
*/
float mediane_3p_pointer_float (float *p)
{
    return mediane_3p_float(p[0], p[1], p[2]);
}

/*!
  \brief Функция экспоненциального сглаживания сигнала.
  Известен также как метод Брауна.
  \return Текущее значение сглаженного сигнала
  \param[in] current - указатель на новое значение выборки
  \param[in] previous - указатель на предыдущее значение сглаженного сигнала
  \param[in] kexp - коэффициент сглаживания, kexp = (0..1)
*/
float exp_smoothing (float *current, float *previous, float kexp)
{
    return (*previous + kexp * (*current-*previous));
}

/*!
  \brief Функция инициализации фильтра низких частот (Low Pass Filter) [int]
  \return не возвращает значений
  \param[in] filter - указатель на объект фильтра [int]
  \param[in] data_in - текущее значение сигнала, которым будет инициирован фильтр
  \details Перед использованием должно быть задано поле filter.length, которое определяет
  эквивалент постоянной времени. Для частоты вызова функции фильтра в 1 кГц, значение поля
  соответствует милисекундам.
  \deprecated Лучше использовать filter_low_pass_boost
*/
void filter_low_pass_init_int (filter_low_pass_i_t *filter, int data_in)
{
    filter->data_sum = data_in * filter->length;
    filter->data_out = data_in;
}

/*!
  \brief Функция шага фильтра низких частот (Low Pass Filter) [int]
  \return возвращает отфильтрованное значение сигнала
  \param[in] filter - указатель на объект фильтра [int]
  \param[in] data_in - текущее значение сигнала
  \deprecated Лучше использовать filter_low_pass_boost
*/
int filter_low_pass_int (filter_low_pass_i_t *filter, int data_in)
{
    filter->data_sum = filter->data_sum + data_in - filter->data_out;
    filter->data_out = filter->data_sum / filter->length;
    return (filter->data_out);
}

/*!
  \brief Функция инициализации фильтра низких частот (Low Pass Filter) [float]
  \return не возвращает значений
  \param[in] filter - указатель на объект фильтра [float]
  \param[in] data_in - текущее значение сигнала, которым будет инициирован фильтр
  \details Перед использованием должно быть задано поле filter.length, которое определяет
  эквивалент постоянной времени. Для частоты вызова функции фильтра в 1 кГц, значение поля
  соответствует милисекундам.
  \deprecated Лучше использовать filter_low_pass_boost
*/
void filter_low_pass_init_float (filter_low_pass_f_t *filter, float data_in)
{
    filter->data_sum = data_in * filter->length;
    filter->data_out = data_in;
}

/*!
  \brief Функция шага фильтра низких частот (Low Pass Filter) [float]
  \return возвращает отфильтрованное значение сигнала
  \param[in] filter - указатель на объект фильтра [float]
  \param[in] data_in - текущее значение сигнала
  \deprecated Лучше использовать filter_low_pass_boost
*/
float filter_low_pass_float (filter_low_pass_f_t *filter, float data_in)
{
    filter->data_sum = filter->data_sum + data_in - filter->data_out;
    filter->data_out = filter->data_sum / filter->length;
    return (filter->data_out);
}

/*!
  \brief Функция фильтра - среднего арифметического
  \return возвращает отфильтрованное значение сигнала
  \param[in] filter - указатель на объект фильтра
  \param[in] data_in - текущее значение сигнала
  \details Перед использованием должно быть задано поле filter.length, которое определеяет
  по какому количеству выборок делать усреднение
*/
int filter_arithmetic_mean (filter_arithmetic_mean_t *filter, int data_in)
{
    filter->data_sum += data_in;
    filter->data_cnt++;

    if (filter->data_cnt > (filter->length - 1))
    {
        filter->data_out = filter->data_sum / filter->length;
        filter->data_sum = 0;
        filter->data_cnt = 0;
    }

    return (filter->data_out);
}

/*!
  \brief Функция фильтра низких частот (Low Pass Filter)
  \details Функция работает как фильтр низких частот с одной оговоркой - если разница между входным сигналом
  и предыдущим выходным значением превышает значение up_boost_delta в плюс или down_boost_delta в минус -
  выходом фильтра на этом шаге будет текущее значение сигнала. Если значения x_boost_delta приравнять 0, то проверка на это производиться не будет.
  \details Перед использованием необходимо установить параметр length, который является эквивалентом постоянной времени RC цепочки.
  для частоты вызова фильтра в 1 кГц, значение length равно количеству мс постоянной времени.
  \return отфильтрованное значение сигнала
  \param[in] filter - указатель на объект фильтра
  \param[in] data_in - текущее значение сигнала
*/
int filter_low_pass_boost (filter_low_pass_boost_t *filter, int data_in)
{
    if (filter->up_boost_delta && (data_in - filter->data_out) > filter->up_boost_delta)
    {
        filter->data_sum = data_in * filter->length;
    }
    else if (filter->down_boost_delta && (filter->data_out - data_in) > filter->down_boost_delta)
    {
        filter->data_sum = data_in * filter->length;
    }

    filter->data_out = filter->data_sum / filter->length;

    return (filter->data_out);
}

/*!
  \brief Функция фильтрации по типу скользящего среднего
  \deprecated Выглядит монструозно, надо бы ее полностью переписать. ИМХО ХВВ.
  \return Возвращает значение отфильтрованного по скользящему среднему
  \param[in] mv - указатель на объект фильтра скользящего среднего
  \param[in] new_sense - текущее значение входного сигнала
*/
float filter_moving_average (filter_moving_average_t *mv, float new_sense)
{
    if (mv->already_do)
    {
        mv->summ -= mv->src[mv->qty];
    }

    mv->src[mv->qty++] = new_sense;

    if (mv->already_do)
    {
        mv->summ += new_sense;
        mv->average_sense = mv->summ / mv->length;
    }

    if (mv->qty >= mv->length)
    {
        if (!mv->already_do)
        {
            for (int i = 0; i < mv->length; i++)
            {
                mv->summ += mv->src[i];
            }
            mv->average_sense = mv->summ/mv->length;
            mv->already_do = 1;
        }

        mv->qty = 0;
    }

    if (!mv->already_do)
    {
        return new_sense;
    }

    return mv->average_sense;
}

/*!
  \brief Функция сброса быстрого фильтра нижних частот.     [int]
  \return Не возвращает значения
  \param[in] filter - указатель на экземпляр объекта filter_low_pass_fast_start
 */
void filter_low_pass_fast_start_reset(filter_low_pass_fast_start_t * filter)
{
    filter->data_sum = filter->data_out;
    filter->data_cnt = 1;
}

/*!
  \brief Функция быстрого фильтра нижних частот.        [int]
  \return отфильтрованне значение сигнала
  \param[in] filter - указатель на экземпляр объекта filter_low_pass_fast_start
  \param[in] data_in - текущее значение входного сигнала
 */
int filter_low_pass_fast_start(filter_low_pass_fast_start_t * filter, int data_in)
{
    if (filter->data_cnt >= filter->length)
    {
        filter->data_sum = filter->data_sum + data_in - filter->data_out;
        filter->data_out = filter->data_sum / filter->length;
    }
    else
    {
        filter->data_sum = filter->data_sum + data_in;
        filter->data_cnt++;
        filter->data_out = filter->data_sum / filter->data_cnt;
    }

    return (filter->data_out);
}

/*!
  \brief Функция сброса быстрого фильтра нижних частот.     [float]
  \return Не возвращает значения
  \param[in] filter - указатель на экземпляр объекта filter_low_pass_fast_start
 */
void filter_low_pass_fast_start_f_reset(filter_low_pass_fast_start_f_t * filter)
{
    filter->data_sum = filter->data_out;
    filter->data_cnt = 1;
}

/*!
  \brief Функция быстрого фильтра нижних частот.        [int]
  \return отфильтрованне значение сигнала
  \param[in] filter - указатель на экземпляр объекта filter_low_pass_fast_start
  \param[in] data_in - текущее значение входного сигнала
 */
float filter_low_pass_fast_start_f(filter_low_pass_fast_start_f_t * filter, float data_in)
{
    if (filter->data_cnt >= filter->length)
    {
        filter->data_sum = filter->data_sum + data_in - filter->data_out;
        filter->data_out = filter->data_sum / filter->length;
    }
    else
    {
        filter->data_sum = filter->data_sum + data_in;
        filter->data_cnt++;
        filter->data_out = filter->data_sum / filter->data_cnt;
    }

    return (filter->data_out);
}