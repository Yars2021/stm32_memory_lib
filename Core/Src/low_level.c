#include "include.h"

static void init_rcc (void);
static void init_gpio (void);
static void init_spi (void);

SPI_HandleTypeDef spi;
I2C_HandleTypeDef hi2c1;

void initLowLevel (void)
{
    HAL_Init();
    init_rcc();
    init_gpio();
    init_spi();
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

static void init_spi (void) {
    __HAL_RCC_SPI2_CLK_ENABLE();
    spi.Instance = SPI2;
    spi.Init.Mode = SPI_MODE_MASTER;
    spi.Init.Direction = SPI_DIRECTION_2LINES;
    spi.Init.DataSize = SPI_DATASIZE_8BIT;
    spi.Init.CLKPolarity = SPI_POLARITY_HIGH;
    spi.Init.CLKPhase = SPI_PHASE_2EDGE;
    spi.Init.NSS = SPI_NSS_SOFT;
    spi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
    spi.Init.FirstBit = SPI_FIRSTBIT_MSB;
    spi.Init.TIMode = SPI_TIMODE_DISABLE;
    spi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    spi.Init.CRCPolynomial = 10;
    if (HAL_SPI_Init(&spi) != HAL_OK)
    {
        Error_Handler();
    }

    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_14;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
    HAL_Delay(10);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
    HAL_Delay(10);
}