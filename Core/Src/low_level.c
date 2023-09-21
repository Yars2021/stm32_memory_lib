#include "include.h"

static void init_rcc (void);
static void init_gpio (void);
static void i2c1_init(void);

I2C_HandleTypeDef hi2c1;

void initLowLevel (void)
{
    HAL_Init();
    init_rcc();
    init_gpio();
    i2c1_init();
}

static void init_rcc (void)
{
    RCC_OscInitTypeDef sRCC = {0};
    RCC_ClkInitTypeDef sCLK = {0};

    sRCC.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    sRCC.HSEState = RCC_HSE_ON;
    sRCC.HSEPredivValue = RCC_HSE_PREDIV_DIV2;
    sRCC.HSIState = RCC_HSI_ON;
    sRCC.PLL.PLLState = RCC_PLL_ON;
    sRCC.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    sRCC.PLL.PLLMUL = RCC_PLL_MUL9;
    if (HAL_RCC_OscConfig(&sRCC) != HAL_OK)
    {
        Error_Handler();
    }

    sCLK.ClockType = RCC_CLOCKTYPE_HCLK |
                     RCC_CLOCKTYPE_SYSCLK |
                     RCC_CLOCKTYPE_PCLK1 |
                     RCC_CLOCKTYPE_PCLK2;
    sCLK.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    sCLK.AHBCLKDivider = RCC_SYSCLK_DIV1;
    sCLK.APB1CLKDivider = RCC_HCLK_DIV2;
    sCLK.APB2CLKDivider = RCC_HCLK_DIV1;
    if (HAL_RCC_ClockConfig(&sCLK, FLASH_LATENCY_2) != HAL_OK)
    {
        Error_Handler();
    }
}

static void init_gpio (void)
{
    __HAL_RCC_AFIO_CLK_ENABLE();
    AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_1;

    pin_init(PIN_LED_GREEN);

    GPIO_InitTypeDef GPIO_InitStructure = {0};

    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitStructure.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    GPIO_InitStructure.Pin = GPIO_PIN_8 | GPIO_PIN_9; // I2C1: SCL | SDA
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
}

static void i2c1_init(void)
{
    __HAL_AFIO_REMAP_I2C1_ENABLE();
    __HAL_RCC_I2C1_CLK_ENABLE();

    hi2c1.Instance = I2C1;
    hi2c1.Init.ClockSpeed = 100000;
    hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
    hi2c1.Init.OwnAddress1 = 0;
    hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c1.Init.OwnAddress2 = 0;
    hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    
    if (HAL_I2C_Init(&hi2c1) != HAL_OK)
    {

    }
}
