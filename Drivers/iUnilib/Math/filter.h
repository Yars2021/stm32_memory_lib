#ifndef _FILTER_H_
#define _FILTER_H_

typedef struct
{
    volatile int length;
    volatile int data_sum;
    volatile int data_out;
} filter_low_pass_i_t;

typedef struct
{
    volatile float length;
    volatile float data_sum;
    volatile float data_out;
} filter_low_pass_f_t;

typedef struct
{
    int length;
    int data_cnt;
    int data_sum;
    int data_out;
} filter_arithmetic_mean_t;

typedef struct
{
    int length;
    int data_sum;
    int data_out;
    int up_boost_delta;
    int down_boost_delta;
} filter_low_pass_boost_t;

typedef struct
{
    float *src;
    uint32_t qty;
    uint32_t already_do;
    float summ;
    float average_sense;
    uint32_t length;
} filter_moving_average_t;

typedef struct
{
    int32_t length;
    int32_t data_cnt;
    long data_sum;
    int32_t data_out;
} filter_low_pass_fast_start_t;

typedef struct
{
    int length;
    float data_cnt;
    float data_sum;
    float data_out;
} filter_low_pass_fast_start_f_t;

void bubble_sort (uint16_t *p, size_t n);
void shall_sort (uint16_t *p, size_t n);
uint16_t mediane_filter (uint16_t *p, size_t n);
uint16_t mediane_3p_u16 (uint16_t a, uint16_t b, uint16_t c);
uint16_t mediane_3p_pointer_u16 (uint16_t *p);
int mediane_3p_int (int a, int b, int c);
int mediane_3p_pointer_int (int *p);
float mediane_3p_float (float a, float b, float c);
float mediane_3p_pointer_float (float *p);
float exp_smoothing (float *current, float *previous, float kexp);
void filter_low_pass_init_int (filter_low_pass_i_t *filter, int data_in);
int filter_low_pass_int (filter_low_pass_i_t *filter, int data_in);
void filter_low_pass_init_float (filter_low_pass_f_t *filter, float data_in);
float filter_low_pass_float (filter_low_pass_f_t *filter, float data_in);
int filter_arithmetic_mean (filter_arithmetic_mean_t *filter, int data_in);
int filter_low_pass_boost (filter_low_pass_boost_t *filter, int data_in);
float filter_moving_average (filter_moving_average_t *mv, float new_sense);

#endif /* _FILTER_H_ */
